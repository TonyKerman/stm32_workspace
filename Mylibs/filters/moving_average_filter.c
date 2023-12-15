//
// Created by tony on 23-12-2.
//

#include "moving_average_filter.h"
#include "stdlib.h"


// 初始化移动平均滤波器
void MAF_init(MovingAverageFilter *filter, int windowSize) {
    filter->buffer = (float *)malloc(windowSize * sizeof(float));
    filter->currentIndex = 0;
    filter->windowSize = windowSize;

    // 初始化缓冲区
    for (int i = 0; i < windowSize; ++i) {
        filter->buffer[i] = 0.0;
    }
}

// 更新移动平均滤波器的状态
float MAF_update(MovingAverageFilter *filter, float input) {
    // 更新缓冲区
    filter->buffer[filter->currentIndex] = input;

    // 更新索引
    filter->currentIndex = (filter->currentIndex + 1) % filter->windowSize;

    // 计算移动平均值
    float sum = 0.0;
    for (int i = 0; i < filter->windowSize; ++i) {
        sum += filter->buffer[i];
    }

    return sum / filter->windowSize;
}

// 释放移动平均滤波器的资源
void MAF_free(MovingAverageFilter *filter) {
    free(filter->buffer);
}