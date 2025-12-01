/**
 * @file    tb6612_example.c
 * @author  syhanjin
 * @date    2025-09-14
 * @brief   an example using the tb6612 and encoder motor
 *
 * --------------------------------------------------------------------------
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Project repository: https://github.com/HITSZ-WTR2026/motor_drivers
 */

#include "drivers/tb6612.h"
#include "interfaces/motor_if.h"
#include "tim.h"

/**
 * 因为并没有配置外设，所以这里直接用一点点魔法避免保存
 * 实际使用时需要配置对应的板上外设
 */
#define htim2          (TIM_HandleTypeDef{})
#define AIN1_GPIO_Port ((GPIO_TypeDef*)NULL)
#define AIN1_Pin       (0x00000000U)
#define AIN2_GPIO_Port ((GPIO_TypeDef*)NULL)
#define AIN2_Pin       (0x00000000U)
#define htim8          (TIM_HandleTypeDef{})

TB6612_t tb6612;
Motor_VelCtrl_t vel_ctrl;
Motor_PosCtrl_t pos_ctrl;


void TIM_Callback(TIM_HandleTypeDef* htim)
{
    /**
     * 处理编码器数据
     *
     * 该函数调用的间隔应当等于 sampling_period (unit: s)
     */
    TB6612_Encoder_DataDecode(&tb6612);

    /**
     * 对于 TB6612 电机，PID 计算的过程中已经完成了 PWM 占空比设置
     * 故无须单独调用发送函数
     */
    Motor_PosCtrlUpdate(&pos_ctrl);
    Motor_VelCtrlUpdate(&vel_ctrl);
}

void TB6612_Control_Init()
{

    /**
     * Step0: 初始化电机，并使能电机
     *
     * 注意 (TB6612_Config_t) 不能省去，否则会编译错误
     */
    TB6612_Init(&tb6612, (TB6612_Config_t){
                             .encoder         = &htim2, //< 编码器使用的定时器
                             .encoder_reverse = false,  //< 编码器反向，编码器方向与实际电机方向相反时启用
                             .motor_reverse   = false,  //< 电机反向，希望电机实际旋转方向与设置的控制方向相反时启用
                             .in1             = {AIN1_GPIO_Port, AIN1_Pin},
                             .in2             = {AIN2_GPIO_Port, AIN2_Pin},
                             .pwm             = {&htim8, TIM_CHANNEL_1},
                             .roto_radio      = 4 * 13, //< 4 * 线数，4 倍是因为开启了双边沿计数
                             .reduction_radio = 30,     //< 减速比
                             .sampling_period = 0.001f, //< 采样间隔 (unit: s)
                         });
    TB6612_Enable(&tb6612);

    /**
     * Step4: 初始化电机控制实例
     *
     * 控制电机所需要的 PID 参数在该步传入
     * 注意：abs_output_max = 1，因为输出代表的是 PWM 的占空比
     */
    Motor_VelCtrl_Init(&vel_ctrl,              //
                       (Motor_VelCtrlConfig_t) //
                       {.motor_type = MOTOR_TYPE_TB6612,
                        .motor      = &tb6612,
                        .pid        = {
                                   .Kp             = 0.1f,
                                   .Ki             = 0.0f,
                                   .Kd             = 0.0f,
                                   .abs_output_max = 1,
                        }});

    Motor_PosCtrl_Init(&pos_ctrl,              //
                       (Motor_PosCtrlConfig_t) //
                       {
                           .motor_type   = MOTOR_TYPE_TB6612,
                           .motor        = &tb6612,
                           .velocity_pid = {
                               .Kp             = 0.001f,
                               .Ki             = 0.0f,
                               .Kd             = 0.0f,
                               .abs_output_max = 1,
                           },
                           .position_pid = {
                               .Kp             = 2.67579f, //
                               .Ki             = 0.26389f, //
                               .Kd             = 0.42164f, //
                               .abs_output_max = 366,      // 本案例使用的电机 MG513P30 峰值转速为 366 rpm
                           },
                           .pos_vel_freq_ratio = 10,
                       });


    /**
     * Step5(可选): 启用或禁用控制实例
     *
     * 控制实例在初始化时默认是启动的，所以大部分情况此步可以省略。
     * 但是在有多个控制实例的情况下，必须仅保持一个控制实例开启
     */
    __MOTOR_CTRL_DISABLE(&vel_ctrl);
    __MOTOR_CTRL_ENABLE(&pos_ctrl);


    /**
     * Step6: 注册定时器回调并开启定时器
     *
     * 需要在 STM32CubeMX -> `Project Manager` -> `Advanced Settings`
     *  -> `Register Callback` 中启用 TIM 回调
     */
    HAL_TIM_RegisterCallback(&htim6, HAL_TIM_PERIOD_ELAPSED_CB_ID, TIM_Callback);
    HAL_TIM_Base_Start_IT(&htim6);
}
