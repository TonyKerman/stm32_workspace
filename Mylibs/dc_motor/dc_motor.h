#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include "stm32f1xx_hal.h" // 根据具体芯片选择适当的 HAL 头文件

// 直流电机控制结构体
typedef struct {
    TIM_HandleTypeDef *pwm_tim;  // PWM 定时器句柄
    uint32_t pwm_channel;        // PWM 定时器通道 (TIM_CHANNEL_1, TIM_CHANNEL_2, etc.)
    GPIO_TypeDef *in1_port;      // IN1 引脚对应的 GPIO 端口
    uint16_t in1_pin;            // IN1 引脚对应的 GPIO 引脚
    GPIO_TypeDef *in2_port;      // IN2 引脚对应的 GPIO 端口
    uint16_t in2_pin;            // IN2 引脚对应的 GPIO 引脚
    uint16_t arr;                // PWM 定时器自动重装载值
    int16_t max_speed;           // 最大速度（例如 1000）
} DCMotor_HandleTypeDef;

// 函数声明
void DCMotor_Init(DCMotor_HandleTypeDef *motor);
void DCMotor_SetSpeed(DCMotor_HandleTypeDef *motor, int16_t speed); // 设置速度，范围 -max_speed 到 max_speed
void DCMotor_Stop(DCMotor_HandleTypeDef *motor);

#endif // DC_MOTOR_H
