//
// Created by tony on 2023/3/27.
//

#ifndef MOTOR_ENCODER_H
#define MOTOR_ENCODER_H
#ifdef __cplusplus
extern "C"
{
#endif
float  get_speed(TIM_HandleTypeDef * tim,int dt=10,int PPR=512);
#ifdef __cplusplus
}
#endif
#endif //MOTOR_ENCODER_H
