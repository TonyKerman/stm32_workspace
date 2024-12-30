//
// Created by tony on 24-11-16.
//
#include "pid_param_setting.h"
#include<stdlib.h>

/**
 * @brief pid自动整定初始化(针对于自衡对象，如电机的速度环)
 * @param pas pid自动整定缓存
 * @param lambda lambda值，即期望闭环时间常数对于tau的比值，取值范围（0,inf）,取值越小，响应越快，一般取1
 * @param init_fdbk 初始反馈值，一般取0
 * @param test_time_ms 测试时间，单位ms
 * @param dt_ms 采样时间，单位ms，对于大疆电调，除非经过设置，否则固定为1ms(见电调手册)
 * @param delta 误差，一般取0.01
 * @param test_output 测试输出，会影响最终的Kp,Ki,Kd
 */
void PID_AutoSetting_Init(
    PID_AutoSetting_Buffers_t *pas,
    double lambda,
    double init_fdbk,
    double test_time_ms,
    double dt_ms,
    double delta,
    double test_output)
{
    pas->lambda = lambda;
    pas->init_fdbk = init_fdbk;
    pas->test_time_ms = test_time_ms;//5000
    pas->test_output = test_output;
    pas->dt_ms = dt_ms;
    pas->delta =delta; //0.05
    pas->pv_num = (int)pas->test_time_ms/pas->dt_ms;
    pas->pv_index = 0;
    pas->pv = (double *)malloc(pas->pv_num*sizeof(double));
}

/**
 * @brief pid自动整定更新
 * @param pas pid自动整定缓存
 * @param fdbk 当前反馈值
 * @return PID_AUTOSETTING_PROCESS:正在进行PID自动整定
 *         PID_AUTOSETTING_DONE:PID自动整定完成
 */
PID_AutoSetting_State_t PID_AutoSetting_Update(PID_AutoSetting_Buffers_t *pas,double fdbk)
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
/**
 * @brief pid自动整定设置PID参数
 * @param pas pid自动整定缓存
 * @param Kp Kp
 * @param Ki Ki
 * @param Kd Kd
 */
void PID_AutoSetting_Set_Param(PID_AutoSetting_Buffers_t *pas,double *Kp,double *Ki,double *Kd)
{
    double final_pv = 0;
    uint16_t index = 0;
    for(int i = pas->pv_num-1;i>pas->pv_num-11;i--)
    {
        final_pv += pas->pv[i];
    }
    final_pv = final_pv/10;
    //测量模型增益K，书中建议多次测试并选择较大值，对于大疆电调，并非较大值。
     pas->K = final_pv/pas->test_output;
    while(abs(pas->pv[index]-pas->init_fdbk) < abs(final_pv*pas->delta))
    {
        index++;
    }
    pas->tau=pas->dt_ms*index;
    while(abs(pas->pv[index]-pas->init_fdbk) < abs(final_pv*0.632))
    {
        index++;
    }
    
    pas->T=pas->dt_ms*index;
    *Kp = pas->T/pas->K/(pas->tau*(pas->lambda+1));
    *Ki = (*Kp)*pas->dt_ms/pas->T;
    *Kd = 0;
    free(pas->pv);
}

