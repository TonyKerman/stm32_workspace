//
// Created by Lenovo on 2022/11/12.
//

#include "motor_control.h"

using namespace std;
double debug[10];
void Pid::reset()
{
    lerr =0;
    intg=0;
}

float Pid::calculate(float tgt_val,float cur_val) {

    float output;
    err = tgt_val - cur_val;
    if(type==incremental)
    {//增量式PID
        intg = err;
        output =loutput + Kp*(err-lerr) + Ki*intg + Kd*(err - 2*lerr + llerr);
        llerr = lerr;
        lerr = err;
        //输出限制
        debug[6]=err;
        if(output > float(max_output))
        {
            output = float(max_output);
        }
        else if(output < float(-1*max_output))
        {
            output= -1*max_output;
        }
        loutput = output;
    }
    else
    {//位置PID
        intg += err;
        //积分限幅
        if(intg>float(max_intg))
        {
            intg=float(max_intg);
        }
        else if(intg<float(-1*max_intg))
        {
            intg=-1*max_intg;
        }
        output = Kp*err + Ki * intg + (err - lerr) * Kd;
        lerr = err;
        //输出限制
        if(output>float(max_output))
        {
            output=float(max_output);
        }
        else if(output<float(-1*max_output))
        {
            output=-1*max_output;
        }
    }

    return output;
}

void Pid::set_args( PID_TypeDefine t,float p, float i, float d, int mi, int mo) {
    type =t;
    float l1[3]={p,i,d};
    float* l2[3]={&Kp,&Ki,&Kd};
    for(int i=0;i<=2;i++)
    {
        if(l1[i] != -1)
        {
            *(l2[i]) = l1[i];
        }
        if(mi!=-1)
        {
            max_intg = mi;
        }
        if(mo != -1)
        {
            max_output = mo;
        }
    }
}

Motor::Motor(TIM_HandleTypeDef *htim, uint32_t Channel, GPIO_TypeDef *in1_GPIOx, uint16_t in1p, GPIO_TypeDef *in2_GPIOx,
             uint16_t in2p, GPIO_TypeDef *Sa_GPIOx, uint16_t Sap,GPIO_TypeDef *Sb_GPIOx, uint16_t Sbp,bool rev)
{
    in1_GPIO_Port= in1_GPIOx;
    in1_Pin = in1p;
    in2_GPIO_Port = in2_GPIOx;
    in2_Pin = in2p;
    Sa_GPIO_Port = Sa_GPIOx;
    Sa_Pin = Sap;
    Sb_GPIO_Port = Sb_GPIOx;
    Sb_Pin = Sbp;
    speed = 0;
    ranger = 0;
    ct = 0;
    mhtim= htim;
    mChannel=Channel;
    state=stop;
    duty=0;
    reverse=rev;
    //180 是tim2 psc的值+1
    period =(__HAL_TIM_GET_AUTORELOAD(mhtim)+1)*(180)/HAL_RCC_GetSysClockFreq();
    bool run_range_FLAG= false;
}


void Motor::start() {
    HAL_TIM_PWM_Start(mhtim,mChannel);
    __HAL_TIM_SET_COMPARE(mhtim, mChannel,0);
    pid_speed.set_args(incremental, 1, 0.3, 1, 1000, 800);
    pid_range.set_args(positional,10,0,0,1000,1000);

}

void Motor::TIM_PeriodElapsedCallback() {
    //speed = ct/32/5.2*100;

    speed = ct*50*40/32/5.2;//tim1周期20ms speed单位：一秒转多少圈*40(满速约1000)
    ct = 0;
    run();

}
/*
 * if(GPIO_Pin == Sa_Pin)
    {
        GPIO_PinState sb = HAL_GPIO_ReadPin(Sb_GPIO_Port,Sb_Pin);
        debug[4]= sb;

        if(sb ==(GPIO_PIN_SET^reverse)){
            ct++;
            state = forward;
            if(tgt_range)
                ranger+=(1.0/32)/5.2;
        }else{
            state = backward;
            ct--;
            if(tgt_range)
                ranger-=(1.0/32)/5.2;
        }
 * */
void Motor::GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Sa_Pin)
    {
        GPIO_PinState sa = HAL_GPIO_ReadPin(Sa_GPIO_Port,Sa_Pin);
        GPIO_PinState sb = HAL_GPIO_ReadPin(Sb_GPIO_Port,Sb_Pin);

        debug[4]= sb;
        if(sa == GPIO_PIN_SET)
        {
            if(sb ==(GPIO_PIN_SET^reverse)){
                ct++;
                state = forward;
                if(tgt_range)
                    ranger+=(1.0/32)/5.2;
            }else{
                state = backward;
                ct--;
                if(tgt_range)
                    ranger-=(1.0/32)/5.2;
            }
        }
        else
        {
            if(sb !=(GPIO_PIN_SET^reverse)){
                ct++;
                state = forward;
                if(tgt_range)
                    ranger+=(1.0/32)/5.2;
            }else{
                state = backward;
                ct--;
                if(tgt_range)
                    ranger-=(1.0/32)/5.2;
            }
        }


    }
}
/*GPIO_PinState sb = HAL_GPIO_ReadPin(Sb_GPIO_Port,Sb_Pin);
        if(sb == GPIO_PIN_SET)
        {
            ct++;
            state = forward;
            if(tgt_range)
                ranger+=(1.0/32)/5.2;
        } else
        {
            ct--;
            state = backward;
            if(tgt_range)
                ranger-=(1.0/32)/5.2;
        }*/
void Motor::set_rounds(float r)
{
    tgt_range = r;
}


void Motor::set_speed(int ts)
{
    tgt_speed = ts;
}


void Motor::run()
{
    if(tgt_range)
    {

        tgt_speed = pid_range.calculate(tgt_range, ranger);

        if(pid_range.err < 0.5)
        {
            tgt_range = 0;
        }
    }


    duty = pid_speed.calculate(tgt_speed, speed);
    debug[5]= speed;
    debug[1] =duty;

    if(duty>0)
    {
        HAL_GPIO_WritePin(in1_GPIO_Port, in1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(in2_GPIO_Port, in2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(mhtim, mChannel,duty);


    }
    else if(duty<0)
    {
        duty = duty*(-1);
        HAL_GPIO_WritePin(in1_GPIO_Port,in1_Pin,GPIO_PIN_RESET) ;
        HAL_GPIO_WritePin(in2_GPIO_Port,in2_Pin,GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(mhtim, mChannel,duty);
    }
    if(duty<170)
    {
        state = stop;
        HAL_GPIO_WritePin(in1_GPIO_Port,in1_Pin,GPIO_PIN_RESET) ;
        HAL_GPIO_WritePin(in2_GPIO_Port,in2_Pin,GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(mhtim, mChannel,duty);
    }

    /*

    else
    {
        if(duty>0)
        {
            HAL_GPIO_WritePin(in1_GPIO_Port, in1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(in2_GPIO_Port, in2_Pin, GPIO_PIN_RESET);
            __HAL_TIM_SET_COMPARE(mhtim, mChannel,duty);
        }
        else
        {
            duty=duty*(-1);
            HAL_GPIO_WritePin(in1_GPIO_Port,in1_Pin,GPIO_PIN_RESET) ;
            HAL_GPIO_WritePin(in2_GPIO_Port,in2_Pin,GPIO_PIN_SET);
            __HAL_TIM_SET_COMPARE(mhtim, mChannel,duty);
        }
    }
    */

}





