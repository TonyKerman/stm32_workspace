//
// Created by tony on 2023/3/23.
//

#include "pid.h"

/**
  *@brief pid 增量式
  *@param vals : pid数据结构体的指针
  *@param tgt_val : 目标值
  *@param cur_val : 当前值
  *@param maxi : 积分限制(没用上)
  *@param maxo : 输出限制
  * 
  */
float pid_incr(PidValsTypeDef* vals,float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo)
{
    maxi = maxi / Ki;
    float output;
    float err = tgt_val - cur_val;
    float intg = err;

    output =vals->loutput + Kp*(err-vals->lerr) + Ki*intg + Kd*(err - 2*vals->lerr + vals->llerr);

    //输出限制
    if(output > (float)maxo)
        output = (float)maxo;
    else if(output < (float)(-1*maxo))
        output= maxo*-1;

    vals->llerr= vals->lerr;
    vals->lerr = err;
    vals->loutput = output;
    return output;
}

/**
  *@brief pid 位置式
  *@param vals : pid数据结构体的指针
  *@param tgt_val : 目标值
  *@param cur_val : 当前值
  *@param maxi : 积分限制
  *@param maxo : 输出限制
  * 
  */
float pid_post(PidValsTypeDef*vals,float tgt_val,float cur_val,float Kp, float Ki, float Kd, int maxi, int maxo) {
    maxi = maxi / Ki;
    float output;
    float err = tgt_val - cur_val;
    vals->intg += err;
    //积分限幅
    if (vals->intg > (float)maxi)
        vals->intg = (float)maxi;
    else if (vals->intg < (float)(-1 * maxi))
        vals->intg = -1 * maxi;
    output = Kp * err + Ki * vals->intg + (err - vals->lerr) * Kd;
    vals->lerr = err;
    //输出限制
    if (output > (float)maxo)
        output = (float)maxo;
    else if (output < (float)(-1 * maxo))
        output = -1 * maxo;
    return output;
}
