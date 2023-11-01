//
// Created by tony on 23-10-24.
//

#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H
#ifdef __cplusplus
extern "C" {
#endif
//#define USE_M2006_AS_ROTATION_MOTOR
#define USE_DEFAULT_MOTOR_PARAM
#ifdef USE_DEFAULT_MOTOR_PARAM
#include "wtr_vesc.h"
extern VESC_t hVESC[4];
#endif
#define STEERING_WHEEL_ROTATIOM_REDUCTION 3.3f//转向齿轮减速比（大比小）
#define STEERING_WHEEL_DIAMETER 0.1f//轮子直径，单位m
#include "main.h"
    typedef enum{CORRECTING,RUNNING,AIMMING,STOP}Swheel_state_e;
    typedef struct {
        uint8_t id;
        GPIO_TypeDef* _LS_GPIOx;//限位开关GPIO
        uint16_t _LS_GPIO_Pin;//限位开关引脚
        uint8_t _light_switch_flag;//开关标志位
        float main_speed;//速度，单位m/s
        float rotation_speed;//转向速度，单位rad/s
        float main_pos;//位置，单位m
        float target_direction;
        float target_main_speed;
        float direction;//方向，弧度制，0为正前方，逆时针为正
        float direction_offset;//方向偏移，弧度制,相对于光电开关的偏移
        float rotation_pos;//弧度制，0为正前方，逆时针为正
        int16_t correcting_stage;
        Swheel_state_e state;
    }steering_wheel_t;

    void steeringWheel_init(steering_wheel_t *this, const uint8_t id, GPIO_TypeDef* LS_GPIOx, const uint16_t LS_GPIO_Pin);
    void steeringWheel_startCorrect(steering_wheel_t *this);
    void steeringWheel_executor(steering_wheel_t *this);
    void steeringWheel_EXTI_Callback(steering_wheel_t *this, uint16_t GPIO_Pin);
    void steeringWheel_rMotor_reset(steering_wheel_t *this);
//    void Swheel_mMotor_getSpeed(steering_wheel_t *this);
//    void Swheel_mMotor_speedServo(steering_wheel_t *this, const float speed);
//    void Swheel_mMotor_getPos(steering_wheel_t *this);

    //内部函数

#ifdef __cplusplus
}
#endif
#endif //STEERING_WHEEL_H
