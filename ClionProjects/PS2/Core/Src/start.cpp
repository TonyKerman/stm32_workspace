
#include "start.h"
#include "main.h"
#include "ps2.h"
#include "spi.h"
#include "serial_debug.h"
using namespace std;
ps2outputTypeDef dat;
uint8_t cmd[9]={0x01,0x42,0};
uint8_t rawdata[9]={0X00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void ps2_receive(SPI_HandleTypeDef * spihandle,ps2outputTypeDef & data)
{

    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

    HAL_SPI_Receive(spihandle,rawdata,9,100);
//    do{ HAL_SPI_TransmitReceive(spihandle,&cmd[0],&rawdata[0],1,100);
//        HAL_Delay(1);
//        HAL_SPI_TransmitReceive(spihandle,&cmd[1],&rawdata[1],1,100);
//        HAL_Delay(1);
//        HAL_SPI_TransmitReceive(spihandle,&cmd[2],&rawdata[2],7,100);
//    } while (!(rawdata[1]==0x73&&rawdata[2]==0x5A));
    dbg_add(rawdata,"data");
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);



}

void startup()
{
    dbg_start(&huart1);
    while(1)
    {
        ps2_receive(&hspi1,dat);
        dbg_display();
        HAL_Delay(100);
    }


}