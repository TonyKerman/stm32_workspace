//
// Created by tony on 2023/1/27.
//

#include "step_motor.h"
#include "start.h"
#include "main.h"
#include "math.h"

void write_4_gpios(char arg)
{
    GPIO_TypeDef* GPIOS[4]={IN1_GPIO_Port,IN2_GPIO_Port,IN3_GPIO_Port,IN4_GPIO_Port};
    uint16_t pins[4]={IN1_Pin,IN2_Pin,IN3_Pin,IN4_Pin};
    for(int i=0;i<4;i++)
    {
        if((arg & (int)pow(2,i))>>i)
            HAL_GPIO_WritePin(GPIOS[i],pins[i],GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(GPIOS[i],pins[i],GPIO_PIN_RESET);
    }
}
StepMotor::StepMotor(GPIO_TypeDef* GPIOS[4], uint16_t PINS[4])
{
    for(int i=0;i<4;i++)
    {
        _GPIOS[i] = GPIOS[i];
        _PINS[i] = PINS[i];
    }


}


void StepMotor::func(float round)
{
    int count =0;
    do{
        for(int i =0;i<8;i++)
        {
            write_4_gpios(steps[i]);
            HAL_Delay(1);
            count++;
        }

    } while (count<round*4096/2);
}