//
// Created by tony on 2023/3/23.
//

#include "pid.h"
#include "stdlib.h"
Pid * Pid_Create(PidTypeDef pid_type,float kp,float ki,float ,kd,int maxi,int maxo)
{
    Pid * me = (Pid*) malloc(sizeof(Pid));
    me->pidType = pid_type;
    me->Kp = kp;
    me->Ki = ki;
    me->Kd = kd;
    me->maxi = maxi;
    me->maxo = maxo;
    //me->err = 0;
    me->lerr =0;
    me->llerr = 0;
    me->intg = 0;
    float me->output = 0;
    float me->loutput =0;
}

void Pid_SetArgs(Pid *me,float newKp,float newKi,float newKp,int newMaxi,int newMaxo)
{
    me->Kp = newkp;
    me->Ki = newki;
    me->Kd = newkd;
    me->maxi = newMaxi;
    me->maxo = newMaxo;
}

float Pid_Calculate(Pid * me, float tgt_val,float cur_val)
{
    if(me->pidType == incr)
    {

        float err =tgt_val - cur_val;
        me->intg = err;
        me->output =me->loutput + me->Kp*(err-me->lerr) + me->Ki*me->intg + me->Kd*(err - 2*me->lerr + me->llerr);
        me->llerr=me->lerr;
        me->lerr = err;

        if(me->output > float(me->maxo))
            me->output = float(me->maxo);
        else if(me->output < float(-1*me->maxo))
            me->output= me->maxo*-1;

        me->loutput = me->output;
        return me->output;
    }else
    {
        float maxi= me->maxi / me->Ki;
        float err = tgt_val - cur_val;
        me->intg += err;
        //积分限幅
        if (me->intg > float(maxi))
            me->intg = float(maxi);
        else if (me->intg < float(-1 * maxi))
            me->intg = -1 * maxi;
        me->output = me->Kp * err + me->Ki * me->intg + (err - me->lerr) * me->Kd;
        me->lerr = err;
        //输出限制
        if (me->output > float(me->maxo))
            me->output = float(me->maxo);
        else if (me->output < float(-1 * me->maxo))
            me->output = -1 * me->maxo;
        return me->output;
    }

}

float pid_incr(float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo)
{
    maxi = maxi / Ki;
    static bool init = true;
    static float lerr,llerr,loutput;
    float output;
    float err = tgt_val - cur_val;
    float intg = err;
    if(init)
    {
        lerr = 0;
        llerr = 0;
        loutput =0;
    }

    output =loutput + Kp*(err-lerr) + Ki*intg + Kd*(err - 2*lerr + llerr);
    llerr= lerr;
    lerr = err;

    //输出限制
    if(output > float(maxo))
        output = float(maxo);
    else if(output < float(-1*maxo))
        output= maxo*-1;

    loutput = output;
    return output;
}


float pid_post(float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo) {
    maxi = maxi / Ki;
    static bool init = true;
    static float lerr;
    float output;
    float err = tgt_val - cur_val;
    static float intg;
    if (init) {
        lerr = 0;
    }

    intg += err;
    //积分限幅
    if (intg > float(maxi))
        intg = float(maxi);
    else if (intg < float(-1 * maxi))
        intg = -1 * maxi;
    output = Kp * err + Ki * intg + (err - lerr) * Kd;
    lerr = err;
    //输出限制
    if (output > float(maxo))
        output = float(maxo);
    else if (output < float(-1 * maxo))
        output = -1 * maxo;
    return output;
}