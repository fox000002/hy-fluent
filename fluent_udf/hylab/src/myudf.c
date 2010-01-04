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
            sum_diss += C_D(c,t)*
            C_VOLUME(c,t);
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
DEFINE_INIT(my_init_func,d)
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

DEFINE_ON_DEMAND(INIT)
{
    int counter;
    int thread_ID;
    Thread *t;
    int i;
    face_t f;   // index of face of wall
    cell_t c0;  // index of cell which is adjacent to wall. 也就是B在本例中。
    thread_ID = 8;  // 这个可以从用户界面得到，在boundary condition窗口下，点击璧面A，然后号  //码显示在ID栏里。
    counter=0;
    i=0;

    t = Lookup_Thread(1, thread_ID);//1代表domain id，一般情况都为1。此句得到璧面A的thread.

    // 计算壁面face数量。
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        counter ++;
    }
    end_f_loop(f,t)

    g_sgrid = (int *) malloc(counter * sizeof(int));

    // 存取所有邻近璧面单元的id到数组a里面。
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        g_sgrid[i] = c0;
        i++;
    }
    end_f_loop(f,t)
}

// Source Term
DEFINE_SOURCE(yourSourceName, c, t, dS, eqn)
{
    real source;
    if(cellTest(c))
    {
        //source = your source; //在这里面会用到变量
        source = 1.0;
		return source;
    }
    return 0;
}

// 判断当前单元是否属于邻近璧面单元
int cellTest(int dd)
{
    int iResult;
    // code for testing dd, if dd is within array a return 1, otherwise return 0.
    //...
    iResult = 0;

    return iResult;
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

DEFINE_DPM_INJECTION_INIT(init_bubbles, I)
{
    int count,i;
    real area, mw[MAX_SPE_EQNS], yi[MAX_SPE_EQNS];
    /* MAX_SPE_EQNS is a Fluent constant in materials.h */
    Particle *p;
    cell_t cell;
    Thread *cthread;
    Material *mix, *sp;
    Message("Initializing Injection: %s\n",I->name);
    loop(p,I->p) /* Standard Fluent Looping Macro to get particlestreams in an Injection */
    {
        cell = P_CELL(p);               /* Get the cell and thread that the particle
                                      is currently in   */
        cthread = P_CELL_THREAD(p);
        /* Set up molecular weight & mass fraction arrays */
        mix = THREAD_MATERIAL(cthread);
        mixture_species_loop(mix,sp,i)
        {
            mw[i] = MATERIAL_PROP(sp,PROP_mwi);
            yi[i] = C_YI(cell,cthread,i);
        }
        area = contact_area(cell, cthread, REACTING_SURFACE_ID,&count);
        /* Function that gets total area of REACTING_SURFACE faces in
         contact with cell */
        /* count is the number of contacting faces, and is needed
         to share the total bubble emission between the faces      */
        if (count > 0) /* if cell is in contact with REACTING_SURFACE */
        {
            //P_FLOW_RATE(p) = (area *MW_H2* STOIC_H2 * reaction_rate(cell, cthread, mw, yi))/(real)count;    
			/* to get correct total flow
                         rate when multiple faces contact the same cell */
            P_DIAM(p) = 1e-3;
            P_RHO(p) = 1.0;
            P_MASS(p) = P_RHO(p)*M_PI*pow(P_DIAM(p),3.0)/6.0;
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
    real area = 0.0, A[ND_ND];
    *n = 0;
    c_face_loop(c,t,i)
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

/*****************************************************
  Simple UDF that uses compiler directives
*****************************************************/
DEFINE_ADJUST(where_am_i, domain)
{
#if RP_HOST
    Message("I am in the host process\n");
#endif /* RP_HOST */

#if RP_NODE
    Message("I am in the node process with ID %d\n",myid);
  /* myid is a global variable which is set to the multiport ID for
     each node */
#endif /* RP_NODE */

#if !PARALLEL
    Message("I am in the serial process\n");
#endif /* !PARALLEL */
}

