#include "encoder.h"

/**
 * @brief 初始化编码器模式
 * @param encoder: 编码器句柄，包含定时器实例、PPR 等信息
 */
void Encoder_Init(Encoder_HandleTypeDef *encoder) {
    // 开启定时器的编码器模式
    HAL_TIM_Encoder_Start(encoder->htim, TIM_CHANNEL_ALL);

    // 重置计数器
    encoder->zero_cnt = (uint16_t)(__HAL_TIM_GET_AUTORELOAD(encoder->htim) / 2);
    __HAL_TIM_SET_COUNTER(encoder->htim,encoder->zero_cnt);


}


/**
 * @brief 计算编码器速度（单位：转每秒，rps）
 * @param encoder: 编码器句柄
 * @param samplingTime: 采样时间，单位为秒
 * @return 当前速度，单位为转每秒（rps）
 */
float Encoder_GetSpeed(Encoder_HandleTypeDef *encoder, float samplingTime) {


    int16_t delta_position =__HAL_TIM_GET_COUNTER(encoder->htim)-encoder->zero_cnt;
    __HAL_TIM_SET_COUNTER(encoder->htim,encoder->zero_cnt);
    // 计算速度 (rps)
    float speed = (float)delta_position / (encoder->ppr * samplingTime);

    return speed;
}
