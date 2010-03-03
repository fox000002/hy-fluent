#include "udf.h"

/******************************************************************
User-defined wall functions: separated into turbulent and
laminar regimes
******************************************************************/

DEFINE_WALL_FUNCTIONS(user_log_law, f, t, c0, t0, wf_ret, yPlus, Emod)
{
  real wf_value;

  switch (wf_ret)
    {
    case UPLUS_LAM:
      wf_value = yPlus;
      break;
    case UPLUS_TRB:
      wf_value = log(Emod*yPlus)/KAPPA;
      break;
    case DUPLUS_LAM:
      wf_value = 1.0;
      break;
    case DUPLUS_TRB:
      wf_value = 1./(KAPPA*yPlus);
      break;
    case D2UPLUS_TRB:
      wf_value = -1./(KAPPA*yPlus*yPlus);
      break;
    default:
    printf("Wall function return value unavailable\n");
    }
  return wf_value;
}

