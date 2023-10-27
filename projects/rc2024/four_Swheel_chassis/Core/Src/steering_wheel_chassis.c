//
// Created by tony on 23-10-27.
//

#include "steering_wheel_chassis.h"
#include "main.h"
#include "math.h"
#ifdef USE_DEFAULT_MOTOR_PARAM
#include "DJI.h"
#include "wtr_can.h"

#endif
#define norm(a,b) sqrtf(a*a+b*b) //计算模长
void calculate_target_velocity(Swheel_chassis_t *this);
void chassis_state_machine(Swheel_chassis_t *this);
void Swheel_chassis_init(Swheel_chassis_t *this)
{
#ifdef USE_DEFAULT_MOTOR_PARAM
    CANFilterInit(&hcan1);
    hDJI[0].motorType = M2006;
    DJI_Init();
    hDJI[0].speedPID.KI=0.01;
    hDJI[0].speedPID.KD=0.1;
    hDJI[0].speedPID.KP=1.5;

    hDJI[0].posPID.KP=150;
    hDJI[0].posPID.KI=1.4;
    hDJI[0].posPID.KD=0;
#endif
#ifdef FOUR_WHEELS_CHASSIS
    Swheel_init(&this->wheels[0],0,SWHEEL_0_LS_GPIO_Port,SWHEEL_0_LS_Pin);
    Swheel_init(&this->wheels[1],1,SWHEEL_1_LS_GPIO_Port,SWHEEL_1_LS_Pin);
    Swheel_init(&this->wheels[2],2,SWHEEL_2_LS_GPIO_Port,SWHEEL_2_LS_Pin);
    Swheel_init(&this->wheels[3],3,SWHEEL_3_LS_GPIO_Port,SWHEEL_3_LS_Pin);
    this->swheel_num = 4;
#endif
    this->state = CHASSIS_STOP;
    this->target_v.vx = 0;
    this->target_v.vy = 0;
    this->target_v.vw = 0;
    this->current_v.vx = 0;
    this->current_v.vy = 0;
    this->current_v.vw = 0;
    this->wheel_y[0]=CHASSIS_WHEEL_DISTANCE/2;
    this->wheel_y[1]=-CHASSIS_WHEEL_DISTANCE/2;
    this->wheel_y[2]=CHASSIS_WHEEL_DISTANCE/2;
    this->wheel_y[3]=-CHASSIS_WHEEL_DISTANCE/2;
    this->wheel_x[0]=CHASSIS_WHEEL_LENGTH/2;
    this->wheel_x[1]=CHASSIS_WHEEL_LENGTH/2;
    this->wheel_x[2]=-CHASSIS_WHEEL_LENGTH/2;
    this->wheel_x[3]=-CHASSIS_WHEEL_LENGTH/2;
}
void Swheel_chassis_startCorrect(Swheel_chassis_t *this)
{
    this->state= CHASSIS_CORRECTING;
    for (uint8_t i = 0; i <this->swheel_num ; i++)
        this->wheels[i].state=CORRECTING;
}
void Swheel_chassis_set_targetVelocity(Swheel_chassis_t *this, const float vx, const float vy, const float vw)
{
    this->target_v.vx = vx;
    this->target_v.vy = vy;
    this->target_v.vw = vw;
}
void chassis_state_machine(Swheel_chassis_t *this)
{

    if(this->state==CHASSIS_CORRECTING)
    {
        for (uint8_t i = 0; i <this->swheel_num ; i++)
        {
            if(this->wheels[i].state!=STOP)
                return;
        }
        this->state = CHASSIS_READY;
    }
    else
    {
        if (this->target_v.vx==0&&this->target_v.vy==0&&this->target_v.vw==0)
        {
            for (uint8_t i = 0;i<this->swheel_num;i++)
                this->wheels[i].state = STOP;
            this->state = CHASSIS_STOP;
            return;
        }
        calculate_target_velocity(this);
        this->state = CHASSIS_RUNNING;
        for (uint8_t i = 0; i <this->swheel_num ; i++)
        {
            if(fabsf(this->wheels[i].target_direction-this->wheels[i].direction)>0.05f)
            {
                this->state = CHASSIS_AIMMING;
                break;
            }
        }
        if (this->state==CHASSIS_AIMMING)
        {
            for (uint8_t i = 0; i <this->swheel_num ; i++)
                this->wheels[i].state = AIMMING;
        }
        else
        {
            for (uint8_t i = 0; i <this->swheel_num ; i++)
                this->wheels[i].state = RUNNING;
        }
    }
}

void Swheel_chassis_executor(Swheel_chassis_t *this)
{
    chassis_state_machine(this);
    for(int i=0;i<this->swheel_num;i++)
        Swheel_executor(&this->wheels[i]);
    CanTransmit_DJI_1234(&hcan1,hDJI[0].speedPID.output,hDJI[1].speedPID.output,hDJI[2].speedPID.output,hDJI[3].speedPID.output);
}

void calculate_target_velocity(Swheel_chassis_t *this)
{
    float vx=this->target_v.vx;
    float vy=this->target_v.vy;
    float vw=this->target_v.vw;
    float v = norm(vx,vy);
    float slip_angle = atan2(vy,vx);
    if(vw!=0)
    {
        float radius = v/vw; //曲率半径

        float theta = slip_angle+M_PI_2;
        float Ix = radius*cos(theta);
        float Iy = radius*sin(theta);
        for (int i = 0; i < this->swheel_num; i++) {
            this->wheels[i].target_direction = atan2(Iy-this->wheel_y[i],Ix-this->wheel_x[i])-M_PI_2;
            //如果下一个周期可以
            this->wheels[i].main_speed = vw* norm(Ix-this->wheel_x[i],Iy-this->wheel_y[i]);
        }
    } else
    {
        for (int i = 0; i < this->swheel_num; i++) {
            this->wheels[i].target_direction = slip_angle;
            this->wheels[i].main_speed = v;
        }
    }
}

void Swheel_chassis_EXTI_Callback(Swheel_chassis_t *this,const uint16_t GPIO_Pin)
{
    for (int i = 0; i < this->swheel_num; i++)
    {
        Swheel_EXTI_Callback(&this->wheels[i],GPIO_Pin);
    }
}