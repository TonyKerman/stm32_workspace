//
// Created by tony on 2023/4/12.
//

#include "Mpu6050.h"

#include "main.h"
#include <stdlib.h>
#include "retarget.h"

void Mpu6050_writeReg(Mpu6050 * me, uint16_t p_reg_addr, uint8_t p_data);
uint8_t Mpu6050_readReg(Mpu6050 * me,uint8_t p_reg);
void MPU_Set_Gyro_Fsr(Mpu6050 * me,uint8_t fsr);
void MPU_Set_Accel_Fsr(Mpu6050 * me,uint8_t fsr);
void MPU_Set_Rate(Mpu6050 * me,uint16_t rate);
HAL_StatusTypeDef MPU6050_readLen(Mpu6050 * me,uint8_t p_reg,uint8_t len,uint8_t *buf);

Mpu6050 * Mpu6050_Create(I2C_HandleTypeDef * p_i2cHandle,int is_AD0_pull_up,enum mpu_AfsrDef a_fsr,enum mpu_GfsrDef g_fsr)
{
    Mpu6050 * me =(Mpu6050*)malloc(sizeof (Mpu6050));
    me->i2cHandle=p_i2cHandle;

    //如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
    //如果接V3.3,则IIC地址为0X69(不包含最低位).
    if(is_AD0_pull_up)
        me->deviceAddr=0x69;
    else
        me->deviceAddr=0x68;
    me->readAddr=(me->deviceAddr<<1)|0x01;
    me->writeAddr=(me->deviceAddr<<1)|0x00;
    //设置量程
    me->aFsr = a_fsr;
    me->gFsr = g_fsr;
    return me;

}


HAL_StatusTypeDef Mpu6050_Init(Mpu6050 * me) {

    HAL_Delay(500);
    Mpu6050_writeReg(me, MPU_PWR_MGMT1_REG, 0X80);    //复位MPU6050
    HAL_Delay(100);
    Mpu6050_writeReg(me,MPU_PWR_MGMT1_REG, 0X00);    //唤醒MPU6050
    MPU_Set_Gyro_Fsr(me,me->gFsr);					//陀螺仪角速度传感器
    MPU_Set_Accel_Fsr(me,me->aFsr);					//加速度传感器
    MPU_Set_Rate(me,100);						//设置采样率100Hz
    Mpu6050_writeReg(me,MPU_INT_EN_REG,0X00);	//关闭所有中断
    Mpu6050_writeReg(me,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
    Mpu6050_writeReg(me,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    Mpu6050_writeReg(me,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
    uint8_t res = Mpu6050_readReg(me,MPU_DEVICE_ID_REG);
    if (res == me->deviceAddr)//器件ID正确
        return HAL_OK;
    else
        return HAL_ERROR;
    HAL_Delay(100);
}



//设置MPU6050角速度传感器满量程范围
void MPU_Set_Gyro_Fsr(Mpu6050 * me,uint8_t fsr)
{
    Mpu6050_writeReg(me,MPU_GYRO_CFG_REG,fsr<<3);
    //Mpu6050_writeReg(me,MPU_GYRO_CFG_REG,0x18);
}


//设置MPU6050加速度传感器满量程范围
void MPU_Set_Accel_Fsr(Mpu6050 * me,uint8_t fsr)
{
     Mpu6050_writeReg(me,MPU_ACCEL_CFG_REG,fsr<<3);
}


//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
uint8_t MPU_Set_LPF(Mpu6050 * me,uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    Mpu6050_writeReg(me,MPU_CFG_REG,data);//设置数字低通滤波器
}


//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
void MPU_Set_Rate(Mpu6050 * me,uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    Mpu6050_writeReg(me,MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
    MPU_Set_LPF(me,rate/2);	//自动设置LPF为采样率的一半
}


//获取温度 单位 摄氏度
HAL_StatusTypeDef Mpu6050_getTemp(Mpu6050 * me)
{
    uint8_t buf[2];
    HAL_StatusTypeDef result = MPU6050_readLen(me,MPU_TEMP_OUTH_REG,2,buf);
    if(result == HAL_OK)
    {
        int16_t raw = (buf[0]<<8)| buf[1];
        me->temp=36.53+((double)raw)/340;
    }
    return result;
}


//得到陀螺仪角速度值 单位 度/秒
HAL_StatusTypeDef Mpu6050_getGyroscope(Mpu6050 * me)
{
    uint8_t buf[6];
    int16_t rawData[3];
    //比率
    float k[4]={131.07,65.535,32.7675,16.38375};

    HAL_StatusTypeDef result =  MPU6050_readLen(me,MPU_GYRO_XOUTH_REG,6,buf);
    if(result == HAL_OK)
    {
    	//16位ADC，原始值
        rawData[0] = ( buf[0] << 8) | buf[1];
        rawData[1] = ( buf[2] << 8) | buf[3];
        rawData[2] = ( buf[4] << 8) | buf[5];
        //单位转换
        me->gx = rawData[0]/k[me->gFsr];
        me->gy = rawData[1]/k[me->gFsr];
        me->gz = rawData[2]/k[me->gFsr];
    }

    return result;

}


//得到加速度值(带符号) 单位 米方/秒
HAL_StatusTypeDef Mpu6050_getAccelerometer(Mpu6050 * me)
{
    uint8_t buf[6];
    int16_t rawData[3];
    //比率 65535/(9.8*正负量程)
    float k[4]={1671.84,835.92,417.96,208.98};

    HAL_StatusTypeDef result = MPU6050_readLen(me,MPU_ACCEL_XOUTH_REG,6,buf);
    if(result == HAL_OK)
    {
        rawData[0] = ( buf[0] << 8) | buf[1];
        rawData[1] = ( buf[2] << 8) | buf[3];
        rawData[2] = ( buf[4] << 8) | buf[5];
        
        me->ax = rawData[0]/k[me->aFsr];
        me->ay = rawData[1]/k[me->aFsr];
        me->az = rawData[2]/k[me->aFsr];
    }
    return result;
}
void Mpu6050_writeReg(Mpu6050 * me, uint16_t p_reg_addr, uint8_t p_data)
{
    HAL_I2C_Mem_Write(me->i2cHandle,me->writeAddr,p_reg_addr,I2C_MEMADD_SIZE_8BIT,&p_data,1,100);
}

uint8_t Mpu6050_readReg(Mpu6050 * me,uint8_t p_reg)
{
    uint8_t data;
    HAL_I2C_Mem_Read(me->i2cHandle,me->readAddr,p_reg,I2C_MEMADD_SIZE_8BIT,&data,1,100);
    return data;
}

HAL_StatusTypeDef MPU6050_readLen(Mpu6050 * me,uint8_t p_reg,uint8_t len,uint8_t *buf)
{
    return HAL_I2C_Mem_Read(me->i2cHandle, me->readAddr, p_reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
}
