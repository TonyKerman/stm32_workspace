/**
 * @file    can_driver.c
 * @author  syhanjin
 * @date    2025-09-04
 * @brief
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
#include "can_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_RTOS
#include "cmsis_os2.h"
static osMutexId_t can_mutex = NULL;
static osMutexId_t get_can_mutex()
{
    if (can_mutex == NULL)
        can_mutex = osMutexNew(&(osMutexAttr_t){.name = "can_mutex"});
    return can_mutex;
}
#else
#include "cmsis_compiler.h"
#endif


static CAN_CallbackMap maps[CAN_NUM];
static size_t map_size = 0;

static CAN_FifoReceiveCallback_t* get_callbacks(const CAN_HandleTypeDef* hcan)
{
    for (size_t i = 0; i < map_size; i++)
        if (maps[i].hcan == hcan)
            return maps[i].callbacks;

    return NULL;
}

/**
 * 发送一条 CAN 消息
 * @param hcan can handle
 * @param header CAN_TxHeaderTypeDef
 * @param data 数据
 * @note 本身想做成内联展开，但是必须写到 .h 文件，调研发现性能损失不大，所以直接放到此处
 * @attention 本函数大部分情况是线程安全的，少数情况（中断被中断打断）会出现不安全的情况。
 * @return mailbox, 0xFFFF 表示发送失败
 */
uint32_t CAN_SendMessage(CAN_HandleTypeDef* hcan, const CAN_TxHeaderTypeDef* header, const uint8_t data[])
{
    uint32_t mailbox = CAN_SEND_FAILED;

    // 等待上一个发送完成
    // while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
    //     ;
    if (__get_IPSR() != 0)
    {
        // 在中断中直接调用
        if (HAL_CAN_AddTxMessage(hcan, header, data, &mailbox) != HAL_OK)
        {
            CAN_ERROR_HANDLER();
        }
    }
    else
#ifdef USE_RTOS
    { // 任务中调用需要加临界保护
        if (osMutexAcquire(get_can_mutex(), CAN_SEND_TIMEOUT) != osOK)
            // 超时
            return CAN_SEND_FAILED;
        if (HAL_CAN_AddTxMessage(hcan, header, data, &mailbox) != HAL_OK)
        {
            CAN_ERROR_HANDLER();
        }
        osMutexRelease(get_can_mutex());
    }
#else
    {
        // 裸机状态下非中断调用需要保护
        __disable_irq();
        if (HAL_CAN_AddTxMessage(hcan, header, data, &mailbox) != HAL_OK)
        {
            CAN_ERROR_HANDLER();
        }
        __enable_irq();
    }
#endif

    return mailbox;
}

/**
 * CAN 初始化
 * @param hcan can handle
 * @param ActiveITs CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO1_MSG_PENDING
 */
void CAN_Start(CAN_HandleTypeDef* hcan, const uint32_t ActiveITs)
{
    if (HAL_CAN_Start(hcan) != HAL_OK)
    {
        CAN_ERROR_HANDLER();
    }

    if (HAL_CAN_ActivateNotification(hcan, ActiveITs) != HAL_OK)
    {
        CAN_ERROR_HANDLER();
    }
}

/**
 * 注册 CAN Fifo 处理回调
 *
 * @attention 本函数非线程安全，调用时请注意
 * @note 重复注册将会覆盖之前的回调
 * @param hcan hcan
 * @param filter_match_index 注册对应的 filter 编号
 * @param callback 回调函数指针
 */
void CAN_RegisterCallback(CAN_HandleTypeDef* hcan, const uint32_t filter_match_index, CAN_FifoReceiveCallback_t callback)
{
    CAN_FifoReceiveCallback_t* callbacks = get_callbacks(hcan);

    if (callbacks == NULL)
    {
        if (map_size >= CAN_NUM)
        {
            CAN_ERROR_HANDLER();
            return;
        }
        maps[map_size] = (CAN_CallbackMap){
            .hcan      = hcan,
            .callbacks = {NULL}};
        callbacks = maps[map_size].callbacks;
        map_size++;
    }

    callbacks[filter_match_index] = callback;
}

/**
 * 取消注册 CAN Fifo 处理回调
 *
 * @attention 本函数非线程安全，调用时请注意
 * @param hcan can handle
 * @param filter_match_index 需要取消注册对应的过滤器对应的 id
 */
void CAN_UnregisterCallback(CAN_HandleTypeDef* hcan, const uint32_t filter_match_index)
{
    CAN_FifoReceiveCallback_t* callbacks = get_callbacks(hcan);
    if (callbacks != NULL)
        callbacks[filter_match_index] = NULL;
}

/**
 * CAN Fifo0 接收处理函数
 *
 * 本函数将会根据 hcan 和 rx_header 内部的 filter_id 来调用对应的回调函数
 * @param hcan can handle
 */
void CAN_Fifo0ReceiveCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
    {
        CAN_ERROR_HANDLER();
        return;
    }
    const CAN_FifoReceiveCallback_t* callbacks = get_callbacks(hcan);
    if (callbacks != NULL && callbacks[header.FilterMatchIndex] != NULL)
        callbacks[header.FilterMatchIndex](hcan, &header, data);
}

/**
 * CAN Fifo1 接收处理函数
 *
 * 本函数将会根据 hcan 和 rx_header 内部的 filter_id 来调用对应的回调函数
 * @param hcan can handle
 */
void CAN_Fifo1ReceiveCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &header, data) != HAL_OK)
    {
        CAN_ERROR_HANDLER();
        return;
    }
    const CAN_FifoReceiveCallback_t* callbacks = get_callbacks(hcan);
    if (callbacks != NULL && callbacks[header.FilterMatchIndex] != NULL)
        callbacks[header.FilterMatchIndex](hcan, &header, data);
}

#ifdef __cplusplus
}
#endif
