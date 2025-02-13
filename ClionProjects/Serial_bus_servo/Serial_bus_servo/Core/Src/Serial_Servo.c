//
// Created by tony on 2023/4/24.
//

#include "Serial_Servo.h"
#include <stdlib.h>
#include "retarget.h"
//宏函数 获得A的低八位
#define GET_LOW_BYTE(A) ((uint8_t)(A))
//宏函数 获得A的高八位
#define GET_HIGH_BYTE(A) ((uint8_t)((A) >> 8))
//宏函数 将高地八位合成为十六位
#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))



Serial_Servo * Serial_Servo_Create(UART_HandleTypeDef * uartx, uint8_t id)
{
    Serial_Servo * me =(Serial_Servo*)malloc(sizeof (Serial_Servo));
    me->uartHandle=uartx;
    me->id = id;
    return me;
}

/*
校验和 计算方法如下：
Checksum=~(ID+Length+Cmd+Prm1+...PrmN)若括号内的计算和超出255,则取最低的一个字节，
“~”表示取反。
*/
uint8_t CheckSum(uint8_t buf[])
{
    uint8_t i;
    uint16_t temp = 0;
    for (i = 2; i < buf[3] + 2; i++) {
        temp += buf[i];
    }
    temp = ~temp;
    i = (uint8_t)temp;
    return i;
}

//设置ID
void Serial_Servo_SetID(UART_HandleTypeDef * uartHandle,uint8_t oldID, uint8_t newID)
{
    uint8_t buf[7];
    buf[0] = buf[1] = SERIAL_SERVO_FRAME_HEADER;
    buf[2] = oldID;
    buf[3] = 4;
    buf[4] = SERIAL_SERVO_ID_WRITE;
    buf[5] = newID;
    buf[6] = CheckSum(buf);
    HAL_HalfDuplex_EnableTransmitter(uartHandle);
    HAL_UART_Transmit(uartHandle,buf,7,1000);
}

//读取ID
int Serial_Servo_ReadID(UART_HandleTypeDef * uartHandle)
{
    uint8_t buf[6];
    uint8_t ref[7];
    buf[0] = buf[1] = SERIAL_SERVO_FRAME_HEADER;
    buf[2] = 0xfe;
    buf[3] = 3;
    buf[4] =SERIAL_SERVO_ID_READ;
    buf[5] = CheckSum(buf);
    HAL_HalfDuplex_EnableTransmitter(uartHandle);
    HAL_UART_Transmit(uartHandle,buf,6,1000);
    HAL_HalfDuplex_EnableReceiver(uartHandle);
    HAL_UART_Receive(uartHandle,ref,7,1000);
    //printf("ID:%d\n",(int)ref[5]);
    return (int)ref[5];
}

//写一个命令
void Serial_Servo_WriteCmd(Serial_Servo * me,uint8_t cmdName,uint8_t* pArgs,uint8_t size)
{
    //每一个命令对应的数据长度，数组下标是命令序号，“0”是占位符 ，具体命令对照.h中宏定义
    const int cmds[36] = {0,7, 3, 0,0,0,0,7, 3,0,0, 3, 3, 4, 3,0,0, 4, 3, 3, 7, 3, 7, 3, 4, 3, 3, 3, 3, 7, 3, 4, 3, 4, 3, 4};
    uint8_t buf[6+size];
    buf[0] = buf[1] = SERIAL_SERVO_FRAME_HEADER;
    buf[2] = me->id;
    buf[3] = cmds[cmdName];
    buf[4] = cmdName;
    for (int i = 0; i < size; i++) {
        buf[4+i+1]=*(pArgs+i);
    }
    buf[5+size] = CheckSum(buf);
    HAL_HalfDuplex_EnableTransmitter(me->uartHandle);
    HAL_UART_Transmit(me->uartHandle,buf,6+size,1000);
}

//读命令
void Serial_Servo_ReadCmd(Serial_Servo * me,uint8_t cmdName,uint8_t* pData)
{
    //第一个元素是读命令的序号，第二个是其对应的数据长度
    const uint8_t readCmds[14][2]={{2,7},{8,7},{14,7},{14,4},
                                {19,4},{21,7},{23,7},{25,4},
                                {26,4},{27,5},{28,5},{30,7},
                                {32,4},{34,4},{36,4}};
    int dataLen;
    for(int i=2;i<=36;i++)
    {
        if(readCmds[i][0]==cmdName)
        {
            dataLen = readCmds[i][1];
            break;
        }
    }
    uint8_t ref[dataLen+3];

    Serial_Servo_WriteCmd(me,cmdName,NULL,0);
    HAL_HalfDuplex_EnableReceiver(me->uartHandle);
    HAL_UART_Receive(me->uartHandle,ref,dataLen+3,100);
    for(int i=0;i<dataLen-3;i++)
        *(pData+i)=ref[5+i];
}

/*
*舵机转动函数
*position:目标位置，范围 0~1000，对应舵机角度的 0~240°，即舵机可变化的最小角度为 0.24 度。 
*time:时间的范围 0~30000 毫秒。该命令发送给舵机，舵机将在参数时间内从当前角度匀速转动到参
*     数角度。该指令到达舵机后，舵机会立即转动。
*/
void Serial_Servo_Move(Serial_Servo *me, int16_t position, uint16_t time)
{
    if(position < 0)
        position = 0;
    if(position > 1000)
        position = 1000;
    uint8_t args[4];
    args[0] = GET_LOW_BYTE(position);
    args[1] = GET_HIGH_BYTE(position);
    args[2] = GET_LOW_BYTE(time);
    args[3] = GET_HIGH_BYTE(time);
    Serial_Servo_WriteCmd(me,SERIAL_SERVO_MOVE_TIME_WRITE,args,4);
}

/*
*设置舵机内部电机是否卸载掉电
*is_load:范围 0 或 1，0 代表卸载掉电，此时舵机
*	 无力矩输出。1 代表装载电机，此时舵机有力矩输出
*
*/

void Ser_Servo_SetLoadOrUnload(Serial_Servo * me,uint8_t is_load)
{
    Serial_Servo_WriteCmd(me,SERIAL_SERVO_LOAD_OR_UNLOAD_WRITE,(uint8_t*)&is_load,1);
}

//设置舵机LED，没发现舵机有LED
void Serial_Servo_SetLED(Serial_Servo *me, uint8_t LED_State)
{
    Serial_Servo_WriteCmd(me,SERIAL_SERVO_LED_CTRL_WRITE,(uint8_t*)&LED_State,1);
}

//读取LED状态
int Serial_Servo_ReadLED(Serial_Servo *me)
{
    uint8_t ledState;
    Serial_Servo_ReadCmd(me,SERIAL_SERVO_LED_CTRL_READ,&ledState);
    return (int)ledState;
}

//读取舵机目前位置，范围0-1000,对应0-240度
int Serial_Servo_ReadPosition(Serial_Servo *me)
{
    uint8_t rawData[2];
    uint16_t pos;
    Serial_Servo_ReadCmd(me,SERIAL_SERVO_POS_READ,rawData);
    pos = (rawData[1]<<8)|rawData[0];
    me->currentPostion = (int16_t)pos;
    return (int16_t)pos;
}
/*
int Serial_Servo_ReadLED(Serial_Servo *me)
{
    uint8_t buf[6];
    uint8_t ref[7];
    buf[0] = buf[1] = SERIAL_SERVO_FRAME_HEADER;
    buf[2] = me->id;
    buf[3] = 3;
    buf[4] = SERIAL_SERVO_LED_CTRL_READ;
    buf[5] = CheckSum(buf);
    HAL_HalfDuplex_EnableTransmitter(me->uartHandle);
    HAL_UART_Transmit(me->uartHandle,buf,6,1000);
    HAL_HalfDuplex_EnableReceiver(me->uartHandle);
    HAL_UART_Receive(me->uartHandle,ref,7,1000);
    //printf("LED STATE:%d\n",(int)ref[5]);
    return (int)ref[5];

}*/
//int LobotSerialServoReadPosition(Serial_Servo *me)
//{
//    int ret;
//    uint8_t buf[6];
//
//    buf[0] = buf[1] = SERIAL_SERVO_FRAME_HEADER;
//    buf[2] = me->id;
//    buf[3] = 3;
//    buf[4] = SERIAL_SERVO_POS_READ;
//    buf[5] = CheckSum(buf);
//
//    HAL_UART_Transmit(me->uartHandle,buf,6,1000);
//
//    ret = LobotSerialMsgHandle();
//    return ret;
//}
