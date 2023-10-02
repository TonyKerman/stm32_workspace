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





void setup()
{

    LED_ON();
    HAL_Delay(500);
    LED_OFF();
    //HAL_Delay(500);
    LED_ON();
    HAL_Delay(500);
    LED_OFF();

    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);

}

void loop()
{
    HAL_Delay(100);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, );

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

