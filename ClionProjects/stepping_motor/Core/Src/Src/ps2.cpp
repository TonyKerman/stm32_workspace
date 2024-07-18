//
// Created by tony on 2023/2/25.
//
#include "ps2.h"
#include "main.h"
#include "serial_debug.h"
#include "gpio.h"
//uint8_t cmd[9]={0x01,0x42,0};
//uint8_t rawdata[9]={0X00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//
//
//
//void ps2_receive(SPI_HandleTypeDef * spihandle,ps2outputTypeDef & data)
//{
//
//
//    do{ HAL_SPI_TransmitReceive(spihandle,&cmd[0],&rawdata[0],1,100);
//        HAL_Delay(1);
//        HAL_SPI_TransmitReceive(spihandle,&cmd[1],&rawdata[1],1,100);
//        HAL_Delay(1);
//        HAL_SPI_TransmitReceive(spihandle,&cmd[2],&rawdata[2],7,100);
//    } while (!(rawdata[1]==0x73&&rawdata[2]==0x5A));
//    dbg_add(rawdata,"data");
//
//
//
//
//}
//
//void delay_us(uint16_t time)
//{
//    uint16_t i=0;
//    while(time--)
//    {
//        i=10;  //自己定义
//        while(i--) ;
//    }
//}


//uint8_t cmd[3] = {0x01,0x42,0x00};  // 请求接受数据
//uint8_t PS2data[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};   //存储手柄返回数据

//void PS2_Get(void)    //接受ps2数据
//{
//    uint8_t i = 0;
//
//
//
//    HAL_SPI_TransmitReceive(&hspi2,&cmd[0],&PS2data[0],1,0xffff); // 发送0x01，请求接受数据
//    delay_us(10);
//    HAL_SPI_TransmitReceive(&hspi2,&cmd[1],&PS2data[1],1,0xffff); // 发送0x42，接受0x01（PS2表示开始通信）
//    delay_us(10);
//    HAL_SPI_TransmitReceive(&hspi2,&cmd[2],&PS2data[2],1,0xffff); // 发送0x00，接受ID（红绿灯模式）
//    delay_us(10);
//    for(i = 3;i <9;i++)
//    {
//        HAL_SPI_TransmitReceive(&hspi2,&cmd[2],&PS2data[i],1,0xffff); // 接受数据
//        delay_us(10);
//
//    }
//    for(int i=0;i<9;i++ )
//    {
//        dbg_add(PS2data[i],"val");
//    }
//
//
//}