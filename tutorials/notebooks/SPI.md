# SPI

##接线

在SPI总线中，一共规定了4条线，分别含义如下:

* SCLK —串行时钟同步输出，同步数据传输，使用主机输出；
* MOSI —主机输出从机输入，主机通过该线发送数据，从机通过该线接收数据；
* MISO —主机输入从机输出，主机通过该线接收数据，从机通过该线发送数据；
* CS(NSS) —片选，主机输出，用来选中具体的从机。 cs拉低开启通信，拉高结束

### 注：
1. SPI会有主从机（或者对于MCU来说会有主模式，从模式）之分，它的区分依据是SCLK同步时钟和SS片选是由谁输出，输出方就会被定义为工作在主机（主模式）状态下。
2. 如果在一个系统中只含有一个采用SPI通信的外设，那么我们可以将外设的SS引脚直接连接板子的GND引脚，这样使得外设始终被选中，从而节省了连接线。

<https://blog.csdn.net/renqingxin2011/article/details/91047802>

<https://blog.csdn.net/as480133937/article/details/105849607>

## CubeMX
#### Mode：主机从机全双工半双工
#### hardware NSS ：硬件片选
    * enable 开启:开启后不用手动在通讯开始前拉低CS 猜测：一对一时主从机都可以开启，一对多时主机不能开启 主机选output从机选input
    * disable：需要手动在spi通讯开始前拉低CS CS的Pin要在GPIO里手动设置 一对多时需要主机定义多个pin作为 cs1 cs2 ..
#### First Bit:一个字节中,先发送哪一位
    * LSB first: 小端在前
    * MSB first: 大端在前
    
    <https://blog.csdn.net/weixin_42868654/article/details/88577851>

* 选择lsb还是msb由连接的硬件决定 ps2:lsb
#### Prescaler(分频)[Rate频率]
    根据硬件选择,不能超过硬件的相关频率
#### CPOL 时钟极性
    时钟空闲时的高低
#### CPHA(edge) 时钟相位
1edge:时钟改变的第一个边沿采样

* CPOL和CPHA根据连接的硬件时序图选择


## 代码
### stm32f4xx_hal_spi.c
 
      (#) Declare a SPI_HandleTypeDef handle structure, for example:
          SPI_HandleTypeDef  hspi;

      (#)Initialize the SPI low level resources by implementing the HAL_SPI_MspInit() API:
          (##) Enable the SPIx interface clock
          (##) SPI pins configuration
              (+++) Enable the clock for the SPI GPIOs
              (+++) Configure these SPI pins as alternate function push-pull
          (##) NVIC configuration if you need to use interrupt process
              (+++) Configure the SPIx interrupt priority
              (+++) Enable the NVIC SPI IRQ handle
          (##) DMA Configuration if you need to use DMA process
              (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive Stream/Channel
              (+++) Enable the DMAx clock
              (+++) Configure the DMA handle parameters
              (+++) Configure the DMA Tx or Rx Stream/Channel
              (+++) Associate the initialized hdma_tx(or _rx)  handle to the hspi DMA Tx or Rx handle
              (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the DMA Tx or Rx Stream/Channel

      (#) Program the Mode, BidirectionalMode , Data size, Baudrate Prescaler, NSS
          management, Clock polarity and phase, FirstBit and CRC configuration in the hspi Init structure.

      (#) Initialize the SPI registers by calling the HAL_SPI_Init() API:
          (++) This API configures also the low level Hardware GPIO, CLOCK, CORTEX...etc)
              by calling the customized HAL_SPI_MspInit() API.
     [..]
       Circular mode restriction:
      (#) The DMA circular mode cannot be used when the SPI is configured in these modes:
          (##) Master 2Lines RxOnly
          (##) Master 1Line Rx
      (#) The CRC feature is not managed when the DMA circular mode is enabled
      (#) When the SPI DMA Pause/Stop features are used, we must use the following APIs
          the HAL_SPI_DMAPause()/ HAL_SPI_DMAStop() only under the SPI callbacks
     [..]
       Master Receive mode restriction:
      (#) In Master unidirectional receive-only mode (MSTR =1, BIDIMODE=0, RXONLY=1) or
          bidirectional receive mode (MSTR=1, BIDIMODE=1, BIDIOE=0), to ensure that the SPI
          does not initiate a new transfer the following procedure has to be respected:
          (##) HAL_SPI_DeInit()
          (##) HAL_SPI_Init()
     [..]
       Callback registration:

      (#) The compilation flag USE_HAL_SPI_REGISTER_CALLBACKS when set to 1U
          allows the user to configure dynamically the driver callbacks.
          Use Functions HAL_SPI_RegisterCallback() to register an interrupt callback.

          Function HAL_SPI_RegisterCallback() allows to register following callbacks:
            (++) TxCpltCallback        : SPI Tx Completed callback
            (++) RxCpltCallback        : SPI Rx Completed callback
            (++) TxRxCpltCallback      : SPI TxRx Completed callback
            (++) TxHalfCpltCallback    : SPI Tx Half Completed callback
            (++) RxHalfCpltCallback    : SPI Rx Half Completed callback
            (++) TxRxHalfCpltCallback  : SPI TxRx Half Completed callback
            (++) ErrorCallback         : SPI Error callback
            (++) AbortCpltCallback     : SPI Abort callback
            (++) MspInitCallback       : SPI Msp Init callback
            (++) MspDeInitCallback     : SPI Msp DeInit callback
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.


      (#) Use function HAL_SPI_UnRegisterCallback to reset a callback to the default
          weak function.
          HAL_SPI_UnRegisterCallback takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (++) TxCpltCallback        : SPI Tx Completed callback
            (++) RxCpltCallback        : SPI Rx Completed callback
            (++) TxRxCpltCallback      : SPI TxRx Completed callback
            (++) TxHalfCpltCallback    : SPI Tx Half Completed callback
            (++) RxHalfCpltCallback    : SPI Rx Half Completed callback
            (++) TxRxHalfCpltCallback  : SPI TxRx Half Completed callback
            (++) ErrorCallback         : SPI Error callback
            (++) AbortCpltCallback     : SPI Abort callback
            (++) MspInitCallback       : SPI Msp Init callback
            (++) MspDeInitCallback     : SPI Msp DeInit callback

       [..]
       By default, after the HAL_SPI_Init() and when the state is HAL_SPI_STATE_RESET
       all callbacks are set to the corresponding weak functions:
       examples HAL_SPI_MasterTxCpltCallback(), HAL_SPI_MasterRxCpltCallback().
       Exception done for MspInit and MspDeInit functions that are
       reset to the legacy weak functions in the HAL_SPI_Init()/ HAL_SPI_DeInit() only when
       these callbacks are null (not registered beforehand).
       If MspInit or MspDeInit are not null, the HAL_SPI_Init()/ HAL_SPI_DeInit()
       keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.

       [..]
       Callbacks can be registered/unregistered in HAL_SPI_STATE_READY state only.
       Exception done MspInit/MspDeInit functions that can be registered/unregistered
       in HAL_SPI_STATE_READY or HAL_SPI_STATE_RESET state,
       thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
       Then, the user first registers the MspInit/MspDeInit user callbacks
       using HAL_SPI_RegisterCallback() before calling HAL_SPI_DeInit()
       or HAL_SPI_Init() function.

       [..]
       When the compilation define USE_HAL_PPP_REGISTER_CALLBACKS is set to 0 or
       not defined, the callback registering feature is not available
       and weak (surcharged) callbacks are used.

     [..]
       Using the HAL it is not possible to reach all supported SPI frequency with the different SPI Modes,
       the following table resume the max SPI frequency reached with data size 8bits/16bits,
         according to frequency of the APBx Peripheral Clock (fPCLK) used by the SPI instance.

  @endverbatim

  Additional table :

       DataSize = SPI_DATASIZE_8BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/8  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/8  | Fpclk/8  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/4  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+

       DataSize = SPI_DATASIZE_16BIT:
       +----------------------------------------------------------------------------------------------+
       |         |                | 2Lines Fullduplex   |     2Lines RxOnly    |         1Line        |
       | Process | Transfer mode  |---------------------|----------------------|----------------------|
       |         |                |  Master  |  Slave   |  Master   |  Slave   |  Master   |  Slave   |
       |==============================================================================================|
       |    T    |     Polling    | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    /    |     Interrupt  | Fpclk/4  | Fpclk/4  |    NA     |    NA    |    NA     |   NA     |
       |    R    |----------------|----------|----------|-----------|----------|-----------|----------|
       |    X    |       DMA      | Fpclk/2  | Fpclk/2  |    NA     |    NA    |    NA     |   NA     |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/32  | Fpclk/2  |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    R    |     Interrupt  | Fpclk/4  | Fpclk/4  | Fpclk/64  | Fpclk/2  | Fpclk/64  | Fpclk/2  |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  | Fpclk/64  | Fpclk/2  | Fpclk/128 | Fpclk/2  |
       |=========|================|==========|==========|===========|==========|===========|==========|
       |         |     Polling    | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/32 |
       |         |----------------|----------|----------|-----------|----------|-----------|----------|
       |    T    |     Interrupt  | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/64 |
       |    X    |----------------|----------|----------|-----------|----------|-----------|----------|
       |         |       DMA      | Fpclk/2  | Fpclk/2  |     NA    |    NA    | Fpclk/2   | Fpclk/128|
       +----------------------------------------------------------------------------------------------+
       @note The max SPI frequency depend on SPI data size (8bits, 16bits),
             SPI mode(2 Lines fullduplex, 2 lines RxOnly, 1 line TX/RX) and Process mode (Polling, IT, DMA).
       @note
            (#) TX/RX processes are HAL_SPI_TransmitReceive(), HAL_SPI_TransmitReceive_IT() and HAL_SPI_TransmitReceive_DMA()
            (#) RX processes are HAL_SPI_Receive(), HAL_SPI_Receive_IT() and HAL_SPI_Receive_DMA()
            (#) TX processes are HAL_SPI_Transmit(), HAL_SPI_Transmit_IT() and HAL_SPI_Transmit_DMA()

  */
### HAL_SPI_TransmitReceive()  发同时收
### 注意：transmit一次最好只发一个，否则进不了中断
##例子： spi1 和 spi2通信
        #include "start.h"
        #include "main.h"
        #include "gpio.h"
        #include "usart.h"
        #include "spi.h"
        #include <string>
        using namespace std;
        
        string str;
        uint8_t t[8] = {1,2,3,4,5,6,7,8};
        uint8_t text[10];
        int count =0;
        
        void setup()
        {
            HAL_SPI_Receive_IT(&hspi3,text,8);
            while(1)
            {
                for(uint8_t i=0; i<=7;i++)
                {
                    //注意&
                    HAL_SPI_Transmit(&hspi2,(uint8_t *) &t[i],1,100);
                }
                HAL_Delay(500);
            }
        }
        
        void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
        {
        
            HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
            count++;
            str = "i heard ";
            for(int i=0;i<=9;i++)
            {
                str+=",";
                str += to_string(text[i]);
        
            }
            str += to_string(count);
            HAL_UART_Transmit(&huart2,(uint8_t *)str.data(),str.size(),100);
            HAL_SPI_Receive_IT(&hspi3,text,8);
        
        }





