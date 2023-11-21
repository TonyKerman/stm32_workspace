////
//// Created by tony on 23-10-3.
////
//#include "main.h"
//#include "cmsis_os.h"
//#include "USER_main.h"
//#include <rcl/rcl.h>
//#include <rcl/error_handling.h>
//#include <rclc/rclc.h>
//#include <rclc/executor.h>
//#include <uxr/client/transport.h>
//#include <rmw_microxrcedds_c/config.h>
//#include <rmw_microros/rmw_microros.h>
//#include <std_msgs/msg/float32.h>
//#include "usart.h"
//#include "tim.h"
//
//bool cubemx_transport_open(struct uxrCustomTransport * transport);
//bool cubemx_transport_close(struct uxrCustomTransport * transport);
//size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
//size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);
//void * microros_allocate(size_t size, void * state);
//void microros_deallocate(void * pointer, void * state);
//void * microros_reallocate(void * pointer, size_t size, void * state);
//void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);
//void timer1_callback(rcl_timer_t *timer, int64_t last_call_time);
//void servo_subscribe_callback(const void * msgin);
//std_msgs__msg__Float32 debug_msg;
//rcl_publisher_t debug_publisher;
//void UserStartDefaultTask(void *argument)
//{
//    // micro-ROS configuration
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//    if(rmw_uros_set_custom_transport(
//            true,
//            (void *) NULL,
//            cubemx_transport_open,
//            cubemx_transport_close,
//            cubemx_transport_write,
//            cubemx_transport_read) == RMW_RET_ERROR)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//
//    rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
//    freeRTOS_allocator.allocate = microros_allocate;
//    freeRTOS_allocator.deallocate = microros_deallocate;
//    freeRTOS_allocator.reallocate = microros_reallocate;
//    freeRTOS_allocator.zero_allocate =  microros_zero_allocate;
//
//    if (!rcutils_set_default_allocator(&freeRTOS_allocator))
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//
//    // micro-ROS app
//
//
//    std_msgs__msg__Float32 servo_cmd_msg;
//
//    rclc_support_t support;
//    rcl_allocator_t allocator;
//    rclc_executor_t executor;
//    rcl_node_t node;
//    rcl_timer_t timer1;
//
//    allocator = rcl_get_default_allocator();
//    //create support
//    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if( rclc_support_init(&support, 0, NULL, &allocator)
//        ==RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
//
//    // create node
//    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if( rclc_node_init_default(&node,"stm32_node","",&support)
//        == RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if (rclc_publisher_init_default(
//            &debug_publisher,
//            &node,
//            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
//            "debug")
//        == RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
//
//    //create timer
//    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if( rclc_timer_init_default(
//            &timer1,
//            &support,
//            RCL_MS_TO_NS(100),
//            timer1_callback)
//        == RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
//
//    //create executor
//    executor = rclc_executor_get_zero_initialized_executor();
//    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if( rclc_executor_init(&executor, &support.context, 1, &allocator)
//        == RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
//    //add subscriber to executor
//    rclc_executor_add_timer(&executor, &timer1);
//
//    osDelay(300);
//    rclc_executor_spin(&executor);
//    for(;;)
//    {}
//}
//void timer1_callback(rcl_timer_t *timer, int64_t last_call_time)
//{
//    HAL_GPIO_WritePin(LDG_GPIO_Port, LDG_Pin, GPIO_PIN_RESET);
//    debug_msg.data = (int32_t)xTaskGetTickCount();
//    rcl_publish(&debug_publisher, &debug_msg, NULL);
//    /*if(xSemaphoreTake(data_mutex, 70/portTICK_RATE_MS)==pdTRUE)
//    {
//        rcl_publish(&msgj_feedback_publisher, &pos_feedback, NULL);
//        xSemaphoreGive(data_mutex);
//    }*/
//    HAL_GPIO_WritePin(LDG_GPIO_Port, LDG_Pin, GPIO_PIN_SET);
//}
//
////void limit_int16(int16_t * value, const int16_t min,const int16_t max)
////{
////    if(*value<min)
////        *value=min;
////    if(*value>max)
////        *value=max;
////}
////void limit_float(float * value, const float min,const float max)
////{
////    if(*value<min)
////        *value=min;
////    if(*value>max)
////        *value=max;
////}
////void servo_subscribe_callback(const void * msgin)
////{
////    const std_msgs__msg__Float32 * msg = (const std_msgs__msg__Float32 *)msgin;
////    float angle=msg->data*180/3.1415926;
////    limit_float(&angle, 25, 180);
////    int16_t ccr1 = (int16_t)(-5.0/9.0*angle+125);
////    int16_t ccr2 = (int16_t)(5.0/9.0*angle+25);
////    limit_int16(&ccr1, 75, 125);
////    limit_int16(&ccr2, 25, 75);
////    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr1);
////    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ccr2);
////    //msg->data.data[0]
////}
