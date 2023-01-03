//
// Created by Lenovo on 2022/9/24.
//使用RTOS
// 1 cubemx设置，sys里改时钟
// 2 CMakelists 增加include
// 取消Uncomment for hardware floating point下面几行注释
// 3
//

#include "start.h"
#include "main.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string>

using namespace std;
#define LED_OFF() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET)
#define LED_ON() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET)
string str;
int a;
void setup()
{
    LED_ON();
    HAL_Delay(400);
    LED_OFF();
    HAL_Delay(400);
    LED_ON();
    HAL_Delay(400);
    LED_OFF();

    str= "hello world!";

}

void task1()
{
    while (true)
    {
        a++;

        int t= xTaskGetTickCount();
        str= to_string(t);
        str += "\n";
        HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str.data(),str.length());
        for(int i =0;i<10;i++)
        {
            str = to_string(i);
            HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.length(),10);
        }

        osDelay(10);
        //osDelayUntil((uint32_t*)&t,10);
        t= xTaskGetTickCount();
        str = "\n";
        str += to_string(t);
        str +="\n";
        HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str.data(),str.length());
        osDelay(1000);
    }

}
void task2()
{
    while (true)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
        osDelay(1000);
    }

}