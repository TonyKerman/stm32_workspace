//
// Created by tony on 23-12-2.
//

#ifndef _MOVING_AVERAGE_FILTER_H
#define _MOVING_AVERAGE_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif
#define WINDOW_SIZE 3  // 移动平均窗口大小
// 移动平均滤波器结构体
typedef struct {
    float *buffer;      // 存储最近一段时间的输入数据
    int currentIndex;   // 当前缓冲区的索引
    int windowSize;     // 移动平均窗口大小
} MovingAverageFilter;
void MAF_init(MovingAverageFilter *filter, int windowSize);
float MAF_update(MovingAverageFilter *filter, float input);
void MAF_free(MovingAverageFilter *filter);
#ifdef __cplusplus
}
#endif
#endif //_MOVING_AVERAGE_FILTER_H
