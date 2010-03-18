#include "udf.h"

DEFINE_ON_DEMAND(hy_test_myvector)
{
    real NV_VEC(a);
    int size;
    int i, j;
	real vec[ND_ND][ND_ND];

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

	for (i=0; i<ND_ND; ++i)
	{
		for (j=0; j<ND_ND; ++j)
		{
			vec[i][j] = i+j;
		}
	}

	for (i=0; i<ND_ND; ++i)
	{
		for (j=0; j<ND_ND; ++j)
		{
			CX_Message("%d %d : %f\n", i, j, vec[i][j]);
		}
	}

	for (i=0; i<ND_ND; ++i)
	{
		CX_Message("sum %d : %f\n", i, ND_SUM(vec[i][0], vec[i][1], vec[i][2]));
	}
	
	for (i=0; i<ND_ND; ++i)
	{
		ND_SET(vec[i][0], vec[i][1], vec[i][2], 2., 2., 2.);
	}

	for (i=0; i<ND_ND; ++i)
	{
		for (j=0; j<ND_ND; ++j)
		{
			CX_Message("%d %d : %f\n", i, j, vec[i][j]);
		}
	}

	CX_Message("Vector Magnitude: %f\n",  NV_MAG(vec[0]));
	CX_Message("Vector Magnitude ^2: %f\n",  NV_MAG2(vec[0]));
	CX_Message("Dot Product: %f\n", NV_DOT(vec[0], vec[1]));
}
