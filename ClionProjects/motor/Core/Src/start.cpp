
#include "start.h"
#include "main.h"
#include "tim.h"
#include "pid.h"
#include "usart.h"
using namespace std;

PidValsTypeDef  pid_r;
PidValsTypeDef  pid_l;
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
    int enc = __HAL_TIM_GET_COUNTER(tim);
    float speed=(enc-32768) * (1000.0/dt) * 60 / PPR;//单位 rpm
    //float  speed = enc -32768;
    __HAL_TIM_SET_COUNTER(tim,32768);

    return  speed;

}

void startup()
{

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2,32768);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim3,32768);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,2000);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,2000);
    HAL_Delay(2000);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,1000);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,1000);
    HAL_Delay(2000);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,1050);
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,1050);
    while(true)
    {
        //时间间隔 ms
        int dt =50;
        HAL_Delay(dt);
        float speedr =get_speed(&htim2,dt=dt);
        float speedl =get_speed(&htim3,dt=dt);
        int outr = 1000+(int)pid_post(&pid_r,12500,speedr,0.075,0.0035,0.0015,300,400);
        int outl = 1000+(int)pid_post(&pid_l,12500,speedl,0.075,0.0035,0.0015,300,400);

        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,outr);
        __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,outl);

        if(speedl>0&&speedr>0)
            HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
        //HAL_UART_Transmit(&huart1,(uint8_t *)str.data(),str.length(),10);
    }
}
