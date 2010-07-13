/*
 * =====================================================================================
 *
 *       Filename:  myudm.c
 *
 *    Description:  UDF for UDM
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

DEFINE_ON_DEMAND(hy_my_udm)
{
    Domain * domain;
    cell_t c;
    Thread *t;

    real phi;

    if (!is_udm_on())
    {
        Error("You shoud set UDM number before execute this function!");
    }

    domain = Get_Domain(1);

    thread_loop_c(t,domain)
    {
        /* if (FLUID_THREAD_P(t)) */
        {
        begin_c_loop(c,t)
        {
            phi = 8 * C_YI(c,t,0) / C_YI(c,t,1);

            if (phi >= 1.)
            {
                C_UDMI(c,t,0) = 1./phi;
            }
            else
            {
                C_UDMI(c,t,0) = 1;
            }
        }
        end_c_loop(c,t)

        }
    }

}
