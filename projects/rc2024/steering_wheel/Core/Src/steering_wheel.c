//
// Created by tony on 23-10-24.
//

#include "steering_wheel.h"
#include "math.h"
#define Delay_ms(x) HAL_Delay(x)




#ifdef USE_DEFAULT_MOTOR_PARAM
#include "DJI.h"
#include "Caculate.h"
#include "wtr_vesc.h"

//默认电机api (转向电机为M2006)
/**
 * @brief 内部函数，转向速度获取
 * @return 轮子转向速度，单位rpm
 */
void _Swheel_get_rotationSpeed(steering_wheel_t *this)
{
    this->rotation_speed = hDJI[this->id].FdbData.rpm/hDJI[this->id].reductionRate/STEERING_WHEEL_ROTATIOM_REDUCTION;
}
/**
 * @brief 内部函数，转向位置获取
 * @return 轮子转过的总弧度，单位rad
 * */
void _Swheel_get_rotationPos(steering_wheel_t *this)
{
    this->rotation_pos= hDJI[this->id].Calculate.RotorAngle_all/hDJI[this->id].reductionRate*M_PI/180.f/STEERING_WHEEL_ROTATIOM_REDUCTION;
    // = fmod(range,2*M_PI);
}
/**
 * @brief 内部函数，转向速度设置
 * @param speed 轮子转向速度，单位rpm
 */
void _Swheel_set_rotationSpeed(steering_wheel_t *this, const float speed)
{
    speedServo(speed*hDJI[this->id].reductionRate*STEERING_WHEEL_ROTATIOM_REDUCTION,&hDJI[this->id]);
}
/**
 * @brief 内部函数，转向位置设置
 * @param pos 轮子转过的总弧度，单位rad
 */
void _Swheel_set_rotationPos(steering_wheel_t *this, const float pos)
{
    positionServo(pos*180.f/M_PI*STEERING_WHEEL_ROTATIOM_REDUCTION,&hDJI[this->id]);
}
/**
 * @brief 转向电机复位
 */
void Swheel_rotationMotor_reset(steering_wheel_t *this)
{
    hDJI[this->id].Calculate.RotorAngle_all = 0;
    hDJI[this->id].Calculate.RotorAngle_0_360_Log[NOW] = 0;
    hDJI[this->id].Calculate.RotorAngle_0_360_Log[LAST] = 0;
    hDJI[this->id].Calculate.RotorRound = 0;
    hDJI[this->id].Calculate.RotorAngle_0_360_OffSet = hDJI[this->id].FdbData.RotorAngle_0_360;
}
void Swheel_set_mainSpeed(steering_wheel_t *this,const float speed)
{
    float erpm = speed;//*k
    VESC_CAN_SET_ERPM(&hVESC[this->id],erpm);
}
#endif




/**
 * @brief 舵轮初始化
 * @param id
 * @param LS_GPIOx 光电开关GPIO
 * @param LS_GPIO_Pin 光电开关引脚
 * @return
 */
void Swheel_init(steering_wheel_t *this,const uint8_t id,GPIO_TypeDef* LS_GPIOx,const uint16_t LS_GPIO_Pin)
{
    this->id = id;
    this->_LS_GPIOx = LS_GPIOx;
    this->_LS_GPIO_Pin = LS_GPIO_Pin;
    this->_light_switch_flag = 0;
    this->direction = 0;
    this->direction_offset = 0;
    this->main_speed = 0;
    this->rotation_speed = 0;
    this->main_pos = 0;
    this->rotation_pos = 0;
    this->state = STOP;
}
/**
 * @brief 转向轮启动校准
 * @param this
 */
void Swheel_startCorrect(steering_wheel_t *this)
{
    this->state = CORRECTING;
}
/**
 * @brief 转向轮限位开关外部中断回调函数
 * @param this
 * @param GPIO_Pin
 */
void Swheel_EXTI_Callback(steering_wheel_t *this,const uint16_t GPIO_Pin)
{
    if(GPIO_Pin==this->_LS_GPIO_Pin)
        this->_light_switch_flag= 1;
}
/**
 * @brief 转向轮校准
 * @param this
 * @description
 * 1.转向轮转动到限位开关处 (stage=0)
 * 2.转向轮反向转动到限位开关处 (stage = 1~100,如果限位开关触发则stage=-1,若一直不触发直到stage=100则stage=0)
 * 3.转向轮停止,等待（stage = -1->-10）
 * 4.电机复位（stage = -10）
 */
static inline void Swheel_correcting(steering_wheel_t *this)
{
    static int8_t stage = 0;
    if(stage==0)
    {
        if(this->_light_switch_flag)
        {
            _Swheel_set_rotationSpeed(this, -2);
            this->_light_switch_flag = 0;
            stage=1;
        }
        else
        {
            _Swheel_set_rotationSpeed(this, 20);
        }
    }
    else if(stage>0&&stage<100)
    {
        if(this->_light_switch_flag)
        {
            _Swheel_set_rotationSpeed(this, 0);
            this->_light_switch_flag = 0;
            stage=-1;
        }
        else
        {   stage++;
            _Swheel_set_rotationSpeed(this, -2);
        }
    }
    else if(stage>=100){stage = 0;}//重头再来
    else if(stage<0&&stage>-10){stage--;Swheel_rotationMotor_reset(this);}//延时
    else if(stage==-10)
    {
        this->state = STOP;
    }

}
/**
 * @brief 舵轮执行器函数
 * @param this
 * @description
 * 1.如果状态为STOP，则电机停止
 * 2.如果状态为CORRECTING，则执行校准
 * 3.如果状态为RUNNING，则执行转向
 */
void Swheel_executor(steering_wheel_t *this)
{

    _Swheel_get_rotationSpeed(this);
    if(this->state==STOP)
    {
        _Swheel_set_rotationSpeed(this, 0);
    }else if(this->state==CORRECTING)
    {
        Swheel_correcting(this);
    }else if(this->state==RUNNING)
    {
        _Swheel_set_rotationPos(this,this->target_direction+2*M_PI*(int)((this->rotation_pos-this->direction_offset)/(2*M_PI)));

    }
}
/**
 * @brief 转向方向获取（0~2pi）
 * @param this
 */
void Swheel_get_direction(steering_wheel_t *this)
{
    this->direction=fmod(this->rotation_pos,2*M_PI)+this->direction_offset;
}
/**
 * @brief 设置转向方向（0~2pi）
 * @param this
 */
void Swheel_set_targetDirection(steering_wheel_t *this,const float theta)
{
    if(theta>0&&theta<2*M_PI)
        this->target_direction = theta;
}