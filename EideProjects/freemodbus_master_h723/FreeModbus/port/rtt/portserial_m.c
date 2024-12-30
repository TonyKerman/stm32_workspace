/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "usart.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define MODBUS_USART_HANDLE       huart2
// #define HARDWARE_RS485_CONTROL     

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/
// ALIGN(RT_ALIGN_SIZE)
/* software simulation serial transmit IRQ handler thread stack */
/* software simulation serial transmit IRQ handler thread */
// static struct rt_thread thread_serial_soft_trans_irq;
/* serial event */
// static struct rt_event event_serial;
EventGroupHandle_t event_serial;
/* modbus master serial device */


/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{
    /**
     * set 485 mode receive and transmit control IO
     * @note MODBUS_MASTER_RT_CONTROL_PIN_INDEX need be defined by user
     */

    /* set serial name */


    /* set serial configure parameter */

    /* software initialize */
    event_serial = xEventGroupCreate();
    return TRUE;
}

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    uint32_t recved_event;
    if (xRxEnable)
    {
        /* enable RX interrupt */
        // serial->ops->control(serial, RT_DEVICE_CTRL_SET_INT, (void *)RT_DEVICE_FLAG_INT_RX);
        uint8_t ucByte;
        HAL_UART_Receive_IT(&MODBUS_USART_HANDLE, &ucByte, 1);
        /* switch 485 to receive mode */
        #ifdef HARDWARE_RS485_CONTROL
        // rt_pin_write(MODBUS_MASTER_RT_CONTROL_PIN_INDEX, PIN_LOW);
        HAL_GPIO_WritePin(MODBUS_CONTROL_PIN_PORT,MODBUS_CONTROL_PIN, GPIO_PIN_RESET);
        #endif
    }
    else
    {
        /* switch 485 to transmit mode */
        #ifdef HARDWARE_RS485_CONTROL
        // rt_pin_write(MODBUS_MASTER_RT_CONTROL_PIN_INDEX, PIN_LOW);
        HAL_GPIO_WritePin(MODBUS_CONTROL_PIN_PORT,MODBUS_CONTROL_PIN, GPIO_PIN_SET);
        #endif
        /* disable RX interrupt */
        // serial->ops->control(serial, RT_DEVICE_CTRL_CLR_INT, (void *)RT_DEVICE_FLAG_INT_RX);
    }
    if (xTxEnable)
    {
        /* start serial transmit */
        // rt_event_send(&event_serial, EVENT_SERIAL_TRANS_START);
        xEventGroupSetBits(event_serial,EVENT_SERIAL_TRANS_START);
        // __HAL_UART_ENABLE_IT(&MODBUS_USART_HANDLE,UART_IT_TXE);
    }
    else
    {
        /* stop serial transmit */
        recved_event=xEventGroupWaitBits(&event_serial,EVENT_SERIAL_TRANS_START,pdTRUE,pdFALSE,0);
    }
}

void vMBMasterPortClose(void)
{
    // serial->parent.close(&(serial->parent));
    // HAL_UART_Abort()
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
    // serial->parent.write(&(serial->parent), 0, &ucByte, 1);
    HAL_UART_Transmit_IT(&MODBUS_USART_HANDLE,&ucByte,1);
    return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
    // serial->parent.read(&(serial->parent), 0, pucByte, 1);
    HAL_UART_Receive_IT(&MODBUS_USART_HANDLE,pucByte,1);
    return TRUE;
}

/*
 * 为发送缓冲区空中断创建中断处理程序
 * 或类似函数）。然后，该函数应
 * 调用 pxMBFrameCBTransmitterEmpty( )，它将告诉协议栈
 * 可以发送新字符。然后，协议栈将调用
 * xMBPortSerialPutByte( ) 发送字符。
 */
void prvvUARTTxReadyISR(void)
{
    pxMBMasterFrameCBTransmitterEmpty();
}


/*
 * 为目标处理器的接收中断创建中断处理程序。
 * 处理器的接收中断创建中断处理程序。然后，该函数应调用 pxMBFrameCBByteReceived( )。然后
 * 然后，协议栈将调用 xMBPortSerialGetByte( ) 来获取字符。
 * 字符。
 */
void prvvUARTRxISR(void)
{
    pxMBMasterFrameCBByteReceived();
}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
// static void serial_soft_trans_irq(void* parameter) {
//     uint32_t recved_event;
//     while (1)
//     {
//         /* waiting for serial transmit start */
//         xEventGroupWaitBits(event_serial,EVENT_SERIAL_TRANS_START,pdTRUE,pdFALSE,portMAX_DELAY)
//         /* execute modbus callback */
//         prvvUARTTxReadyISR();
//     }
// }
HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&MODBUS_USART_HANDLE)
    {
        prvvUARTTxReadyISR();
    }
}
/**
 * This function is serial receive callback function
 *
 * @param dev the device of serial
 * @param size the data size that receive
 *
 * @return return RT_EOK
 */
HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart==&MODBUS_USART_HANDLE)
    {
        prvvUARTRxISR();
    }
}

#endif
