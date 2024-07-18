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
#include "serial_debug.h"
using namespace std;
#define LED_OFF() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_RESET)
#define LED_ON() HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,GPIO_PIN_SET)
string str;

void setup()
{
    LED_ON();
    HAL_Delay(400);
    LED_OFF();
    HAL_Delay(400);
    LED_ON();
    HAL_Delay(400);
    LED_OFF();
    start_debug(&huart2);
    for(int t=1;t<=100;t++)
    {
        float p =t/3.0;
        int v = t*2;
        dbg_add(t,"t");
        dbg_add(p,"t/3");
        dbg_add(v,"t*2");
        dbg_display();
    }

}



void task1()
{
    while (true)
    {

        /*
        a++;

        int t= xTaskGetTickCount();
        str= to_string(t);
        str += "a\n";
        HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str.data(),str.length());
        for(int i =0;i<10;i++)
        {
            str = to_string(i);
            HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.length(),10);
        }

        osDelay(10);
        //osDelayUntil((uint32_t*)&t,10);
        t= xTaskGetTickCount();
        str = "b\n";
        str += to_string(t);
        str +="\n";
        HAL_UART_Transmit_DMA(&huart2,(uint8_t *)str.data(),str.length());*/
        osDelay(100);
    }

}

void task2()
{
    while (true)
    {
        //int t= xTaskGetTickCount();
        HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
        osDelay(1000);
    }

}