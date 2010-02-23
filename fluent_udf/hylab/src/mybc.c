/*
* =====================================================================================
*
*       Filename:  mybc.c
*
*    Description:  UDF for B.C.
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

/**********************************************************************
UDF for specifying a steady-state velocity profile boundary condition
**********************************************************************/
DEFINE_PROFILE(x_velocity, thread, index)
{
    real x[ND_ND];        /* this will hold the position vector */
    real y;
    face_t f;

    begin_f_loop(f, thread)
      {
        F_CENTROID(x,f,thread);
        y = x[1];
        F_PROFILE(f,thread,index) = 20. - y*y/(.0745*.0745)*20.;
      }
    end_f_loop(f, thread)
}


/***********************************************************************
UDF for specifying steady-state parabolic pressure profile boundary
profile for a turbine vane
************************************************************************/
DEFINE_PROFILE(pressure_profile, t, i)
{
    real x[ND_ND];
    real y;
    face_t f;

    begin_f_loop(f, t)
      {
        F_CENTROID(x, f, t);
        y = x[1];
        F_PROFILE(f, t, i) = 1.1e5 - y * y /(.0745 * 0.0745) * 0.1e5;
      }
    end_f_loop(f, t)

}


