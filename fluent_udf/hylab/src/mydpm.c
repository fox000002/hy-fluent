#include "udf.h"
#include "dpm.h"
#include "surf.h"
#include "random.h"

#define NMAX 100000

static int last_id = -1;
static int count =-1;
static int i=-1;
static int k=0;
static real x0;
static real x1;
static real x2;
static real time_p[100];
static real time_pinit[100];
static real len[NMAX];
static int count1[100];
static int count_check=0;
static char so[100];
static char sn[100];
static int no_of_tries[100];

DEFINE_DPM_SCALAR_UPDATE(hy_dpm_store_path,c,t,in,p)
{
    real xn0,xn1,xn2;
    int id_n = p->part_id;

    sprintf(sn, p->injection->name);
    if(strcmp(sn,so)!=0 || k==0)
    {
        count1[k] = p->injection->n_particles;
        no_of_tries[k] = 1;
        if(p->injection->stochastic_p !=0)
        {
            no_of_tries[k] = p->injection->ntries;
        }
        k++;
    }

    if(strcmp(sn,so)!= 0 || id_n != last_id)
    {
        last_id=id_n;
        count = -1;
        i++;
    }
    count ++ ;
    if(count==0)
    {
        len[i]=0.0;
        time_pinit[i]=P_TIME(p);
        x0=P_POS(p)[0];
        x1=P_POS(p)[1];
        x2=P_POS(p)[2];
    }

    xn0=P_POS(p)[0];
    xn1=P_POS(p)[1];
    xn2=P_POS(p)[2];
    len[i] += sqrt(pow((xn0-x0),2)+pow((xn1-x1),2)+pow((xn2-x2),2));
    time_p[i]=P_TIME(p);
    x0=P_POS(p)[0];
    x1=P_POS(p)[1];
    x2=P_POS(p)[2];
    sprintf(so,sn);
}

DEFINE_ON_DEMAND(hy_dpm_exec)
{
    int j;
    int jc;
    int kc=0;
    int kc1=0;
    int counter=0;

    real avg[100];
    real sum[100];
    real avg_glob;
    real t_sum=0.0;
    real sum_time[100];

    for(jc=0;jc<k;jc++)
    {
        sum[jc]=0.0;
        sum_time[jc]=0.0;
        if(jc>0)
        {
            kc += count1[jc-1]* no_of_tries[jc-1];
        }
        kc1 += count1[jc] * no_of_tries[jc];
        for(j=kc;j<kc1;j++)
        {
            CX_Message("Path length for particle- %d in injection-%d is %f\n",j,jc,len[j]);
            sum[jc] += len[j];
            sum_time[jc] +=(time_p[j]-time_pinit[j]);
        }
        CX_Message("avg path length from injection-%d is %f\n",jc,sum[jc]/(count1[jc]*no_of_tries[jc]));
        CX_Message("avg res time from injection-%d is %f\n",jc,sum_time[jc]/(no_of_tries[jc]*count1[jc]));
    }
}


DEFINE_DPM_TIMESTEP(limit_to_e_minus_four, p, dt)
{
    if(dt>1.e-4)
    {
        /* p->next_time_step=1.e-4; */
        return 1.e-4;
    }
    return dt;
}

DEFINE_DPM_TIMESTEP(limit_to_fifth_of_prt, p, dt)
{
    real drag_factor = 0.;
    real p_relax_time;
    cphase_state_t *c= &(p->cphase);
    /* compute particle relaxation time */
    if(P_DIAM(p)!=0.0)
    {
        drag_factor = DragCoeff(p)*c->mu/(P_RHO(p)*P_DIAM(p)*P_DIAM(p));
    }
    else
    {
        drag_factor=1.;
    }
    p_relax_time=1./drag_factor;
    /* check the condition and return the time step */
    if(dt>p_relax_time/5.)
    {
        return p_relax_time/5.;
    }
    return dt;
}

/* reflect boundary condition for inert particles */

/* define a user-defined dpm boundary condition routine
 * bc_reflect: name
 * p:          the tracked particle
 * t:          the touched face thread
 * f:          the touched face
 * f_normal:   normal vector of touched face
 * dim:        dimension of the problem (2 in 2d and 2d-axi-swirl, 3 in 3d)
 *
 * return is the status of the particle, see enumeration of Path_Status
 * in dpm.h
 */

DEFINE_DPM_BC(hy_bc_reflect,p,t,f,f_normal,dim)
{
  real alpha;  /* angle of particle path with face normal */
  real vn=0.;
  real nor_coeff = 1.;
  real tan_coeff = 0.3;
  real normal[3];
  int i, idim = dim;
  real NV_VEC(x);

  real R;

#if RP_2D
    /* dim is always 2 in 2D compilation. Need special treatment for 2d
     axisymmetric and swirl flows */
    if (rp_axi_swirl)
    {
        R = sqrt(p->state.pos[1]*p->state.pos[1] +
                    p->state.pos[2]*p->state.pos[2]);
        if (R > 1.e-20)
        {
          idim = 3;
          normal[0] = f_normal[0];
          normal[1] = (f_normal[1]*p->state.pos[1])/R;
          normal[2] = (f_normal[1]*p->state.pos[2])/R;
        }
        else
        {
          for (i=0; i<idim; i++)
            normal[i] = f_normal[i];
        }
    }
    else
#endif
    for (i=0; i<idim; i++)
      normal[i] = f_normal[i];

    if(p->type==DPM_TYPE_INERT)
    {
        alpha = M_PI/2. - acos(MAX(-1.,MIN(1.,NV_DOT(normal,p->state.V)/
                                  MAX(NV_MAG(p->state.V),DPM_SMALL))));
        if ((NNULLP(t)) && (THREAD_TYPE(t) == THREAD_F_WALL))
            F_CENTROID(x,f,t);

        /* calculate the normal component, rescale its magnitude by
         the coefficient of restitution and subtract the change */

        /* Compute normal velocity. */
        for(i=0; i<idim; i++)
            vn += p->state.V[i]*normal[i];

        /* Subtract off normal velocity. */
        for(i=0; i<idim; i++)
            p->state.V[i] -= vn*normal[i];

        /* Apply tangential coefficient of restitution. */
        for(i=0; i<idim; i++)
            p->state.V[i] *= tan_coeff;

        /* Add reflected normal velocity. */
        for(i=0; i<idim; i++)
            p->state.V[i] -= nor_coeff*vn*normal[i];

        /* Store new velocity in state0 of particle */
        for(i=0; i<idim; i++)
            p->state0.V[i] = p->state.V[i];

        return PATH_ACTIVE;
    }

    return PATH_ABORT;
}


#define V_CROSS(a,b,r)\
        ((r)[0] = (a)[1]*(b)[2] - (b)[1]*(a)[2],\
         (r)[1] = (a)[2]*(b)[0] - (b)[2]*(a)[0],\
         (r)[2] = (a)[0]*(b)[1] - (b)[0]*(a)[1])

DEFINE_DPM_BC(hy_bc_wall_jet, p, thread, f, f_normal, dim)
{
  /*
     Routine implementing the Naber and Reitz Wall
     impingement model (SAE 880107)
  */

  real normal[3];
  real tan_1[3];
  real tan_2[3];
  real rel_vel[3];
  real face_vel[3];

  real alpha, beta, phi, cp, sp;
  real rel_dot_n, vmag, vnew, dum;
  real weber_in, weber_out;

  int i, idim = dim;

  cxboolean moving = (SV_ALLOCATED_P (thread,SV_WALL_GRID_V) &&
            SV_ALLOCATED_P (thread,SV_WALL_V     )   );

#if RP_2D
  if (rp_axi_swirl)
    {
      real R = sqrt(p->state.pos[1]*p->state.pos[1] +
            p->state.pos[2]*p->state.pos[2]);

      if (R > 1.e-20)
    {
      idim = 3;
      normal[0] = f_normal[0];
      normal[1] = (f_normal[1]*p->state.pos[1])/R;
      normal[2] = (f_normal[1]*p->state.pos[2])/R;
    }
      else
    {
      for (i=0; i<idim; i++)
        normal[i] = f_normal[i];
    }
    }
  else
#endif
    for (i=0; i<idim; i++)
      normal[i] = f_normal[i];

  /*
     Set up velocity vectors and calculate the Weber number
     to determine the regime.
  */

  for (i=0; i < idim; i++)
    {
      if (moving)
    face_vel[i] = WALL_F_VV(f,thread)[i] + WALL_F_GRID_VV(f,thread)[i];
      else
    face_vel[i] = 0.0;

      rel_vel[i] = P_VEL(p)[i] - face_vel[i];
    }

  vmag = MAX(NV_MAG(rel_vel),DPM_SMALL);

  rel_dot_n = MAX(NV_DOT(rel_vel,normal),DPM_SMALL);

  /* weber_in = P_RHO(p) * DPM_SQR(rel_dot_n) * P_DIAM(p) /
     MAX(DPM_SURFTEN(p), DPM_SMALL); */
  weber_in = P_RHO(p) * SQR(rel_dot_n) * P_DIAM(p) /
     MAX(DPM_SURFTEN(p), DPM_SMALL); 

  /*
     Regime where bouncing occurs (We_in < 80).
     (Data from Mundo, Sommerfeld and Tropea
      Int. J. of Multiphase Flow, v21, #2, pp151-173, 1995)
  */

  if (weber_in <= 80.)
    {
      weber_out = 0.6785*weber_in*exp(-0.04415*weber_in);
      vnew = rel_dot_n * (1.0 + sqrt( weber_out /
         MAX( weber_in, DPM_SMALL )));

      /*
     The normal component of the velocity is changed based
     on the experimental paper above (i.e. the Weber number
     is based on the relative velocity).
      */

      for (i=0; i < idim; i++)
    P_VEL(p)[i] = rel_vel[i] - vnew*normal[i] + face_vel[i];

    }

  if (weber_in > 80.)
    {
      alpha = acos(-rel_dot_n/vmag);

      /*
         Get one tangent vector by subtracting off the normal
         component from the impingement vector, then cross the
         normal with the tangent to get an out of plane vector.
      */

      for (i=0; i < idim; i++)
    tan_1[i] = rel_vel[i] - rel_dot_n*normal[i];

      UNIT_VECT(tan_1,tan_1);

      V_CROSS(tan_1,normal,tan_2);

      /*
         beta is calculated by neglecting the coth(alpha)
         term in the paper (it is approximately right).
      */

      beta = MAX(M_PI*sqrt(sin(alpha)/(1.0-sin(alpha))),DPM_SMALL);

      phi= -M_PI/beta*log(1.0-cheap_uniform_random()*(1.0-exp(-beta)));

      if (cheap_uniform_random() > 0.5)
    phi = -phi;

      vnew = vmag;

      cp = cos(phi);
      sp = sin(phi);

      for (i=0; i < idim; i++)
    P_VEL(p)[i] = vnew*(tan_1[i]*cp + tan_2[i]*sp) + face_vel[i];

    }

  /*
    Subtract off from the original state.
  */
  for (i=0; i < idim; i++)
    P_VEL0(p)[i] = P_VEL(p)[i];

  if ( DPM_STOCHASTIC_P(p->injection) )
    {

      /* Reflect turbulent fluctuations also */
      /* Compute normal velocity. */

      dum = 0;
      for(i=0; i<idim; i++)
    dum += p->V_prime[i]*normal[i];

      /* Subtract off normal velocity. */

      for(i=0; i<idim; i++)
    p->V_prime[i] -= 2.*dum*normal[i];
    }
  return PATH_ACTIVE;
}



/* UDF for computing the magnetic force on a charged particle */

#define Q 1.0         /* particle electric charge      */
#define BZ 3.0        /* z component of magnetic field */
#define TSTART 18.0   /* field applied at t = tstart   */

/* Calculate magnetic force on charged particle.  Magnetic   */
/* force is particle charge times cross product of particle  */
/* velocity with magnetic field: Fx= q*bz*Vy,  Fy= -q*bz*Vx  */

DEFINE_DPM_BODY_FORCE(particle_body_force, p, i)
{
        real bforce;
        if(P_TIME(p)>=TSTART)
          {
           if(i==0) bforce=Q*BZ*P_VEL(p)[1];

           else if(i==1) bforce=-Q*BZ*P_VEL(p)[0];

          }
        else
           bforce=0.0;
        /* an acceleration should be returned */
        return (bforce/P_MASS(p));
}


/***********************************************************************
   UDF for computing particle drag coefficient (18 Cd Re/24)
   curve as suggested by R. Clift, J. R. Grace and M.E. Weber
   "Bubbles, Drops, and Particles" (1978)
************************************************************************/

/*
    real Re
      particle Reynolds number based on the particle diameter and relative gas velocity.

    Tracked_Particle *p
      Pointer to the Tracked_Particle data structure which contains data related to
      the particle being tracked.
*/

DEFINE_DPM_DRAG(hy_particle_drag_force, Re, p)
{
    real w, drag_force;

    if (Re < 0.01)
    {
        drag_force=18.0;
    }
    else if (Re < 20.0)
    {
        w = log10(Re);
        drag_force = 18.0 + 2.367*pow(Re,0.82-0.05*w);
    }
    else
    /* Note: suggested valid range 20 < Re < 260 */
    {
        drag_force = 18.0 + 3.483*pow(Re,0.6305) ;
    }

    return drag_force;
}


/*********************************************************************
   UDF that specifies discrete phase materials
**********************************************************************/

DEFINE_DPM_PROPERTY(hy_coal_emissivity,c,t,p)
{
    real mp0= P_INIT_MASS(p);
    real mp = P_MASS(p);
    real vf, cf;

    /* get the material char and volatile fractions and store them */
    /* in vf and cf                                                */
    vf = DPM_VOLATILE_FRACTION(p);
    cf = DPM_CHAR_FRACTION(p);

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

DEFINE_DPM_PROPERTY(hy_coal_scattering,c,t,p)
{
    real mp0= P_INIT_MASS(p);
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

/**********************************************************************
   UDF that models a custom law for evaporation swelling of particles
***********************************************************************/

DEFINE_DPM_LAW(Evapor_Swelling_Law,p,ci)
{
    real swelling_coeff = 1.1;

    /* first, call standard evaporation routine to calculate
       the mass and heat transfer                            */
    VaporizationLaw(p);
    /* compute new particle diameter and density */
    P_DIAM(p) = P_INIT_DIAM(p)*(1. + (swelling_coeff - 1.)*
       (P_INIT_MASS(p)-P_MASS(p))/(DPM_VOLATILE_FRACTION(p)*P_INIT_MASS(p)));
    P_RHO(p) = P_MASS(p) / (3.14159*P_DIAM(p)*P_DIAM(p)*P_DIAM(p)/6);
    P_RHO(p) = MAX(0.1, MIN(1e5, P_RHO(p)));
}


/*********************************************************************
  UDF for computing the melting index along a particle trajectory
**********************************************************************/
static real viscosity_0;

DEFINE_INIT(melt_setup,domain)
{
        /* if memory for the particle variable titles has not been
         * allocated yet, do it now */

  if (NULLP(user_particle_vars)) Init_User_Particle_Vars();

         /* now set the name and label */

  strcpy(user_particle_vars[0].name,"melting-index");
  strcpy(user_particle_vars[0].label,"Melting Index");
}

        /* update the user scalar variables */

DEFINE_DPM_SCALAR_UPDATE(melting_index,cell,thread,initialize,p)
{
  cphase_state_t *c = &(p->cphase);
  if (initialize)
    {
     /* this is the initialization call, set:
      * p->user[0] contains the melting index, initialize to 0
      * viscosity_0 contains the viscosity at the start of a time step*/

      p->user[0] = 0.;
      viscosity_0 = c->mu;
    }

  else
    {
      /* use a trapezoidal rule to integrate the melting index */
      p->user[0] += P_DT(p) * .5 * (1/viscosity_0 + 1/c->mu);

      /* save current fluid viscosity for start of next step */
      viscosity_0 = c->mu;
    }
}

    /* write melting index when sorting particles at surfaces */
DEFINE_DPM_OUTPUT(melting_output,header,fp,p,thread,plane)
{
  char name[100];

  if (header)
   {
   if (NNULLP(thread))
     par_fprintf_head(fp,"(%s %d)\n",thread->head->
                     dpm_summary.sort_file_name,11);
   else
     par_fprintf_head(fp,"(%s %d)\n",plane->sort_file_name,11);
     par_fprintf_head(fp,"(%10s %10s  %10s  %10s  %10s  %10s  %10s"
               " %10s  %10s  %10s  %10s  %s)\n",
                 "X","Y","Z","U","V","W","diameter","T","mass-flow",
                 "time","melt-index","name");
    }

  else
    {
      sprintf(name,"%s:%d",p->injection->name,p->part_id);
#if PARALLEL
      /* add try_id and part_id for sorting in parallel */
      par_fprintf(fp,
           "%d %d ((%10.6g  %10.6g  %10.6g  %10.6g  %10.6g  %10.6g  "
           "%10.6g  %10.6g  %10.6g  %10.6g %10.6g) %s)\n",
           p->injection->try_id, p->part_id,
           p->state.pos[0], p->state.pos[1], p->state.pos[2],
           p->state.V[0], p->state.V[1], p->state.V[2],
           p->state.diam, p->state.temp, p->flow_rate, p->state.time,
           p->user[0], name);
#else
      par_fprintf(fp,
           "((%10.6g  %10.6g  %10.6g  %10.6g  %10.6g  %10.6g  "
           "%10.6g  %10.6g  %10.6g  %10.6g %10.6g) %s)\n",
           p->state.pos[0], p->state.pos[1], p->state.pos[2],
           p->state.V[0], p->state.V[1], p->state.V[2],
           p->state.diam, p->state.temp, p->flow_rate, p->state.time,
           p->user[0], name);
#endif
    }
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
#endif /*  P_CELL */

#ifndef P_CELL_THREAD
#define       P_CELL_THREAD(P) RP_THREAD(&((P)->cCell))
#endif /*  P_CELL_THREAD */

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
