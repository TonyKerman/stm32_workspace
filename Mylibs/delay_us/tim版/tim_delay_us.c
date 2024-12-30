//
// Created by tony on 23-7-16.
//

#include "tim_delay_us.h"
#include "tim.h"

void TIM_Delay_Us(TIM_HandleTypeDef *timx,uint16_t us)
{
	__HAL_TIM_SET_COUNTER(timx, 0); //把计数器的值设置为0
	HAL_TIM_Base_Start(timx);
	while (__HAL_TIM_GET_COUNTER(timx) < us); //每计数一次，就是1us，直到计数器值等于我们需要的时间
	HAL_TIM_Base_Stop(timx);
}