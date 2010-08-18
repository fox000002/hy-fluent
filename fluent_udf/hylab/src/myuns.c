/*
 * =====================================================================================
 *
 *       Filename:  myuns.c
 *
 *    Description:  UDF for unsteady
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

#include "myudf.h"

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
        s_pfile = 0;
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
    /*  int num = 0; */
    /*  */
    face_t f;
    Thread *t;
    /*  Node *node; */
    Domain *domain;
    /*  */
    real NV_VEC(A);
    real x[ND_ND];

    /*  */
    real area_cell = 0.0f;

    domain = Get_Domain(did);  /* returns fluid domain pointer */
    t = Lookup_Thread(domain, tid);
    /* CX_Message("There are %d nodes in Face.\n", F_NNODES(f,t)); */


    /* area = A[0]; */

    begin_f_loop(f, t)    /* loops over faces in a face thread  */
    {
        F_CENTROID(x,f,t);
        F_AREA(A, f, t);

        area_cell = fabs(A[1]);
#ifdef DEBUG
        CX_Message("area : %f\n", area_cell);
        CX_Message("temperature : %f  %f\n", x[0], F_T(f, t));
#endif /* DEBUG */
        temperature += F_T(f,t) * area_cell;
        area += area_cell;
    }
    end_f_loop(f, t)

    face_t_avg  = temperature / area;

#ifdef DEBUG
    CX_Message("Total area of Face %d : %.2f\n", THREAD_ID(t), area);
    CX_Message("The average temperature of the Face : %.2f\n", face_t_avg);
#endif /* DEBUG */

    return face_t_avg;
}

/*   */
real get_avg_density(int zone_id)
{
    real rho_avg = 0.0;
    real mass = 0.0;
    real vol = 0.0;
    Domain *domain;
    Thread *t;
    cell_t c;

    domain = Get_Domain(1);
    /* t = Lookup_Thread(domain, zone_id); */

    thread_loop_c(t,domain)
    {
        if (!FLUID_THREAD_P(t))
        {
            begin_c_loop(c,t)
            {
                vol += C_VOLUME(c,t);
                mass += C_R(c, t) * C_VOLUME(c,t);
            }
            end_c_loop(c,t)
        }
    }

    rho_avg = mass / vol;

    return rho_avg;
}

DEFINE_ON_DEMAND(hy_test_avg_func)
{
    real avg = get_avg_density(10);

    CX_Message("Average Density : %g\n", avg);

    CX_Message("Current State: \n");
    CX_Message(" Time: %g\n", CURRENT_TIME);
    CX_Message(" Step: %g\n", CURRENT_TIMESTEP);
    CX_Message(" End:  %g\n", END_TIME);
}

static int last_ts = -1;   /*  Global variable.  Time step is never <0 */

/*    */

DEFINE_ADJUST(do_every_timestep, domain)
{
    int curr_ts;
    real avg_t;
    real avg_t2;

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
        avg_t = get_face_avg_temp(1, 19);
        avg_t2 = get_face_avg_temp(1, 20);

        open_files();

        fprintf(s_pfile, "%d     %.2f     %.2f\n", last_ts+1, avg_t, avg_t2);

        close_files();

    }
}

static real latent_heat_mat = 263.56e3;
/* static real latent_heat_water = 2331.0e3; //0.1 MPa */
static real rho_0_mat = 1490.0;

/*
    real t : temperature in Celsius degree
 */
static real cal_beta_t(real t)
{
    real b = 0.;

    if (t < 60.0)
    {
        b = 0.0;
    }
    else if ( t > 60.0 && t < 200.0 )
    {
        b = 0.03 / 140.0 * (t-60.0);
    }
    else if ( t > 200.0 && t < 310.0 )
    {
        b = 0.03 + 0.02 / 110.0 * (t-200.0);
    }
    else if ( t > 310.0 && t < 400.0 )
    {
        b = 0.05 + 0.12 / 90.0 * (t-310.0);
    }
    else if ( t > 400.0 && t < 480.0 )
    {
        b = 0.17 + 0.20 / 80.0 * (t-400.0);
    }
    else /*  > 480 */
    {
        b = 0.37;
    }

    return b;
}

DEFINE_PROPERTY(cell_density,c,t)
{
    real density;
    /* real dt; */
    real t1;
    /* real t2; */
    real beta;
    /* real r; */

    int curr_ts = RP_Get_Integer("time-step");

    if (0 < curr_ts)
    {
        t1 = C_T_M1(c,t);

        beta = cal_beta_t(t1-273.15);


        density = rho_0_mat * (1 - beta);

#ifdef HU_DEBUG
        CX_Message("!!! Density !!! :  %f   %f    %f\n", C_R_M1(c,t), density, dt);
#endif /*  HU_DEBUG */
    }
    else
    {
        density = rho_0_mat;
    }

    return density;
}

/* energy loss of mass change and latent heat */
DEFINE_SOURCE(energy_loss, c, t, dS, eqn)
{
    /* real x[ND_ND]; */
    real source;
    /* real dt; */
    real dm;
    /* real t1, t2; */

    /* int curr_ts = RP_Get_Integer("time-step"); */

    if (!is_uds_on())
    {
        Error("UDS should be turned on to accessing C_R_M1 variables!");
    }


    if (0 < N_TIME)
    {
        /* Get dt, dm */
        dm = 0.0 - (C_R(c,t) - C_R_M1(c,t)) * C_VOLUME(c,t);

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

static real T_RAD_SOURCE = 1073.15;
/* static real HFLUX_MAX_RAD = 60000; */
static real ALPHA_RAD_HFLUX = 6.365;

DEFINE_PROFILE(wall_rad_flux, t, i)
{
    real t_0;
    real t_w;
    face_t f;

    begin_f_loop(f,t)
    {
        t_w = F_T(f,t);

        if (t_w >= T_RAD_SOURCE)
        {
            F_PROFILE(f,t,i) = 0.0;
        }
        else
        {
            t_0 = T_RAD_SOURCE / 100.;
            t_w = t_w / 100.;
            F_PROFILE(f,t,i) = ALPHA_RAD_HFLUX * ( t_0 * t_0 * t_0 * t_0 - t_w * t_w * t_w * t_w);
        }
    }
    end_f_loop(f,t)
}

DEFINE_PROFILE(temp_free_stream, t, i)
{
    real t_f;
    face_t f;

    begin_f_loop(f,t)
    {
        t_f = get_face_avg_temp(1, 20);

        F_PROFILE(f,t,i) = t_f;
    }
    end_f_loop(f,t)
}

/***********************************************************************/
/* UDF that changes the time step value for a time-dependent solution  */
/***********************************************************************/
DEFINE_DELTAT(mydeltat, d)
{
    real time_step;
    real flow_time = CURRENT_TIME;
    if (flow_time < 0.5)
    {
        time_step = 0.1;
    }
    else
    {
        time_step = 0.2;
    }
    return time_step;
}


DEFINE_PROFILE(hy_ex_wall_heat_flux, t, i)
{
    real x[ND_ND];
    face_t f;
    real pos_x;
    real pos_y;
    real pos_z;
    real flow_time = CURRENT_TIME;
    real flux;

    /* CX_Message("!!! flow_time!!! :  %f\n", flow_time); */

    begin_f_loop(f,t)
    {
        /* CX_Message("!!!\n");*/

        F_CENTROID(x,f,t);
        pos_x = x[0];
        pos_y = x[1];
#if RP_3D
        pos_z = x[2];
#endif
        if (flow_time  <= 10.0)
        {
            /*  angle   */
            F_PROFILE(f,t,i) = (1.0 - flow_time / 10.0) * (-625 * pos_x + 1.25e6);

            flux = (1.0 - flow_time / 10.0) * (-625 * pos_x + 1.25e6);
            /* CX_Message("!!! Heat Flux!!! :  %f\n", flux); */
        }
        else
        {
            F_PROFILE(f,t,i) = 0.0;
            /* CX_Message("!!! Heat Flux!!! :  %f\n", 0.0); */
        }
    }
    end_f_loop(f,t)
}

