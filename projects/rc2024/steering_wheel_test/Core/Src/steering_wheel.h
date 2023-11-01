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
#define STEERING_WHEEL_ROTATIOM_REDUCTION 3.3f//转向齿轮减速比（大比小）
#include "main.h"
    typedef enum{CORRECTING,RUNNING,STOP}Swheel_state_e;
    typedef struct {
        uint8_t id;
        GPIO_TypeDef* _LS_GPIOx;//限位开关GPIO
        uint16_t _LS_GPIO_Pin;//限位开关引脚
        uint8_t _light_switch_flag;//开关标志位
        float main_speed;//速度，单位m/s
        float rotation_speed;//转向速度，单位rad/s
        float main_pos;//位置，单位m
        float target_direction;
        float direction;//方向，弧度制，0为正前方，逆时针为正
        float direction_offset;//方向偏移，弧度制,相对于光电开关的偏移
        float rotation_pos;//弧度制，0为正前方，逆时针为正
        Swheel_state_e state;
    }steering_wheel_t;
    //steering_wheel_t * Swheel_create(const uint8_t id,GPIO_TypeDef* LS_GPIOx,const uint16_t LS_GPIO_Pin);
    void Swheel_init(steering_wheel_t *this,const uint8_t id,GPIO_TypeDef* LS_GPIOx,const uint16_t LS_GPIO_Pin);
    void Swheel_startCorrect(steering_wheel_t *this);
    void Swheel_executor(steering_wheel_t *this);
    void Swheel_EXTI_Callback(steering_wheel_t *this,uint16_t GPIO_Pin);
    void Swheel_get_mainSpeed( steering_wheel_t *this);
    void Swheel_set_mainSpeed( steering_wheel_t *this,const float speed);
    void Swheel_get_mainPos( steering_wheel_t *this);
    void Swheel_get_direction(steering_wheel_t *this);
    void Swheel_set_targetDirection(steering_wheel_t *this,const float theta);
    void Swheel_rotationMotor_reset(steering_wheel_t *this);
    //内部函数
    void _Swheel_get_rotationSpeed(steering_wheel_t *this);
    void _Swheel_get_rotationPos(steering_wheel_t *this);
    void _Swheel_set_rotationSpeed(steering_wheel_t *this, const float speed);
    void _Swheel_set_rotationPos(steering_wheel_t *this, const float pos);

#ifdef __cplusplus
}
#endif
#endif //STEERING_WHEEL_H
