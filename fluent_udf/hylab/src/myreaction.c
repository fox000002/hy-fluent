#include "udf.h"

#define K1 2.0e-2
#define K2 5.

DEFINE_VR_RATE(hy_user_rate, c, t, r, mole_weight, species_mf, rate, rr_t)
{
    real s1 = species_mf[0];
    real mw1 = mole_weight[0];
    if(FLUID_THREAD_P(t) && THREAD_VAR(t).fluid.porous)
        *rate = K1*s1/pow((1.+K2*s1),2.0)/mw1;
    else
        *rate=0.;
    *rr_t = *rate;
}
