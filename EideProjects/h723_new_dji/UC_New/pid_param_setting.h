//
// Created by tony on 24-11-16.
//

#ifndef PID_AUTOSETTING_H
#define PID_AUTOSETTING_H
#include "pid.h"

typedef enum
{
PID_AUTOSETTING_PROCESS=0,
PID_AUTOSETTING_DONE=1
}PID_AutoSetting_State;
typedef struct
{
double lambda;
double test_output;
double test_time_ms;
double init_fdbk;

double delta;
double dt_ms;
double *pv;
uint16_t pv_num;
uint16_t pv_index;
}PID_AutoSetting_Buffers;


void PID_AutoSetting_Init(PID_AutoSetting_Buffers *pas,double lambda,double init_fdbk,double dt_ms,double test_output);
PID_AutoSetting_State PID_AutoSetting_Update(PID_AutoSetting_Buffers *pas,double fdbk);
void PID_AutoSetting_Set(PIDController *pid,PID_AutoSetting_Buffers *pas);

#endif //PID_AUTOSETTING_H