//
// Created by tony on 2023/4/24.
//

#ifndef SERIAL_BUS_SERVO_SERIAL_SERVO_H
#define SERIAL_BUS_SERVO_SERIAL_SERVO_H

#include "usart.h"

#define SERIAL_SERVO_FRAME_HEADER         0x55
#define SERIAL_SERVO_MOVE_TIME_WRITE      1
#define SERIAL_SERVO_MOVE_TIME_READ       2
#define SERIAL_SERVO_MOVE_TIME_WAIT_WRITE 7
#define SERIAL_SERVO_MOVE_TIME_WAIT_READ  8
#define SERIAL_SERVO_MOVE_START           11
#define SERIAL_SERVO_MOVE_STOP            12
#define SERIAL_SERVO_ID_WRITE             13
#define SERIAL_SERVO_ID_READ              14
#define SERIAL_SERVO_ANGLE_OFFSET_ADJUST  17
#define SERIAL_SERVO_ANGLE_OFFSET_WRITE   18
#define SERIAL_SERVO_ANGLE_OFFSET_READ    19
#define SERIAL_SERVO_ANGLE_LIMIT_WRITE    20
#define SERIAL_SERVO_ANGLE_LIMIT_READ     21
#define SERIAL_SERVO_VIN_LIMIT_WRITE      22
#define SERIAL_SERVO_VIN_LIMIT_READ       23
#define SERIAL_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define SERIAL_SERVO_TEMP_MAX_LIMIT_READ  25
#define SERIAL_SERVO_TEMP_READ            26
#define SERIAL_SERVO_VIN_READ             27
#define SERIAL_SERVO_POS_READ             28
#define SERIAL_SERVO_OR_MOTOR_MODE_WRITE  29
#define SERIAL_SERVO_OR_MOTOR_MODE_READ   30
#define SERIAL_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define SERIAL_SERVO_LOAD_OR_UNLOAD_READ  32
#define SERIAL_SERVO_LED_CTRL_WRITE       33
#define SERIAL_SERVO_LED_CTRL_READ        34
#define SERIAL_SERVO_LED_ERROR_WRITE      35
#define SERIAL_SERVO_LED_ERROR_READ       36


typedef struct Serial_Servo Serial_Servo;
struct Serial_Servo{
    UART_HandleTypeDef * uartHandle;
    uint8_t id;
    int16_t currentPostion;
};

void Serial_Servo_SetID(UART_HandleTypeDef * uartHandle,uint8_t oldID, uint8_t newID);
int Serial_Servo_ReadID(UART_HandleTypeDef * uartHandle);
Serial_Servo * Serial_Servo_Create(UART_HandleTypeDef *uartx, uint8_t id);

void Serial_Servo_WriteCmd(Serial_Servo * me,uint8_t cmdName,uint8_t* pArgs,uint8_t size);
void Serial_Servo_Move(Serial_Servo *me, int16_t position, uint16_t time);
int Serial_Servo_ReadPosition(Serial_Servo *me);
void Ser_Servo_SetLoadOrUnload(Serial_Servo * me,uint8_t is_load);
void Serial_Servo_SetLED(Serial_Servo *me, uint8_t LED_State);
int Serial_Servo_ReadLED(Serial_Servo *me);

#endif //SERIAL_BUS_SERVO_SERIAL_SERVO_H
