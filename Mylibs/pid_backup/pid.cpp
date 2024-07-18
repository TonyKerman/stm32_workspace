//
// Created by tony on 2023/3/23.
//

#include "pid.h"
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
struct Pid{
    float lerr=0;
    float llerr=0;
    float intq=0;
    float loutput=0;
};

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