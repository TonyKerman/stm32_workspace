#I2C
##原理

<https://blog.csdn.net/as480133937/article/details/105366932>
##物理层：接线
SCL<---->SCL 

SDK<---->SDK
注意：4.7k上拉
##代码
详见 stm32f4xx_hal_i2c.h

仍然分为 轮询，中断 DMA 三种

与串口不同点： 

1. 分为 主机Master 从机Slave 
2. 需要提供 设备地址 DevAddress [寄存器地址MemAddress]

### 常用：
1. 读取寄存器（常用于读取某个传感器的值）：
    
        HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
        HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);#回调函数：HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
        HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

其中 设备地址 DevAddress [寄存器地址MemAddress] 要查阅传感器手册

2.写寄存器：

(同理，有IT和DMA)

        HAL_I2C_Mem_Write();

3.作为主机发送数据：

(同理，有IT和DMA)
(作为从机就是Slave)
(接收就是Receive)

        HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);


##例子：i2c1 和 i2c2通信

        #include "start.h"
        #include "main.h"
        #include "gpio.h"
        #include "i2c.h"
        #include "usart.h"
        #include <string>
        using namespace std;
        
        string str;
        uint8_t text[10];
        int count =0;
        
        void setup()
        {
            HAL_I2C_Slave_Receive_IT(&hi2c2,text,1);
            while(1)
            {
                HAL_I2C_Master_Transmit(&hi2c1,0x11,(uint8_t*)"hello",5,100);
                HAL_Delay(500);
            }
        }
        
        void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
        {
        
            HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
            count++;
            str = "i heard ";
            //str += text;
            str += std::to_string(count);
            HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),100);
            HAL_I2C_Slave_Receive_IT(&hi2c2,(uint8_t *)text,1);
        
        
        }





