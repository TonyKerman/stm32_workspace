//
// Created by tony on 23-1-6.
//
#include "start.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include <string>
using namespace std;

string str;
uint8_t t[8] = {1,2,3,4,5,6,7,8};
uint8_t text[10];
int count =0;

void setup()
{
    HAL_SPI_Receive_IT(&hspi3,text,8);
    while(1)
    {
        for(uint8_t i=0; i<=7;i++)
        {
            HAL_SPI_Transmit(&hspi2,(uint8_t *) &t[i],1,100);
        }
        HAL_Delay(500);
    }
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

    HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    count++;
    str = "i heard ";
    for(int i=0;i<=9;i++)
    {
        str+=",";
        str += to_string(text[i]);

    }
    str += to_string(count);
    HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),100);
    HAL_SPI_Receive_IT(&hspi3,text,8);


}
