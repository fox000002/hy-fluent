#include "udf.h"

/**********************************************************************/
/* UDF that implements a simplified advective term in the             */
/*  scalar transport equation                                         */ 
/**********************************************************************/
DEFINE_UDS_FLUX(my_uds_flux, f, t, i)
{
    cell_t c0;
    cell_t c1=-1;
    
    Thread *t0;
    Thread *t1=NULL;
    
    real NV_VEC(psi_vec);
    real NV_VEC(A);
    
    real flux=0.0;
    
    c0 = F_C0(f,t);
    t0 = F_C0_THREAD(f,t);
    
    real dens;
    
    F_AREA(A,f,t);
    
    /*If face lies at domain boundary,use face values;*/
    if(BOUNDARY_FACE_THREAD_P(t)) /*Most face values will be available*/
    {
        /*Depending on its BC,density may not be set on face thread*/
        if(NNULLP(THREAD_STORAGE(t, SV_DENSITY)))
        {
            dens=F_R(f,t); /*Set dens to face value if available*/
        }
        else
        {  
            dens=C_R(c0,t0);/*else,set dens to cell value*/
        }
        
        NV_DS(psi_vec,=,F_U(f,t),F_V(f,t),F_W(f,t),*,dens);
        
        flux = NV_DOT(psi_vec,A); /*flux through Face*/
    }
    else /*If face lies in the domain,use average of adjacent cells.*/
    {
        c1 = F_C1(f,t); /*Get cell on other side of face*/
        t1 = F_C1_THREAD(f,t);
        NV_DS(psi_vec,=,C_U(c0,t0),C_V(c0,t0),C_W(c0,t0),*,C_R(c0,t0));
        NV_DS(psi_vec,+=,C_U(c1,t1),C_V(c1,t1),C_W(c1,t1),*,C_R(c1,t1));
        
        flux=NV_DOT(psi_vec,A)/2.0;/*Average flux through face*/
    }
    
    /*Fluent will multiply the returned value by phi_f(the scalar's value at the face)
      to get the "complete" advective term.*/
    return flux;
}


/***********************************************************************/
/* UDF for specifying user-defined scalar time derivatives             */
/***********************************************************************/
DEFINE_UDS_UNSTEADY(uns_time, c, t, i, apu, su)
{
    real physical_dt;
    real vol;
    real rho;
    real phi_old;

    physical_dt = RP_Get_Real("physical-time-step");
    
    vol = C_VOLUME(c,t);

    rho = C_R_M1(c,t);
    
    *apu = -rho*vol / physical_dt; /*implicit part*/
    
    phi_old = C_STORAGE_R(c,t,SV_UDSI_M1(i));
    
    *su  = rho * vol * phi_old/physical_dt; /*explicit part*/
}
