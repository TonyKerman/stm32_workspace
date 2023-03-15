#ifndef __MOTOR_CONTROL_H
#define __MOTOR_CONTROL_H
#ifdef __cplusplus
/*
 *
 * 适用于驱动使用类似L298N驱动板的直流编码电机
 *
 */
extern "C"
{
#endif

#include "main.h"
#include "start.h"
    extern double debug[10];
    enum PID_TypeDefine{positional ,incremental};
class Pid
{
    PID_TypeDefine type;
    float loutput;
    float intg;
    float lerr;
    float llerr;
    int max_output;
    int max_intg;
public:
    float Kp,Ki,Kd;
    float err;
    float calculate(float tgt_val,float cur_val);
    Pid()
    {
        Kp = 0;
        Ki = 0;
        Kd = 0;
        lerr =0;
        llerr = 0;
        loutput =0;
        intg=0;
        max_output=2147483646;
        max_intg=2147483646;
    }
    void set_args(PID_TypeDefine t,float p=-1,float i=-1,float d=-1,int mi =-1,int mo =-1);
    void reset();
};

class Motor
{
    /*
     * 用ena in1，in2 pwm控制的电机
     * 电机参数：减速比5.2
     * 编码器 16ppm 二分频（检测上升沿和下降沿）
     *
     */

    //电机引脚
    TIM_HandleTypeDef *mhtim; uint32_t mChannel;
    GPIO_TypeDef *in1_GPIO_Port; uint16_t in1_Pin;
    GPIO_TypeDef *in2_GPIO_Port; uint16_t in2_Pin;
    GPIO_TypeDef *Sa_GPIO_Port;uint16_t Sa_Pin;
    GPIO_TypeDef *Sb_GPIO_Port;uint16_t Sb_Pin;
    Pid pid_range;
    Pid pid_speed;
    enum states{stop,forward,backward};

    double period;
    int ct;
public:
    int duty;
    states state;
    float speed ;
    float ranger ;
    float tgt_speed;
    float tgt_range;
    void set_rounds(float r);
    void set_speed(int tgt_speed);
    void TIM_PeriodElapsedCallback();
    void run();
    void GPIO_EXTI_Callback(uint16_t GPIO_Pin);
    void start();
    Motor(TIM_HandleTypeDef *htim, uint32_t Channel,
          GPIO_TypeDef *in1_GPIOx, uint16_t in1p,
          GPIO_TypeDef *in2_GPIOx, uint16_t in2p,
          GPIO_TypeDef *Sa_GPIOx,uint16_t Sap,
          GPIO_TypeDef *Sb_GPIOx,uint16_t Sbp);



};

#ifdef __cplusplus
}
#endif

#endif
