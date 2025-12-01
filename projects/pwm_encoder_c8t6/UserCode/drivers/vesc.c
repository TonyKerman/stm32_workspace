/**
 * @file    vesc.c
 * @author  syhanjin
 * @date    2025-09-15
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
#include "vesc.h"

#include <string.h>
#include "bsp/can_driver.h"
#include "main.h"

static VESC_FeedbackMap map[VESC_CAN_NUM];
static size_t map_size = 0;

static inline int to_map_id(const int id) { return id - VESC_ID_OFFSET; }

static inline VESC_t* get_vesc_handle(VESC_t* motors[VESC_NUM], const CAN_RxHeaderTypeDef* header)
{
    if (header->IDE != CAN_ID_EXT)
        return NULL;
    const uint8_t id = header->ExtId & 0xFF;
    // 不在注册范围内
    if (id < VESC_ID_OFFSET || id >= VESC_ID_OFFSET + VESC_NUM)
        return NULL;
    if (motors[to_map_id(id)] == NULL)
    {
        return NULL;
    }
    return motors[to_map_id(id)];
}

static float clamp_value(const float value, const float max)
{
    if (value > max)
        return max;
    if (value < -max)
        return -max;
    return value;
}

/**
 * 获取 CAN 指令数据 ( - | int32 ) 类
 * @param pocket_id vesc can pocket id (set)
 * @param value 参数值
 * @param data 数据缓冲区
 */
static void get_set_command_data(VESC_t* hvesc, const VESC_CAN_PocketSet_t pocket_id, const float value, uint8_t data[])
{
    int32_t data_value;
    switch (pocket_id)
    {
    case VESC_CAN_SET_DUTY:
        // 设置占空比, Data: Duty Circle * 100,000 (int32)
        data_value = (int32_t)(clamp_value(value, VESC_SET_DUTY_MAX) * 1e5f);
        break;
    case VESC_CAN_SET_CURRENT:
        // 设置电流, Data: current * 1000 (int32)
        data_value = (int32_t)(clamp_value(value, VESC_SET_CURRENT_MAX) * 1e3f);
        break;
    case VESC_CAN_SET_CURRENT_BRAKE:
        // 设置刹车电流, Data: current * 1000 (int32)
        data_value = (int32_t)(clamp_value(value, VESC_SET_CURRENT_BRAKE_MAX) * 1e3f);
        break;
    case VESC_CAN_SET_RPM:
        // 设置转速, Data: ERPM (int32)
        data_value = (int32_t)(clamp_value(value, VESC_SET_RPM_MAX)) * hvesc->electrodes;
        break;
    case VESC_CAN_SET_POS:
        // 设置位置, Data: pos * 1,000,000 (int32) unit: ?
        data_value = (int32_t)(clamp_value(value, VESC_SET_POS_MAX) * 1e6f);
        break;
    case VESC_CAN_SET_CURRENT_REL:
        // 设置相对电流，Data: ratio (-1 to 1) * 100,000 (int32). 相对于最大值和最小值
        data_value = (int32_t)(clamp_value(value, VESC_SET_CURRENT_REL_MAX) * 1e5);
        break;
    case VESC_CAN_SET_CURRENT_BRAKE_REL:
        // 设置相对电流，Data: ratio (-1 to 1) * 100,000 (int32). 相对于最大值和最小值
        data_value = (int32_t)(clamp_value(value, VESC_SET_CURRENT_BRAKE_REL_MAX) * 1e5);
        break;
    default:
        return;
    }
    data[0] = data_value >> 24;
    data[1] = data_value >> 16;
    data[2] = data_value >> 8;
    data[3] = data_value;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x00;
}

static void get_conf_command_data(const VESC_CAN_PocketConf_t pocket_id, const float value0, const float value1, uint8_t data[8])
{
#warning "not implemented!";
    int16_t data_value0, data_value1;
    switch (pocket_id)
    {
    case VESC_CAN_CONF_CURRENT_LIMITS:
        break;
    case VESC_CAN_CONF_STORE_CURRENT_LIMITS:
        break;
    case VESC_CAN_CONF_CURRENT_LIMITS_IN:
        break;
    case VESC_CAN_CONF_STORE_CURRENT_LIMITS_IN:
        break;
    }
}

/**
 * 初始化 VESC CAN 过滤器
 * @param hcan can handle
 * @param filter_bank 过滤器编号
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef VESC_CAN_FilterInit(CAN_HandleTypeDef* hcan, const uint32_t filter_bank)
{
    const CAN_FilterTypeDef sFilterConfig = {.FilterIdHigh         = 0x0000,
                                             .FilterIdLow          = 0x0000 | CAN_ID_EXT, /// 匹配扩展帧
                                             .FilterMaskIdHigh     = 0x0000,
                                             .FilterMaskIdLow      = 0x0000 | CAN_ID_EXT,
                                             .FilterFIFOAssignment = CAN_FILTER_FIFO0,
                                             .FilterBank           = filter_bank,
                                             .FilterMode           = CAN_FILTERMODE_IDMASK,
                                             .FilterScale          = CAN_FILTERSCALE_32BIT,
                                             .FilterActivation     = ENABLE,
                                             .SlaveStartFilterBank = 14};
    return HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
}

static int32_t be_to_i32(const uint8_t* bytes)
{
    return (int32_t)((uint32_t)bytes[0] << 24 | (uint32_t)bytes[1] << 16 | (uint32_t)bytes[2] << 8 | (uint32_t)bytes[3]);
}
static int16_t be_to_i16(const uint8_t* bytes) { return (int16_t)((uint16_t)bytes[0] << 8 | (uint16_t)bytes[1]); }

/**
 * VESC 反馈数据解算
 * @param hvesc vesc handle
 * @param pocket_id 数据包编号
 * @param data 数据
 */
void VESC_CAN_DataDecode(VESC_t* hvesc, const VESC_CAN_PocketStatus_t pocket_id, const uint8_t data[8])
{
    ++hvesc->feedback_count;

    switch (pocket_id)
    {
    case VESC_CAN_STATUS:
        hvesc->feedback.erpm          = (float)be_to_i32(data + 0);
        hvesc->feedback.current_motor = (float)be_to_i16(data + 4) / 10.0f;
        hvesc->feedback.duty          = (float)be_to_i16(data + 6) / 1000.0f;
        hvesc->velocity               = hvesc->feedback.erpm / (float)hvesc->electrodes;
        break;
    case VESC_CAN_STATUS_2:
        hvesc->feedback.amp_hours         = (float)be_to_i32(data + 0) / 10000.0f;
        hvesc->feedback.amp_hours_charged = (float)be_to_i32(data + 4) / 10000.0f;
        break;
    case VESC_CAN_STATUS_3:
        hvesc->feedback.watt_hours         = (float)be_to_i32(data + 0) / 10000.0f;
        hvesc->feedback.watt_hours_charged = (float)be_to_i32(data + 4) / 10000.0f;
        break;
    case VESC_CAN_STATUS_4:
        hvesc->feedback.mos_temperature   = (float)be_to_i16(data + 0) / 10.0f;
        hvesc->feedback.motor_temperature = (float)be_to_i16(data + 2) / 10.0f;
        hvesc->feedback.current_in        = (float)be_to_i16(data + 4) / 10.0f;
        const float new_pos               = (float)be_to_i16(data + 6) / 50.0f;
        // 统计旋转圈数，反馈频率必须 > 转速(rpm) / 30
        if (new_pos < 90 && hvesc->feedback.pos > 270)
            hvesc->feedback.round_cnt++;
        if (new_pos > 270 && hvesc->feedback.pos < 90)
            hvesc->feedback.round_cnt--;
        hvesc->feedback.pos = new_pos;
        hvesc->abs_angle    = (float)hvesc->feedback.round_cnt * 360.0f + hvesc->feedback.pos - hvesc->angle_zero;
        break;
    case VESC_CAN_STATUS_5:
        hvesc->feedback.tachometer_value = (float)be_to_i32(data + 0);
        hvesc->feedback.vin              = (float)be_to_i16(data + 4) / 10.0f;
        break;
    default: // 其他数据乱入
        return;
    }
    if (hvesc->feedback_count == 50 && hvesc->auto_zero) // 第 50 次反馈时清零角度
        VESC_ResetAngle(hvesc);
}

/**
 * 清零 VESC 输出角度
 * @param hvesc vesc handle
 */
void VESC_ResetAngle(VESC_t* hvesc)
{
    hvesc->feedback.round_cnt = 0;
    hvesc->angle_zero         = hvesc->feedback.pos;
    hvesc->abs_angle          = 0;
}

/**
 * 初始化 VESC
 * @param hvesc vesc handle
 * @param config 配置
 */
void VESC_Init(VESC_t* hvesc, const VESC_Config_t config)
{
    memset(hvesc, 0, sizeof(VESC_t));

    hvesc->hcan       = config.hcan;
    hvesc->id         = config.id;
    hvesc->electrodes = config.electrodes;
    hvesc->enable     = true;
    hvesc->auto_zero  = config.auto_zero;

    VESC_t** mapped_motors = NULL;
    for (int i = 0; i < map_size; i++)
        if (map[i].hcan == hvesc->hcan)
            mapped_motors = map[i].motors;
    if (mapped_motors == NULL)
    {
        // CAN 未被注册，添加到 map
        map[map_size] = (VESC_FeedbackMap){.hcan = hvesc->hcan, .motors = {NULL}};
        mapped_motors = map[map_size].motors;
        map_size++;
    }
    if (mapped_motors[to_map_id(hvesc->id)] != NULL)
    {
        // 电调 ID 冲突
        Error_Handler();
    }
    else
    {
        mapped_motors[to_map_id(hvesc->id)] = hvesc;
    }
}

/**
 * 发送指令
 * @param hvesc vesc handle
 * @param pocket_id 数据包类型
 * @param value 指令值
 */
void VESC_SendSetCmd(VESC_t* hvesc, const VESC_CAN_PocketSet_t pocket_id, const float value)
{
    static uint8_t data[8] = {0};
    get_set_command_data(hvesc, pocket_id, value, data);
    CAN_SendMessage(hvesc->hcan,
                    &(CAN_TxHeaderTypeDef){
                        .ExtId = pocket_id << 8 | hvesc->id,
                        .IDE   = CAN_ID_EXT,
                        .RTR   = CAN_RTR_DATA,
                        .DLC   = 4,
                    },
                    data);
}

/**
 * CAN FIFO0 接收回调函数
 * @attention 必须*注册*回调函数或者在更高级的回调函数内调用此回调函数
 * @code 使用
 *          HAL_CAN_RegisterCallback(hcan, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, VESC_CAN_Fifo0ReceiveCallback);
 *       来注册回调函数
 * @param hcan
 */
void VESC_CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
    {
        return;
    }
    VESC_CAN_BaseReceiveCallback(hcan, &header, data);
}

/**
 * CAN 基本接收回调函数
 * @param hcan
 * @param header
 * @param data
 */
void VESC_CAN_BaseReceiveCallback(CAN_HandleTypeDef* hcan, const CAN_RxHeaderTypeDef* header, const uint8_t data[])
{
    for (int i = 0; i < map_size; i++)
    {
        if (hcan == map[i].hcan)
        {
            VESC_t* hvesc = get_vesc_handle(map[i].motors, header);
            if (hvesc != NULL)
                VESC_CAN_DataDecode(hvesc, header->ExtId >> 8, data);
            return;
        }
    }
}
