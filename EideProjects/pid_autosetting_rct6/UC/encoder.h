#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f1xx_hal.h" // 根据你的芯片型号选择适当的 HAL 头文件

// 编码器配置结构体
typedef struct {
    TIM_HandleTypeDef *htim; // 定时器句柄
    uint32_t ppr;            // 编码器每转的脉冲数 (Pulses Per Revolution)
    uint32_t zero_cnt;     // 零点位置
} Encoder_HandleTypeDef;

// 函数声明
void Encoder_Init(Encoder_HandleTypeDef *encoder);
float Encoder_GetSpeed(Encoder_HandleTypeDef *encoder, float samplingTime);

#endif // ENCODER_H
