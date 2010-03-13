#include "udf.h"

#include "myudf.h"

/************************************************************************/
/*  EOD functions                                                       */
/************************************************************************/
DEFINE_ON_DEMAND(hy_test_myudf_func)
{
    hy_report();
}

/*======================================================================*/
typedef struct udf_lib_struct
{

  void *lib;
  char name[4096];

  UDF_Data *udf_data;
  int n_udf_data;

  UDF_Data *udf_util_data;
  int n_udf_util_data;

  struct udf_lib_struct *next;
} UDF_Lib;

#define UDF_NAME(u) ((u)->name)
#define UDF_DATA_LENGTH(u) ((u)->n_udf_data)
#define UDF_DATA(u) ((u)->udf_data)
#define UDF_LIBRARY(u) ((u)->lib)
#define UDF_NEXT_LIBRARY(u) ((u)->next)
#define UDF_UTIL_DATA(u) ((u)->udf_util_data)
#define UDF_UTIL_DATA_LENGTH(u) ((u)->n_udf_util_data)

extern FLUENT_EXPORT UDF_Lib *udf_list;

/*======================================================================*/

void hy_report()
{
    int major, minor, revision;
    int i;

    CX_Message("%s\n", Inquire_Fluent_Build_Time());

    Report_Build_Times();

    Inquire_Release(&major, &minor, &revision);

    CX_Message("Fluent Release : %d.%d.%d\n", major, minor, revision);

    if (udf_list)
    {
        CX_Message("UDF name: %s\n", udf_list->name);
        CX_Message("%30s%10s\n", "name", "type");
        for (i=0; i < udf_list->n_udf_data; ++i)
        {
            CX_Message("%30s%10d\n", udf_list->udf_data[i].name, udf_list->udf_data[i].type);
        }
    }
}

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

int is_udm_on()
{
    return sg_udm;
}

int num_udm()
{
    return n_udm;
}

int is_dpm_on()
{
    return sg_dpm;
}

int is_turb_on()
{
    return rp_turb;
}

int is_ke_on()
{
    return rp_ke;
}
