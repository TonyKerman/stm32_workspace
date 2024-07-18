# guide

## 作用

二值信号量：同步线程
互斥信号量：防止同时使用资源

## 使用

0. include
```c
    #include "semphr.h"
```

1. 声明
```c
    SemaphoreHandle_t sem;
```

2. 根据类型定义
```c
    sem = xSemaphoreCreateBinary(); //二值信号量
    sem = xSemaphoreCreateMutex(); //互斥信号量
```
    

3. 使用
```c
    xSemaphoreTake(sem);
    xSemaphoreGive(sem);
```
    

# notebook

## Binary semaphore 二值信号量

二值信号量可以在某个特殊的中断发生时，让任务解除阻塞，相当于让任务与中断
同步。这样就可以让中断事件处理量大的工作在同步任务中完成，中断服务例程(ISR)
中只是快速处理少部份工作。如此，中断处理可以说是被”推迟(deferred)”到一个”处理
(handler)”任务

信号量可以看作是一个深度为 1 的队列。这个队列由于
最多只能保存一个数据单元，所以其不为空则为满(所谓”二值”)。延迟处理任务调用
xSemaphoreTake()时，等效于带阻塞时间地读取队列，如果队列为空的话任务则进入
阻塞态。当事件发生后，ISR 简单地通过调用 xSemaphoreGiveFromISR()放置一个令
牌(信号量)到队列中，使得队列成为满状态。这也使得延迟处理任务切出阻塞态，并移
除令牌，使得队列再次成为空。当任务完成处理后，再次读取队列，发现队列为空，又
进入阻塞态，等待下一次事件发生。

### 创建

in USERmain.h

    //声明
    extern SemaphoreHandle_t servo_start_sem;

in USERmain.c

    //定义
    SemaphoreHandle_t sem;
    void main(){
        //赋值
        servo_start_sem = xSemaphoreCreateBinary();
    }

### 使用

#### xSemaphoreTake()

xSemaphoreTake()不能在中断服务例程中调用。

* 参数名

xSemaphore 获取得到的信号量

信号量由定义为 xSemaphoreHandle 类型的变量引用。信号量在使
用前必须先创建。

xTicksToWait 阻塞超时时间。任务进入阻塞态以等待信号量有效的最长时间。
如果 xTicksToWait 为 0，则 xSemaphoreTake()在信号量无效时会
立即返回。
阻塞时间是以系统心跳周期为单位的，所以绝对时间取决于系统心
跳频率。常量 portTICK_RATE_MS 可以用来把心跳时间单位转换
为毫秒时间单位。
如果把 xTicksToWait 设置为 portMAX_DELAY ，并且在
FreeRTOSConig.h 中设定 INCLUDE_vTaskSuspend 为 1，那么阻
塞等待将没有超时限制。

* 返回值 有两个可能的返回值:

1. pdPASS

只有一种情况会返回 pdPASS，那就是成功获得信号量。

如果设定了阻塞超时时间(xTicksToWait 非 0)，在函数返回之前任务
将被转移到阻塞态以等待信号量有效。如果在超时到来前信号量变
为有效，亦可被成功获取，返回 pdPASS。

2. pdFALSE

未能获得信号量。

如果设定了阻塞超时时间（xTicksToWait 非 0），在函数返回之前任
务将被转移到阻塞态以等待信号量有效。但直到超时信号量也没有
变为有效，所以不会获得信号量，返回 pdFALSE。

### Eg

    xSemaphoreTake(sem, portMAX_DELAY);

#### xSemaphoreGiveFromISR() and xSemaphoreGive()

参数名

xSemaphore 给出的信号量

信号量由定义为 xSemaphoreHandle 类型的变量引用。
信号量在使用前必须先创建。

pxHigherPriorityTaskWoken 对某个信号量而言，可能有不止一个任务处于阻塞态在
等待其有效。调用 xSemaphoreGiveFromISR()会让信
号量变为有效，所以会让其中一个等待任务切出阻塞
态。如果调用 xSemaphoreGiveFromISR()使得一个任
务解除阻塞，并且这个任务的优先级高于当前任务(也就
是被中断的任务)，那么 xSemaphoreGiveFromISR()会
在函数内部将 *pxHigherPriorityTaskWoken 设 为
pdTRUE。

如 果 xSemaphoreGiveFromISR() 将此值设为
pdTRUE，则在中断退出前应当进行一次上下文切换。
这样才能保证中断直接返回到就绪态任务中优先级最
高的任务中。

返回值 有两个可能的返回值:

1. pdPASS

xSemaphoreGiveFromISR()调用成功。

2. pdFAIL

如果信号量已经有效，无法给出，则返回 pdFAIL
