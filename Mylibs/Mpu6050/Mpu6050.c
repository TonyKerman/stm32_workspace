//
// Created by tony on 2023/4/12.
//https://blog.csdn.net/black_sneak/article/details/131253645

#include "Mpu6050.h"
#include "main.h"
#include <stdlib.h>
//#include "retarget.h"
#if   defined ( ARM_MATH_CM4 )
#include "arm_math.h"
#else
#include <math.h>
#endif
#ifdef SOFT_I2C
#include "i2c_sw.h"
#endif
#include "cmsis_os.h"
#define Delay_ms(s) osDelay((s)/portTICK_RATE_MS)
void Mpu6050_writeReg(Mpu6050 * me, uint16_t reg_addr, uint8_t p_data);
uint8_t Mpu6050_readReg(Mpu6050 * me,uint8_t regAddr);
void MPU_Set_Gyro_Fsr(Mpu6050 * me,uint8_t fsr);
void MPU_Set_Accel_Fsr(Mpu6050 * me,uint8_t fsr);
void MPU_Set_Rate(Mpu6050 * me,uint16_t rate);
HAL_StatusTypeDef MPU6050_readLen(Mpu6050 * me,uint8_t p_reg,uint8_t len,uint8_t *buf);



HAL_StatusTypeDef Mpu6050_Init(Mpu6050 * me,void * p_i2cHandle,int is_AD0_pull_up,enum mpu_AfsrDef a_fsr,enum mpu_GfsrDef g_fsr,uint16_t samplingRate) {

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
    //采样率 (recommend 100Hz)
    me->samplingRate=samplingRate;
    me->dt=1.0/samplingRate;
    me->quat[0] = 1.0;
    me->quat[1] = 0.0;
    me->quat[2] = 0.0;
    me->quat[3] = 0.0;
    me->exInt = 0.0;
    me->eyInt = 0.0;
    me->ezInt = 0.0;
    me->pitch = 0;
    me->roll = 0;
    me->yaw = 0;
    Delay_ms(500);
    Mpu6050_writeReg(me, MPU_PWR_MGMT1_REG, 0X80);    //复位MPU6050
    Delay_ms(100);
    Mpu6050_writeReg(me,MPU_PWR_MGMT1_REG, 0X00);    //唤醒MPU6050
    MPU_Set_Gyro_Fsr(me,me->gFsr);					//陀螺仪角速度传感器
    MPU_Set_Accel_Fsr(me,me->aFsr);					//加速度传感器
    MPU_Set_Rate(me,me->samplingRate);						//设置采样率100Hz
    Mpu6050_writeReg(me,MPU_INT_EN_REG,0X00);	//关闭所有中断
    Mpu6050_writeReg(me,MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
    Mpu6050_writeReg(me,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    Mpu6050_writeReg(me,MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
    uint8_t res = Mpu6050_readReg(me,MPU_DEVICE_ID_REG);
    if (res == me->deviceAddr)//器件ID正确
        return HAL_OK;
    else
        return HAL_ERROR;
    Delay_ms(100);
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
    //printf("%d",result);
    if(result == HAL_OK)
    {
        int16_t raw = (buf[0]<<8)| buf[1];
        me->temp=36.53+((double)raw)/340;
    }
    return result;
}


//得到陀螺仪角速度值 单位 rad/秒
HAL_StatusTypeDef Mpu6050_getGyroscope(Mpu6050 * me)
{
    uint8_t buf[6];
    int16_t rawData[3];
    //比率
    float k[4]={0.00133125,0.0026625,0.005325,0.001065};

    HAL_StatusTypeDef result =  MPU6050_readLen(me,MPU_GYRO_XOUTH_REG,6,buf);
    if(result == HAL_OK)
    {
        //16位ADC，原始值
        rawData[0] = ( buf[0] << 8) | buf[1];
        rawData[1] = ( buf[2] << 8) | buf[3];
        rawData[2] = ( buf[4] << 8) | buf[5];
        //单位转换
        me->gx = rawData[0]*k[me->gFsr];
        me->gy = rawData[1]*k[me->gFsr];
        me->gz = rawData[2]*k[me->gFsr];
    }

    return result;

}


//得到加速度值(带符号) 单位 米方/秒
HAL_StatusTypeDef Mpu6050_getAccelerometer(Mpu6050 * me)
{
    uint8_t buf[6];
    int16_t rawData[3];
    //比率= (9.8*量程绝对值)/32767
    float k[4]={0.000598163,0.001196326,0.002392652,0.004785304};

    HAL_StatusTypeDef result = MPU6050_readLen(me,MPU_ACCEL_XOUTH_REG,6,buf);
    if(result == HAL_OK)
    {
        rawData[0] = ( buf[0] << 8) | buf[1];
        rawData[1] = ( buf[2] << 8) | buf[3];
        rawData[2] = ( buf[4] << 8) | buf[5];

        me->ax =rawData[0]*k[me->aFsr];
        me->ay =rawData[1]*k[me->aFsr];
        me->az =rawData[2]*k[me->aFsr];
    }
    return result;
}

#ifdef SOFT_I2C
HAL_StatusTypeDef MPU6050_readLen(Mpu6050 * me,uint8_t p_reg,uint8_t len,uint8_t *buf)
{
    I2C_SW_Handle_t * i2chandle = (I2C_SW_Handle_t * )me->i2cHandle;
    i2chandle->slave_addr = me->readAddr;
    uint8_t r = i2c_reg_read(me->i2cHandle,p_reg,len,buf);
    if(!r)
        return HAL_OK;
    else
        return HAL_ERROR;
}
uint8_t Mpu6050_readReg(Mpu6050 * me,uint8_t regAddr)
{
    uint8_t data;
    I2C_SW_Handle_t * i2chandle = (I2C_SW_Handle_t * )me->i2cHandle;
    i2c_reg_read(i2chandle,regAddr,One_Byte,&data);
    return data;
}
void Mpu6050_writeReg(Mpu6050 * me, uint16_t reg_addr, uint8_t p_data)
{
    I2C_SW_Handle_t * i2chandle = (I2C_SW_Handle_t * )me->i2cHandle;
    i2c_reg_write(i2chandle,reg_addr,One_Byte,&p_data);
}
//HAL_I2C_Mem_Write(me->i2cHandle, me->writeAddr, reg_addr, I2C_MEMADD_SIZE_8BIT, &p_data, 1, 100);

#else
void Mpu6050_writeReg(Mpu6050 * me, uint16_t reg_addr, uint8_t p_data)
{
HAL_I2C_Mem_Write(me->i2cHandle, me->writeAddr, reg_addr, I2C_MEMADD_SIZE_8BIT, &p_data, 1, 100);
}

uint8_t Mpu6050_readReg(Mpu6050 * me,uint8_t regAddr)
{
    uint8_t data;
    HAL_I2C_Mem_Read(me->i2cHandle, me->readAddr, regAddr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}
HAL_StatusTypeDef MPU6050_readLen(Mpu6050 * me,uint8_t p_reg,uint8_t len,uint8_t *buf)
{
    return HAL_I2C_Mem_Read(me->i2cHandle, me->readAddr, p_reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
}
#endif


int Mpup6050_update(Mpu6050 *me)
{
    float norm;
    float ax,ay,az;
    float gx,gy,gz;
    float vx, vy, vz;
    float ex, ey, ez;

    float Kp =50; //互补滤波系数
    float Ki =0.0001;//0.005; //积分增益支配率的陀螺仪偏见的衔接
    float halfT = me->dt/2;
    // 读取加速度计和陀螺仪数据
    Mpu6050_getAccelerometer(me);
    Mpu6050_getGyroscope(me);

    if(me->ax*me->ay*me->az == 0)
        return-1; // 未更新
    // 加速度测量归一化
    norm = sqrt(me->ax*me->ax + me->ay*me->ay + me->az*me->az);
    ax = me->ax / norm;                   //单位化
    ay = me->ay / norm;
    az = me->az / norm;

    //提取等效旋转矩阵中的重力分量
    vx = 2*(me->quat[1]*me->quat[3] - me->quat[0]*me->quat[2]);
    vy = 2*(me->quat[0]*me->quat[1] + me->quat[2]*me->quat[3]);
    vz = me->quat[0]*me->quat[0] - me->quat[1]*me->quat[1] - me->quat[2]*me->quat[2] + me->quat[3]*me->quat[3];

    // 求姿态误差，对两向量进行叉乘(定义ex、ey、ez为三个轴误差元素),可以说是由加速度算出的重力矢量
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    // 对加速度算出的重力矢量积分增益
    me->exInt = me->exInt + ex*Ki;
    me->eyInt = me->eyInt + ey*Ki;
    me->ezInt = me->ezInt + ez*Ki;

    // 调整后的陀螺仪测量
    gx = me->gx + Kp*ex + me->exInt;
    gy = me->gy + Kp*ey + me->eyInt;
    gz = me->gz + Kp*ez + me->ezInt;

    // 整合四元数
    me->quat[0] = me->quat[0] + (-me->quat[1]*gx - me->quat[2]*gy - me->quat[3]*gz)*halfT;
    me->quat[1] = me->quat[1] + (me->quat[0]*gx + me->quat[2]*gz - me->quat[3]*gy)*halfT;
    me->quat[2] = me->quat[2] + (me->quat[0]*gy - me->quat[1]*gz + me->quat[3]*gx)*halfT;
    me->quat[3] = me->quat[3] + (me->quat[0]*gz + me->quat[1]*gy - me->quat[2]*gx)*halfT;

    // 四元数归一化
    norm = sqrt(me->quat[0]*me->quat[0] + me->quat[1]*me->quat[1] + me->quat[2]*me->quat[2] + me->quat[3]*me->quat[3]);
    me->quat[0] = me->quat[0] / norm;
    me->quat[1] = me->quat[1] / norm;
    me->quat[2] = me->quat[2] / norm;
    me->quat[3] = me->quat[3] / norm;
    me->pitch  = asin(-2 * me->quat[1] * me->quat[3] + 2 * me->quat[0]* me->quat[2]); //rad
    me->roll = atan2(2 * me->quat[2] * me->quat[3] + 2 * me->quat[0] * me->quat[1], -2 * me->quat[1] * me->quat[1] - 2 * me->quat[2]* me->quat[2] + 1);
    //me->yaw = atan2(2*(me->quat[1]*me->quat[2] + me->quat[0]*me->quat[3]),me->quat[0]*me->quat[0]+me->quat[1]*me->quat[1]-me->quat[2]*me->quat[2]-me->quat[3]*me->quat[3]) * 57.3;                //此处没有价值，注掉
    //printf("pitch=%.2f,Roll=%.2f,Yaw=%.2f,\n",Pitch,Roll);
    return 0;
}
