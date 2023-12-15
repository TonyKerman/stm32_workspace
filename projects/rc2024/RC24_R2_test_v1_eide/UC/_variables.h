//
// Created by tony on 23-11-4.
//

#ifndef _VARIABLES_H
#define _VARIABLES_H

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"

#include "semphr.h"
#include "steering_wheel_chassis.h"
#include "wtr_mavlink.h"
extern swChassis_t mychassis;
extern mavlink_pose_t  actual_pose;
extern mavlink_speed_t mv_cmd;
extern SemaphoreHandle_t data_mutex;
extern SemaphoreHandle_t sync_mutex;
#ifdef __cplusplus
}
#endif
#endif //_VARIABLES_H
