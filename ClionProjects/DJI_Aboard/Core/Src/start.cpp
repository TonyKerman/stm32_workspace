//
// Created by Lenovo on 2022/9/24.
//
#include "tim.h"
#include "start.h"
#include "main.h"
#include "usart.h"
#include "can.h"
#include "spi.h"
#include "gpio.h"
#include <string>
using namespace std;
#define LED_GREEN_ON() HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET)
#define LED_GREEN_OFF() HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET)

void setup()
{
    string str = "hello world!";
    //HAL_UART_Transmit(&huart1,(uint8_t *)str.data(),str.size(),0xff);
    LED_GREEN_ON();
    HAL_Delay(500);
    LED_GREEN_OFF();
    HAL_Delay(200);
    LED_GREEN_ON();
    HAL_Delay(500);
    LED_GREEN_OFF();
    HAL_TIM_Base_Start_IT(&htim1);
}


void loop()
{
    
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim ->Instance==htim1.Instance)
    {
        //HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);
        //HAL_Delay(100);
    }
}