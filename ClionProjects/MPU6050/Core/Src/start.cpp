
#include "start.h"
#include "main.h"
#include "serial_debug.h"
#include "mpu6050.h"
using namespace std;

void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    
    while(1)
    {
        HAL_Delay(100);
       
    }
}