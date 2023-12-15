//
// Created by tony on 23-10-27.
//

#include "steering_wheel_chassis.h"
#include "main.h"
#include "math.h"
#ifdef USE_DEFAULT_MOTOR_PARAM
#include "DJI.h"
#include "wtr_can.h"
void swChassis_rM_setFcurrent(swChassis_t *this);
#endif
#define norm(a,b) sqrt((a)*(a)+(b)*(b)) //计算模长
#ifdef FOUR_WHEELS_CHASSIS
const float swChassis_wheel_x[4]={CHASSIS_WHEEL_LENGTH/2,CHASSIS_WHEEL_LENGTH/2,-CHASSIS_WHEEL_LENGTH/2,-CHASSIS_WHEEL_LENGTH/2};
const float swChassis_wheel_y[4]={CHASSIS_WHEEL_DISTANCE/2,-CHASSIS_WHEEL_DISTANCE/2,CHASSIS_WHEEL_DISTANCE/2,-CHASSIS_WHEEL_DISTANCE/2};
#endif
void calculate_target_velocity(swChassis_t *this);
void chassis_state_machine(swChassis_t *this);

/**
 * @brief 底盘初始化
 */
void swChassis_init(swChassis_t *this)
{
    #ifdef USE_DEFAULT_MOTOR_PARAM
    //DJI电机初始化
    CANFilterInit(&hcan1);
    for (int i = 0; i < 4; i++) {
        hDJI[i].motorType = M2006;}
    DJI_Init();
    for (int i = 0; i < 4; i++) {
//        hDJI[i].speedPID.KI=0.9;
//        hDJI[i].speedPID.KD=12.5;
//        hDJI[i].speedPID.KP=14.8;
//        hDJI[i].posPID.KP=140;
        hDJI[i].posPID.KI=1.5;
        hDJI[i].posPID.KD=0;
        hDJI[i].speedPID.outputMax=4000;
        hDJI[i].speedPID.KI=0.02;
        hDJI[i].speedPID.KD=0.1;
        hDJI[i].speedPID.KP=0.8;
//        hDJI[i].posPID.KP=150;
//        hDJI[i].posPID.KI=2;
//        hDJI[i].posPID.KD=0;
    }
    for (int i=1;i<5;i++)
    {
        hVESC[i-1].hcann= &hcan1;
        hVESC[i-1].controller_id= 100+i;
    }
    #endif
    #ifdef FOUR_WHEELS_CHASSIS
    steeringWheel_init(&this->wheels[0], 0, SWHEEL_0_LS_GPIO_Port, SWHEEL_0_LS_Pin);
    steeringWheel_init(&this->wheels[1], 1, SWHEEL_1_LS_GPIO_Port, SWHEEL_1_LS_Pin);
    steeringWheel_init(&this->wheels[2], 2, SWHEEL_2_LS_GPIO_Port, SWHEEL_2_LS_Pin);
    steeringWheel_init(&this->wheels[3], 3, SWHEEL_3_LS_GPIO_Port, SWHEEL_3_LS_Pin);
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
/**
 * @brief 底盘开始校正
 */
void swChassis_startCorrect(swChassis_t *this)
{
    this->state= CHASSIS_CORRECTING;
    #ifdef USE_DEFAULT_MOTOR_PARAM
    //测出M2006电机的静态摩擦力，设置前馈电流
    swChassis_rM_setFcurrent(this);
    #endif
    for (uint8_t i = 0; i <this->swheel_num ; i++)
        this->wheels[i].state=CORRECTING;
}
/**
 * @brief 设置底盘目标速度
 * @param vx x方向速度
 * @param vy y方向速度
 * @param vw 角速度
 */
void swChassis_set_targetVelocity(swChassis_t *this, const float vx, const float vy, const float vw)
{
    this->target_v.vx = vx;
    this->target_v.vy = vy;
    this->target_v.vw = vw;
}
/**
 * @brief 底盘状态机
 * @param this
 * @description
 * 1.如果底盘处于校准状态，检查是否校准完成，如果完成则进入准备状态
 * 2.如果底盘处于非校准状态，检查是否有速度指令，如果无速度指令则进入停止状态
 * 3.如果有速度指令，则计算目标速度，如果目标速度方向与当前方向偏差太大，则进入转向状态，否则进入运行状态
 */
void chassis_state_machine(swChassis_t *this)
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
/**
 * @brief 底盘执行器函数
 * @param this
 */
void swChassis_executor(swChassis_t *this)
{
    chassis_state_machine(this);
    float dji_output_withfc[4];
    for(int i=0;i<this->swheel_num;i++)
    {
        steeringWheel_executor(&this->wheels[i]);
        if(hDJI[i].speedPID.output>2000)
            dji_output_withfc[i]=hDJI[i].speedPID.output+hDJI[i].f_current;
        else if(hDJI[i].speedPID.output<-2000)
            dji_output_withfc[i]=hDJI[i].speedPID.output-hDJI[i].f_current;
        else
            dji_output_withfc[i]=hDJI[i].speedPID.output;
    }
    CanTransmit_DJI_1234(&hcan1,dji_output_withfc[0],dji_output_withfc[1],dji_output_withfc[2],dji_output_withfc[3]);
    //CanTransmit_DJI_1234(&hcan1,dji_output_withfc[0],0,0,0);

}
/**
 * @brief 底盘速度解算
 * @param this
 */
#ifdef FOUR_WHEELS_CHASSIS
void calculate_target_velocity(swChassis_t *this)
{
    float vx=this->target_v.vx;
    float vy=this->target_v.vy;
    float vw=this->target_v.vw;
    if(vx==0)
        vx=0.0001f;
    float v = norm(vx,vy);
    float slip_angle = atan2(vy,vx);
    if(vw!=0)
    {
        float radius = v/vw; //曲率半径

        float theta = slip_angle+M_PI_2;
        float Ix = radius*cos(theta);
        float Iy = radius*sin(theta);
        for (int i = 0; i < this->swheel_num; i++) {
            //??this->wheels[i].target_direction = atan2(swChassis_wheel_y[i]-Iy,swChassis_wheel_x[i]-Ix)+M_PI_2;
            //如果下一个周期可以
            this->wheels[i].target_main_speed = vw * norm(Ix-swChassis_wheel_x[i],Iy-swChassis_wheel_y[i]);
            //this->wheels[i].target_main_speed = vw * norm(0.15,0.15);
        }

    } else
    {
        for (int i = 0; i < this->swheel_num; i++) {
            this->wheels[i].target_direction = slip_angle;
            this->wheels[i].target_main_speed = v;
        }
    }
}
#endif
/**
 * @brief 底盘外部中断回调函数
 * @param this
 * @param GPIO_Pin
 */
void swChassis_EXTI_Callback(swChassis_t *this, uint16_t GPIO_Pin)
{
    for (int i = 0; i < this->swheel_num; i++)
    {
        steeringWheel_EXTI_Callback(&this->wheels[i], GPIO_Pin);
    }
}
/**
 * @brief 测出M2006电机的静态摩擦力，设置前馈电流
 * @param this
 */
#ifdef USE_DEFAULT_MOTOR_PARAM
void swChassis_rM_setFcurrent(swChassis_t *this)
{
    uint8_t flag=0x00;
    while (flag!=0x0f)
    {
        for (uint8_t i = 0; i <this->swheel_num ; i++)
        {
            if(hDJI[i].FdbData.rpm>0.1)
            {
                flag = flag|0x01<<i;
                continue;
            }
            hDJI[i].f_current++;
        }
        CanTransmit_DJI_1234(&hcan1,hDJI[0].f_current,hDJI[1].f_current,hDJI[2].f_current,hDJI[3].f_current);
        Delay_ms(2);
    }
    CanTransmit_DJI_1234(&hcan1,0,0,0,0);
}
#endif
