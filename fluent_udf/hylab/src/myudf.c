/*
 * =====================================================================================
 *
 *       Filename:  myudf.c
 *
 *    Description:  Implement for my UDF wrappers
 *
 *        Version:  1.0
 *        Created:  2010-2-23 12:01:23
 *       Revision:  none
 *       Compiler:  gcc/msc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

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

DEFINE_ON_DEMAND(hy_show_info_dialog)
{
    showInfoDialog();
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
        {
            sum_diss += C_D(c,t) * C_VOLUME(c,t);
        }
        end_c_loop(c,t)
    }

    printf("Volume integral of turbulent dissipation: %g\n", sum_diss);
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
    face_t f;        /*  index of face of wall */
    cell_t c0;      /*  index of cell which is adjacent to wall. */
    Domain *domain;

    thread_ID = 8;  /*  */
    count=0;
    i=0;


    domain = Get_Domain(1);  /* returns fluid domain pointer */
    t = Lookup_Thread(domain, thread_ID);

    /*  */
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        count++;
    }
    end_f_loop(f,t)

    g_sgrid = (int *) malloc(count * sizeof(int));
    g_ssize = count;
    /*  */
    begin_f_loop(f,t)
    {
        c0 = F_C0(f,t);
        g_sgrid[i] = c0;
        i++;
    }
    end_f_loop(f,t)
}

/*  */
int cellTest(int d)
{
    int iResult;
    int i;
    /*  if d is within array g_sgrid return 1, otherwise return 0. */
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

real g_hy_face_t_avg = 0.0;

/*  Fetch the average temperature of a specific face */
DEFINE_ON_DEMAND(hy_face_temp_avg)
{
    real temperature = 0.0f;
    real area  = 0.0f;
    int num = 0;
    int thread_ID = 8;
    /*  */
    face_t f;
    Thread *t;
    int n;
    Node *node;
    Domain *domain;
    /*  */
    real NV_VEC(A);
    real x[ND_ND];

    domain = Get_Domain(1);  /* returns fluid domain pointer */
    t = Lookup_Thread(domain, thread_ID);

    /*
    CX_Message("There are %d nodes in Face.\n", F_NNODES(f,t));
    */

    /* area = A[0]; */

    begin_f_loop(f, t)    /* loops over faces in a face thread  */
    {
        F_CENTROID(x,f,t);
        F_AREA(A, f, t);
#ifdef DEBUG
        CX_Message("area : %f\n", -A[1]);
        CX_Message("temperature : %f  %f\n", x[0], F_T(f, t));
#endif /*  DEBUG */
        temperature -= F_T(f,t) * A[1];
        area -= A[1];
    }
    end_f_loop(f, t)

    g_hy_face_t_avg  = temperature / area;
    /*  */
    CX_Message("Total area of Face %d : %f\n", THREAD_ID(t), area);
    CX_Message("The average temperature of the Face : %f\n", g_hy_face_t_avg);
}


FILE *fout;

void Print_Thread_Face_Centroids(Domain *domain, int id)
{
    real FC[3] = {0., 0.};
    face_t f;
    Thread *t = Lookup_Thread(domain, id);

    fprintf(fout,"thread id %d\n", id);
    begin_f_loop(f,t)
    {
        F_CENTROID(FC,f,t);
        fprintf(fout, "f%d %g %g %g\n", f, FC[0], FC[1], FC[2]);
    }
    end_f_loop(f,t)
        fprintf(fout, "\n");
}

DEFINE_ON_DEMAND(get_coords)
{
    Domain *domain;
    domain = Get_Domain(1);
    fout = fopen("faces.out", "w");
    Print_Thread_Face_Centroids(domain, 2);
    Print_Thread_Face_Centroids(domain, 4);
    fclose(fout);
}


/*  Source Term */
DEFINE_SOURCE(MySourceTerm, c, t, dS, eqn)
{
    real source;
    if(cellTest(c))
    {
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


/**********************************************************************
UDF that computes diffusivity for mean age using a user-defined    
scalar.                                                            
***********************************************************************/
DEFINE_DIFFUSIVITY(mean_age_diff,c,t,i)
{
    return C_R(c,t) * 2.88e-05 + C_MU_EFF(c,t) / 0.7;
}

