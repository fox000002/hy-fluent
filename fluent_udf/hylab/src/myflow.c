#include "udf.h"

extern FLUENT_EXPORT Domain *root_domain;

void hy_do_iteration(int nIter)
{
    if (!Case_Valid_P())
    {
        Error("Case is not vaild!\n");
    }

    if (!Data_Valid_P())
    {
        Error("Data is not vaild!\n");
    }


    Flow_Iterate(root_domain, nIter);
}


/* EOD for testing flow stuffs in fluent */
DEFINE_ON_DEMAND(hy_test_fluent_flow)
{
    /* residual */
    CX_Message("Default max residuals: %d\n", DEFAULT_MAX_RES);

    CX_Message("Current number of residuals: %d\n", nres);

    CX_Message("size of residual history buffer: %d\n", max_res);

    CX_Message("Count2: %f\n", count2[nres-1]);

    CX_Message("current_iter: %d\n", current_iter);

    CX_Message("step_iteration: %d\n", step_iteration);

    Set_Residual_History_Size(5000);

    hy_do_iteration(1);
}
