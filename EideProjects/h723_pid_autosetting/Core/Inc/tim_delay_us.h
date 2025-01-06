//
// Created by tony on 23-7-16.
//

#ifndef TIM_DELAY_US_H
#define TIM_DELAY_US_H
#include "tim.h"

void TIM_Delay_init(TIM_HandleTypeDef *timx);
void TIM_Delay_Us(uint16_t us);
void TIM_Delay_Us_Start();
void TIM_Delay_Us_Until(uint16_t us);
#endif //TIM_DELAY_US_H
