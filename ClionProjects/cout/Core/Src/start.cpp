
#include "start.h"
#include "main.h"
#include "usart.h"
#include <string>
#include "serial_debug.h"
#include "ps2.h"

using namespace std;
ps2outputTypeDef dat;

void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    dbg_start(&huart1);
    while(1)
    {
        ps2_receive(&hspi1,dat);
        dbg_display();
        HAL_Delay(10);

    }
}