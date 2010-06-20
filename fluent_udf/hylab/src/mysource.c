/*
* =====================================================================================
*
*       Filename:  mysource.c
*
*    Description:  UDF for source term
*
*        Version:  1.0
*        Created:  2010-2-23 12:01:23
*       Revision:  none
*       Compiler:  gcc/msc
*
*         Author:  huys (hys), huys03@gmail.com
*        Company:  hu
*
* =====================================================================================
*/

#include "udf.h"
#include "surf.h"

typedef int boolean;

#define SCOLD -940600.0

DEFINE_SOURCE(cell_cold, cell, thread, dS, eqn)
{
    real NV_VEC(origin), NV_VEC(axis);
    real NV_VEC(V), NV_VEC(r), NV_VEC(R), NV_VEC(B);
    real NV_VEC(er), NV_VEC(et);
    real xc[ND_ND];
    real Bmag, rmag, source;
    real ua, ur, ut;

    /* Get origin vector of fluid region */
    NV_V (origin, =, THREAD_VAR(thread).cell.origin);

    /* Get axis of fluid region */
    NV_V (axis, =, THREAD_VAR(thread).cell.axis);

    /* Store the 3 cartesian velocity components in vector V */
    N3V_D(V, =, C_U(cell,thread), C_V(cell,thread), C_W(cell,thread));

    /* Get current cell coordinate */
    C_CENTROID(xc,cell,thread);

    /* Calculate (R) = (Xc)-(Origin) */
    NV_VV(R, =, xc, -, origin);

    /* Calculate |B| = (R) dot (axis)*/
    Bmag = NV_DOT(R,axis);

    /* Calculate (B) = |B|*axis */
    NV_VS(B,=,axis,*,Bmag);

    /* Calculate (r) = (R)-(B) This is the local radial vector*/
    NV_VV(r, =, R, -, B);

    /* Calculate |r|*/
    rmag = NV_MAG(r);

    if (rmag != 0.)
    {
        NV_VS (er,=,r,/,rmag);
        NV_CROSS(et, axis, er);
        ur = NV_DOT(V,er);
        ut = NV_DOT(V,et);
        ua = NV_DOT(V,axis);
    }
    else
    {
        ur = 0.0;
        ut = 0.0;
        ua = NV_DOT(V,axis);
    }

    /*source term */
    source = SCOLD * ur;

    /* derivative of source term w.r.t. enthalpy */
    dS[eqn] = 0.0;

    return source;
}


#define MAXPOINTS 20000
#define UDM_SOURCE 0

static boolean check(void);

static int np = 0;
real coordinates[MAXPOINTS][ND_ND] = {{0.}};


struct interpolation_point{
    cell_t c;
    Thread* t;
};

struct interpolation_point point_list[MAXPOINTS];


boolean check(void)
{
    if (sg_udm < 1)
    {
        Error("Error: You must define at least 1 UDM\n");
        return FALSE;
    }

    if (!Data_Valid_P())
    {
        Error("Error: You must initialize the data field\n");
        return FALSE;
    }

    return TRUE;
}


DEFINE_ON_DEMAND(hy_mark_reset)
{
    Domain *d = Get_Domain(1);
    Thread *t;
    cell_t c;

    if (!check()) return;

    thread_loop_c(t,d)
    {
        begin_c_loop(c,t)
        {
            C_UDMI(c, t, UDM_SOURCE) = 0.0;
        }
        end_c_loop(c,t);
    }
}

DEFINE_ON_DEMAND(hy_mark)
{
#if !RP_HOST
    Domain *d = Get_Domain(1);
    cell_t c;
    CX_Cell_Id cx_cell;
    Thread* t;
    int points_found = 0, total_points_found=0;
#endif

#if !RP_NODE
    FILE* input;
#endif
    int n;


    /* Check for UDM */
    if (!check()) return;

    /* READ COORDINATES */
#if !RP_NODE

    /* Open input file */
    if (!(input = fopen("interp-input.dat","r")))
    {
        Message0("\nWarning: Could not open interpolation input file...\n");
        return;
    }

    /* Initialize */
    for(n=0; n<MAXPOINTS; n++)
    {
        point_list[n].c = 0;
        point_list[n].t = NULL;
    }

    /* Read points from input file */
    n = -1;
    while (!feof(input))
    {
        n++;
#if RP_DOUBLE
#if RP_3D
        fscanf(input,"%lg %lg %lg", &coordinates[n][0], &coordinates[n][1], &coordinates[n][2])
        ;
#else
        fscanf(input,"%lg %lg", &coordinates[n][0], &coordinates[n][1]);
#endif
#else
#if RP_3D
        fscanf(input,"%g %g %g", &coordinates[n][0], &coordinates[n][1], &coordinates[n][2])
        ;
#else
        fscanf(input,"%g %g", &coordinates[n][0], &coordinates[n][1]);
#endif
#endif

    }
    np = n;


    /* Check np with MAXPOINTS */
    if (np > MAXPOINTS)
    {
        Message0("ERROR: You must recompile interpolate UDF with MAXPOINTS at least %i\n", np)
        ;
        return;
    }

    /* Close input file */
    fclose(input);

#endif

    /* FIND COORDINATES IN CELLS */

    /* Pass coordinates and np to the compute nodes */
    host_to_node_real(&coordinates[0][0],ND_ND*MAXPOINTS);
    host_to_node_int_1(np);

#if !RP_HOST

    /* Do computations */
    for(n=0;n<np;n++)
    {
        thread_loop_c(t,d)
        {
            begin_c_loop_int(c,t)
            {
                if (SV_is_point_in_cell(&cx_cell, c, t, coordinates[n]))
                {
                    point_list[n].c = RP_CELL(&cx_cell);
                    point_list[n].t = RP_THREAD(&cx_cell);

                    /* SET UDM HERE */
                    C_UDMI(point_list[n].c, point_list[n].t, UDM_SOURCE) = 1.0;

                    points_found++;
                    goto label;
                }
            }
            end_c_loop_int(c,t);
        }
label: continue;
    }

    total_points_found += points_found;

#if PARALLEL
    total_points_found = PRF_GISUM1(total_points_found);
#endif


    /* PRINT MESSAGE */
    if (np != total_points_found)
    Message0("\n\n Warning.... %i points found in domain out of %i points in input file\n",
    total_points_found, np);
    else
    Message0("\n\n Interpolate... all %i points located!\n", np);

#endif

}


/* Volume Heat Sink */
DEFINE_SOURCE(hy_mass_source, c, t, dS, eqn)
{
    real source = 0.0;
    real mdot = 1.0;
    source = (C_UDMI(c,t,UDM_SOURCE) == 1.0) ? (mdot/C_VOLUME(c,t)) : 0.0;
    dS[eqn] = 0.0;
    return source;
}

/* Volume Heat Sink */
#define TOTAL_VOL 1.507388
#define TOTAL_Q   56000

DEFINE_SOURCE(hy_vol_heat_sink, c, t, dS, eqn)
{
    real source;

    source = - TOTAL_Q * C_VOLUME(c,t) / TOTAL_VOL;

    dS[eqn] = 0.0;

    return source;
}
