//
// Created by tony on 23-11-10.
//

#include "sys_delay.h"
#include "stm32f4xx_hal.h"

void Delay_us( uint32_t cnt)
{
    uint32_t temp;
    SysTick->LOAD = SystemCoreClock/8000000 * cnt;//systick计数器的重载值
    SysTick->VAL=0X00;//清空计数器
    SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
    do
    {
        temp=SysTick->CTRL;//读取当前倒计数值
    }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}

void Delay_ms( uint32_t cnt)
{   uint32_t i=0;
    for(i=0;i<cnt;i++)
        Delay_us(1000);
}

void HAL_Delay(uint32_t cnt)
{
    Delay_ms(cnt);
}
