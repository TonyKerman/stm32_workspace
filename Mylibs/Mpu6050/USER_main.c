
#include "main.h"
#include "USER_main.h"
#include "retarget.h"
#include "Mpu6050.h"


void USER_main(void)
{
    printf("hello world\n");
    Mpu6050 * mpu1 = Mpu6050_Create(&hi2c1,0,Afsr_2g,Gfsr_500LSB,100);
    while (Mpu6050_Init(mpu1)!=HAL_OK)
    {
        printf("not init%d\n",(int)mpu1->deviceAddr);
    }
    Mpu6050_getAccelerometer(mpu1);
    Mpu6050_getGyroscope(mpu1);
    while (1)
    {

        //Mpu6050_getTemp(mpu1);
        Mpu6050_getAccelerometer(mpu1);
        Mpu6050_getGyroscope(mpu1);
        Mpup6050_update(mpu1);
        HAL_Delay(10);
        printf("pitch=%f,roll=%f\n",mpu1->pitch,mpu1->roll);

    }
}

