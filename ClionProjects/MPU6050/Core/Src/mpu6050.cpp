//
// Created by tony on 2023/3/4.
//

#include "mpu6050.h"

void MPU::Read_Temp(void) {

}


void MPU::write_reg(uint16_t MemAddress, uint8_t pData) {
    HAL_I2C_Mem_Write(i2c,dev_addr,MemAddress,I2C_MEMADD_SIZE_8BIT,&pData,1,10);

}
uint8_t MPU::read_reg(uint16_t MemAddress) {
    uint8_t data;
    HAL_I2C_Mem_Read(i2c,dev_addr,MemAddress,I2C_MEMADD_SIZE_8BIT,&data,1,10);
    return data;
}

MPU::MPU(I2C_HandleTypeDef *mi2c,uint8_t mdev_addr) {
    i2c = mi2c;
    dev_addr =mdev_addr;
}

uint8_t MPU::init() {
    write_reg(MPU_PWR_MGMT1_REG, 0x00);//解除休眠状态
   
//    write_reg(MPU_SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(125Hz)
//    HAL_Delay(10);
//    write_reg(MPU_CONFIG, 0x06);        //低通滤波频率，典型值：0x06(5Hz)
//    HAL_Delay(10);
//    write_reg(MPU_GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
//    HAL_Delay(10);
//    write_reg(MPU_ACCEL_CONFIG, 0x01);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

    write_reg(MPU_INT_EN_REG,0X00);	//关闭所有中断
    write_reg(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
    write_reg(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    write_reg(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
    uint8_t who_am_i = read_reg(0x75);
    if(!who_am_i)
        return 0;
    return who_am_i;
}




