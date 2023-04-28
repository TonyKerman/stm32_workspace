
#include "main.h"
#include "USER_main.h"
#include "retarget.h"
#include "Serial_Servo.h"
#include "usart.h"

void USER_main(void)
{
    printf("hello world\n");
    Serial_Servo * Servo1 = Serial_Servo_Create(&huart3,1);
    Serial_Servo * Servo2 = Serial_Servo_Create(&huart3,2);
    HAL_Delay(10);
    //记得设置ID
    //Serial_Servo_SetID(&huart3,254,2);


    while (1)
    {
        Ser_Servo_SetLoadOrUnload(Servo1,0);
        Serial_Servo_ReadPosition(Servo1);
        HAL_Delay(20);
        Ser_Servo_SetLoadOrUnload(Servo2,0);
        Serial_Servo_ReadPosition(Servo2);
        printf("pos1:%d,pos2:%d\n",Servo1->currentPostion,Servo2->currentPostion);
        HAL_Delay(500);
    }
}

