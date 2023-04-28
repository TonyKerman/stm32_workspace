# Tim 计时器


### 原理

源时钟经过PSC+1个周期后计数值+1(-1),计数值达到ARR+1时重载

### 周期
(PSC+1)*(ARR+1)/源时钟频率

##API
    #include"tim.h"
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 3;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 65535;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
# 编码器模式 (Encoder mode)
    专门测编码器的, Channel1 ,Channel2 分别连接编码器A相,B相
<https://blog.csdn.net/xuzhexing/article/details/107152512>
## cubemx
###Encoder Mode
#### PSC = 3 
3+1=4,当编码器为四分频时,就不用再/4了
#### Encoder Mode 
Tl1 and Tl2 双边沿触发(编码器4分频)

仅在TI1计数时 相对信号的电平其实就是TI2的电平（不考虑反向的情况）这样再看这张表就会比较容易理解了

在TI2为高电平的时候TI1为上升沿时脉冲计数减1，TI1位下降沿时脉冲计数加1

在TI2为低电平的时候TI1为上升沿时脉冲计数加1，TI1位下降沿时脉冲计数减1
####Polarity
参数设置的是Rising Edge。这个参数的意思是在检测到上升沿的时候就触发encoder捕获AB相的值，而并不是这里设置的是上升沿就只检测AB相的上升沿，下降沿还是同样会计数的。
####Input Filter
滤波值是从1-15，看情况设定，是用来滤除一些杂波的。
## code
打开定时器的encoder模式

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
获取到encoder编码器的计数值

    enc=(uint32_t)(__HAL_TIM_GET_COUNTER(&htim2));
获得当前电机的转向
    #include "stm32f1xx_hal_tim.h"
    __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) # 0为正、1为负
关闭编码器计数

    HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);

设置计数器的计数值

__HAL_TIM_SET_COUNTER(&htimx,number);

设置编码器的最大计数值ARR

__HAL_TIM_SET_AUTORELOAD(&htimx,number)

