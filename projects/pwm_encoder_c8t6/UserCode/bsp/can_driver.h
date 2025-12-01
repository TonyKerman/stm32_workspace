/**
 * @file    can.h
 * @author  syhanjin
 * @date    2025-09-04
 * @brief   CAN wrapper based on HAL library
 *
 * 本驱动是对 HAL 库的一层简要封装
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
 * Project repository: https://github.com/HITSZ-WTR2026/bsp_drivers
 */
#ifndef CAN_H
#define CAN_H

#include "main.h"

#define CAN_ERROR_HANDLER() Error_Handler()
#define CAN_SEND_FAILED     (0xFFFF)
#define CAN_SEND_TIMEOUT    (10)

#ifdef __cplusplus
extern "C" {
#endif
#define CAN_NUM (2)

typedef void (*CAN_FifoReceiveCallback_t)(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* header, uint8_t data[]);

typedef struct
{
    CAN_HandleTypeDef* hcan;
    CAN_FifoReceiveCallback_t callbacks[28];
} CAN_CallbackMap;

// TODO: 增加更完善的错误返回逻辑


uint32_t CAN_SendMessage(CAN_HandleTypeDef* hcan, const CAN_TxHeaderTypeDef* header, const uint8_t data[]);
void CAN_Start(CAN_HandleTypeDef* hcan, uint32_t ActiveITs);

void CAN_RegisterCallback(CAN_HandleTypeDef* hcan, uint32_t filter_match_index, CAN_FifoReceiveCallback_t callback);
void CAN_UnregisterCallback(CAN_HandleTypeDef* hcan, uint32_t filter_match_index);
void CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan);
void CAN_Fifo1ReceiveCallback(CAN_HandleTypeDef* hcan);

#ifdef __cplusplus
}
#endif
#endif // CAN_H
