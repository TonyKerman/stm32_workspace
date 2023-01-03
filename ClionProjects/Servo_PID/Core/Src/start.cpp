//
// Created by Lenovo on 2022/9/24.
//
#include "adc.h"
#include "start.h"
#include "main.h"
#include "usart.h"
#include "tim.h"
#include <string>
using namespace std;

#define LED_ON() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET)
#define LED_OFF() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET)

string str;

struct pidVals{
     float kp,ki,kd;
     int targetPosition;
     int presentPosition ;
     int integration ;
     int err;
     int lastErr;
};

pidVals pid1=
        {
        0.5,0.02,0.5,
        0,
        0,
        0,
        0,
        0,
};

void printPID(pidVals pid)
{
    str ="  targetPosition "+to_string(pid.targetPosition);
    str+="  presentPosition "+ to_string(pid.presentPosition);
    str+="\r\n";
    HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),0xff);
}


pidVals GetPID(pidVals pid)
{
    pid.err  = pid.targetPosition-pid.presentPosition;
    pid.integration+= pid.err;
    pid.presentPosition = pid.presentPosition+int(pid.err*pid.kp+pid.integration*pid.ki+pid.lastErr*pid.kd);
    pid.lastErr = pid.err;
    str ="                  pid.presentPosition before return is"+ to_string(pid.presentPosition)+"\r\n";
    //HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),10);
    return pid;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim ->Instance==htim1.Instance)
    {
        HAL_ADC_Start(&hadc1);
        int ADC_Value = HAL_ADC_GetValue(&hadc1);
        pid1.targetPosition = ADC_Value * 100 / 4096;

    }
}


void setup()
{
    str = "hello world!";
    LED_ON();
    HAL_Delay(500);
    LED_OFF();
    //HAL_Delay(500);
    LED_ON();
    HAL_Delay(500);
    LED_OFF();
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    printPID(pid1);
    HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),10);
    str = "";
}

void loop()
{
    pid1 = GetPID(pid1);
    HAL_Delay(100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pid1.presentPosition);
    printPID(pid1);
//    if(t%2==0)
//    {
//        t=0;
//        str ="  ADC_Value is "+to_string(ADC_Value);
//        str+="  targetPosition is "+ to_string(targetPosition);
//        str+="  set_value"+ to_string(position);
//        str+="\r\n";
//        HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),0xff);
//        str = "";
//    }






}

