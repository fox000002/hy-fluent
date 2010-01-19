#include "udf.h"

#include "mydialog.h"

/************************************************************************/
/*  EOD functions                                                       */
/************************************************************************/
DEFINE_ON_DEMAND(showMsg)
{
    /*Message("message\n");*/
    showMyMessage("Hello!");
}

DEFINE_ON_DEMAND(myAdd)
{
    Message("1+1");
}

DEFINE_ON_DEMAND(runSchemeProc)
{
    CX_Interpret_String("(%udf-on-demand \"showMsg::libhylab\")");
}


/************************************************************************/
/*   2009.12.26: Sigh!!!! I do not know how to use it!                  */
/*   2009.12.26: Aha! Use (%run-udf-apply mode)!                        */
/************************************************************************/
DEFINE_EXECUTE_FROM_GUI(sayHello, libhylab, mode)
{
    if(0 == mode)
    {
        showMyMessage("Hello!");
    }
    Message("Hello From the C Function\n");
}


/************************************************************************/
/* UDF for integrating turbulent dissipation                            */
/* and printing it to console window                                    */
/************************************************************************/
DEFINE_ADJUST(my_adjust, d)
{
    Thread *t;
    /* Integrate dissipation. */
    real sum_diss=0.;
    cell_t c;

    thread_loop_c(t,d)
    {
        begin_c_loop(c,t)
            sum_diss += C_D(c,t) * C_VOLUME(c,t);
        end_c_loop(c,t)
    }

    printf("Volume integral of turbulent dissipation: %g\n", sum_diss);
}

/***********************************************************************/
/* UDF that changes the time step value for a time-dependent solution  */
/***********************************************************************/
DEFINE_DELTAT(mydeltat,d)
{
    real time_step;
    real flow_time = CURRENT_TIME;
    if (flow_time < 0.5)
        time_step = 0.1;
    else
        time_step = 0.2;
    return time_step;
}

/***********************************************************************
UDF for initializing flow field variables
************************************************************************/
DEFINE_INIT(my_init_func, d)
{
    cell_t c;
    Thread *t;
    real xc[ND_ND];

    /* loop over all cell threads in the domain  */
    thread_loop_c(t,d)
    {

        /* loop over all cells  */
        begin_c_loop_all(c,t)
        {
            C_CENTROID(xc,c,t);
            if (sqrt(ND_SUM(pow(xc[0] - 0.5,2.),
                pow(xc[1] - 0.5,2.),
                pow(xc[2] - 0.5,2.))) < 0.25)
                C_T(c,t) = 400.;
            else
                C_T(c,t) = 300.;
        }
        end_c_loop_all(c,t)
    }
}

/***********************************************************************
UDFs that increment a variable, write it to a data file
and read it back in
************************************************************************/
int kount = 0;  /* define global variable kount */

DEFINE_ADJUST(demo_calc, d)
{
    kount++;
    printf("kount = %d\n",kount);
}

DEFINE_RW_FILE(writer,fp)
{
    printf("Writing UDF data to data file...\n");
    fprintf(fp, "%d", kount); /* write out kount to data file */
}

DEFINE_RW_FILE(reader,fp)
{
    printf("Reading UDF data from data file...\n");
    fscanf(fp, "%d", &kount); /* read kount from data file */
}


/***********************************************************************
 UDFs that operating the grid
************************************************************************/
int * g_sgrid = 0;
int g_ssize = 0;

DEFINE_ON_DEMAND(INIT)
{
    int count;
    int thread_ID;
    Thread *t;
    int i;
    face_t f;       // index of face of wall
    cell_t c0;      // index of cell which is adjacent to wall.
    thread_ID = 8;  //
    count=0;
    i=0;

    t = Lookup_Thread(1, thread_ID);

    //
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        count++;
    }
    end_f_loop(f,t)

    g_sgrid = (int *) malloc(count * sizeof(int));
	g_ssize = count;
    //
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        g_sgrid[i] = c0;
        i++;
    }
    end_f_loop(f,t)
}

//
int cellTest(int d)
{
	int iResult;
	int i;
	// if d is within array g_sgrid return 1, otherwise return 0.
	iResult = 0;

	for (i=0; i<g_ssize; ++i)
	{
		if (d == g_sgrid[i])
		{
			iResult = 1;
			break;
		}
	}

	return iResult;
}

// Source Term
DEFINE_SOURCE(MySourceTerm, c, t, dS, eqn)
{
    real source;
    if(cellTest(c))
    {
        //source = your source; //在这里面会用到变量
        source = 1.0;
		dS[eqn] = 0.0;
        return source;
    }
    return 0;
}



/*******************************************************************/
/* UDF for specifying an x-momentum source term in a spatially     */
/* dependent porous media                                          */
/*******************************************************************/
/*
 * source = -0.5 C_2 \rho y |v_x| v_x
 */
#define C2 100.0

DEFINE_SOURCE(xmom_source, c, t, dS, eqn)
{
    real x[ND_ND];
    real con, source;
    C_CENTROID(x, c, t);
    con = C2*0.5*C_R(c, t)*x[1];
    source = -con*fabs(C_U(c, t))*C_U(c, t);
    dS[eqn] = -2.*con*fabs(C_U(c, t));
    return source;
}

/*******************************************************************/
/*    UDF that initializes particles on a surface injection due    */
/*    to a surface reaction                                        */
/*******************************************************************/
#define       REACTING_SURFACE_ID 2
#define       MW_H2 2
#define       STOIC_H2 1

#ifndef P_CELL
#define       P_CELL(P) RP_CELL(&((P)->cCell))           /* Non-standard macros */
#endif // P_CELL

#ifndef P_CELL_THREAD
#define       P_CELL_THREAD(P) RP_THREAD(&((P)->cCell))
#endif // P_CELL_THREAD

real contact_area(cell_t c, Thread *t, int s_id, int *n);

/* ARRHENIUS CONSTANTS */
#define PRE_EXP 1e+15
#define ACTIVE 1e+08
#define BETA 0.0
real arrhenius_rate(real temp)
{
	return PRE_EXP*pow(temp,BETA)*exp(-ACTIVE/(UNIVERSAL_GAS_CONSTANT*temp));
}

/* Species numbers. Must match order in Fluent panel */
#define HF  0

/* Reaction Exponents */
#define HF_EXP  2.0

/* Reaction Rate Routine used in UDF */
real reaction_rate(cell_t c, Thread *cthread, real mw[], real yi[])
/* Note that all arguments in the reaction_rate function
call in your .c source file MUST be on the same line or a
compilation error will occur */
{
	real concenHF = C_R(c,cthread)*yi[HF]/mw[HF];

	return arrhenius_rate(C_T(c,cthread)) * pow(concenHF, HF_EXP);
}

DEFINE_DPM_INJECTION_INIT(init_bubbles, I)
{
    int count,i;
    real area, mw[MAX_SPE_EQNS], yi[MAX_SPE_EQNS];
    /* MAX_SPE_EQNS is a Fluent constant in materials.h */
    Particle *p;
    cell_t cell;
    Thread *cthread;
    Material *mix, *sp;
    Message("Initializing Injection: %s\n", I->name);
    loop(p, I->p) /* Standard Fluent Looping Macro to get particlestreams in an Injection */
    {
        cell = P_CELL(p);               /* Get the cell and thread that the particle
                                      is currently in   */
        cthread = P_CELL_THREAD(p);
        /* Set up molecular weight & mass fraction arrays */
        mix = THREAD_MATERIAL(cthread);
        mixture_species_loop(mix,sp,i)
        {
            mw[i] = MATERIAL_PROP(sp, PROP_mwi);
            yi[i] = C_YI(cell, cthread, i);
        }
        area = contact_area(cell, cthread, REACTING_SURFACE_ID, &count);
        /* Function that gets total area of REACTING_SURFACE faces in
         contact with cell */
        /* count is the number of contacting faces, and is needed
         to share the total bubble emission between the faces      */
        if (count > 0) /* if cell is in contact with REACTING_SURFACE */
        {
            P_FLOW_RATE(p) = (area *MW_H2* STOIC_H2 * reaction_rate(cell, cthread, mw, yi))/(real)count;
            /* to get correct total flow
                         rate when multiple faces contact the same cell */
            P_DIAM(p) = 1e-3;
            P_RHO(p) = 1.0;
            P_MASS(p) = P_RHO(p) * M_PI * pow(P_DIAM(p),3.0)/6.0;
        }
        else
        {
            P_FLOW_RATE(p) = 0.0;
        }
    }
}

real contact_area(cell_t c, Thread *t, int s_id, int *n)
{
    int i = 0;
    real area = 0.0;
	real A[ND_ND];
    *n = 0;
    c_face_loop(c, t, i)
    {
        if(THREAD_ID(C_FACE_THREAD(c,t,i)) == s_id)
        {
            (*n)++;
            F_AREA(A,C_FACE(c,t,i), C_FACE_THREAD(c,t,i));
            area += NV_MAG(A);
        }
    }
    return area;
}

/*********************************************************************
UDF that specifies discrete phase materials                       
**********************************************************************/

#include "udf.h"

DEFINE_DPM_PROPERTY(coal_emissivity,c,t p)
{
	real mp0= P_INIT_MASS(p);  
	real mp = P_MASS(p);      
	real vf, cf;

	/* get the material char and volatile fractions and store them */
	/* in vf and cf                                               */
	vf=DPM_VOLATILE_FRACTION(p);
	cf=DPM_CHAR_FRACTION(p);

	if (!(((mp/mp0) >= 1) || ((mp/mp0) <= 0)))
	{
		if ((mp/mp0) < (1-(vf)-(cf)))
		{
			/* only ash left */
			/* vf = cf = 0; */
			return .001;
		}
		else if ((mp/mp0) < (1-(vf)))
		{
			/* only ash and char left */
			/* cf = 1 - (1-(vf)-(cf))/(mp/mp0); */
			/* vf = 0; */
			return 1.0;
		}

		else
		{
			/* volatiles, char, and ash left */
			/* cf =  (cf)/(mp/mp0); */
			/* vf = 1. - (1.-(vf))/(mp/mp0); */
			return 1.0;
		}
	}
	return 1.0;
} 

DEFINE_DPM_PROPERTY(coal_scattering,c,t,p)
{
	real mp0 = P_INIT_MASS(p);  
	real mp = P_MASS(p);      
	real cf, vf;

	/* get the original char and volatile fractions and store them */
	/* in vf and cf                                               */
	vf = DPM_VOLATILE_FRACTION(p);
	cf = DPM_CHAR_FRACTION(p);

	if (!(((mp/mp0) >= 1) || ((mp/mp0) <= 0)))
	{
		if ((mp/mp0) < (1-(vf)-(cf)))
		{
			/* only ash left */
			/* vf = cf = 0; */
			return 1.1;
		}
		else if ((mp/mp0) < (1-(vf)))
		{
			/* only ash and char left */
			/* cf = 1 - (1-(vf)-(cf))/(mp/mp0); */
			/* vf = 0; */
			return 0.9;
		}

		else
		{
			/* volatiles, char, and ash left */
			/* cf =  (cf)/(mp/mp0); */
			/* vf = 1. - (1.-(vf))/(mp/mp0); */
			return 1.0;
		}
	}
	return 1.0;
}

/****************************************************/
/*  Simple UDF that uses compiler directives        */
/****************************************************/
DEFINE_ADJUST(where_am_i, domain)
{
#if RP_HOST
    CX_Message("I am in the host process\n");
#endif /* RP_HOST */

#if RP_NODE
    CX_Message("I am in the node process with ID %d\n",myid);
  /* myid is a global variable which is set to the multiport ID for
     each node */
#endif /* RP_NODE */

#if !PARALLEL
    CX_Message("I am in the serial process\n");
#endif /* !PARALLEL */
}

