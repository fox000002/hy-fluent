#include "udf.h"

/********************************************************************
  UDF for integrating turbulent dissipation and printing it to     
  console window at the end of the current iteration or time step  
*********************************************************************/
DEFINE_EXECUTE_AT_END(execute_at_end)
{
	Domain *d;
	Thread *t;
	/* Integrate dissipation. */
	real sum_diss=0.;
	cell_t c;
	d = Get_Domain(1);   /* mixture domain if multiphase */
	
	thread_loop_c(t,d)
    {
		if (FLUID_THREAD_P(t))
		{
			begin_c_loop(c,t)
			{
				sum_diss += C_D(c,t) * C_VOLUME(c,t); 
			}
			end_c_loop(c,t)
		}
    }
	
	CX_Message("Volume integral of turbulent dissipation: %g\n", sum_diss);
}
