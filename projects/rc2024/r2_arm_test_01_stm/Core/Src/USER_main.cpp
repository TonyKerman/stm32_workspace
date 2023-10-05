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
#include <std_msgs/msg/float32.h>
#include "usart.h"
#include "tim.h"
#include "retarget.h"
#include "FeeTech.hpp"
#include "Servo.hpp"

#ifdef __cplusplus
extern "C" {
#endif
bool cubemx_transport_open(struct uxrCustomTransport *transport);

bool cubemx_transport_close(struct uxrCustomTransport *transport);

size_t cubemx_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *err);

size_t cubemx_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *err);

void *microros_allocate(size_t size, void *state);

void microros_deallocate(void *pointer, void *state);

void *microros_reallocate(void *pointer, size_t size, void *state);

void *microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void *state);
#ifdef __cplusplus
}
#endif
void micro_ros_node_start(void);
void servo_subscribe_callback(const void * msgin);

Genetic_Servo servo1 = Genetic_Servo(75, 115);
Genetic_Servo servo2 = Genetic_Servo(40, 75);
Genetic_Servo genetic_feet = Genetic_Servo(1700, 4000);
FEET_Servo feet_servo = FEET_Servo(huart7);
void UserStartDefaultTask(void *argument)
{

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    servo1.set_trans_in2out(0,1.57,0,125,75,125);
    servo2.set_trans_in2out(1.57,0,1.57,75,25,75);
    genetic_feet.set_trans_in2out(-3.14,3.14,-3.14,0,4096,2815);
    // micro-ROS configuration
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    micro_ros_node_start();
    for(;;)
    {}
}


void micro_ros_node_start()
{

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
    std_msgs__msg__Float32 servo_cmd_msg;
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
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
            "servos_open_cmd")
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
            &servo_cmd_msg,
            &servo_subscribe_callback,
            ON_NEW_DATA);
    osDelay(300);
    rclc_executor_spin(&executor);
}

//void limit_int16(int16_t * value, const int16_t min,const int16_t max)
//{
//    if(*value<min)
//        *value=min;
//    if(*value>max)
//        *value=max;
//}
//void limit_float(float * value, const float min,const float max)
//{
//    if(*value<min)
//        *value=min;
//    if(*value>max)
//        *value=max;
//}
void servo_subscribe_callback(const void * msgin)
{
    const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, servo1.run(msg->data));
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, servo2.run(msg->data));
    //msg->data.data[0]
}

void TestTask(void *argument)
{
    osDelay(300);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    servo1.set_trans_in2out(0,1.57,0,75,125,75);

    osDelay(10);
    int p = 0;
    for(;;)
    {
        HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
        // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 30);
        servo1.run(1.57);
        osDelay(300);
        //genetic_feet.run(p);
        feet_servo.Servo_Write_PosEx(genetic_feet.pos_out,254,254);
        p+=0.1;
        //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 75);
        osDelay(300);
        //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 75);


    }
}
