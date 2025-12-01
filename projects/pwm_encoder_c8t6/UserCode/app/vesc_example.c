/**
 * @file    vesc_example.c
 * @author  syhanjin
 * @date    2025-09-30
 * @brief   an example using the vesc driver
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
#include "drivers/vesc.h"
#include "interfaces/motor_if.h"
#include "tim.h"
/**
 * 这是需要控制的 VESC 电机实例
 * 本驱动中 *电机实例* 和 *控制实例* 是分开的，意味着你可以对同一个电机定义不同的 *控制实例* 从而实现切换控制
 * @note 为保证控制接口的一致性，即便 VESC 只支持速度控制，并且完全不需要本驱动进行 PID 计算，
 *       我们仍然保留了 PID 控制器的结构体成员。你无需传入对应配置，VESC 不会识别它。
 */
VESC_t vesc;
/**
 * 速度环控制实例
 */
Motor_VelCtrl_t vel_ctrl;

/**
 * 位置环控制实例
 */
Motor_PosCtrl_t pos_ctrl;

uint32_t prescaler = 0;

/**
 * 定时器回调函数，用于定时进行 PID 计算和 CAN 指令发送
 * @note freq 1000Hz
 * @param htim unused
 */
void TIM_Callback(TIM_HandleTypeDef* htim)
{
    ++prescaler;
    if (prescaler == 100)
    {
        prescaler = 0;
        /**
         * 控制状态更新，不小于 5Hz
         */
        Motor_VelCtrlUpdate(&vel_ctrl);
    }
    Motor_PosCtrlUpdate(&pos_ctrl);
}

void VESC_Control_Init()
{

    /**
     * Step0: 初始化 CAN 过滤器
     *
     * 默认使用一个过滤器 + 掩码模式
     * 亦可以使用其他过滤器模式，处理函数与使用什么过滤器无关，只要保证数据能被接收到即可
     */
    VESC_CAN_FilterInit(&hcan1, 1);

    /**
     * Step1: 注册 VESC CAN 处理回调
     *
     * 需要在 STM32CubeMX -> `Project Manager` -> `Advanced Settings`
     *  -> `Register Callback` 中启用 CAN 回调
     *
     * 我们只使用 Fifo0，因为 Fifo0 的优先度比 Fifo1 高
     */
    HAL_CAN_RegisterCallback(&hcan1, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, VESC_CAN_Fifo0ReceiveCallback);

    /**
     * Step2: 启动 CAN
     *
     * CAN 必须在注册回调后再启用，否则回调无法正常注册，同样地，我们也只使用 Fifo0
     * @note: 如果同时启用 DJI 和 VESC，你需要另定义一个回调处理函数，手动获取数据并手动调用以下两个函数
              void VESC_CAN_BaseReceiveCallback(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* header, uint8_t data[])
              void DJI_CAN_BaseReceiveCallback(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* header, uint8_t data[])
     */
    CAN_Start(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    /**
     * Step3: 初始化电机
     */
    VESC_Init(&vesc, (VESC_Config_t){
                         .hcan       = &hcan1, ///< 电机挂载在的 CAN 句柄
                         .id         = 15,     ///< ID 范围 [VESC_ID_OFFSET, VESC_ID_OFFSET + VESC_NUM)
                         .electrodes = 14,     ///< 电极数，用于 erpm 和 rpm 换算
                     });

    /**
     * Step4: 初始化电机控制实例
     *
     * VESC 的速度环由其本身完成，位置环由于多圈控制问题，由驱动完成，所以速度环控制不需要 PID 参数，
     * 位置环控制只需要第三环参数。
     */
    Motor_VelCtrl_Init(&vel_ctrl, (Motor_VelCtrlConfig_t){
                                      .motor_type = MOTOR_TYPE_VESC,
                                      .motor      = &vesc,
                                  });
    Motor_PosCtrl_Init(&pos_ctrl, (Motor_PosCtrlConfig_t){.motor_type   = MOTOR_TYPE_VESC,
                                                          .motor        = &vesc,
                                                          .position_pid = (MotorPID_Config_t){
                                                              .Kp             = 2.000f,
                                                              .Ki             = 0.0015f,
                                                              .Kd             = 1.200f,
                                                              .abs_output_max = 2000,
                                                          }});


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
