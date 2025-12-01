/**
 * @file    dji_example.c
 * @author  syhanjin
 * @date    2025-09-14
 * @brief   an example using the dji driver
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


#include "bsp/can_driver.h"
#include "can.h"
#include "drivers/DJI.h"
#include "interfaces/motor_if.h"
#include "tim.h"

/**
 * 这是需要控制的大疆电机实例
 * 本驱动中 *电机实例* 和 *控制实例* 是分开的，意味着你可以对同一个电机定义不同的 *控制实例* 从而实现切换控制
 */
DJI_t dji;

/**
 * 位置环控制实例
 */
Motor_PosCtrl_t pos_dji;

/**
 * 速度环控制实例
 */
Motor_VelCtrl_t vel_dji;

/**
 * 定时器回调函数，用于定时进行 PID 计算和 CAN 指令发送
 * @param htim unused
 */
void TIM_Callback(TIM_HandleTypeDef* htim)
{
    /**
     * 进行 PID 计算
     *
     * 只有被启用 (hctrl->enable == true) 的控制实例才会执行计算
     */
    Motor_PosCtrlUpdate(&pos_dji);
    Motor_VelCtrlUpdate(&vel_dji);

    /**
     * 发送控制信号
     *
     * IQ_CMD_GROUP_1_4 和 IQ_CMD_GROUP_5_8 代表发送的电调 ID 组
     */
    DJI_SendSetIqCommand(&hcan1, IQ_CMD_GROUP_1_4);
    // DJI_SendSetIqCommand(&hcan1, IQ_CMD_GROUP_5_8);
}


void DJI_Control_Init()
{

    /**
     * Step0: 初始化 CAN 过滤器
     *
     * 默认使用一个过滤器 + 掩码模式
     * 亦可以使用其他过滤器模式，处理函数与使用什么过滤器无关，只要保证数据能被接收到即可
     */
    DJI_CAN_FilterInit(&hcan1, 0);

    /**
     * Step1: 注册 DJI CAN 处理回调
     *
     * 需要在 STM32CubeMX -> `Project Manager` -> `Advanced Settings`
     *  -> `Register Callback` 中启用 CAN 回调
     *
     * 一般情况下我们只使用 Fifo0，因为 Fifo0 的优先度比 Fifo1 高，当然也可以两个都使用
     */
    HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, DJI_CAN_Fifo0ReceiveCallback);
    // HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID, DJI_CAN_Fifo1ReceiveCallback);

    /* Step2: 启动 CAN
     *
     * CAN 必须在注册回调后再启用，否则回调无法正常注册，同样地，我们一般也只使用 Fifo0，亦可以两个都开
     */
    CAN_Start(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    // CAN_Start(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING);

    /**
     * Step3: 初始化电机
     *
     * auto_zero 这个参数实际上没有意义，因为 DJI 电机上电时本来就是清零了的，
     * 只是参考的学长的驱动莫名的有这个东西，所以写的时候也写了这个东西，只是发现
     * 用不上而已。
     *
     * 注意 (DJI_Config_t) 不能省去，否则会编译错误
     */
    DJI_Init(&dji, (DJI_Config_t){
                       .auto_zero  = false,      //< 是否在启动时自动清零角度
                       .hcan       = &hcan1,     //< 电机挂载在的 CAN 句柄
                       .motor_type = M3508_C620, //< 电机类型
                       .id1        = 1,          //< 电调 ID (1~8)
                   });

    /**
     * Step4: 初始化电机控制实例
     *
     * 控制电机所需要的 PID 参数在该步传入
     * 注意：整个控制器内部并不会对输出的电流值进行限制，所以如果 abs_output_max
     *      超过限幅，输出参数很可能溢出，故 abs_output_max 必须小于或等于电机
     *      实际能接收的最大值
     */
    Motor_VelCtrl_Init(&vel_dji, //
                       (Motor_VelCtrlConfig_t){
                           .motor_type = MOTOR_TYPE_DJI, //< 电机类型
                           .motor      = &dji,           //< 控制的电机
                           .pid        = (MotorPID_Config_t){
                                      .Kp             = 4.7f,                 //
                                      .Ki             = 0.15f,                //
                                      .Kd             = 0.15f,                //
                                      .abs_output_max = DJI_M3508_C620_IQ_MAX //< 限幅为电流控制最大值
                           },
                       });
    /**
     * 如之前所言：同一个电机可以有不同的控制实例，故这两个初始化都可以传入同一个电机
     */
    Motor_PosCtrl_Init(&pos_dji, //
                       (Motor_PosCtrlConfig_t){
                           .motor_type   = MOTOR_TYPE_DJI, //< 电机类型
                           .motor        = &dji,           //< 控制的电机
                           .velocity_pid = (MotorPID_Config_t){
                               .Kp             = 12.0f,  //<
                               .Ki             = 0.20f,  //<
                               .Kd             = 5.00f,  //<
                               .abs_output_max = 8000.0f //< DJI_M3508_C620_IQ_MAX //< 限幅为电流控制最大值
                           },
                           .position_pid = (MotorPID_Config_t){
                               .Kp             = 80.0f,  //<
                               .Ki             = 1.00f,  //<
                               .Kd             = 0.00f,  //<
                               .abs_output_max = 2000.0f //< 限速，这是外环对内环的输出限幅
                           },
                           .pos_vel_freq_ratio = 1, //< 内外环频率比（外环的频率可能需要比内环低）
                       });

    /**
     * Step5(可选): 启用或禁用控制实例
     *
     * 控制实例在初始化时默认是启动的，所以大部分情况此步可以省略。
     * 但是在有多个控制实例的情况下，必须仅保持一个控制实例开启
     */
    __MOTOR_CTRL_DISABLE(&vel_dji);
    __MOTOR_CTRL_ENABLE(&pos_dji);

    /**
     * Step6: 注册定时器回调并开启定时器
     *
     * 需要在 STM32CubeMX -> `Project Manager` -> `Advanced Settings`
     *  -> `Register Callback` 中启用 TIM 回调
     */
    HAL_TIM_RegisterCallback(&htim6, HAL_TIM_PERIOD_ELAPSED_CB_ID, TIM_Callback);
    HAL_TIM_Base_Start_IT(&htim6);
}
