#include "udf.h"

static FILE * s_pfile = 0;

void open_files()
{
    if (0 == s_pfile)
    {
        s_pfile = fopen("avg_t.dat", "a+");
    }
}

void close_files()
{
    if (0 != s_pfile )
    {
        fclose(s_pfile);
    }
}

/*
* @param int did : Domain ID (1 for single phase)
* @param int tid : Thread ID
*/
real get_face_avg_temp(int did, int tid)
{
    real face_t_avg = 0.0f;
    real temperature = 0.0f;
    real area  = 0.0f;
    int num = 0;
    //int thread_ID = tid;
    //
    face_t f;
    Thread *t;
    int n;
    Node *node;
    Domain *domain;
    //
    real NV_VEC(A);
    real x[ND_ND];

    //
    real area_cell = 0.0f;

    domain = Get_Domain(did);  /* returns fluid domain pointer */
    t = Lookup_Thread(domain, tid);
    //CX_Message("There are %d nodes in Face.\n", F_NNODES(f,t));


    //area = A[0];

    begin_f_loop(f, t)    /* loops over faces in a face thread  */
    {
        F_CENTROID(x,f,t);
        F_AREA(A, f, t);

        area_cell = fabs(A[1]);
#ifdef DEBUG
        CX_Message("area : %f\n", area_cell);
        CX_Message("temperature : %f  %f\n", x[0], F_T(f, t));
#endif // DEBUG
        temperature += F_T(f,t) * area_cell;
        area += area_cell;
    }
    end_f_loop(f, t)

        face_t_avg  = temperature / area;
    //
    CX_Message("Total area of Face %d : %.2f\n", THREAD_ID(t), area);
    CX_Message("The average temperature of the Face : %.2f\n", face_t_avg);

    return face_t_avg;
}


static int last_ts = -1;   /*  Global variable.  Time step is never <0 */

/*    */

DEFINE_ADJUST(do_every_timestep, domain)
{
    int curr_ts;
    real avg_t;

    curr_ts = RP_Get_Integer("time-step");

    if (last_ts != curr_ts)
    {
        last_ts = curr_ts;

        CX_Message("Current Time Step : %d\n", curr_ts);

        /* things to be done only on first iteration of each time step */
        /* can be put here  */

        /* adjust rho */


        /* report current mass & rho */

        /*             */
        avg_t = get_face_avg_temp(1, 9);

        open_files();

        fprintf(s_pfile, "%d     %f", last_ts+1, avg_t);

        close_files();

    }
}

static real beta_t = 3.0;
static real latent_heat_mat = 1000.0;

/* energy loss of mass change and latent heat */
DEFINE_SOURCE(energy_loss, c, t, dS, eqn)
{
    real x[ND_ND];
    real source;
    real dt;
    real dm;
    real t1, t2;

    int curr_ts = RP_Get_Integer("time-step");

    if (1 < curr_ts)
    {
        /* Get DT */
        /* C_CENTROID(x, c, t); */

        t1 = C_T(c,t);

        t2 = C_T_M1(c,t);

        dt = t1 - t2;

        dm = 0.0 - beta_t * dt;

        source = dm * latent_heat_mat;

        dS[eqn] = 0.0;
    }
    else
    {
        source = 0.0;
        dS[eqn] = 0.0;
    }

    return source;
}
