# FreeRTOS
../STM32RTOS培训_嵌入式操作系统介绍.pdf
## 创建项目
### CubeMX
<https://blog.csdn.net/qq_36347513/article/details/122187066>
->System Core->SYS:->imbase:TIM1

->FREERTOS

* mode：v1 
* Tasks and Queues:添加任务,名称,优先级
* Include parameters:选择是否添加某些指令

####Kernel settings：
    
    USE_PREEMPTION： Enabled：RTOS使用抢占式调度器；Disabled：RTOS使用协作式调度器（时间片）。
    TICK_RATE_HZ： 值设置为1000，即周期就是1ms。RTOS系统节拍中断的频率，单位为HZ。
    MAX_PRIORITIES： 可使用的最大优先级数量。设置好以后任务就可以使用从0到（MAX_PRIORITIES - 1）的优先级，其中0位最低优先级，（MAX_PRIORITIES - 1）为最高优先级。
    MINIMAL_STACK_SIZE： 设置空闲任务的最小任务堆栈大小，以字为单位，而不是字节。如该值设置为128 Words，那么真正的堆栈大小就是 128*4 = 512 Byte。
    MAX_TASK_NAME_LEN： 设置任务名最大长度。
    IDLE_SHOULD_YIELD： Enabled 空闲任务放弃CPU使用权给其他同优先级的用户任务。
    USE_MUTEXES： 为1时使用互斥信号量，相关的API函数会被编译。
    USE_RECURSIVE_MUTEXES： 为1时使用递归互斥信号量，相关的API函数会被编译。
    USE_COUNTING_SEMAPHORES： 为1时启用计数型信号量， 相关的API函数会被编译。
    QUEUE_REGISTRY_SIZE： 设置可以注册的队列和信号量的最大数量，在使用内核调试器查看信号量和队列的时候需要设置此宏，而且要先将消息队列和信号量进行注册，只有注册了的队列和信号量才会在内核调试器中看到，如果不使用内核调试器的话次宏设置为0即可。
    USE_APPLICATION_TASK_TAG： 为1时可以使用vTaskSetApplicationTaskTag函数。
    ENABLE_BACKWARD_COMPATIBILITY： 为1时可以使V8.0.0之前的FreeRTOS用户代码直接升级到V8.0.0之后，而不需要做任何修改。
    USE_PORT_OPTIMISED_TASK_SELECTION： FreeRTOS有两种方法来选择下一个要运行的任务，一个是通用的方法，另外一个是特殊的方法，也就是硬件方法，使用MCU自带的硬件指令来实现。STM32有计算前导零指令吗，所以这里强制置1。
    USE_TICKLESS_IDLE： 置1：使能低功耗tickless模式；置0：保持系统节拍（tick）中断一直运行。假设开启低功耗的话可能会导致下载出现问题，因为程序在睡眠中，可用ISP下载办法解决。
    USE_TASK_NOTIFICATIONS： 为1时使用任务通知功能，相关的API函数会被编译。开启了此功能，每个任务会多消耗8个字节。
    RECORD_STACK_HIGH_ADDRESS： 为1时栈开始地址会被保存到每个任务的TCB中（假如栈是向下生长的）。
    Memory management settings：
    
    Memory Allocation： Dynamic/Static 支持动态/静态内存申请
    TOTAL_HEAP_SIZE： 设置堆大小，如果使用了动态内存管理，FreeRTOS在创建 task, queue, mutex, software timer or semaphore的时候就会使用heap_x.c(x为1~5)中的内存申请函数来申请内存。这些内存就是从堆ucHeap[configTOTAL_HEAP_SIZE]中申请的。
    Memory Management scheme： 内存管理策略 heap_4。
    Hook function related definitions：
    
    USE_IDLE_HOOK： 置1：使用空闲钩子（Idle Hook类似于回调函数）；置0：忽略空闲钩子。
    USE_TICK_HOOK： 置1：使用时间片钩子（Tick Hook）；置0：忽略时间片钩子。
    USE_MALLOC_FAILED_HOOK： 使用内存申请失败钩子函数。
    CHECK_FOR_STACK_OVERFLOW： 大于0时启用堆栈溢出检测功能，如果使用此功能用户必须提供一个栈溢出钩子函数，如果使用的话此值可以为1或者2，因为有两种栈溢出检测方法。
    Run time and task stats gathering related definitions：
    
    GENERATE_RUN_TIME_STATS： 启用运行时间统计功能。
    USE_TRACE_FACILITY： 启用可视化跟踪调试。
    USE_STATS_FORMATTING_FUNCTIONS： 与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数prvWriteNameToBuffer()、vTaskList()、vTaskGetRunTimeStats()。
    Co-routine related definitions：
    
    USE_CO_ROUTINES： 启用协程。
    MAX_CO_ROUTINE_PRIORITIES： 协程的有效优先级数目。
    Software timer definitions：
    
    USE_TIMERS： 启用软件定时器。
    Interrupt nesting behaviour configuration：
    
    LIBRARY_LOWEST_INTERRUPT_PRIORITY： 中断最低优先级。
    LIBRARY_LOWEST_INTERRUPT_PRIORITY： 系统可管理的最高中断优先级。
### Code
1. CMakeLists.txt:去掉注释

        add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)

        add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

        add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
2.freertos.c 中
    void StartTask01(void const * argument)函数下写task01代码


##任务，优先级
### 调度算法
* 抢占式调度：高优先级可打断低优先级任务 高优先级任务osdelay()时,低优先级任务可以运行
* 协作式调度：只可能在运行态任务进入阻塞态或是
运行态任务显式调用taskYIELD()时，才会进行上下文切换

FreeRTOS 调度器可确保在就绪或运行状态的任务将 始终在同样处于就绪状态的较低优先级任务之前获得处理器 (CPU) 时间 。 换句话说，被置于运行状态的任务 始终是可运行的最高优先级任务。

任意数量的任务可共用相同的优先级。 如果 configUSE_TIME_SLICING 未经定义， 或者如果 configUSE_TIME_SLICING 设置为 1，则相同优先级的就绪状态任务 将使用时间切片轮询调度方案共享可用的处理时间
### 空闲任务
<https://blog.csdn.net/qq_51963216/article/details/122788392>

优先级最低的任务

空闲任务钩子函数被用于执行低优先级，后台或需要不停处理的
功能代码，

要开启空闲任务钩子函数，在CubeMX中 IDLE_SHOULD_YIELD =enable(默认开启)

然后编写函数
#### vApplicationIdleHook()

###任务API

* 新建task[可在cubemx实现] osThreadId osThreadCreate(const osThreadDef_t * thread_def,void *argument)

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
####阻塞任务 osDelay(ms);
#### 修改优先级  
    
        priority = osThreadGetPriority(Task1Handle);
        osThreadSetPriority(Task1Handle,priority+1);


##内存分配
有heap_1.c到heap4.c

在CubeMX  freertos config parameters中更改









