
#include "start.h"
#include "main.h"
#include "serial_debug.h"
//#include "i2c.h"
#include "lcd1602_i2c.h"
using namespace std;

void startup()
{
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    HAL_Delay(300);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    //dbg_start(&huart1);
    lcd_init();

    while(1)
    {
        lcd_send_string("12345678");
        HAL_Delay(1000);
        lcd_put_cur(1, 0);
        lcd_send_string("1234567");


    }
}