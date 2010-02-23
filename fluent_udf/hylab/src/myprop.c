/*
 * =====================================================================================
 *
 *       Filename:  myprop.c
 *
 *    Description:  UDF for Material Properties
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

/*********************************************************************
 UDF that specifies a custom mass-fraction weighted conductivity   
**********************************************************************/
DEFINE_PROPERTY(mass_wtd_k, c, t)
{
    real sum = 0.;
    int i;
    Material *sp;
    real ktc;
    Property *prop;

    mixture_species_loop(THREAD_MATERIAL(t), sp, i)
      {
        prop = (MATERIAL_PROPERTY(sp));
        ktc = generic_property(c, t, prop, PROP_ktc, C_T(c, t));
        sum += C_YI(c, t, i) * ktc;
      }

    return sum;
}
