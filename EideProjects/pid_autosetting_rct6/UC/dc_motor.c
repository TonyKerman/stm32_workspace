#include "dc_motor.h"

/**
 * @brief 初始化直流电机
 * @param motor: 电机句柄
 */
void DCMotor_Init(DCMotor_HandleTypeDef *motor) {

    motor->max_speed = 1000; // 默认最大速度为 1000
    motor->arr = __HAL_TIM_GET_AUTORELOAD(motor->pwm_tim); // 获取 PWM 定时器的自动重装载值
    // 设置初始状态为停止
    HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);

    // 启动 PWM 定时器
    HAL_TIM_PWM_Start(motor->pwm_tim, motor->pwm_channel);
}

/**
 * @brief 设置电机速度
 * @param motor: 电机句柄
 * @param speed: 电机速度，范围 -max_speed 到 max_speed
 */
void DCMotor_SetSpeed(DCMotor_HandleTypeDef *motor, int16_t speed) {
    // 限制速度范围
    if (speed > motor->max_speed) {
        speed = motor->max_speed;
    } else if (speed < -motor->max_speed) {
        speed = -motor->max_speed;
    }

    // 计算 PWM 占空比（0~100%）
    uint16_t pwm_value = (uint16_t)((abs(speed) * (motor->arr + 1)) / motor->max_speed);

    // 设置方向引脚
    if (speed > 0) {
        // 正转：IN1 = HIGH, IN2 = LOW
        HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
    } else if (speed < 0) {
        // 反转：IN1 = LOW, IN2 = HIGH
        HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
    } else {
        // 停止：IN1 = LOW, IN2 = LOW
        HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
    }

    // 设置 PWM 占空比
    __HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel, pwm_value);
}

/**
 * @brief 停止电机
 * @param motor: 电机句柄
 */
void DCMotor_Stop(DCMotor_HandleTypeDef *motor) {
    // 停止时将 IN1 和 IN2 都拉低
    HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);

    // 设置 PWM 为 0
    __HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel, 0);
}
