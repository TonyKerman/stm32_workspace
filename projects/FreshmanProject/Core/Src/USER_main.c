
#include "main.h"
#include "USER_main.h"
#include "retarget.h"
#include "Serial_Servo.h"
#include "../Mpu6050/Mpu6050.h"
#include "../Mpu6050/driver/eMPL/inv_mpu.h"
#include "../Mpu6050/driver/eMPL/inv_mpu_dmp_motion_driver.h"
#define DEFAULT_MPU_HZ  (20)
Serial_Servo * S1 ;
Serial_Servo * S2 ;

void USER_main(void)
{
    printf("hello world\n");
    S1 = Serial_Servo_Create(&huart2,1);
    S2 = Serial_Servo_Create(&huart2,2);
    int state = mpu_dmp_init();
    printf("start with%d\n",state);
    while (1)
    {
//        Ser_Servo_SetLoadOrUnload(S1,0);
//        Serial_Servo_ReadPosition(S1);
//        HAL_Delay(20);
//        Ser_Servo_SetLoadOrUnload(S2,0);
//        Serial_Servo_ReadPosition(S2);
//        printf("pos1:%d,pos2:%d\n",S1->currentPostion,S2->currentPostion);

        //printf("ax:%.2f,ay:%.2f,az:%.2f\n",mpu1->ax,mpu1->ay,mpu1->az);
        state = mpu_dmp_get_data();
        //printf("mpu get data with%d,pitch=%f,raw=%f\n",state,mpu1->pitch,mpu1->roll);
        printf("/*%.2f,%.2f,%.2f*/\n",mpu6050.roll,mpu6050.pitch,mpu6050.yaw);
        //Mpup6050_update(mpu1);

        HAL_Delay(20);

    }
}

