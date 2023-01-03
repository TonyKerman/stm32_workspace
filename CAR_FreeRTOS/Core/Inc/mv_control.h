//
// Created by Lenovo on 2022/11/14.
//

#ifndef CAR_MV_CONTROL_H
#define CAR_MV_CONTROL_H

#ifdef __cplusplus

extern "C"
{
#endif
#include "main.h"
extern enum mv_states{stop,forword,backword,tnleft,tnright};
void move(mv_states s, int arg);


#ifdef __cplusplus
}
#endif





#endif

