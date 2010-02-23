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


