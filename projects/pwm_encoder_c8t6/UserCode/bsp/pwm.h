
#ifndef PWM_H
#define PWM_H

#include "tim.h"

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t channel;
} PWM_t;

static  void PWM_Start(PWM_t* hpwm) { HAL_TIM_PWM_Start(hpwm->htim, hpwm->channel); }

static  void PWM_Stop(PWM_t* hpwm) { HAL_TIM_PWM_Stop(hpwm->htim, hpwm->channel); }

static  void PWM_SetCompare(PWM_t* hpwm, const uint32_t compare)
{
    if (compare <= __HAL_TIM_GET_AUTORELOAD(hpwm->htim))
        __HAL_TIM_SET_COMPARE(hpwm->htim, hpwm->channel, compare);
}

static  void PWM_SetDutyCircle(PWM_t* hpwm, const float duty_circle)
{
    if (duty_circle < 0.0f)
        PWM_SetCompare(hpwm, 0);
    else if (duty_circle > 1.0f)
        PWM_SetCompare(hpwm, __HAL_TIM_GET_AUTORELOAD(hpwm->htim));
    else
        PWM_SetCompare(hpwm, __HAL_TIM_GET_AUTORELOAD(hpwm->htim) * duty_circle + 0.5);
}

#endif // PWM_H
