//
// Created by Lenovo on 2022/9/24.
//
#include "start.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "motor_control.h"
#include "Caculate.h"
#include "DJI.h"
#include "wtr_can.h"
#include "wtr_uart.h"
#include <string>
using namespace std;

#define LED_GREEN_ON() HAL_GPIO_WritePin(LDG_GPIO_Port,LDG_Pin,GPIO_PIN_RESET)
#define LED_GREEN_OFF() HAL_GPIO_WritePin(LDG_GPIO_Port,LDG_Pin,GPIO_PIN_SET)
Motor ml(&htim2,TIM_CHANNEL_1,
         IN1_GPIO_Port,IN1_Pin,
         IN2_GPIO_Port,IN2_Pin,
         HALL_LA_GPIO_Port,HALL_LA_Pin,
         HALL_LB_GPIO_Port,HALL_LB_Pin);
Motor mr(&htim2,TIM_CHANNEL_2,
         IN3_GPIO_Port,IN3_Pin,
         IN4_GPIO_Port,IN4_Pin,
         HALL_RA_GPIO_Port,HALL_RA_Pin,
         HALL_RB_GPIO_Port,HALL_RB_Pin);

string str;
class Servo
{
    int duty;
    uint32_t mChannel;
    TIM_HandleTypeDef *mhtim;
public:
    Servo(TIM_HandleTypeDef *tim,uint32_t mCh)
    {
        mhtim = tim;
        mChannel = mCh;
        HAL_TIM_PWM_Start(mhtim,mChannel);
    }
    void set(int angle)
    {
        __HAL_TIM_SET_COMPARE(mhtim,mChannel,int(  (float(angle)/90+0.5)/20* __HAL_TIM_GET_AUTORELOAD(mhtim)  )   );
    }
};


void setup()
{
    //灯
    HAL_GPIO_WritePin(LDR_GPIO_Port,LDR_Pin,GPIO_PIN_RESET);

    LED_GREEN_ON();
    HAL_Delay(500);
    LED_GREEN_OFF();
    HAL_Delay(500);
    LED_GREEN_ON();
    HAL_Delay(500);
    LED_GREEN_OFF();
    HAL_GPIO_WritePin(LDR_GPIO_Port,LDR_Pin,GPIO_PIN_SET);
    //串口
    str= "--------------Hello,Stm32!--------------\n\n\n\n\n\n\n\n\n";
    str += to_string(__HAL_TIM_GET_AUTORELOAD(&htim2)+1) ;
    str+="  ";
    str += to_string(__HAL_TIM_GET_CLOCKDIVISION(&htim2)+1);
    str+="  ";
    str += to_string(HAL_RCC_GetSysClockFreq());
    HAL_UART_Transmit(&huart6,(uint8_t *)str.data(),str.size(),100);
    HAL_Delay(500);
    //时钟
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim9);
    //电机
    ml.start();
    mr.start();
    HAL_GPIO_WritePin(VCC1_GPIO_Port,VCC1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(VCC2_GPIO_Port,VCC2_Pin,GPIO_PIN_SET);
    //初始化控制3508
    CANFilterInit(&hcan1);
    hDJI[0].motorType =M3508;
    hDJI[1].motorType =M3508;
    hDJI[2].motorType =M2006;
    DJI_Init();
    //舵机
//    Servo S1(&htim8,TIM_CHANNEL_1);
//    S1.set(90);
//    HAL_Delay(1000);
//    S1.set(0);
//    HAL_Delay(500);
//    S1.set(90);

    //ml.tgt_speed = 100;
    //HAL_UART_Receive_IT(&huart1,JoyStickReceiveData,18);
    while(true)
    {
        //wait for start
        HAL_UART_Receive_IT(&huart1,JoyStickReceiveData,18);
        int s = Raw_Data.ch1-1024;
        ml.tgt_speed = s;

        //speedServo(500,&hDJI[0]);
//        speedServo(500*(-1),&hDJI[1]);
        positionServo(posRef*1.5,&hDJI[2]);
        CanTransmit_DJI_1234(&hcan1,hDJI[0].speedPID.output,hDJI[1].speedPID.output,hDJI[2].speedPID.output,0);
        HAL_Delay(1);

    }
}

void test()
{
    HAL_GPIO_WritePin(LDR_GPIO_Port,LDR_Pin,GPIO_PIN_RESET);
    LED_GREEN_ON();
    HAL_Delay(200);
    LED_GREEN_OFF();
    HAL_Delay(200);
    LED_GREEN_ON();
    HAL_Delay(200);
    LED_GREEN_OFF();
    HAL_GPIO_WritePin(LDR_GPIO_Port,LDR_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(VCC1_GPIO_Port,VCC1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(VCC2_GPIO_Port,VCC2_Pin,GPIO_PIN_SET);
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim9);
    ml.start();
    ml.tgt_speed =150;
    while (true)
    {

        HAL_Delay(3000);
        ml.tgt_speed =ml.tgt_speed*(-1);
    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==htim8.Instance)
    {
        /*
         * TIM8:Pried = 20,ms
         * Contrel Servos,measure speed
         */
        //HAL_GPIO_TogglePin(LDG_GPIO_Port,LDG_Pin);
        ml.TIM_PeriodElapsedCallback();
        mr.TIM_PeriodElapsedCallback();
        // car move functions:
    }
    if(htim->Instance==htim9.Instance)
    {
        str =":" ;
//        str += to_string(mr.tgt_speed);
//        str +=",";
//        str += to_string(mr.speed);
//        str +=",";
//        str += to_string(mr.duty);
//        str +=",";
//        str += to_string(ml.tgt_speed);
//        str +=",";
//        str += to_string(ml.speed);
//        str +=",";
//        str += to_string(ml.duty);
        str +=",";
        str += to_string(HAL_GPIO_ReadPin(HALL_RA_GPIO_Port,HALL_RA_Pin));
        str +=",";
        str += to_string(HAL_GPIO_ReadPin(HALL_RB_GPIO_Port,HALL_RB_Pin));
        str +="\n";
        HAL_UART_Transmit_DMA(&huart6,(uint8_t *)str.data(),str.size());
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    ml.GPIO_EXTI_Callback(GPIO_Pin);
    mr.GPIO_EXTI_Callback(GPIO_Pin);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == huart1.Instance)
    {
        UART1Decode();

        //str = to_string(Raw_Data.ch0);
		//	  str+="\n";
        //HAL_UART_Transmit_DMA(&huart6,(uint8_t *)str.data(),str.size());
    }
}

