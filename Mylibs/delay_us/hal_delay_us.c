//
// Created by tony on 23-7-16.
//

#include "hal_delay_us.h"
#include "tim.h"

void hal_Delay_us(TIM_HandleTypeDef *uartx,uint16_t us)
{
    uint16_t delay = 0xffff-us-5;		//从此数进行计数

    HAL_TIM_Base_Start(uartx);  	    //开启计数器
    __HAL_TIM_SetCounter(uartx,delay);	//设置计数器
    while(delay<0xffff-5)
    {
        delay = __HAL_TIM_GetCounter(uartx); //获取当前计数值
    }
    HAL_TIM_Base_Stop(uartx);
}