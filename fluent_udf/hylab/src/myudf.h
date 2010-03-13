#ifndef HU_MYUDF_H
#define  HU_MYUDF_H

#ifndef TRUE
# define TRUE (1)
#endif
#ifndef FALSE
# define FALSE (0)
#endif

void hy_report();

int is_energy_on();

int is_uds_on();

int num_uds();

int is_udm_on();

int num_udm();

int is_dpm_on();

/************************************/
int is_turb_on();
int is_ke_on();

#endif /*  HU_MYUDF_H */
