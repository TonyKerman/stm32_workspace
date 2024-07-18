# 任务，优先级

## 调度算法

* 抢占式调度：高优先级可打断低优先级任务 高优先级任务osdelay()时,低优先级任务可以运行
* 协作式调度：只可能在运行态任务进入阻塞态或是

运行态任务显式调用taskYIELD()时，才会进行上下文切换

任意数量的任务可共用相同的优先级。 如果 configUSE_TIME_SLICING 未经定义， 或者如果 configUSE_TIME_SLICING 设置为 1，则相同优先级的就绪状态任务 将使用时间切片轮询调度方案共享可用的处理时间

## 任务API

### xTaskCreate 创建任务

portBASE_TYPE xTaskCreate( pdTASK_CODE pvTaskCode, const signed portCHAR * const pcName, unsigned portSHORT usStackDepth, void *pvParameters,unsigned portBASE_TYPE uxPriority,xTaskHandle *pxCreatedTask );

#### 参数

* pvTaskCode 任务函数的函数名 eg: mainTask

* pcName 具有描述性的任务名。这个参数不会被 FreeRTOS 使用。其只是单纯地用于辅助调试。eg:"main task"

* usStackDepth 分配多大的栈空间，这个值指定的是栈空间可以保存多少个字(word)，而不是多少个字
节(byte)。比如说，如果是 32 位宽的栈空间，传入的 usStackDepth
值为 100，则将会分配 400 字节的栈空间(100 * 4bytes)。

* pvParameters 任务函数接受一个指向 void 的指针(void*)。pvParameters 的值即是传递到任务中的值。
  
* 指定任务执行的优先级。优先级的取值范围可以从最低优先级 0 到
最高优先级(configMAX_PRIORITIES – 1)

* pxCreatedTask pxCreatedTask 用于传出任务的句柄。这个句柄将在 API 调用中对
该创建出来的任务进行引用，比如改变任务优先级，或者删除任务。
如果应用程序中不会用到这个任务的句柄，则 pxCreatedTask 可以
被设为 NULL。
#### 返回值 
有两个可能的返回值：
1. pdTRUE 
表明任务创建成功。
2. errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY 
由于内存堆空间不足，FreeRTOS 无法分配足够的空间来保存任务
结构数据和任务栈，因此无法创建任务

###  任务优先级api
#### vTaskPrioritySet 改变任务的优先级
void vTaskPrioritySet( xTaskHandle pxTask, unsigned portBASE_TYPE uxNewPriority );


#### uxTaskPriorityGet 查询任务的优先级
uxTaskPriorityGet( xTaskHandle pxTask) ;

### vTaskDelete 删除任务
void vTaskDelete( xTaskHandle pxTaskToDelete );
* 该函数把删除的任务从各个列表中删除，若删除的是当前正在执行的任务，先放入等待删除列表中，等到空闲任务才真正删除该任务。
* 在任务创建时，堆栈和任务控制块的有不同的创建方式，则在任务删除时会用不同的方式回收空间。
* 若通过动态方式创建的任务，会自动回收空间，静态方式需要通过程序员自身释放空间。









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

