//
// Created by tony on 23-10-3.
//
#include "main.h"
#include "cmsis_os.h"
#include "USER_main.h"
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32_multi_array.h>
#include "usart.h"

bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);
void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

void servo_subscribe_callback(const void * msgin);


void UserStartDefaultTask(void *argument)
{
    // micro-ROS configuration
    if(rmw_uros_set_custom_transport(
            true,
            (void *) &huart6,
            cubemx_transport_open,
            cubemx_transport_close,
            cubemx_transport_write,
            cubemx_transport_read) == RMW_RET_ERROR)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);

    rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
    freeRTOS_allocator.allocate = microros_allocate;
    freeRTOS_allocator.deallocate = microros_deallocate;
    freeRTOS_allocator.reallocate = microros_reallocate;
    freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

    if (!rcutils_set_default_allocator(&freeRTOS_allocator))
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);

    // micro-ROS app

    rcl_subscription_t servo_cmd_subscriber;
    std_msgs__msg__Int32MultiArray msg;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rclc_executor_t executor;
    rcl_node_t node;

    allocator = rcl_get_default_allocator();
    //create support
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if( rclc_support_init(&support, 0, NULL, &allocator)
        ==RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);

    // create node
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if( rclc_node_init_default(&node,"stm32_node","",&support)
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    // create subscriber
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if( rclc_subscription_init_default(
            &servo_cmd_subscriber,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32MultiArray),
            "servo_cmd_subscriber")
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    //create executor
    executor = rclc_executor_get_zero_initialized_executor();
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if( rclc_executor_init(&executor, &support.context, 1, &allocator)
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    //add subscriber to executor
    rclc_executor_add_subscription(
            &executor,
            &servo_cmd_subscriber,
            &msg,
            &servo_subscribe_callback,
            ON_NEW_DATA);
    osDelay(300);
    rclc_executor_spin(&executor);
    for(;;)
    {}
}

void servo_subscribe_callback(const void * msgin)
{
    const std_msgs__msg__Int32MultiArray * msg = (const std_msgs__msg__Int32MultiArray *)msgin;
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    osDelay()
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
}
