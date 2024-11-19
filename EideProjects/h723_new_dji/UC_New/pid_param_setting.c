//
// Created by tony on 24-11-16.
//
#include "pid_param_setting.h"
#include "stdlib.h"
void PID_AutoSetting_Init(PID_AutoSetting_Buffers *pas,double lambda,double init_fdbk,double dt_ms,double test_output)
{
    pas->lambda = lambda;
    pas->init_fdbk = init_fdbk;
    pas->test_time_ms = 5000;
    pas->test_output = test_output;
    pas->dt_ms = dt_ms;
    pas->delta = 0.05;
    pas->pv_num = (int)pas->test_time_ms/pas->dt_ms;
    pas->pv_index = 0;
    pas->pv = (double *)malloc(pas->pv_num*sizeof(double));
}

PID_AutoSetting_State PID_AutoSetting_Update(PID_AutoSetting_Buffers *pas,double fdbk)
{
    if(pas->pv_index < pas->pv_num)
    {
        pas->pv[pas->pv_index] = fdbk;
        pas->pv_index++;
        return PID_AUTOSETTING_PROCESS;
    }
    else
    {
        return PID_AUTOSETTING_DONE;
    }
}
void PID_AutoSetting_Set(PIDController *pid,PID_AutoSetting_Buffers *pas)
{
    double final_pv = 0;
    uint16_t index = 0;
    double K = 0;
    double tau=0;
    double T=0;
    for(int i = pas->pv_num;i>pas->pv_num-10;i--)
    {
        final_pv += pas->pv[i];
    }
    final_pv = final_pv/10;
    K = final_pv/pas->test_output;
    while(abs(pas->pv[index]-pas->init_fdbk) < abs(final_pv*pas->delta))
    {
        index++;
    }
    tau=pas->dt_ms*index;
    while(abs(pas->pv[index]-pas->init_fdbk) < abs(final_pv*0.632))
    {
        index++;
    }
    T=pas->dt_ms*index;
    pid->kp=T/K/(tau*(pas->lambda+1));
    pid->ki=T;
    pid->kd=0;
    free(pas->pv);

}
