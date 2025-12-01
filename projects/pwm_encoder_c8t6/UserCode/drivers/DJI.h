/**
 * @file    DJI.h
 * @author  syhanjin
 * @date    2025-09-03
 * @version 0.0.1
 * @brief   DJI Motor driver
 *
 * 大疆电机驱动层（并非控制层）
 * 本驱动仅实现对大疆电机的*控制和数据解析*，不负责控制大疆电机
 *
 * 支持的电机类型
 *  - M3508_C620
 *  - M2006_C610
 *
 * Detailed description (optional).
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
#ifndef DJI_H
#define DJI_H
#define __DJI_VERSION__       "0.1.0"

#define DJI_ERROR_HANDLER()   Error_Handler()

#define CAN_NUM               (2)

#define DJI_M2006_C610_IQ_MAX (10000)
#define DJI_M3508_C620_IQ_MAX (16384)


#include <stdbool.h>
#include "main.h"

typedef enum
{
    M3508_C620 = 0U,
    M2006_C610,

    DJI_MOTOR_TYPE_COUNT
} DJI_MotorType_t;

typedef enum
{
    IQ_CMD_GROUP_1_4 = 0U,
    IQ_CMD_GROUP_5_8 = 4U,
} DJI_IqSetCmdGroup_t;

typedef struct
{
    bool enable;    // 是否启用
    bool auto_zero; // 是否自动判断零点
    bool reverse;   ///< 是否反转

    DJI_MotorType_t motor_type; //< 电机类型
    CAN_TypeDef* can;           //< CAN 实例
    uint8_t id1;                //< 电调 ID (1 ~ 8)
    float angle_zero;           //< 零点角度 (unit: degree)

    float inv_reduction_rate; ///< 减速比

    /* Feedback */
    uint32_t feedback_count; //< 接收到的反馈数据数量
    struct
    {
        float mech_angle; //< 单圈机械角度 (unit: degree)
        float rpm;        //< 转速
        // float current; //< 电流大小
        // float temperature; //< 温度

        int32_t round_cnt; //< 圈数
    } feedback;

    /* Data */
    float abs_angle; //< 电机轴输出角度 (unit: degree)
    float velocity;  //< 电机轴输出速度 (unit: rpm)

    /* Output */
    uint16_t iq_cmd; //< 电流指令值
} DJI_t;

typedef struct
{
    CAN_TypeDef* can; //< CAN 实例
    DJI_t* motors[8]; //< 电机指针数组
} DJI_FeedbackMap;

typedef struct
{
    bool auto_zero;
    bool reverse; ///< 是否反转
    DJI_MotorType_t motor_type;
    CAN_HandleTypeDef* hcan;
    uint8_t id1;          ///< 电机编号 1~8
    float reduction_rate; ///< 外接减速比
} DJI_Config_t;

/**
 * 设置电流值（不发送）
 * @param __DJI_HANDLE__
 * @param __IQ_CMD__ 设置电流值，int16_t，最大值参考 DJI_[Type]_IQ_MAX
 */
#define __DJI_SET_IQ_CMD(__DJI_HANDLE__, __IQ_CMD__) (((DJI_t*)(__DJI_HANDLE__))->iq_cmd = (int16_t)(__IQ_CMD__))

#define __DJI_GET_ANGLE(__DJI_HANDLE__)              (((DJI_t*)(__DJI_HANDLE__))->abs_angle)
#define __DJI_GET_VELOCITY(__DJI_HANDLE__)           (((DJI_t*)(__DJI_HANDLE__))->velocity)

void DJI_ResetAngle(DJI_t* hdji);
void DJI_Init(DJI_t* hdji, DJI_Config_t dji_config);
void DJI_CAN_FilterInit(CAN_HandleTypeDef* hcan, uint32_t filter_bank);

void DJI_CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan);
void DJI_CAN_Fifo1ReceiveCallback(CAN_HandleTypeDef* hcan);
void DJI_CAN_BaseReceiveCallback(const CAN_HandleTypeDef* hcan, const CAN_RxHeaderTypeDef* header, uint8_t data[]);

void DJI_SendSetIqCommand(CAN_HandleTypeDef* hcan, DJI_IqSetCmdGroup_t cmd_group);

#endif // DJI_H
