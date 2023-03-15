
#include "start.h"
#include "main.h"
#include "oled.h"
#include "spi.h"
using namespace std;
HAL_StatusTypeDef state;

void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);

    // Init
    OLED_Init(&hspi1);
    OLED_DisPlay_On();

    while(1)
    {
        OLED_DrawCircle(10,10,10);
        OLED_ShowNum(10,30,1234,4);
        OLED_ShowString(10,40,(uint8_t * )&"HELLO");

        // Refresh is necessary
        OLED_Refresh();


        HAL_Delay(10);
    }
}