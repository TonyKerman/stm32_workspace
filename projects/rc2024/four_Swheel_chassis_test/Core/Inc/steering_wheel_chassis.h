//
// Created by tony on 23-10-27.
//

#ifndef STEERING_WHEEL_CHASSIS_H
#define STEERING_WHEEL_CHASSIS_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "steering_wheel.h"

#define FOUR_WHEELS_CHASSIS
#define CHASSIS_WHEEL_DISTANCE 0.3f//轮子在y方向间距，单位m
#define CHASSIS_WHEEL_LENGTH 0.3f//轮子在x方向间距，单位m
#define Delay_ms(t) HAL_Delay(t)
    typedef enum{CHASSIS_CORRECTING,CHASSIS_AIMMING,CHASSIS_RUNNING,
                 CHASSIS_STOP,CHASSIS_READY}swChassis_state_e;

    typedef struct {
     float vx;
     float vy;
     float vw;
    }swChassis_velocity_t;

    typedef struct{
        swChassis_velocity_t target_v;
        swChassis_velocity_t current_v;
        uint8_t swheel_num;
        swChassis_state_e state;
        #ifdef FOUR_WHEELS_CHASSIS
        steering_wheel_t wheels[4];
        #endif
    }swChassis_t;
    void swChassis_init(swChassis_t *this);
    void swChassis_startCorrect(swChassis_t *this);
    void swChassis_set_targetVelocity(swChassis_t *this, const float vx, const float vy, const float vw);
    void swChassis_executor(swChassis_t *this);
    void swChassis_EXTI_Callback(swChassis_t *this, uint16_t GPIO_Pin);
#ifdef __cplusplus
}
#endif
#endif //STEERING_WHEEL_CHASSIS_H
