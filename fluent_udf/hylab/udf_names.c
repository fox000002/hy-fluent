/* This file generated automatically. */
/*          Do not modify.            */
#include "udf.h"
#include "prop.h"
#include "dpm.h"
extern DEFINE_ON_DEMAND(test_avg_func);
extern DEFINE_ADJUST(do_every_timestep, domain);
extern DEFINE_PROPERTY(cell_density,c,t);
extern DEFINE_SOURCE(energy_loss, c, t, dS, eqn);
extern DEFINE_PROFILE(wall_rad_flux, t, i);
extern DEFINE_PROFILE(temp_free_stream, t, i);
extern DEFINE_DELTAT(mydeltat, d);
extern DEFINE_UDS_UNSTEADY(uns_time, c, t, i, apu, su);
UDF_Data udf_data[] = {
{"test_avg_func", (void (*)())test_avg_func, UDF_TYPE_ON_DEMAND},
{"do_every_timestep", (void (*)())do_every_timestep, UDF_TYPE_ADJUST},
{"cell_density", (void (*)())cell_density, UDF_TYPE_PROPERTY},
{"energy_loss", (void (*)())energy_loss, UDF_TYPE_SOURCE},
{"wall_rad_flux", (void (*)())wall_rad_flux, UDF_TYPE_PROFILE},
{"temp_free_stream", (void (*)())temp_free_stream, UDF_TYPE_PROFILE},
{"mydeltat", (void (*)())mydeltat, UDF_TYPE_DELTAT},
{"uns_time", (void (*)())uns_time, UDF_TYPE_UDS_UNSTEADY},
};
int n_udf_data = sizeof(udf_data)/sizeof(UDF_Data);
#include "version.h"
void UDF_Inquire_Release(int *major, int *minor, int *revision)
{
  *major = RampantReleaseMajor;
  *minor = RampantReleaseMinor;
  *revision = RampantReleaseRevision;
}
