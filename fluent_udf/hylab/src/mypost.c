#include "udf.h"

void hy_report_temp_diff_shadow(int wall_id)
{
    /* Domain *d = Get_Domain(1); */
    Domain *d = root_domain;
    Thread *wall_t = Lookup_Thread(d, wall_id); /* get the wall thread*/
    Thread *wall_shadow_t = THREAD_SHADOW(wall_t); /*get the wall shadow thread */

    Thread *wall_cell_t;
    Thread *shadow_cell_t;
    cell_t wall_c;
    cell_t shadow_c;
    face_t f;
    face_t f_shadow;
    real temp_diff;

    begin_f_loop(f,wall_t) /* loop over all the faces of the wall */
    {
        wall_cell_t = THREAD_T0(wall_t); /* get the cell zone in the wall side */
        wall_c = F_C0(f,wall_t);

        f_shadow = F_SHADOW(f,wall_t); /*get the corresponding face in the shadow thread */

        shadow_cell_t = THREAD_T0(wall_shadow_t); /*get the cell zone in the shadow side */
        shadow_c = F_C0(f_shadow,wall_shadow_t);

        temp_diff = C_T(wall_c, wall_cell_t) - C_T(shadow_c, shadow_cell_t);
        CX_Message("Temp Diff = %g\n",temp_diff);
    }
    end_f_loop(f,wall_t)
}

#define axi 0 /* the Flag for an axisymmetric case */

void hy_report_mass_flow_and_tmax(int zoneID)
{
    cell_t c0;
    cell_t c1;
    Thread *tc0;
    Thread *tc1;
    Thread *tf;
    face_t f;

    real MFR;
    real sumMFR;
    real area;
    real temp0;
    real temp;
    real rho;

    real face_area[ND_ND];
    real temp_max;
    real uvel,vvel,wvel;

    /* Domain *domain=Get_Domain(1);*/
    tf = Lookup_Thread(root_domain, zoneID);

    sumMFR = 0.0;
    temp_max =0.0;

    begin_f_loop(f,tf)
    {
        c0 = F_C0(f,tf);
        tc0 = F_C0_THREAD(f,tf);

        c1 = F_C1(f,tf);
        tc1 = F_C1_THREAD(f,tf);

        F_AREA(face_area, f, tf);
        area = NV_MAG(face_area);

        temp = 0.5*( C_T(c0,tc0) + C_T(c1,tc1));

        if (temp > temp_max) temp_max = temp;

        /* check for SOLVER */
        if (is_seg_solver())
        {
            MFR = F_FLUX(f,tf);
        }
        else
        {
            uvel = 0.5*( C_U(c0,tc0) + C_U(c1,tc1));
            vvel = 0.5*( C_V(c0,tc0) + C_V(c1,tc1));
            wvel = 0.5*( C_W(c0,tc0) + C_W(c1,tc1));
            rho = 0.5*(C_R(c0,tc0) + C_R(c1,tc1));

            /* MFR = C_R(c0,tc0)*( uvel*face_area[0] + vvel*face_area[1]+ wvel*face_area[2]); */
            MFR = C_R(c0, tc0) * NVD_DOT(face_area, uvel, vvel, wvel);
        }

        if (axi==1)
        {
            MFR = 2*M_PI*MFR;
        }

        sumMFR += MFR;
    }
    end_f_loop(f,tf)

    MFR = sumMFR;

    CX_Message(" The mass flow rate through the face=%lf\n",MFR);
    CX_Message(" The max temperature on the face is =%lf\n",temp_max);

}
