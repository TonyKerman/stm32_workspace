//
// Created by tony on 23-1-6.
//
#include "start.h"
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include <string>
using namespace std;

string str;
uint8_t text[10];
int count =0;

void setup()
{
    HAL_I2C_Slave_Receive_IT(&hi2c2,text,1);
    while(1)
    {
        HAL_I2C_Master_Transmit(&hi2c1,0x11,(uint8_t*)"hello",5,100);
        HAL_Delay(500);
    }
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{

    HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    count++;
    str = "i heard ";
    //str += text;
    str += std::to_string(count);
    HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),100);
    HAL_I2C_Slave_Receive_IT(&hi2c2,(uint8_t *)text,1);


}
