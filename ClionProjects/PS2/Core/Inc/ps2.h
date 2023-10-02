//
// Created by tony on 2023/2/25.
//

#ifndef FREERTOS_DEMO1_PS2_H
#define FREERTOS_DEMO1_PS2_H
#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __cplusplus
}
#include "spi.h"
struct ps2outputTypeDef{
    bool SELECT,L3,R3,START,LUP,LRIGHT,LDOWN,LLEFT;
    bool L2,R2,L1,R1,RUP,RRIGHT,RDOWN,RLEFT;
    uint8_t RX,RY,LX,LY;
};
//void ps2_receive(SPI_HandleTypeDef * spihandle,ps2outputTypeDef & data);
//void PS2_Get(void);



#endif



#endif //FREERTOS_DEMO1_PS2_H
