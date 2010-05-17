/*
* =====================================================================================
*
*       Filename:  mybc.c
*
*    Description:  UDF for B.C.
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

/**********************************************************************
UDF for specifying a steady-state velocity profile boundary condition
**********************************************************************/
DEFINE_PROFILE(hy_x_velocity, thread, index)
{
    real x[ND_ND];
    real y;
    face_t f;

    begin_f_loop(f, thread)
      {
        F_CENTROID(x,f,thread);
        y = x[1];
        F_PROFILE(f,thread,index) = 20. - y*y/(.0745*.0745)*20.;
      }
    end_f_loop(f, thread)
}


/***********************************************************************
UDF for specifying steady-state parabolic pressure profile boundary
profile for a turbine vane
************************************************************************/
DEFINE_PROFILE(hy_pressure_profile, t, i)
{
    real x[ND_ND];
    real y;
    face_t f;

    begin_f_loop(f, t)
      {
        F_CENTROID(x, f, t);
        y = x[1];
        F_PROFILE(f, t, i) = 1.1e5 - y * y /(.0745 * 0.0745) * 0.1e5;
      }
    end_f_loop(f, t)

}

/***********************************************************************
UDF for specifying boundary for regenerative cooling
************************************************************************/

/* Free Stream Temperature */
DEFINE_PROFILE(FST_profile, t, i)
{
  real x[ND_ND];
  real position;

  face_t f;
  begin_f_loop(f, t)
    {
      F_CENTROID(x,f,t);
      position = x[0];
      F_PROFILE(f, t, i) = 2500 - 2000 * position;
    }
  end_f_loop(f, t)
}


/* Heat Transfer Coefficient */
DEFINE_PROFILE(HTC_profile, t, i)
{
  real x[ND_ND] = {0};
  real pos_x;
  real pos_y;
  real pos_z;
  face_t f;
/*
  real Tf;
  real Tw;
*/
  real sigma= 1.0;
  real A_t = 1.0;
  real A = 5.0;

  /* throat radius */
  real y0 = 0.030;

  begin_f_loop(f, t)
    {
      F_CENTROID(x,f,t);
      pos_x = x[0];
      pos_y = x[1];
      pos_z = x[2];
/*
      Tf = 300 + 2 * position;
      Tw = F_T(f, t);
*/
      A = (pos_y * pos_y + pos_z * pos_z) / y0 / y0;

      F_PROFILE(f, t, i) = 8271.39 * pow(A_t/A, 0.9) * sigma;
    }
  end_f_loop(f, t)
}


#define TOTAL_SMP_POINTS 361

#ifndef TOTAL_SMP_POINTS
  #include "config.h"
#endif /* TOTAL_SMP_POINTS */

#define DX_SIZE TOTAL_SMP_POINTS

int g_nReadInput = 0;

real g_pos_x[DX_SIZE];
real g_Ma[DX_SIZE];
real g_Ts[DX_SIZE];
real g_Ps[DX_SIZE];
real g_Gamma[DX_SIZE];
real g_Mol_Weight[DX_SIZE];

int read_data(char *filename, real *ppos, real *pval, int size)
{
    FILE *fp = 0;
    int i;

    fp = fopen(filename, "r");

    if (!fp)
    {
        Error("Failed to open file : %s !!!", filename);
    }

    for (i=0; i<size; i++)
    {
        fscanf(fp, "%lf %lf", &ppos[i], &pval[i]);
    }
    fclose(fp);

    return 0;
}

int read_static_ts(real *ppos, real *pval, int size)
{
    read_data("temperature.dat", ppos, pval, size);
    return 0;
}

int read_static_ps(real *ppos, real *pval, int size)
{
    read_data("pressure.dat", ppos, pval, size);
    return 0;
}

int read_Ma_x(real *ppos, real *pval, int size)
{
    read_data("ma.dat", ppos, pval, size);
    return 0;
}

int read_gammna(real *ppos, real *pval, int size)
{
    read_data("gamma.dat", ppos, pval, size);
    return 0;
}

int read_mol_weight(real *ppos, real *pval, int size)
{
    read_data("molecular_weight.dat", ppos, pval, size);
    return 0;
}

void read_all()
{
    int i;

    read_static_ts(g_pos_x, g_Ts, DX_SIZE);
    read_static_ps(g_pos_x, g_Ps, DX_SIZE);
    read_Ma_x(g_pos_x, g_Ma, DX_SIZE);
    read_gammna(g_pos_x, g_Gamma, DX_SIZE);
    read_mol_weight(g_pos_x, g_Mol_Weight, DX_SIZE);

    for (i=0; i<DX_SIZE; i++)
    {
        CX_Message("%lf  %lf  %lf  %lf  %lf  %lf\n", g_pos_x[i], g_Ts[i], g_Ps[i], g_Ma[i], g_Gamma[i], g_Mol_Weight[i]);
    }

    /* fflush(stdout) */;
    /* return 0;*/
}


void write_all()
{
    int i;
    FILE *fp = 0;
/*
    read_static_tp(g_pos_x, g_Ts, DX_SIZE);
    read_Ma_x(g_pos_x, g_Ma, DX_SIZE);
*/

    fp = fopen("input.dat", "w");

    if (!fp)
    {
        Error("Failed to open file : %s !!!", "input.dat");
    }

    for (i=0; i<DX_SIZE; i++)
    {
        fprintf(fp, "%lf  %lf  %lf  %lf  %lf  %lf\n", g_pos_x[i], g_Ts[i], g_Ps[i], g_Ma[i], g_Gamma[i], g_Mol_Weight[i]);
    }
    fclose(fp);
    /*return 0;*/
}

real search_value(real *ppos, real *pval, int size, real pos)
{
    int i;
    real tp;
    real w;
    real o;

    tp  = pval[0];

    /* Out of bound */
    if (pos > ppos[size-1])
    {
        tp = pval[size-1];
        return tp;
    }

    for (i=0; i<size-1; i++)
    {
        if ((pos >= ppos[i]) && (pos <= ppos[i+1]))
        {
            w = ppos[i+1] - ppos[i];
            o = pos - ppos[i];
            tp = pval[i] * o / w + pval[i+1] * (w-o) / w;
        }
    }

    return tp;
}
real search_ts(real pos)
{
    return search_value(g_pos_x, g_Ts, DX_SIZE, pos);
}

real search_ma(real pos)
{
    return search_value(g_pos_x, g_Ma, DX_SIZE, pos);
}

/* Modified HTC */
DEFINE_PROFILE(HTC_modified, t, i)
{
    real x[ND_ND] = {0};
    real pos_x;
    real pos_y;
    real pos_z;

    real T_wg;
    real T_aw;

    real A;
    real A_t = 1.0;

    /* */
    real T_c_ns = 3500;
    real Ma_x = 1;

    /*  */
    real D_t = 0.060;
    real r_t = 0.030;
    real y0 = r_t;

    /* The curve radius of the throat */
    real R_1 = 0.045;
    real R_2 = 0.030;
    real R = (R_1 + R_2) / 2;

    /* */
    real sigma;
    real cxstar;

    /* */
    real g = 9.8;
    real gamma = 1.22;
    real molecular_weight = 22.5;
    real gas_constant = 8.314;

    /*  */
    real mu = 1184.1e-10 * pow(molecular_weight, 0.5) * pow(T_c_ns, 0.6);
    real p_c = 68 * 1.01325e5;
    real c_p = 1000/molecular_weight * gamma * gas_constant/ (gamma - 1);
    real Pr = 4 * gamma / (9 * gamma - 5);

    real h_g;

    face_t f;

    real tmpx;
    real tmpx2;
    real power_val = (gamma + 1) / (gamma-1);

    tmpx = gamma * sqrt( pow(2/(gamma+1), power_val));
    cxstar = sqrt( g * gamma * gas_constant * T_c_ns) / gamma / tmpx;

    /*cxstar = 1723;*/

    /* T_aw = (Tc)_ns * R */
    T_aw = T_c_ns * 0.9;


    /* Pr = 4 * gamma / (9 * gamma - 5); */

    if (g_nReadInput==0)
    {
      read_all();
      write_all();
      g_nReadInput = 1;
    }

    begin_f_loop(f, t)
    {
      F_CENTROID(x,f,t);
      pos_x = x[0];
      pos_y = x[1];
      pos_z = x[2];

      A = (pos_y * pos_y + pos_z * pos_z) / y0 / y0;

      /* interpolate */
      molecular_weight = search_value(g_pos_x, g_Mol_Weight, DX_SIZE, pos_x);
      T_c_ns = search_value(g_pos_x, g_Ts, DX_SIZE, pos_x);
      p_c = search_value(g_pos_x, g_Ps, DX_SIZE, pos_x);
      gamma = search_value(g_pos_x, g_Gamma, DX_SIZE, pos_x);
      Ma_x = search_value(g_pos_x, g_Ma, DX_SIZE, pos_x);

      /*  */
      T_wg = F_T(f, t);

      /* */
      tmpx = (1 + (gamma - 1)/2 * Ma_x * Ma_x);
      tmpx2 = T_wg/T_c_ns/2 * tmpx + 0.5;
      sigma = 1/ pow(tmpx2, 0.68) / pow(tmpx, 0.12);


      /*  */
      mu = 1184.1e-10 * pow(molecular_weight, 0.5) * pow(T_c_ns, 0.6);
      Pr = 4 * gamma / (9 * gamma - 5);

      c_p = 1000/molecular_weight * gamma * gas_constant/ (gamma - 1);

      power_val = (gamma + 1) / (gamma-1);
      tmpx = gamma * sqrt( pow(2/(gamma+1), power_val));
      cxstar = sqrt( g * gamma * gas_constant * T_c_ns) / gamma / tmpx;

      /*  */
      h_g  = 0.0043 / pow(D_t, 0.2);
      h_g *= pow(mu, 0.2) * c_p / pow(Pr, 0.6);
      h_g *= pow(p_c * g / cxstar, 0.8);
      h_g *= pow(D_t/R, 0.1);

      F_PROFILE(f, t, i) =  h_g * pow(A_t/A, 0.9) * sigma;

    }
    end_f_loop(f, t)
}

/* Modified Free Stream Temperature */
/* T_{aw} */
#define RESTITUTION_COEFF 0.9 /* restitution coefficient */

DEFINE_PROFILE(FST_modify, t, i)
{
  real x[ND_ND];
  real position;
  real tc; /* (tc)_{ns} */

  face_t f;

  if (g_nReadInput==0)
  {
      read_all();
      write_all();
      g_nReadInput = 1;
  }

  begin_f_loop(f, t)
    {
      F_CENTROID(x,f,t);
      position = x[0];
      /* tc = 2500 - 2000 * position; */
      /* tc = 3500; */
      tc = search_ts(position);
      F_PROFILE(f, t, i) = RESTITUTION_COEFF * tc;
    }
  end_f_loop(f, t)
}


DEFINE_PROFILE(FST_r_mod, t, i)
{
  /*  */
  real x[ND_ND];
  real pos_x;
  face_t f;
  /*  */
  real tc; /* (tc)_{ns} */

  /*    */
  real rc;
  real erc;

  /*  */
  real Ma_x;
  real gamma;
  real tmp;

  real Pr;


  if (g_nReadInput==0)
  {
      read_all();
      write_all();
      g_nReadInput = 1;
  }

  begin_f_loop(f, t)
    {
      F_CENTROID(x,f,t);
      pos_x = x[0];
      /* tc = 2500 - 2000 * position; */
      /* tc = 2500 - 2000 * position; */
      /* tc = 3500; */
      tc = search_ts(pos_x);
      Ma_x = search_value(g_pos_x, g_Ma, DX_SIZE, pos_x);
      gamma = search_value(g_pos_x, g_Gamma, DX_SIZE, pos_x);

      tmp = (gamma-1)/2 * Ma_x * Ma_x;

      Pr = 4 * gamma / (9 * gamma - 5);

      rc = pow(Pr, 0.33);
      erc = (1 + rc * tmp)/(1+tmp);


      F_PROFILE(f, t, i) = erc * tc;
    }
  end_f_loop(f, t)
}

/* Wall Roughness */
DEFINE_PROFILE(hy_wall_roughness, t, i)
{
    /*  */
  real x[ND_ND];
  real pos_x;
  face_t f;

  begin_f_loop(f, t)
  {
      F_CENTROID(x,f,t);
      pos_x = x[0];

      F_PROFILE(f, t, i) = 0.001;
  }
  end_f_loop(f, t)
}

