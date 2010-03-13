#include "udf.h"

DEFINE_ON_DEMAND(hy_test_myvector)
{
    real NV_VEC(a);
    int size;
    int i;
    size = sizeof(a)/sizeof(real);

    CX_Message("sizeof a : %d\n", sizeof(a));
    CX_Message("Number of element : %d\n", size);

    NV_S(a, =, 1.);

    for (i=0; i<size; ++i)
    {
        CX_Message("%d : %f\n", i, a[i]);
    }

    ND_S(a[0], a[1], a[2], =, 0.1);
    CX_Message("value : x(%f), y(%f)\n", a[0], a[1]);

}
