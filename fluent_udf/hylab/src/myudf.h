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

/************************************/
int is_seg_solver();

/*************************************/
double rp_get_var_real(char *name);
int rp_get_var_int(char *name);
char* rp_get_var_string(char *name);
int rp_get_var_boolean(char *name);

void rp_set_var_real(char *name, double v);
void rp_set_var_int(char *name, int v);
void rp_set_var_string(char *name, char *v);

#endif /*  HU_MYUDF_H */
