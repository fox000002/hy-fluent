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
    
    CX_Message ("time = %f, x_vel= %f, force = %f\n", time, v_prev, force);
    /* set x-component of velocity */
    vel[0] = v_prev;
}

/************************************************************
 * defining parabola through points (0, 1), (1/2, 5/4), (1, 1)
 ************************************************************/
DEFINE_GEOM(parabola, domain, dt, position)
{
  /* set y = -x^2 + x + 1 */
  position[1] = - position[0]*position[0] + position[0] + 1;
}

/**********************************************************
 node motion based on simple beam deflection equation
 compiled UDF
 **********************************************************/
DEFINE_GRID_MOTION(beam,domain,dt,time,dtime)
{
  Thread *tf = DT_THREAD(dt);
  face_t f;
  Node *v;
  real NV_VEC(omega), NV_VEC(axis), NV_VEC(dx);
  real NV_VEC(origin), NV_VEC(rvec);
  real sign;
  int n;

  /* set deforming flag on adjacent cell zone */
  SET_DEFORMING_THREAD_FLAG(THREAD_T0(tf));

  sign = -5.0 * sin (26.178 * time);

  Message ("time = %f, omega = %f\n", time, sign);

  NV_S(omega, =, 0.0);
  NV_D(axis, =, 0.0, 1.0, 0.0);
  NV_D(origin, =, 0.0, 0.0, 0.152);

  begin_f_loop(f,tf)
    {
      f_node_loop(f,tf,n)
        {
          v = F_NODE(f,tf,n);

          /* update node if x position is greater than 0.02
             and that the current node has not been previously
             visited when looping through previous faces */
          if (NODE_X(v) > 0.020 && NODE_POS_NEED_UPDATE (v))
            {
              /* indicate that node position has been update
                 so that it's not updated more than once */
              NODE_POS_UPDATED(v);

              omega[1] = sign * pow (NODE_X(v)/0.230, 0.5);
              NV_VV(rvec, =, NODE_COORD(v), -, origin);
              NV_CROSS(dx, omega, rvec);
              NV_S(dx, *=, dtime);
              NV_V(NODE_COORD(v), +=, dx);
            }
        }
    }

  end_f_loop(f,tf);
}

/*******************************************************
SDOF property compiled UDF with external forces/moments
*******************************************************/
DEFINE_SDOF_PROPERTIES(delta_missile, prop, dt, time, dtime)
{
   prop[SDOF_MASS]       = 907.185;
   prop[SDOF_IXX]        = 27.116;
   prop[SDOF_IYY]        = 488.094;
   prop[SDOF_IZZ]        = 488.094;

   /* add injector forces, moments */
   {
     register real dfront = fabs (DT_CG (dt)[2] -
                            (0.179832*DT_THETA (dt)[1]));
     register real dback  = fabs (DT_CG (dt)[2] +
                            (0.329184*DT_THETA (dt)[1]));

     if (dfront <= 0.100584)
       {
         prop[SDOF_LOAD_F_Z] = 10676.0;
         prop[SDOF_LOAD_M_Y] = -1920.0;
       }

     if (dback <= 0.100584)
       {
         prop[SDOF_LOAD_F_Z] += 42703.0;
         prop[SDOF_LOAD_M_Y] += 14057.0;
       }
   }

   printf ("\ndelta_missile: updated 6DOF properties");
}

/*                                        */
static real G = 9.80; /* 重力*/
static real valve_M = 0.1; /* 质量*/
static real valve_S = 0.0; /* 位移*/
static real valve_V = 0.0; /* 速度*/
static real valve_F = 0.0; /*受力*/
static int is_valve = 0; /* 判断物体是否运动的阀值*/
static real ball_S = 0.0; /* 位移*/
static real ball_V = 0.0; /* 速度*/
static real ball_F = 0.0; /*受力*/
static int is_ball = 0; /* 判断物体是否运动的阀值*/

DEFINE_CG_MOTION(valve, dt, vel, omega, time, dtime)
{
#if !RP_HOST
    Thread *t;
    Domain *d;

    real dv;
    real CG[ND_ND];
    real force[3];
    real moment[3];
    if (!Data_Valid_P())
    {
        Message0("\n\n!!! No data->No mesh motion !!!\n\n");
        return;
    }

    /* */
    NV_S(vel, =, 0.0);
    NV_S(omega, =, 0.0);

    /* force */
    d = THREAD_DOMAIN(DT_THREAD((Dynamic_Thread *)dt));
    t = DT_THREAD(dt);
    NV_S(CG, =, 0.0);
    Compute_Force_And_Moment(d, t, CG, force, moment, FALSE);

    /* force change */
    dv = dtime * (force[0] - G * valve_M) / valve_M;
    if (is_valve == 0 && dv >= 0.0)
    {
        is_valve = 1;
        Message0("\n\n======= BEGIN VALVE MOTION INFO =========\n");
        Message0("\n VALVE BEGIN MOVE! \n");
        Message0("\n========= END VALVE MOTION INFO ==========\n");
    }

    if(is_valve == 0 && dv < 0.0)
    {
        dv = 0.0;
    }

    valve_V += dv;
    if(valve_S >= 0.010)
    {
        valve_V = 0.0;
    }

    valve_S += valve_V * dtime;

    /* x velocity */
    vel[0] = valve_V;
    valve_F = force[0];

    Message0("\n\n==========BEGIN VALVE MOTION INFO ==========\n");
    Message0("\ntime=%.5e F(x)=%.4e S(x)=%.6e V(x)=%.6e move?=%d\n", time, force[0], valve_S, valve_V, is_valve);
    Message0("\n=========== END VALVE MOTION INFO ===========\n");
#endif /* RP_HOST */

    node_to_host_real_1(valve_S);
    node_to_host_real_1(valve_V);
    node_to_host_real_1(valve_F);
    node_to_host_real_1(is_valve);

    node_to_host_real(vel, ND_ND);
    node_to_host_real(omega, ND_ND);
}

/*  */
static void write_data(FILE *fp)
{
    fprintf(fp, "%e ", valve_V);
    fprintf(fp, "\n");
    fprintf(fp, "%e ", valve_S);
    fprintf(fp, "\n");
    fprintf(fp, "%e ", valve_F);
    fprintf(fp, "\n");
    fprintf(fp, "%d ", is_valve);
    fprintf(fp, "\n");
}

/* 读数据函数*/
static void read_data(FILE * fp)
{
    fscanf(fp, "%e", &valve_V);
    fscanf(fp, "%e", &valve_S);
    fscanf(fp, "%e", &valve_F);
    fscanf(fp, "%d", &is_valve);

    fscanf(fp, "%e", &ball_V);
    fscanf(fp, "%e", &ball_S);
    fscanf(fp, "%e", &ball_F);
    fscanf(fp, "%d", &is_ball);
}

DEFINE_RW_FILE(dm_reader, fp)
{
    Message0("\n\n========== BEGIN UDF INFOMATION ==========\n");
    Message0("\nReading UDF data from data file...\n");

#if PARALLEL
  #if RP_HOST
    read_data(fp);
  #endif
#else
    read_data(fp);
#endif

    host_to_node_real_1(valve_V);
    host_to_node_real_1(valve_S);
    host_to_node_real_1(valve_F);
    host_to_node_int_1(is_valve);

    host_to_node_real_1(ball_V);
    host_to_node_real_1(ball_S);
    host_to_node_real_1(ball_F);
    host_to_node_int_1(is_ball);

    Message0("\n------------------------------ BEGIN VALVE MOTION INFO ------------------------------\n");
    Message0("\nS(x)=%.6e V(x)=%.6e F(x)=%.4e move?=%d\n", valve_S, valve_V, valve_F, is_valve);
    Message0("\n------------------------------ END VALVE MOTION INFO ------------------------------\n");

    Message0("\n------------------------------ BEGIN BALL MOTION INFO ------------------------------\n");
    Message0("\nS(x)=%.6e V(x)=%.6e F(x)=%.4e move?=%d\n", ball_S, ball_V, ball_F, is_ball);
    Message0("\n------------------------------- END BALL MOTION INFO -------------------------------\n");
    Message0("\n===============END UDF INFOMATION ==================\n\n");
}

DEFINE_RW_FILE(dm_writer, fp)
{
    Message0("\n\n=============BEGIN UDF INFOMATION =================\n");
    Message0("\n\nWriting UDF data to data file...\n");

#if PARALLEL
#if RP_HOST
    write_data(fp);
#endif
#else
    write_data(fp);
#endif
    Message0("\n=============== END UDF INFOMATION =================\n\n");
}
