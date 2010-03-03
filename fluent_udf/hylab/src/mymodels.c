#include "udf.h"

#include "myudf.h"

int is_energy_on()
{
    return rf_energy;
}

int is_uds_on()
{
    return sg_uds;
}

int num_uds()
{
    return n_uds;
}

