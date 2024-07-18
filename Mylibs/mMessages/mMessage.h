//
// Created by tony on 2023/5/20.
//

#ifndef FRESHMANPROJECT_MMESSAGE_H
#define FRESHMANPROJECT_MMESSAGE_H

#define FRAMEHEAD 0x55
#define DATALEN 50
#include "main.h"

typedef struct msgData_t msgData_t;
struct msgData_t{
    uint8_t* content;
    uint8_t type;
    uint8_t length;
};
msgData_t *add32(uint32_t *pdata, uint8_t size);
msgData_t *add16(uint16_t *pdata, uint8_t size);
msgData_t *addu8(uint8_t *pdata, uint8_t size);
int8_t getMsg(UART_HandleTypeDef * huartx, msgData_t *data, uint32_t countout);
int8_t sendMsg(UART_HandleTypeDef * huartx,uint8_t header,msgData_t * data);
#endif //FRESHMANPROJECT_MMESSAGE_H
