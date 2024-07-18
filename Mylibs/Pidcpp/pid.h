//
// Created by tony on 2023/3/23.
//

#ifndef MOTOR_PID_H
#define MOTOR_PID_H
#ifdef __cplusplus
extern "C"
{
#endif
    enum PidTypeDef{post,incr};
    typedef struct Pid Pid;
    struct Pid
    {
        PidTypeDef pidType;
        //float tgtVal;
        //float curVal;
        float output,loutput;
        //float  err;
        float lerr,llerr;
        float Kp, float Ki, float Kd;
        float intg;
        int maxi,maxo;
    };
    Pid * Pid_Create(PidTypeDef pid_type,float kp,float ki,float ,kd,int maxi,int maxo);
    float Pid_Calculate(Pid * me, float tgt_val,float cur_val);
    void Pid_SetArgs(Pid *me,float newKp,float newKi,float newKp,int newMaxi,int newMaxo);
//float pid_incr(float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo);


#ifdef __cplusplus
}
#endif
#endif //MOTOR_PID_H
