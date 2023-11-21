//
// Created by tony on 23-11-10.
//

#ifndef I2C_SW_H
#define I2C_SW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define I2C_WRITE (0x00)
#define I2C_READ  (0x01)
#define I2C_10BIT_SLAVE_ADDRESS_INDICATOR (0b11110)

    typedef enum
    {
        Standard_Mode = 100 * 1000,
        Fast_Mode = 400 * 1000,
        Fast_Mode_Plus = 1000 * 1000,
    } I2C_SW_Speed_Mode_e; //i2c速率 unit Hz

    typedef enum
    {
        One_Byte = 1,
        Two_Byte = 2,
        Four_Byte = 4
    } Data_Width_e;//数据长度（寄存器地址或者值的数据长度）

    typedef enum
    {
        Slave_Address_7Bit,
        Slave_Address_10Bit,
    } Slave_Address_Width_e;//i2c slave address的位宽

    typedef struct
    {
        uint16_t scl_pin;
        uint16_t sda_pin;
        GPIO_TypeDef* scl_port;
        GPIO_TypeDef* sda_port;
        I2C_SW_Speed_Mode_e speed_mode;
        uint16_t slave_addr;
        Slave_Address_Width_e slave_address_width;
        Data_Width_e slave_reg_addr_width;
        Data_Width_e slave_reg_value_width;
    } I2C_SW_Handle_t;

    /**
     * @description: 检测slave address的i2c设备是否存在
     * @param {I2C_SW_Handle_t*} handle
     * @return {*}
     */
    bool i2c_device_exist(I2C_SW_Handle_t* handle);

    /**
     * @description: 读寄存器
     * @param {I2C_SW_Handle_t*} handle
     * @param {uint32_t} reg_addr
     * @param {uint32_t} len
     * @param {uint32_t*} buf
     * @return {*}
     */
    int i2c_reg_read(I2C_SW_Handle_t* handle, uint32_t reg_addr, uint32_t len, uint32_t* buf);

    /**
     * @description: 写寄存器
     * @param {I2C_SW_Handle_t*} handle
     * @param {uint32_t} reg_addr
     * @param {uint32_t} len
     * @param {uint32_t*} buf
     * @return {*}
     */
    int i2c_reg_write(I2C_SW_Handle_t* handle, uint32_t reg_addr, uint32_t len, const uint32_t* buf);

#ifdef __cplusplus
}
#endif

#endif //I2C_SW_H
