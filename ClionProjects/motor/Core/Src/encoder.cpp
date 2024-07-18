//
// Created by tony on 2023/3/27.
//
#include "tim.h"
#include "encoder.h"
/**
  *@brief 编码器测速(4分频)
  * cubemx ->tim
  * PSC: 3
  * Encoder mode: Tl1 and Tl2
  *@param dt : 两次测量的间隔 ms
  *@param PPR: 编码器分辨率
  *@return speed: 速度 rpm
  *
  */
float get_speed(TIM_HandleTypeDef * tim,int dt,int PPR)
{
    static bool init = true;
    //上一次编码器的值

    if(init)
    {
        __HAL_TIM_SET_COUNTER(&htim2,32768);
        init = false;
    }
    int enc = __HAL_TIM_GET_COUNTER(&htim2);
    float speed=(enc-32768) * (1000.0/dt) * 60 / PPR;//单位 rpm
    //float  speed = enc -32768;
    __HAL_TIM_SET_COUNTER(&htim2,32768);

    return  speed;

}