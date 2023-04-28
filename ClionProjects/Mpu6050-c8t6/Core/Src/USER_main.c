
#include "main.h"
#include "USER_main.h"
#include "retarget.h"
#include "Mpu6050.h"


void USER_main(void)
{
    printf("hello world\n");
    Mpu6050 * mpu1 = Mpu6050_Create(&hi2c1,0,Afsr_2g,Gfsr_500LSB);
    while (Mpu6050_Init(mpu1)!=HAL_OK)
    {
        printf("not init%d\n",(int)mpu1->deviceAddr);
    }
    while (1)
    {

        //Mpu6050_getTemp(mpu1);
        Mpu6050_getGyroscope(mpu1);
        Mpu6050_getAccelerometer(mpu1);
        HAL_Delay(10);
        printf("gx=%f,gy=%f,gz=%f\n",mpu1->gx,mpu1->gy,mpu1->gz);

    }
}

