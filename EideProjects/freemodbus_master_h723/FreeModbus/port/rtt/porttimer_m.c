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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/


#include "mb_m.h"
#include "mbport.h"
#include "tim.h"
#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/

static USHORT usT35TimeOut50us; //1750us
/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us;//波特率大于19200时=50us
    __HAL_TIM_ENABLE(&MODBUS_TIMER);
    return TRUE;
}

void  vMBMasterPortTimersT35Enable()
{
    __HAL_TIM_SET_AUTORELOAD(&MODBUS_TIMER, 35);// 1750us/1tick = 35
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    //STM32HAL库定时器打开就进一次中断的问题
    //原因是定时器初始化时没有清除中断标志位，所以在打开定时器中断前将中断标志位清除一下就可以了
    __HAL_TIM_CLEAR_FLAG(&MODBUS_TIMER,TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&MODBUS_TIMER);
}

void vMBMasterPortTimersConvertDelayEnable()
{
    __HAL_TIM_SET_AUTORELOAD(&MODBUS_TIMER, 4000);//MB_MASTER_DELAY_MS_CONVERT(200ms)/1tick = 4000
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
    __HAL_TIM_CLEAR_FLAG(&MODBUS_TIMER,TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&MODBUS_TIMER);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    __HAL_TIM_SET_AUTORELOAD(&MODBUS_TIMER, 2000);//MB_MASTER_DELAY_MS_CONVERT(200ms)/1tick = 4000
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    __HAL_TIM_CLEAR_FLAG(&MODBUS_TIMER,TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&MODBUS_TIMER);
}

void vMBMasterPortTimersDisable()
{
    // __HAL_TIM_DISABLE(&MODBUS_TIMER);
    HAL_TIM_Base_Stop_IT(&MODBUS_TIMER);
}

void prvvTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
    
    /*以下代码添加到 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
    if(htim->Instance == MODBUS_TIMER.Instance)
    {
    prvvTIMERExpiredISR();
    }
    */
}






#endif
