//
// Created by tony on 2023/3/23.
//

#ifndef MOTOR_PID_H
#define MOTOR_PID_H
#ifdef __cplusplus
extern "C"
{
#endif
float pid_incr(float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo);
float pid_post(float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo);
#ifdef __cplusplus
}
#endif
#endif //MOTOR_PID_H
