#ifndef _PID_H__
#define _PID_H__
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"

typedef struct {
    double kp;
    double ki;
    double kd;
    uint8_t integral_limit_enable; // 是否启用积分限幅
    double integral_max;  // 积分项最大值
    double output_min;  // 输出最小值
    double output_max;  // 输出最大值
    

    double setpoint;    // 设定值
    double integral;    // 积分项
    double prev_error;  // 上一次误差
    double prev_measured; // 上一次测量值，用于微分计算
    double output;      // 控制输出
    uint8_t initialed; // PID控制器是否初始化
} PIDController;
void PID_Init(PIDController *pid, double Kp, double Ki, double Kd);
void PID_Init_NoParams(PIDController *pid);
void PID_Init_Classic(PIDController *pid, double Kc, double Ti, double Td,double Dt);
int PID_Set_integral_limit(PIDController *pid, double integral_max);
int PID_update(PIDController *pid, double measured_value);
#ifdef __cplusplus
}
#endif

#endif