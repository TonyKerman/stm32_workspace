#include "pid.h"
#include<stdlib.h>

void PID_Init(PIDController *pid, double Kp, double Ki, double Kd)
{
    pid->kp = Kp;
    pid->ki = Ki;
    pid->kd = Kd;
    pid->integral_limit_enable = 0;
    pid->initialed = 1;
}

void PID_Init_No_Params(PIDController *pid)
{
    pid->integral_limit_enable = 0;
    pid->initialed = 1;
}

void PID_Init_Classic(PIDController *pid, double Kc, double Ti, double Td, double Dt)
{
    double kp = Kc ;
    double ki = Kc * Dt / Ti;
    double kd = Kc * Td / Dt;
    PID_Init(pid, kp, ki, kd);
}

int PID_Set_integral_limit(PIDController *pid, double integral_max)
{
    if (!pid->initialed||integral_max<0)
    {
        return -1;
    }
    pid->integral_limit_enable = 1;
    pid->integral_max = integral_max;
    return 0;
}

int PID_update(PIDController *pid, double feedback)
{
    if (!pid->initialed||pid->kp*pid->ki==0)
    {
        return -1;
    }
    double error = pid->setpoint - feedback;
    pid->integral += error;
    if (pid->integral_limit_enable)
    {
        if (pid->integral > pid->integral_max)
        {
            pid->integral = pid->integral_max;
        }
        else if (pid->integral < -pid->integral_max)
        {
            pid->integral = -pid->integral_max;
        }
    }
    pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * (error - pid->prev_error);
    
    return 0;
}
