# 任务，优先级

## 调度算法

* 抢占式调度：高优先级可打断低优先级任务 高优先级任务osdelay()时,低优先级任务可以运行
* 协作式调度：只可能在运行态任务进入阻塞态或是

运行态任务显式调用taskYIELD()时，才会进行上下文切换

任意数量的任务可共用相同的优先级。 如果 configUSE_TIME_SLICING 未经定义， 或者如果 configUSE_TIME_SLICING 设置为 1，则相同优先级的就绪状态任务 将使用时间切片轮询调度方案共享可用的处理时间

## 任务API




### hal api(不使用)

* 新建task[可在cubemx实现] osThreadId osThreadCreate(const osThreadDef_t *thread_def,void*argument)

    eg:

        osThreadDef(myTask01, StartDefaultTask, osPriorityNormal, 0, 128);//设置task
        myTask01Handle = osThreadCreate(osThread(myTask01), NULL);

* 删除task        osStatus osThreadTerminate(osThreadId thread_id);
* 获取task ID     osThreadID osThreadGetId();
* 检查task是否挂起 osStatus osThreadIsSuspended(osThreadId thread_id)
* 恢复task        osStatus osThreadResume(osThreadId thread_id)
* 检查task状态     osThreadState osThreadGetState(osThreadId thread_id)
* 挂起task        osStatus osThreadSuspend (osThreadId thread_id)
* 恢复所有tasks    osStatus osThreadResumeAll (void)
* 挂起所有tasks    osStatus osThreadSuspendAll (void)
### 阻塞任务 osDelay(ms);

### 修改优先级  

        priority = osThreadGetPriority(Task1Handle);
        osThreadSetPriority(Task1Handle,priority+1);

## 内存分配
有heap_1.c到heap4.c

在CubeMX  freertos config parameters中更改


## 空闲任务

优先级最低的任务

空闲任务钩子函数被用于执行低优先级，后台或需要不停处理的
功能代码，

要开启空闲任务钩子函数，在CubeMX中 IDLE_SHOULD_YIELD =enable(默认开启)

然后编写函数

### vApplicationIdleHook()

