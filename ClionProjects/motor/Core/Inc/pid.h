//
// Created by tony on 2023/3/23.
//

#ifndef MOTOR_PID_H
#define MOTOR_PID_H
#ifdef __cplusplus
extern "C"
{
#endif
typedef struct {
    float lerr=0;
    float llerr=0;
    float intg=0;
    float loutput=0;
}PidValsTypeDef;
float pid_incr(PidValsTypeDef* vals,float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo);
float pid_post(PidValsTypeDef* vals,float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo);

#ifdef __cplusplus
}
#endif
#endif //MOTOR_PID_H
