/*
 * =====================================================================================
 *
 *       Filename:  myreaction.c
 *
 *    Description:  UDF for reaction
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

/**
 DEFINE_VR_RATE:
   specify a custom volumetric reaction rate for a single reaction
   or for multiple reactions.
   Parameters:
     symbol name  --- UDF name
     cell_t c     --- Cell index
     Thread *t    --- Pointer to cell thread
     Reaction *r  --- Pointer to data structure that represents the
                      current reaction
     real *mw     --- Pointer to array of species molecular weights
     real *yi     --- Pointer to array of the species mass fractions
     real *rr     --- Pointer to laminar reaction rate
     real *rr_t   --- Pointer to turbulent reaction rate
 */

#define K1 2.0e-2
#define K2 5.

DEFINE_VR_RATE(hy_user_rate, c, t, r, mole_weight, species_mf, rate, rr_t)
{
    real s1 = species_mf[0];
    real mw1 = mole_weight[0];
    if(FLUID_THREAD_P(t) && THREAD_VAR(t).fluid.porous)
    {
        *rate = K1*s1/pow((1.+K2*s1),2.0)/mw1;
    }
    else
    {
        *rate=0.;
    }
    *rr_t = *rate;
}


/*********************************************************************
  UDF for specifying a volume reaction rate
  The basics of Fluent's calculation of reaction rates: only an
  Arrhenius("finite rate") reaction rate is calculated
  from the inputs given by the user in the graphical user interface
**********************************************************************/
DEFINE_VR_RATE(vol_reac_rate,c,t,r,wk,yk,rate,rr_t)
{
    real ci, prod;
    int i;
    /*Calculate Arrhenius reaction rate*/
    prod=1.;
    for(i=0; i<r->n_reactants; i++)
    {
        ci = C_R(c,t)*yk[r->reactant[i]]/wk[r->reactant[i]];
        prod *= pow(ci, r->exp_reactant[i]);
    }
    *rate=r->A*exp(-r->E/(UNIVERSAL_GAS_CONSTANT*C_T(c,t)))*pow(C_T(c,t),r->b)*prod;
    *rr_t=*rate;
    /*No"return..;"value.*/
}
