/**
 * @file    DJI.c
 * @author  syhanjin
 * @date    2025-09-03
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
#include "DJI.h"
#include <string.h>
#include "bsp/can_driver.h"

static DJI_FeedbackMap map[CAN_NUM];
static size_t map_size = 0;

/**
 * 电机减速比 map
 */
static float reduction_rate_map[DJI_MOTOR_TYPE_COUNT] = {
    [M3508_C620] = (3591.0f / 187.0f),
    [M2006_C610] = (36.0f),
};

static inline DJI_t* getDJIHandle(DJI_t* motors[8], const CAN_RxHeaderTypeDef* header)
{
    if (header->IDE != CAN_ID_STD)
        return NULL;
    const uint8_t id0 = header->StdId - 0x201;
    // 不是 DJI 的反馈数据
    if (id0 >= 8)
        return NULL;
    if (motors[id0] == NULL)
    {
        DJI_ERROR_HANDLER();
        return NULL;
    }
    return motors[id0];
}

void DJI_Init(DJI_t* hdji, const DJI_Config_t dji_config)
{
    memset(hdji, 0, sizeof(DJI_t));

    hdji->enable             = true;
    hdji->reverse            = dji_config.reverse;
    hdji->auto_zero          = dji_config.auto_zero;
    hdji->can                = dji_config.hcan->Instance;
    hdji->id1                = dji_config.id1;
    hdji->inv_reduction_rate = 1.0f /                                                              // 取倒数将除法转为乘法加快运算速度
                               ((dji_config.reduction_rate > 0 ? dji_config.reduction_rate : 1.0f) // 外接减速比
                                * reduction_rate_map[dji_config.motor_type]);                      // 电机内部减速比

    /* 注册回调 */
    DJI_t** mapped_motors = NULL;
    for (int i = 0; i < map_size; i++)
        if (map[i].can == hdji->can)
            mapped_motors = map[i].motors;
    if (mapped_motors == NULL)
    {
        // CAN 未被注册，添加到 map
        map[map_size] = (DJI_FeedbackMap){
            .can    = hdji->can,
            .motors = {NULL} // 为了好看
        };
        mapped_motors = map[map_size].motors;
        map_size++;
    }
    if (mapped_motors[hdji->id1 - 1] != NULL)
    {
        // 电调 ID 冲突
        DJI_ERROR_HANDLER();
    }
    else
    {
        mapped_motors[hdji->id1 - 1] = hdji;
    }
}

/**
 * DJI CAN 反馈数据解包
 * @param hdji DJI handle
 * @param data 反馈数据
 */
void DJI_DataDecode(DJI_t* hdji, const uint8_t data[8])
{
    const float feedback_angle = (float)((uint16_t)data[0] << 8 | data[1]) * 360.0f / 8192.0f;
    const float feedback_rpm   = (int16_t)((uint16_t)data[2] << 8 | data[3]);
    // TODO: 堵转电流检测
    // const float feedback_current = (float)((int16_t)data[4] << 8 | data[5]) / 16384.0f * 20.0f;

    // M3508 和 M2006 的转速均不会超过 120 deg/s
    if (feedback_angle < 90 && hdji->feedback.mech_angle > 270)
        hdji->feedback.round_cnt++;
    if (feedback_angle > 270 && hdji->feedback.mech_angle < 90)
        hdji->feedback.round_cnt--;

    hdji->feedback.mech_angle = feedback_angle;
    hdji->abs_angle           = (hdji->reverse ? -1.0f : 1.0f) * // 反转时需要反转角度输入
                      ((float)hdji->feedback.round_cnt * 360.0f + hdji->feedback.mech_angle - hdji->angle_zero) * hdji->inv_reduction_rate;

    hdji->feedback.rpm = feedback_rpm;
    hdji->velocity     = (hdji->reverse ? -1.0f : 1.0f) * // 反转时需要反转速度输入
                     hdji->feedback.rpm * hdji->inv_reduction_rate;

    hdji->feedback_count++;
    if (hdji->feedback_count == 50 && hdji->auto_zero)
    {
        // 上电后第 50 次反馈执行输出轴清零操作
        DJI_ResetAngle(hdji);
    }
}

/**
 * 清零 DJI 输出角度
 * @param hdji DJI handle
 */
void DJI_ResetAngle(DJI_t* hdji)
{
    hdji->feedback.round_cnt = 0;
    hdji->angle_zero         = hdji->feedback.mech_angle;
    hdji->abs_angle          = 0;
}

/**
 *
 * @param hcan CAN handle
 * @param cmd_group ID 组
 */
void DJI_SendSetIqCommand(CAN_HandleTypeDef* hcan, const DJI_IqSetCmdGroup_t cmd_group)
{
    for (int i = 0; i < map_size; i++)
    {
        if (hcan->Instance == map[i].can)
        {
            uint8_t iq_data[8] = {};
            for (int j = 0; j < 4; j++)
            {
                const DJI_t* hdji = map[i].motors[j + cmd_group];
                if (hdji != NULL)
                {
                    const int32_t iq_cmd = hdji->reverse ? -hdji->iq_cmd : hdji->iq_cmd;
                    iq_data[1 + j * 2]   = (uint8_t)(iq_cmd & 0xFF);      // 电流值低 8 位
                    iq_data[0 + j * 2]   = (uint8_t)(iq_cmd >> 8 & 0xFF); // 电流值高 8 位
                }
            }
            CAN_SendMessage(hcan,
                            &(CAN_TxHeaderTypeDef){
                                .StdId = cmd_group == IQ_CMD_GROUP_1_4 ? 0x200 : 0x1FF,
                                .IDE   = CAN_ID_STD,
                                .RTR   = CAN_RTR_DATA,
                                .DLC   = 8},
                            iq_data);
            return;
        }
    }
}

void DJI_CAN_FilterInit(CAN_HandleTypeDef* hcan, const uint32_t filter_bank)
{
    const CAN_FilterTypeDef sFilterConfig = {
        .FilterIdHigh         = 0x200 << 5,
        .FilterIdLow          = 0x0000,
        .FilterMaskIdHigh     = 0x7F0 << 5, //< 高 7 位匹配，第 4 位忽略
        .FilterMaskIdLow      = 0x0000,
        .FilterFIFOAssignment = CAN_FILTER_FIFO0,
        .FilterBank           = filter_bank,
        .FilterMode           = CAN_FILTERMODE_IDMASK,
        .FilterScale          = CAN_FILTERSCALE_32BIT,
        .FilterActivation     = ENABLE,
        .SlaveStartFilterBank = 14};
    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
    {
        DJI_ERROR_HANDLER();
    }
}

/**
 * CAN FIFO0 接收回调函数
 * @attention 必须*注册*回调函数或者在更高级的回调函数内调用此回调函数
 * @note 使用
 *          HAL_CAN_RegisterCallback(hcan, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, DJI_CAN_Fifo0ReceiveCallback);
 *       来注册回调函数
 * @param hcan
 */
void DJI_CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
    {
        DJI_ERROR_HANDLER();
        return;
    }
    DJI_CAN_BaseReceiveCallback(hcan, &header, data);
}

/**
 * CAN FIFI1 接收回调函数
 * @deprecated Fifo1 should not be used in DJI feedback! Use Fifo0
 * @param hcan
 */
void DJI_CAN_Fifo1ReceiveCallback(CAN_HandleTypeDef* hcan)
{

    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &header, data) != HAL_OK)
    {
        DJI_ERROR_HANDLER();
        return;
    }
    DJI_CAN_BaseReceiveCallback(hcan, &header, data);
}

/**
 * CAN 基本接收回调函数
 * @param hcan
 * @param header
 * @param data
 */
void DJI_CAN_BaseReceiveCallback(const CAN_HandleTypeDef* hcan, const CAN_RxHeaderTypeDef* header, uint8_t data[])
{
    for (int i = 0; i < map_size; i++)
    {
        if (hcan->Instance == map[i].can)
        {
            DJI_t* hdji = getDJIHandle(map[i].motors, header);
            if (hdji != NULL)
                DJI_DataDecode(hdji, data);
            return;
        }
    }
}
