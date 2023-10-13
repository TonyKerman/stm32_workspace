//
// Created by tony on 23-10-3.
//

#ifndef USER_MAIN_H
#define USER_MAIN_H
#ifdef __cplusplus
extern "C" {
#endif
#include "semphr.h"
    extern SemaphoreHandle_t data_mutex;
    extern SemaphoreHandle_t sync_mutex;
    void StartTestTask(void *argument);
    void UserStartDefaultTask(void *argument);
    void TestTask(void *argument);
#ifdef __cplusplus
}

#endif
#endif //DJI_ABOARD_USER_MAIN_H
