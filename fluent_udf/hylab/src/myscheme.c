#include "udf.h"

#include "myscheme.h"
#include "myudf.h"

double rp_get_var_real(char *name)
{
    return RP_Get_Real(name);
}

int rp_get_var_int(char *name)
{
    return RP_Get_Integer(name);
}

char* rp_get_var_string(char *name)
{
    return RP_Get_String(name);
}

int rp_get_var_boolean(char *name)
{
    return RP_Get_Boolean(name);
}

void rp_set_var_real(char *name, double v)
{
    RP_Set_Real(name, v);
}

void rp_set_var_int(char *name, int v)
{
    RP_Set_Integer(name, v);
}

void rp_set_var_string(char *name, char *v)
{
    RP_Set_String(name, v);
}
