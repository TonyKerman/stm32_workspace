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
#define CHASSIS_WHEEL_LENGTH_X 0.3f//轮子在x方向间距，单位m

    typedef enum{CHASSIS_CORRECTING,CHASSIS_AIMMING,CHASSIS_MOVING,CHASSIS_STOP}Swheel_chassis_state_e;
    typedef struct {
     float vx;
     float vy;
     float vw;
    }Swheel_chassis_velocity_t;
    typedef struct{
        Swheel_chassis_velocity_t target_v;
        Swheel_chassis_velocity_t current_v;
        uint8_t swheel_num;
        Swheel_chassis_state_e state;
#ifdef FOUR_WHEELS_CHASSIS
        steering_wheel_t wheels[4];
        float wheel_x[4];
        float wheel_y[4];
#endif
    }Swheel_chassis_t;
    void Swheel_chassis_init(Swheel_chassis_t *this);
    void Swheel_chassis_startCorrect(Swheel_chassis_t *this);
    void Swheel_chassis_set_targetVelocity(Swheel_chassis_t *this, const float vx, const float vy, const float vw);
    void Swheel_chassis_executor(Swheel_chassis_t *this);
#ifdef __cplusplus
}
#endif
#endif //STEERING_WHEEL_CHASSIS_H
