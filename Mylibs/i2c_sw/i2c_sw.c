//
// Created by tony on 23-11-10.
//

#include "i2c_sw.h"
#include "sys_delay.h"
/**
 * @description: 延时函数，用于控制i2c频率
 * @param {I2C_SW_Speed_Mode_e} speed_mode
 * @return {*}
 */
//static void i2c_delay(I2C_SW_Speed_Mode_e speed_mode);
#define i2c_delay(speed_mode) Delay_us(1000000/(speed_mode * 2))
/**
 * @description: 将时钟信号拉低
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void scl_l(I2C_SW_Handle_t* handle);

/**
 * @description: 将时钟信号拉高
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void scl_h(I2C_SW_Handle_t* handle);

/**
 * @description: 将数据信号拉低
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void sda_l(I2C_SW_Handle_t* handle);

/**
 * @description: 将数据信号拉高
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void sda_h(I2C_SW_Handle_t* handle);

/**
 * @description: 读取数据信号的电平
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static char sda_read(I2C_SW_Handle_t* handle);


/**
 * @description: 产生i2c起始信号
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void i2c_start(I2C_SW_Handle_t* handle);

/**
 * @description: 产生i2c终止信号
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void i2c_stop(I2C_SW_Handle_t* handle);

/**
 * @description: 产生i2c应答信号
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void i2c_ack(I2C_SW_Handle_t* handle);

/**
 * @description: 产生i2c非应答信号
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static void i2c_no_ack(I2C_SW_Handle_t* handle);

/**
 * @description: 等待i2c应答信号
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static bool i2c_wait_ack(I2C_SW_Handle_t* handle);

/**
 * @description: i2c发送一字节数据
 * @param {I2C_SW_Handle_t*} handle
 * @param {char} data
 * @return {*}
 */
static void i2c_send_byte(I2C_SW_Handle_t* handle, char data);

/**
 * @description: i2c读取一字节数据
 * @param {I2C_SW_Handle_t*} handle
 * @return {*}
 */
static char i2c_recv_byte(I2C_SW_Handle_t* handle);

/**
 * @description: 发送slave address信息
 * @param {I2C_SW_Handle_t*} handle
 * @param {char} rw 读或者写方向
 * @return {*}
 */
static bool i2c_send_slave_address_with_wait_ack(I2C_SW_Handle_t* handle, char rw);

/**
 * @description: 发送要读或写的地址信息
 * @param {I2C_SW_Handle_t*} handle
 * @param {uint32_t} address
 * @return {*}
 */
static bool i2c_send_one_reg_address_with_wait_ack(I2C_SW_Handle_t* handle, uint32_t address);

/**
 * @description: 接收寄存器地址值信息
 * @param {I2C_SW_Handle_t*} handle
 * @param {uint32_t*} value
 * @param {bool} no_ack
 * @return {*}
 */
static bool i2c_recv_one_reg_value_with_ack(I2C_SW_Handle_t* handle, uint32_t* value, bool no_ack);

/**
 * @description: 发送寄存器值信息
 * @param {I2C_SW_Handle_t*} handle
 * @param {uint32_t} value
 * @return {*}
 */
static bool i2c_send_one_reg_value_with_wait_ack(I2C_SW_Handle_t* handle, uint32_t value);

//static void i2c_delay(I2C_SW_Speed_Mode_e speed_mode)
//{
//    uint32_t temp;
//    SysTick->LOAD = SystemCoreClock / 8 / (speed_mode * 2) - 1;
//    SysTick->VAL = 0X00; //
//    SysTick->CTRL = 0X01; //
//    do
//    {
//        temp = SysTick->CTRL; //
//    }
//    while((temp & 0x01) && (!(temp & (1 << 16)))); //
//    SysTick->CTRL = 0x00; //
//    SysTick->VAL = 0X00; //
//}

static void scl_l(I2C_SW_Handle_t* handle)
{
    HAL_GPIO_WritePin(handle->scl_port, handle->scl_pin, GPIO_PIN_RESET);
}

static void scl_h(I2C_SW_Handle_t* handle)
{
    HAL_GPIO_WritePin(handle->scl_port, handle->scl_pin, GPIO_PIN_SET);
}

static void sda_l(I2C_SW_Handle_t* handle)
{
    HAL_GPIO_WritePin(handle->sda_port, handle->sda_pin, GPIO_PIN_RESET);
}

static void sda_h(I2C_SW_Handle_t* handle)
{
    HAL_GPIO_WritePin(handle->sda_port, handle->sda_pin, GPIO_PIN_SET);
}
static char sda_read(I2C_SW_Handle_t* handle)
{
    return HAL_GPIO_ReadPin(handle->sda_port, handle->sda_pin);
}

static void i2c_start(I2C_SW_Handle_t* handle)
{
    scl_h(handle);
    sda_h(handle);
    i2c_delay(handle->speed_mode);

    sda_l(handle);
    i2c_delay(handle->speed_mode);

    scl_l(handle);
    i2c_delay(handle->speed_mode);
}

static void i2c_stop(I2C_SW_Handle_t* handle)
{
    scl_h(handle);
    i2c_delay(handle->speed_mode);

    sda_l(handle);
    i2c_delay(handle->speed_mode);

    sda_h(handle);
    i2c_delay(handle->speed_mode);
}

static void i2c_ack(I2C_SW_Handle_t* handle)
{
    scl_l(handle);
    i2c_delay(handle->speed_mode);

    sda_l(handle);
    i2c_delay(handle->speed_mode);

    scl_h(handle);
    i2c_delay(handle->speed_mode);

    scl_l(handle);
    i2c_delay(handle->speed_mode);

    sda_h(handle);
    i2c_delay(handle->speed_mode);
}

static void i2c_no_ack(I2C_SW_Handle_t* handle)
{
    sda_h(handle);
    i2c_delay(handle->speed_mode);

    scl_h(handle);
    i2c_delay(handle->speed_mode);

    scl_l(handle);
    i2c_delay(handle->speed_mode);
}

static bool i2c_wait_ack(I2C_SW_Handle_t* handle)
{
    sda_h(handle);
    i2c_delay(handle->speed_mode);

    scl_h(handle);
    i2c_delay(handle->speed_mode);

    int retry_cnt = 10;
    while(retry_cnt--)
    {
        if(!sda_read(handle))
        {
            scl_l(handle);
            i2c_delay(handle->speed_mode);
            return true;
        }
    }
    scl_l(handle);
    i2c_delay(handle->speed_mode);

    return false;
}

static void i2c_send_byte(I2C_SW_Handle_t* handle, char data)
{
    for(int offset = 0; offset < 8; offset++)
    {
        scl_l(handle);
        i2c_delay(handle->speed_mode);

        if(data & 0x80)
            sda_h(handle);
        else
            sda_l(handle);
        i2c_delay(handle->speed_mode);

        scl_h(handle);
        i2c_delay(handle->speed_mode);

        data <<= 1;
    }

    scl_l(handle);
    i2c_delay(handle->speed_mode);
}

static char i2c_recv_byte(I2C_SW_Handle_t* handle)
{
    sda_h(handle);
    i2c_delay(handle->speed_mode);

    char value = 0;
    for(int offset = 0; offset < 8; offset++)
    {
        scl_h(handle);
        i2c_delay(handle->speed_mode);

        value <<= 1;
        if(sda_read(handle))
            value++;

        scl_l(handle);
        i2c_delay(handle->speed_mode);
    }

    return value;
}

static bool i2c_send_slave_address_with_wait_ack(I2C_SW_Handle_t* handle, char rw)
{
    if(handle->slave_address_width == Slave_Address_7Bit)
    {
        i2c_send_byte(handle, (handle->slave_addr << 1) | rw);
        if(!i2c_wait_ack(handle))
            return false;
    }
    else if(handle->slave_address_width == Slave_Address_10Bit)
    {
        uint8_t h_addr = (I2C_10BIT_SLAVE_ADDRESS_INDICATOR << 7)
                         | ((handle->slave_addr >> 8 & 0x03) << 2) | rw;
        i2c_send_byte(handle, h_addr);
        if(!i2c_wait_ack(handle))
            return false;

        uint8_t l_addr = handle->slave_addr & 0xff;
        i2c_send_byte(handle, l_addr);
        if(!i2c_wait_ack(handle))
            return false;
    }
    return true;
}

static bool i2c_send_one_reg_address_with_wait_ack(I2C_SW_Handle_t* handle, uint32_t address)
{
    for(int j = 0; j < handle->slave_reg_addr_width; j++)
    {
        uint8_t offset = (handle->slave_reg_addr_width - 1 - j) * 8;
        uint8_t temp_reg_addr = (address >> offset) & 0xff;

        i2c_send_byte(handle, temp_reg_addr);
        if(!i2c_wait_ack(handle))
            return false;
    }
    return true;
}

static bool i2c_recv_one_reg_value_with_ack(I2C_SW_Handle_t* handle, uint32_t* value, bool no_ack)
{
    for(int k = 0; k < handle->slave_reg_value_width; k++)
    {
        uint8_t offset = (handle->slave_reg_value_width - 1 - k) * 8;
        *value |= (i2c_recv_byte(handle) << offset);

        if(no_ack)
            i2c_no_ack(handle);
        else
            i2c_ack(handle);
    }
    return true;
}

static bool i2c_send_one_reg_value_with_wait_ack(I2C_SW_Handle_t* handle, uint32_t value)
{
    for(int k = 0; k < handle->slave_reg_value_width; k++)
    {
        uint8_t offset = (handle->slave_reg_value_width - 1 - k) * 8;

        i2c_send_byte(handle, (value >> offset) & 0xff);
        if(!i2c_wait_ack(handle))
            return false;
    }
    return true;
}
bool i2c_device_exist(I2C_SW_Handle_t* handle)
{
    bool ret = 0;

    i2c_start(handle);

    ret = i2c_send_slave_address_with_wait_ack(handle, I2C_WRITE);

    i2c_stop(handle);

    return ret;
}

int i2c_reg_read(I2C_SW_Handle_t* handle, uint32_t reg_addr, uint32_t len, uint32_t* buf)
{
    i2c_start(handle);

    if(!i2c_send_slave_address_with_wait_ack(handle, I2C_WRITE))
    {
        i2c_stop(handle);
        return -1;
    }

    if(!i2c_send_one_reg_address_with_wait_ack(handle, reg_addr))
    {
        i2c_stop(handle);
        return -1;
    }

    i2c_start(handle);

    if(!i2c_send_slave_address_with_wait_ack(handle, I2C_READ))
    {
        i2c_stop(handle);
        return -1;
    }

    for(int i = 0; i < len; i++)
    {
        i2c_recv_one_reg_value_with_ack(handle, &buf[i], len - 1 == i);
    }
    i2c_stop(handle);
    return 0;

}

int i2c_reg_write(I2C_SW_Handle_t* handle, uint32_t reg_addr, uint32_t len, const uint32_t* buf)
{
    i2c_start(handle);

    if(!i2c_send_slave_address_with_wait_ack(handle, I2C_WRITE))
    {
        i2c_stop(handle);
        return -1;
    }

    if(!i2c_send_one_reg_address_with_wait_ack(handle, reg_addr))
    {
        i2c_stop(handle);
        return -1;
    }

    for(int i = 0; i < len; i++)
    {
        if(! i2c_send_one_reg_value_with_wait_ack(handle, buf[i]))
        {
            i2c_stop(handle);
            return -1;
        }
    }

    i2c_stop(handle);
    return 0;


}