#include "udf.h"
#include "dynamesh_tools.h"

static real v_prev= 0.0;

DEFINE_CG_MOTION(piston, dt, vel, omega, time, dtime)
{
    Thread *t;
    face_t f;
    real NV_VEC (A);
    real force, dv;
    /* reset velocities */
    NV_S (vel, =, 0.0);
    NV_S (omega, =, 0.0);
    if (!Data_Valid_P ())
    return;
    /* get the thread pointer for which this motion is defined */
    t = DT_THREAD ((Dynamic_Thread *)dt);
    /* compute pressure force on body by looping through all faces */
    force = 0.0;
    begin_f_loop (f, t)
    {
        F_AREA (A, f, t);
        force += F_P (f, t) * NV_MAG (A);
    }
    end_f_loop (f, t)
    /* compute change in velocity, i.e., dv= F * dt/ mass;
       velocity update using explicit eulerformula */
    dv= dtime* force / 50.0;
    v_prev+= dv;
    Message ("time = %f, x_vel= %f, force = %f\n", time, v_prev, force);
    /* set x-component of velocity */
    vel[0] = v_prev;
}
