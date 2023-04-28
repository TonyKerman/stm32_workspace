//
// Created by tony on 2023/4/24.
//

#ifndef SERIAL_BUS_SERVO_SERIAL_SERVO_H
#define SERIAL_BUS_SERVO_SERIAL_SERVO_H

#include "usart.h"

#define LOBOT_SERVO_FRAME_HEADER         0x55
#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14
#define LOBOT_SERVO_ANGLE_OFFSET_ADJUST  17
#define LOBOT_SERVO_ANGLE_OFFSET_WRITE   18
#define LOBOT_SERVO_ANGLE_OFFSET_READ    19
#define LOBOT_SERVO_ANGLE_LIMIT_WRITE    20
#define LOBOT_SERVO_ANGLE_LIMIT_READ     21
#define LOBOT_SERVO_VIN_LIMIT_WRITE      22
#define LOBOT_SERVO_VIN_LIMIT_READ       23
#define LOBOT_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LOBOT_SERVO_TEMP_MAX_LIMIT_READ  25
#define LOBOT_SERVO_TEMP_READ            26
#define LOBOT_SERVO_VIN_READ             27
#define LOBOT_SERVO_POS_READ             28
#define LOBOT_SERVO_OR_MOTOR_MODE_WRITE  29
#define LOBOT_SERVO_OR_MOTOR_MODE_READ   30
#define LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LOBOT_SERVO_LOAD_OR_UNLOAD_READ  32
#define LOBOT_SERVO_LED_CTRL_WRITE       33
#define LOBOT_SERVO_LED_CTRL_READ        34
#define LOBOT_SERVO_LED_ERROR_WRITE      35
#define LOBOT_SERVO_LED_ERROR_READ       36


typedef struct Serial_Servo Serial_Servo;
struct Serial_Servo{
    UART_HandleTypeDef * uartHandle;
    uint8_t id;
};

void Serial_Servo_SetID(UART_HandleTypeDef * uartHandle,uint8_t oldID, uint8_t newID);
void Serial_Servo_ReadID(UART_HandleTypeDef * uartHandle);
Serial_Servo * Serial_Servo_Create(UART_HandleTypeDef *uartx, uint8_t id);

void Serial_Servo_Move(Serial_Servo *me, int16_t position, uint16_t time);
void Serial_Servo_SetLED(Serial_Servo *me, uint8_t LED_State);
void Serial_Servo_ReadLED(Serial_Servo *me);

#endif //SERIAL_BUS_SERVO_SERIAL_SERVO_H
