//
// Created by tony on 23-10-27.
//

#include "steering_wheel_chassis.h"
#include "main.h"
#include "math.h"

#define norm(a,b) sqrtf(a*a+b*b) //计算模长

void chassis_init(Swheel_chassis_t *this)
{
#ifdef FOUR_WHEELS_CHASSIS
    Swheel_init(&this->wheels[0],0,SWHEEL_1_LS_GPIO_Port,SWHEEL_1_LS_Pin);
    Swheel_init(&this->wheels[1],1,SWHEEL_2_LS_GPIO_Port,SWHEEL_2_LS_Pin);
    Swheel_init(&this->wheels[2],2,SWHEEL_3_LS_GPIO_Port,SWHEEL_3_LS_Pin);
    Swheel_init(&this->wheels[3],3,SWHEEL_4_LS_GPIO_Port,SWHEEL_4_LS_Pin);
    this->swheel_num = 4;
#endif
    this->state = CHASSIS_STOP;
    this->target_v.vx = 0;
    this->target_v.vy = 0;
    this->target_v.vw = 0;
    this->current_v.vx = 0;
    this->current_v.vy = 0;
    this->current_v.vw = 0;
}
void chassis_startCorrect(Swheel_chassis_t *this)
{
    this->state= CHASSIS_CORRECTING;

}
void chassis_set_targetVelocity(Swheel_chassis_t *this, const float vx, const float vy, const float vw)
{
    this->target_v.vx = vx;
    this->target_v.vy = vy;
    this->target_v.vw = vw;
}
void chassis_state_estimation(Swheel_chassis_t *this)
{
    if(this->target_v.vx==0&&this->target_v.vy==0&&this->target_v.vw==0)
    {
        this->state = CHASSIS_STOP;
        return;
    }
    for (uint8_t i = 0; i <this->swheel_num ; i++)
    {
        if(this->wheels[i].state==CORRECTING)
        {
            this->state = CHASSIS_CORRECTING;
            return;
        }
        if(fabsf(this->wheels[i].target_direction-this->wheels[i].direction)>0.05f)
        {
            this->state = CHASSIS_AIMMING;
            return;
        }
    }
    this->state= CHASSIS_MOVING;
}
void chassis_executor(Swheel_chassis_t *this)
{
    chassis_state_estimation(this);
#ifdef FOUR_WHEELS_CHASSIS
    for(int i=0;i<this->swheel_num;i++)
    {
        Swheel_executor(&this->wheels[i]);
    }
#endif
}


void calculate_target_velocity(Swheel_chassis_t *this)
{
    float vx=this->target_v.vx;
    float vy=this->target_v.vy;
    float vw=this->target_v.vw;
    float v = norm(vx,vy);
    if(vw!=0)
    {
        float radius = v/vw; //曲率半径
        float slip_angle = atan2(vy,vx);
        float theta = slip_angle+M_PI_2;
        float Ix = radius*cos(theta);
        float Iy = radius*sin(theta);
        for (int i = 0; i < this->swheel_num; i++) {
            this->wheels[i].target_direction = atan2(Iy-CHASSIS_WHEEL_LENGTH_X/2,Ix-CHASSIS_WHEEL_DISTANCE/2)-M_PI_2;
            //如果下一个周期可以
            this->wheels[i].main_speed = vw* norm(Ix-this->wheel_x[i],Iy-this->wheel_y[i]);
        }
    }




}
