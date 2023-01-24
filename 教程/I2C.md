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


##例子：读取陀螺仪加速度传感器MPU-6500数据
1. 数据手册 <http://invensense.wpenginepowered.com/wp-content/uploads/2020/06/PS-MPU-6500A-01-v1.3.pdf>
可知 The slave address of the MPU-6500 is b110100X which is 7 bits long.

2. 寄存器地址表 <https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf>

3. 启动方式 见数据手册 5.1 WAKE-ON-MOTION INTERRUPT 

        Make Sure Accel is running:
        • In PWR_MGMT_1 (0x6B) make CYCLE =0, SLEEP = 0 and STANDBY = 0
        • In PWR_MGMT_2 (0x6C) set DIS_XA, DIS_YA, DIS_ZA = 0 and DIS_XG, DIS_YG, DIS_ZG = 1
        Set Accel LPF setting to 184 Hz Bandwidth:
        • In ACCEL_CONFIG 2 (0x1D) set ACCEL_FCHOICE_B = 0 and A_DLPFCFG[2:0]=1(b001)
        Enable Motion Interrupt:
        • In INT_ENABLE (0x38), set the whole register to 0x40 to enable motion interrupt only.
        Enable Accel Hardware Intelligence:
        • In MOT_DETECT_CTRL (0x69), set ACCEL_INTEL_EN = 1 and ACCEL_INTEL_MODE = 1
        Set Motion Threshold:
        • In WOM_THR (0x1F), set the WOM_Threshold [7:0] to 1~255 LSBs (0~1020mg)
        Set Frequency of Wake-up:
        • In LP_ACCEL_ODR (0x1E), set Lposc_clksel [3:0] = 0.24Hz ~ 500Hz
        Enable Cycle Mode (Accel Low Power Mode):
        • In PWR_MGMT_1 (0x6B) make CYCLE =1
        Motion Interrupt Configuration Completed

4.实现：

* 初始化 使用 HAL_I2C_Mem_Write()对传感器进行设置（根据3）
* 读取数值 根据寄存器地址表 59-72号寄存器对应了传感器数值，HAL_I2C_Mem_Read(）

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





