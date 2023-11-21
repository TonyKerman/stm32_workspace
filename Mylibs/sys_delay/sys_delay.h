//
// Created by tony on 23-11-10.
//

#ifndef SYS_DELAY_H
#define SYS_DELAY_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f4xx_hal.h"
    //阻塞延时，使用systick定时器，最大延时2^24/168M=0.1s
    //重写完基于systick的延时之后，hal库自带的HAL_Delay就没用了，如果有程序中还有HAL_Delay()函数，程序不会报错，但在单片机运行的时候会卡在这里。
    //请一定记住不要同时使用两者
    void Delay_us( uint32_t cnt);
    void Delay_ms( uint32_t cnt);
    void HAL_Delay(uint32_t Delay);
#ifdef __cplusplus
}
#endif
#endif //SYS_DELAY_H
