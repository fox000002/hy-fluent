#include "udf.h"

/********************************************************************
  UDF for integrating turbulent dissipation and printing it to
  console window at the end of the current iteration or time step
*********************************************************************/
DEFINE_EXECUTE_AT_END(hy_execute_at_end)
{
    Domain *d;
    Thread *t;
    /* Integrate dissipation. */
    real sum_diss=0.;
    cell_t c;
    d = Get_Domain(1);   /* mixture domain if multiphase */

    thread_loop_c(t,d)
    {
        if (FLUID_THREAD_P(t))
        {
            begin_c_loop(c,t)
            {
                sum_diss += C_D(c,t) * C_VOLUME(c,t);
            }
            end_c_loop(c,t)
        }
    }

    CX_Message("Volume integral of turbulent dissipation: %g\n", sum_diss);
}

/********************************************************************/
static int version=1;
static int release=2;

DEFINE_EXECUTE_ON_LOADING(hy_report_lib_version,libname)
{
    CX_Message("\nLoading %s version %d.%d\n",libname,version,release);
}

/**********************************************************************
udm_res1.c contains two UDFs:an execute on loading UDF that reserves
three UDMsfor libudf and renames the UDMs to enhance postprocessing,
and an on-demand UDF that sets the initial value of the UDMs.
**********************************************************************/
#define NUM_UDM 3
static int udm_offset = UDM_UNRESERVED;

DEFINE_EXECUTE_ON_LOADING(hy_on_lib_loading,libname)
{
    if(udm_offset==UDM_UNRESERVED)
    {
        udm_offset = Reserve_User_Memory_Vars(NUM_UDM);
        if(udm_offset==UDM_UNRESERVED)
        {
            CX_Message("\nYou need to define up to %d extra UDMs in GUI and then reload current library: %s\n", NUM_UDM, libname);
        }
        else
        {
            CX_Message("%d UDMs have been  reserved by the current library %s\n", NUM_UDM, libname);
            Set_User_Memory_Name(udm_offset, "libhylab-UDM-0");
            Set_User_Memory_Name(udm_offset+1, "libhylab-UDM-1");
            Set_User_Memory_Name(udm_offset+2, "libhylab-UDM-2");
        }
        CX_Message("\nUDM Offset for Current Loaded Library=%d\n", udm_offset);
    }
}

DEFINE_ON_DEMAND(hy_set_lib_udms)
{
    Domain *d;
    Thread *ct;
    cell_t c;
    int i;

    /* root domain (fluid) */
    d = Get_Domain(1);

    if (udm_offset!=UDM_UNRESERVED)
    {
        CX_Message("Setting UDMs.\n");
        for(i=0; i<NUM_UDM; i++)
        {
            thread_loop_c(ct,d)
            {
                begin_c_loop(c,ct)
                {
                    C_UDMI(c,ct,udm_offset+i)=3.0+i/10.0;
                }
                end_c_loop(c,ct)
            }
        }
    }
    else
    {
        CX_Message("UDMs have not yet been reserved for library1\n");
    }
}
