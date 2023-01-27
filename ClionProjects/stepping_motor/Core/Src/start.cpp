//
// Created by tony on 23-1-6.
//
#include "start.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

#include <string>
using namespace std;
//四相四拍
//char steps[4] ={0x08,0x04,0x02,0x01};
char steps[8] ={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
string str;
void write_4_gpios(char arg)
{
    if((arg & 0x08) == 0x08)
        HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,GPIO_PIN_RESET);
    if((arg & 0x04) == 0x04)
        HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,GPIO_PIN_RESET);
    if((arg & 0x02) == 0x02)
        HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_RESET);
    if((arg & 0x01) == 0x01)
        HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_RESET);
}

void setup()
{

    while(1)
    {
        int count =0;
        for (count=0;count<(4096/4);count+=4)
        {
            for(int i =0;i<=7;i++)
            {
                write_4_gpios(steps[i]);
                HAL_Delay(2);
            }
        }
        HAL_Delay(3000);

    }
}


