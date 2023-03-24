
#include "start.h"
#include "main.h"
#include "tim.h"
#include "serial_debug.h"
#include "pid.h"
#include "string"
#include "usart.h"
using namespace std;


float  get_speed(TIM_HandleTypeDef * tim,int dt=10,int PPR=512);
/**
  *@brief 编码器测速(4分频)
  * cubemx ->tim  
  * PSC: 3
  * Encoder mode: Tl1 and Tl2
  *@param dt : 两次测量的间隔 ms
  *@param PPR: 编码器分辨率
  *@return speed: 速度 rpm
  * 
  */
float get_speed(TIM_HandleTypeDef * tim,int dt,int PPR)
{
    static bool init = true;
    //上一次编码器的值

    if(init)
    {
        __HAL_TIM_SET_COUNTER(&htim2,32768);
        init = false;
    }
    int enc = __HAL_TIM_GET_COUNTER(&htim2);
    float speed=(enc-32768) * (1000.0/dt) * 60 / PPR;//单位 rpm
    //float  speed = enc -32768;
    __HAL_TIM_SET_COUNTER(&htim2,32768);

    return  speed;
    //dbg_add(enc,"enc");
    //dbg_add(lenc,"lenc"); __HAL_TIM_SET_COUNTER(&htim2,32768);
    /*float speed;

    uint8_t direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2); //0正转(countup),1反转(countdown)
    dbg_add(direction,"direction");

    if (direction)
    {
	// 反转时
        if(denc<=0)
            speed = denc * (1000.0/dt) * 60 / PPR; //单位 rpm
            //speed=-1;
        else //计时器溢出时
        {
            speed = (denc-65536) * (1000.0/dt) * 60 / PPR;
            if(speed<-70000)
                dbg_add(0,"fatal");
                    }


    }
    else
    {
        if(denc>=0)
            speed = denc * (1000.0/dt) * 60 / PPR;
        else
            speed = (denc+65536) * (1000.0/dt) * 60 / PPR;
            //speed =-2;
    }

    lenc = enc;

    return speed;*/
}

void startup()
{

    dbg_start(&huart1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);

    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,2000);
    HAL_Delay(2000);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1000);
    HAL_Delay(2000);
    __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1050);

    while(true)
    {
        int dt =20;
        HAL_Delay(dt);
        float speed =get_speed(&htim2,dt=dt);
        //string str ="/*";
        int out = (int)pid_post(600,speed,10,0,0,2500,1500);
        //str+= to_string(out);
        //str+="*/";

        dbg_add(speed,"speed");
        dbg_add(out,"out");
        __HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,out);

        dbg_display();
        //HAL_UART_Transmit(&huart1,(uint8_t *)str.data(),str.length(),10);
    }
}
