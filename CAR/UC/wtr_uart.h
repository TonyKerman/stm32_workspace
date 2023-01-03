#ifndef WTR_UART_H
#define WTR_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

#define CH0_BIAS 1024
#define CH1_BIAS 1024
#define CH2_BIAS 1024
#define CH3_BIAS 1024
#define WHEEL_BIAS 1024
#define CH_RANGE 660

typedef struct
{
    int16_t ch0;
    int16_t ch1;
    int16_t ch2;
    int16_t ch3;
    int8_t left;
    int8_t right;
    int16_t wheel;
}Remote_t;

extern Remote_t Raw_Data;
extern uint8_t JoyStickReceiveData[18];
extern double posRef;
    void UART1Decode();

#ifdef __cplusplus
}
#endif

#endif