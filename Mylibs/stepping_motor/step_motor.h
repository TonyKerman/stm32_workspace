//
// Created by tony on 2023/1/27.
//

#ifndef STEPPING_MOTOR_STEP_MOTOR_H
#define STEPPING_MOTOR_STEP_MOTOR_H
#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}

#include "gpio.h"

class StepMotor
{

    char steps[8] ={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
    GPIO_TypeDef* _GPIOS[4];
    uint16_t _PINS[4];
public:
    StepMotor(GPIO_TypeDef* GPIOS[4],uint16_t PINS[4]);
    void func(float round);

};

#endif

#endif //STEPPING_MOTOR_STEP_MOTOR_H
