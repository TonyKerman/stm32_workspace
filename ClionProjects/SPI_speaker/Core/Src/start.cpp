
#include "start.h"
#include "main.h"
#include "spi.h"
using namespace std;

uint8_t data[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    while(1)
    {
        for(int i=0;i<16;i++)
        {
            HAL_SPI_Transmit(&hspi1,&data[i],1,100);
            HAL_Delay(5);
        }
        HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
        HAL_Delay(100);

    }
}
