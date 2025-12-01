/**
 * @file    vesc.h
 * @author  syhanjin
 * @date    2025-09-15
 * @brief   VESC driver
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
 *
 * reference:
 */
#ifndef VESC_H
#define VESC_H

#include <stdbool.h>


#include "main.h"

#ifndef VESC_CAN_NUM
#define VESC_CAN_NUM (2)
#endif

#ifndef VESC_NUM
/**
 * VESC 电机数量
 */
#define VESC_NUM (16)
#endif

#ifndef VESC_ID_OFFSET
/**
 * VESC 电调 id 偏移量，驱动内只能注册 id 范围
 *      [VESC_ID_OFFSET, VESC_ID_OFFSET + VESC_NUM)
 * 的电机，会静态分配
 *      4 * VESC_CAN_NUM * ( 1 + VESC_NUM )
 * 的内存.
 */
#define VESC_ID_OFFSET (0)
#endif

/* 参数范围限制 */
#define VESC_SET_DUTY_MAX              (1.0f)
#define VESC_SET_CURRENT_MAX           (2e6f)
#define VESC_SET_CURRENT_BRAKE_MAX     (2e6f)
#define VESC_SET_RPM_MAX               (2e4f)
#define VESC_SET_POS_MAX               (360.0f)
#define VESC_SET_CURRENT_REL_MAX       (1.0f)
#define VESC_SET_CURRENT_BRAKE_REL_MAX (1.0f)

typedef enum
{
    VESC_CAN_SET_DUTY          = 0U, ///< 设置占空比, Data: Duty Circle * 100,000 (int32)
    VESC_CAN_SET_CURRENT       = 1U, ///< 设置电流, Data: current * 1000 (int32)
    VESC_CAN_SET_CURRENT_BRAKE = 2U, ///< 设置刹车电流, Data: current * 1000 (int32)
    VESC_CAN_SET_RPM           = 3U, ///< 设置转速, Data: ERPM (int32)
    VESC_CAN_SET_POS           = 4U, ///< 设置位置, Data: pos * 1,000,000 (int32) unit: ?

    VESC_CAN_SET_CURRENT_REL       = 10U, ///< 设置相对电流，Data: ratio (-1 to 1) * 100,000 (int32). 相对于最大值和最小值
    VESC_CAN_SET_CURRENT_BRAKE_REL = 11U, ///< 设置相对刹车电流，Data: ratio (-1 to 1) * 100,100 (int32). 相对于最大值和最小值
} VESC_CAN_PocketSet_t;

typedef enum
{
    /**
     * There are two versions of this command, command 21 sets the operating
     * current limits, command 22 sets the operating current limits and sends
     * them to EEPROM
     */
    VESC_CAN_CONF_CURRENT_LIMITS       = 21U, ///< 设置电流限制, Data: Max Current Limit * 1000 (int32), Min Current Limit * 1000 (int32)
    VESC_CAN_CONF_STORE_CURRENT_LIMITS = 22U, ///< 设置电流限制, Data: Max Current Limit * 1000 (int32), Min Current Limit * 1000 (int32)

    /**
     * There are two versions of this command, command 23 sets the operating
     * current limits, command 24 sets the operating current limits and sends
     * them to EEPROM
     */
    VESC_CAN_CONF_CURRENT_LIMITS_IN       = 23U, ///< 设置输入电流限制, Data: Max Current Limit * 1000 (int32), Min Current Limit * 1000 (int32)
    VESC_CAN_CONF_STORE_CURRENT_LIMITS_IN = 24U, ///< 设置输入电流限制, Data: Max Current Limit * 1000 (int32), Min Current Limit * 1000 (int32)
} VESC_CAN_PocketConf_t;

typedef enum
{
    VESC_CAN_FILL_RX_BUFFER       = 5U, ///< unknown
    VESC_CAN_FILL_RX_BUFFER_LONG  = 6U, ///< unknown
    VESC_CAN_PROCESS_RX_BUFFER    = 7U, ///< unknown
    VESC_CAN_PROCESS_SHORT_BUFFER = 8U, ///< unknown

    VESC_CAN_SET_CURRENT_HANDBRAKE     = 12U, ///< unknown
    VESC_CAN_SET_CURRENT_HANDBRAKE_REL = 13U, ///< unknown

    /**
     * 猜测是连接状态检测
     */
    VESC_CAN_PING = 17U,
    VESC_CAN_PONG = 18U,

    VESC_CAN_DETECT_APPLY_ALL_FOC     = 19U, ///< unknown
    VESC_CAN_DETECT_APPLY_ALL_FOC_RES = 20U, ///< unknown

    VESC_CAN_CONF_FOC_ERPMS       = 25U, ///< unknown
    VESC_CAN_CONF_STORE_FOC_ERPMS = 26U, ///< unknown
} VESC_CAN_PocketUnknown_t;

typedef enum
{
    /**
     * Status Message 1
     * Data 6 ~ 7: Duty Cycle * 1000 (int16) - latest duty cycle (-1 to 1) multiplied by 1000
     * Data 4 ~ 5: Toal Current * 10 (int16) - Current in all units summed together with a scale factor of 10 - assumed amps
     * Data 0 ~ 3: ERPM (int32) - 32 bits probably because int16 is +/- 32k which may be less than needed for some high speed motors
     */
    VESC_CAN_STATUS = 9U,

    /**
     * Status Message 2
     * Data 4 ~ 7: Amp Hours Charged * 10000 (int32) - total regenerative amp hours put back in battery
     * Data 0 ~ 3: Amp Hours * 10000 (int32) - total amp hours consumed by unit
     */
    VESC_CAN_STATUS_2 = 14U,

    /**
     * Status Message 3
     * Data 4 ~ 7: watt_hours_charged * 10000 (int32) - total regenerative watt-hours put back in battery
     * Data 0 ~ 3: watt_hours * 10000 (int32) - total watt-hours consumed by unit
     */
    VESC_CAN_STATUS_3 = 15U,

    /**
     * Status Message 4
     * Data 6 ~ 7: PID Pos * 50 (int16) - not sure about units
     * Data 4 ~ 5: Toal Current In * 10 (int16) - assumed amps
     * Data 2 ~ 3: Motor Temp * 10 (int16) - assumed °C
     * Data 0 ~ 1: FET Temp * 10 (int16) - assumed °C
     */
    VESC_CAN_STATUS_4 = 16U,

    /**
     * Status Message 5
     * Data 6 ~ 7: Reserved
     * Data 4 ~ 5: Input Voltage * 10
     * Data 0 ~ 3: Tachometer value - assumed rpm(erpm)
     */
    VESC_CAN_STATUS_5 = 27U,
} VESC_CAN_PocketStatus_t;


typedef struct
{
    bool enable;    // 是否启用
    bool auto_zero; // 是否自动判断零点

    CAN_HandleTypeDef* hcan;
    uint8_t id;         ///< 控制器 id，0xFF 代表广播
    uint8_t electrodes; ///< 电极数
    float angle_zero;   ///< 零点角度

    uint32_t feedback_count; ///< 反馈数
    struct
    {
        float erpm;          ///< 电转速
        float pos;           ///< 绝对角度 0~360
        float duty;          ///< 占空比
        float current_motor; ///< 电机电流
        float current_in;    ///< 输入电流

        float amp_hours; ///< AH
        float amp_hours_charged;
        float watt_hours; ///< WH
        float watt_hours_charged;

        float motor_temperature; ///< 电机温度
        float mos_temperature;   ///< MOSFET 温度

        float vin; ///< 输入电压
        float tachometer_value;

        int32_t round_cnt; ///< 圈数统计
    } feedback;

    float velocity;
    float abs_angle;
} VESC_t;

typedef struct
{
    bool auto_zero; ///< 自动重置零点
    CAN_HandleTypeDef* hcan;
    uint8_t id;         ///< 控制器 id，0xFF 代表广播
    uint8_t electrodes; ///< 电极数
} VESC_Config_t;

typedef struct
{
    CAN_HandleTypeDef* hcan;
    VESC_t* motors[VESC_NUM];
} VESC_FeedbackMap;

#define __VESC_GET_ANGLE(__VESC_HANDLE__)    (((VESC_t*)(__VESC_HANDLE__))->abs_angle)
#define __VESC_GET_VELOCITY(__VESC_HANDLE__) (((VESC_t*)(__VESC_HANDLE__))->velocity)

void VESC_Init(VESC_t* hvesc, VESC_Config_t config);
HAL_StatusTypeDef VESC_CAN_FilterInit(CAN_HandleTypeDef* hcan, uint32_t filter_bank);
void VESC_ResetAngle(VESC_t* hvesc);
void VESC_SendSetCmd(VESC_t* hvesc, VESC_CAN_PocketSet_t pocket_id, float value);
void VESC_CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan);
void VESC_CAN_BaseReceiveCallback(CAN_HandleTypeDef* hcan, const CAN_RxHeaderTypeDef* header, const uint8_t data[]);
#endif // VESC_H
