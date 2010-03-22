#include "udf.h"
#include "dpm.h"

#define NMAX 100000

static int last_id = -1;
int count =-1;
int i=-1,k=0;
real x0,x1,x2,time_p[100],time_pinit[100];
real len[NMAX];
int count1[100],count_check=0;
char so[100]="";
char sn[100]="";
int no_of_tries[100];

DEFINE_DPM_SCALAR_UPDATE(store_path,c,t,in,p)
{
    real xn0,xn1,xn2;
    int id_n = p->part_id;

    sprintf(sn,p->injection->name);
    if(strcmp(sn,so)!=0 || k==0){
        count1[k]=p->injection->n_particles;
        no_of_tries[k]=1;
        if(p->injection->stochastic_p !=0)
        no_of_tries[k]=p->injection->ntries;
        k++;
    }

    if(strcmp(sn,so)!= 0 || id_n != last_id){
        last_id=id_n;
        count = -1;
        i++;
    }
    count ++ ;
    if(count==0){
        len[i]=0.0;
        time_pinit[i]=P_TIME(p);
        x0=P_POS(p)[0];
        x1=P_POS(p)[1];
        x2=P_POS(p)[2];
    }

    xn0=P_POS(p)[0];
    xn1=P_POS(p)[1];
    xn2=P_POS(p)[2];
    len[i] += sqrt(pow((xn0-x0),2)+pow((xn1-x1),2)+pow((xn2-x2),2));
    time_p[i]=P_TIME(p);
    x0=P_POS(p)[0];
    x1=P_POS(p)[1];
    x2=P_POS(p)[2];
    sprintf(so,sn);
}


DEFINE_ON_DEMAND(hy_dpm_exec)
{
    int j,jc,kc=0,kc1=0,counter=0;
    real avg[100],sum[100],avg_glob,t_sum=0.0,sum_time[100];

    for(jc=0;jc<k;jc++)
    {
        sum[jc]=0.0;
        sum_time[jc]=0.0;
        if(jc>0)
        kc += count1[jc-1]* no_of_tries[jc-1];
        kc1 += count1[jc] * no_of_tries[jc];
        for(j=kc;j<kc1;j++){
            Message("Path length for particle- %d in injection-%d is %f\n",j,jc,len[j]);
            sum[jc] += len[j];
            sum_time[jc] +=(time_p[j]-time_pinit[j]);
        }
        Message("avg path length from injection-%d is %f\n",jc,sum[jc]/(count1[jc]*no_of_tries[jc]));
        Message("avg res time from injection-%d is %f\n",jc,sum_time[jc]/(no_of_tries[jc]*count1[jc]));
    }
}


DEFINE_DPM_TIMESTEP(limit_to_e_minus_four, p, dt)
{
    if(dt>1.e-4)
    {
        /*p->next_time_step=1.e-4;*/
        return 1.e-4;
    }
    return dt;
}

DEFINE_DPM_TIMESTEP(limit_to_fifth_of_prt, p, dt)
{
    real drag_factor = 0.;
    real p_relax_time;
    cphase_state_t *c= &(p->cphase);
    /*compute particle relaxation time*/
    if(P_DIAM(p)!=0.0)
    {
        drag_factor = DragCoeff(p)*c->mu/(P_RHO(p)*P_DIAM(p)*P_DIAM(p));
    }
    else
    {
        drag_factor=1.;
    }
    p_relax_time=1./drag_factor;
    /*check the condition and return the timestep*/
    if(dt>p_relax_time/5.)
    {
        return p_relax_time/5.;
    }
    return dt;
}
