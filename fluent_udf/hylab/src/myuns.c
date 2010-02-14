#include "udf.h"

static int last_ts = -1;   /*  Global variable.  Time step is never <0 */


/*    */
real get_face_avg_temp(int did)
{
	real face_t_avg = 0.0f;
    real temperature = 0.0f;
	real area  = 0.0f;
	int num = 0;
	int thread_ID = 8;
	//
	face_t f;
	Thread *t;
	int n;
	Node *node;
	Domain *domain;
	//
	real NV_VEC(A);
	real x[ND_ND];

	domain = Get_Domain(did);  /* returns fluid domain pointer */
	t = Lookup_Thread(domain, thread_ID);

	//CX_Message("There are %d nodes in Face.\n", F_NNODES(f,t));

	
	//area = A[0];

	begin_f_loop(f, t)    /* loops over faces in a face thread  */
	{
		F_CENTROID(x,f,t);
		F_AREA(A, f, t);
#ifdef DEBUG
		CX_Message("area : %f\n", -A[1]);
		CX_Message("temperature : %f  %f\n", x[0], F_T(f, t));
#endif // DEBUG
		temperature -= F_T(f,t) * A[1];
		area -= A[1];
	}                         
	end_f_loop(f, t)
	
	face_t_avg  = temperature / area;
	//
	CX_Message("Total area of Face %d : %f\n", THREAD_ID(t), area);
	CX_Message("The average temperature of the Face : %f\n", face_t_avg);
    
    return face_t_avg;
}


/*    */

DEFINE_ADJUST(do_every_timestep, domain)
{
    int curr_ts;
    curr_ts = RP_Get_Integer("time-step");
    if (last_ts != curr_ts)
    {
        last_ts = curr_ts;

        /* things to be done only on first iteration of each time step */
        /* can be put here  */
        
        /* adjust rho */
        
        
        /* report current mass & rho */
        
        /*             */
        
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
  
  /* Get DT */
  C_CENTROID(x, c, t);
  
  dt = C_T(c,t) - C_T_M1(c,t);
  
  dm = - beta_t * dt;

  source = dm * latent_heat_mat;
  dS[eqn] = 0.0;

  return source;
}

