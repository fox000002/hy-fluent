#include "udf.h"

#include "sg_mphase.h"  /* for CVOF(C,T)*/

/*==============================================================*/
/* water vaporition */
#define T_SAT 373.15 /* 定义蒸发温度100℃*/
#define LAT_HT 1.e3  /* 定义蒸发潜热J/Kg*/

DEFINE_SOURCE(liq_src, cell, pri_th, dS, eqn) /* 液相质量源项UDF */
{
  Thread *mix_th, *sec_th;  /* 定义计算区域线指针*/
  real m_dot_l; /* 定义液相质量转移 kg/(m2.s) */

  mix_th = THREAD_SUPER_THREAD(pri_th);  /* 指向混合区的主相即液相的指针 */
  sec_th = THREAD_SUB_THREAD(mix_th, 1); /*指向单相控制区的气相的指针，气相为第二相*/

  if(C_T(cell, mix_th)>=T_SAT)  /* 如果液相单元的温度高于蒸发温度，液相向气相的质量质量转移*/
  {
    m_dot_l = -0.1*C_VOF(cell, pri_th)*C_R(cell, pri_th)*fabs(C_T(cell, mix_th) - T_SAT)/T_SAT;

    dS[eqn] = -0.1*C_R(cell, pri_th)*fabs(C_T(cell, mix_th) - T_SAT)/T_SAT; /*  定义源项对质量转移偏导 */
  }

  else
  {
    m_dot_l = 0.1*C_VOF(cell, sec_th)*C_R(cell, sec_th) * fabs(T_SAT-C_T(cell,mix_th))/T_SAT;

    /* 如果指向混合区液相的单元温度小于蒸发温度，气相向液相的质量转移，液相得*/
    dS[eqn] = 0.;  /* 由于是气相向液相转移，所以液相的质量源项对质量转移的偏导为零*/
  }
  return m_dot_l;
}

DEFINE_SOURCE(vap_src, cell, sec_th, dS, eqn) /* 气相质量源项UDF */

{

  Thread * mix_th, *pri_th;

  real m_dot_v;

  mix_th = THREAD_SUPER_THREAD(sec_th); /* 指向混合区的第二相即气相的指针 */

  pri_th = THREAD_SUB_THREAD(mix_th, 0); /* 指向单相控制区的液相的指针，液相为主相 */

  if(C_T(cell, mix_th)>=T_SAT) /* 如果混合区单元的温度高于蒸发温度，液相向气相的质量质量转移 */

{

    m_dot_v = 0.1*C_VOF(cell, pri_th)*C_R(cell, pri_th)*

                fabs(C_T(cell, mix_th) - T_SAT)/T_SAT;

    dS[eqn] = 0.; /* 由于是液相向气相转移，所以气相的质量源项对来自液相的质量转移的偏导为零 */

   }

  else {

    m_dot_v = -0.1*C_VOF(cell, sec_th)*C_R(cell, sec_th)*

                fabs(T_SAT-C_T(cell,mix_th))/T_SAT;

                    /* 如果指向混合区的单元温度小于蒸发温度，气相向液相的质量转移 */

    dS[eqn] = -0.1*C_R(cell, sec_th)* fabs(C_T(cell, mix_th) - T_SAT)/T_SAT;

                     /* 由于是气相向液相转移，所以气相的质量源项对自身的质量转移的偏导不为零 */

}

  return m_dot_v;

}

DEFINE_SOURCE(enrg_src, cell, mix_th, dS, eqn) /* 混合模型能量源项UDF */

{

  Thread *pri_th, *sec_th;

  real m_dot;

  pri_th = THREAD_SUB_THREAD(mix_th, 0);/* 指向混合区的液相的指针 */

  sec_th = THREAD_SUB_THREAD(mix_th, 1); /* 指向混合区的气相的指针 */

  if(C_T(cell, mix_th)>=T_SAT) /* 如果混合区的单元温度高于蒸发温度。质量转移由液相向气相转移，吸 热，质量转移量前有负号 */

{

    m_dot = -0.1*C_VOF(cell, pri_th)*C_R(cell, pri_th)*

                fabs(C_T(cell, mix_th) - T_SAT)/T_SAT;

    dS[eqn] =  -0.1*C_VOF(cell, pri_th)*C_R(cell, pri_th)/T_SAT;

   }

  else {                              /*相反，气相向液相转移则放热 */

    m_dot = 0.1*C_VOF(cell, sec_th)*C_R(cell, sec_th)*

                fabs(T_SAT-C_T(cell,mix_th))/T_SAT;

    dS[eqn] = 0.1*C_VOF(cell, sec_th)*C_R(cell, sec_th)/T_SAT;}

  return LAT_HT*m_dot;  /*气化潜热与质量转移率相乘得能量源项W/m3 */

}




int get_species1(Domain *d)
{
    Material *mix = mixture_material(d);
    Material *sp;
    int i;
    int n_species;
    real mw[MAX_SPE_EQNS];
    char *sp_name;

    CX_Message("\nIndex\tName\tMW\n");
    n_species = 0; /* reset species counter */

    /* loop over species in mixture */
    mixture_species_loop(mix, sp, i)
    {
        /* get name of species i */
        sp_name = MIXTURE_SITE_SPECIE_NAME(mix, i);
        /* get species molecular weight */
        mw[i] = MATERIAL_PROP(sp, PROP_mwi);
        /* display species data */
        CX_Message("%d\t%s\t%f\n", i, sp_name, mw[i]);
        /* update species counter */
        ++n_species;
    }
    return n_species;
}

int get_species2(Domain *d)
{
    Material *mix = mixture_material(d);
    Material *sp;
    int i;
    int n_species;
    real hf[MAX_SPE_EQNS];
    char *sp_name;

    CX_Message("\nIndex\tName\th_form\n");
    /* get total number of species in mixture */
    n_species = MIXTURE_NSPECIES(mix);

    /* loop over species from 0 to n_species-1 */
    spe_loop(i, n_species)
    {
        /* get species material pointer */
        sp = MIXTURE_SPECIE(mix, i);
        /* get name of species i */
        sp_name = MIXTURE_SITE_SPECIE_NAME(mix, i);

        /* get species hentalpy of formation */
        hf[i] = MATERIAL_PROP(sp, PROP_hform);
        /* display species data */
        CX_Message("%d\t%s\t%f\n", i, sp_name, hf[i]);
    }
    return n_species;
}

int find_species(Domain *d, char * sp_name)
{
    Material *mix = mixture_material(d);
    int sp_index;

    sp_index = mixture_specie_index(mix, sp_name);

    return sp_index;
}

int get_reactions(Domain *d)
{
    Material *mix = mixture_material(d);
    Reaction *r_list = mix->reaction_list;
    Reaction *r;
    Material *sp;
    int n_reactions;
    int n_r;
    int n_p;
    int i;
    int j;
    int m;
    char *r_name;
    char *sp_name;
    real mw;

    /* loop over reactions in the reaction list */
    n_reactions = 0;
    loop(r, r_list)
    {
        r_name = r->name;
        n_r = r->n_reactants;
        n_p = r->n_products;

        /* display reaction data */
        CX_Message("\nReaction name: %s\n", r_name);
        CX_Message("Reactions:\n");
        for (i=0; i<n_r; ++i)
        {
            sp = r->mat_reactant[i];
            j = r->reactant[i];
            m = r->stoich_reactant[i];
            sp_name = MIXTURE_SITE_SPECIE_NAME(mix, j);
            mw = MATERIAL_PROP(sp, PROP_mwi);
            CX_Message("%d\t%s\t%d\t%f\n", j, sp_name, m, mw);
        }

        CX_Message("Products:\n");
        for (i=0; i<n_p; ++i)
        {
            sp = r->mat_product[i];
            j = r->product[i];
            m = r->stoich_product[i];
            sp_name = MIXTURE_SITE_SPECIE_NAME(mix, j);
            mw = MATERIAL_PROP(sp, PROP_mwi);
            CX_Message("%d\t%s\t%d\t%f\n", j, sp_name, m, mw);
        }

        ++n_reactions;
    }

    return n_reactions;
}

DEFINE_ON_DEMAND(hy_list_species)
{
    Domain *fl_domain = Get_Domain(1);
    int ns;
    int method = RP_Get_Integer("spe/method");

    switch (method)
    {
    case 1:
        ns = get_species1(fl_domain);
        break;
    case 2:
        ns = get_species2(fl_domain);
        break;
    default:
        Error("Unknown method!\n");
        break;
    }
    CX_Message("Total number of species: %d\n", ns);
}

DEFINE_ON_DEMAND(locate_species)
{
    Domain *fl_domain = Get_Domain(1);
    int sp_index;
    char *sp_name = RP_Get_String("spe/name");

    sp_index = find_species(fl_domain, sp_name);

    if (-1 != sp_index)
    {
        CX_Message("%s has index: %d\n", sp_name, sp_index);
    }
    else
    {
        CX_Message("Sorry! Didn't find species %s in mixture!\n", sp_name);
    }
}

DEFINE_ON_DEMAND(hy_list_reactions)
{
    Domain *fl_domain = Get_Domain(1);
    int nr;

    nr = get_reactions(fl_domain);
    CX_Message("Total number of reactions: %d\n", nr);
}


