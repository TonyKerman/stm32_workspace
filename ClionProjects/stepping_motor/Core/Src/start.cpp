//
// Created by tony on 23-1-6.
//
#include "start.h"
#include "main.h"
#include "math.h"
#include "gpio.h"
#include "usart.h"

#include <string>
using namespace std;
//四相四拍

//char steps[8] ={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
string str;

/*void write_4_gpios(char arg)
{
    GPIO_TypeDef* GPIOS[4]={IN1_GPIO_Port,IN2_GPIO_Port,IN3_GPIO_Port,IN4_GPIO_Port};
    uint16_t pins[4]={IN1_Pin,IN2_Pin,IN3_Pin,IN4_Pin};
    for(int i=0;i<4;i++)
    {
        if((arg & (int)pow(2,i))>>i)
            HAL_GPIO_WritePin(GPIOS[i],pins[i],GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(GPIOS[i],pins[i],GPIO_PIN_RESET);
    }
}
 */
void setup()
{

    while(1)
    {
        /*int count =0;
        do{
            for(int i =0;i<8;i++)
            {
                write_4_gpios(steps[i]);
                HAL_Delay(1);
                count++;
            }

        } while (count<4096/2);

        HAL_Delay(3000);
*/
    }
}


