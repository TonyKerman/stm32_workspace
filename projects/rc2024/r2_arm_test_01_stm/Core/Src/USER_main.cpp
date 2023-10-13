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
#include <sensor_msgs/msg/joint_state.h>
#include <std_msgs/msg/string.h>
#include "usart.h"
#include "tim.h"
#include "FeeTech.hpp"
#include "Servo.hpp"
#include "Unitree_user.h"

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

//void timer1_callback(rcl_timer_t *timer, int64_t last_call_time);

void servo_subscribe_callback(const void *msgin);

void joint_msg_init(sensor_msgs__msg__JointState *msg);

rcl_subscription_t servo_cmd_subscriber;
rcl_publisher_t debug_publisher;
//rcl_timer_t timer1;
//std_msgs__msg__Float32MultiArray msgJointState;
sensor_msgs__msg__JointState _msgj;
sensor_msgs__msg__JointState msgj;
std_msgs__msg__String debugmsg;
rclc_support_t support;
rcl_allocator_t allocator;
rclc_executor_t executor;
rcl_node_t node;




void servo_subscribe_callback(const void *msgin) {
    const sensor_msgs__msg__JointState *_msgj = (const sensor_msgs__msg__JointState *) msgin;

//    sprintf(debugmsg.data.data,"%f",msgj->position.data[2]);
//    rcl_publish(&debug_publisher, &debugmsg, NULL);
//    HAL_GPIO_TogglePin(LDG_GPIO_Port, LDG_Pin);
//
//    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, servo1.run(msgj->position.data[2]));
//    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, servo2.run(msgj->position.data[2]));
//    feet_servo.Servo_Write_PosEx(genetic_feet.run(msgj->position.data[1]),254,254);
    //msg->data.data[0]
    if (xSemaphoreTake(data_mutex, 10 / portTICK_RATE_MS) == pdTRUE) {
        sensor_msgs__msg__JointState__copy(_msgj, &msgj);
        xSemaphoreGive(data_mutex);
    } else {
        return;
    }
    debugmsg.data.capacity = 20;
    debugmsg.data.size = 20;
    debugmsg.data.data = (char *) pvPortMalloc(20 * sizeof(char));
    sprintf(debugmsg.data.data, "%f", msgj.position.data[2]);
    rcl_publish(&debug_publisher, &debugmsg, NULL);
    vPortFree(debugmsg.data.data);

}

void StartControllerTask(void *argument) {
    Genetic_Servo servo1 = Genetic_Servo(75, 115);
    Genetic_Servo servo2 = Genetic_Servo(40, 75);
    Genetic_Servo genetic_feet = Genetic_Servo(1700, 4000);
    FEET_Servo feet_servo;
    UnitreeMotor  * unitree_motor1 =Unitree_Create_Motor();

    while(Unitree_init(unitree_motor1,&huart6)==HAL_ERROR)
    {
        osDelay(50);
        HAL_GPIO_TogglePin(LDR_GPIO_Port, LDR_Pin);
    }
    feet_servo.FEET_Servo_Init(huart7);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    servo1.set_trans_in2out(0, 1.57, 0, 125, 75, 125);
    servo2.set_trans_in2out(1.57, 0, 1.57, 75, 25, 75);
    genetic_feet.set_trans_in2out(3.14, -3.14, 0, 0, 4096, 2815);
    xSemaphoreTake(sync_mutex, portMAX_DELAY);
    for (;;) {
        if (xSemaphoreTake(data_mutex, 100 / portTICK_RATE_MS) != pdTRUE)
            continue;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, servo1.run(msgj.position.data[2]));
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, servo2.run(msgj.position.data[2]));
        feet_servo.Servo_Write_PosEx(genetic_feet.run(msgj.position.data[1]), 254, 254);
        Unitree_UART_tranANDrev(unitree_motor1, 0, 1, 0, 0, 0, 0, 0);

        xSemaphoreGive(data_mutex);
        HAL_GPIO_TogglePin(LDG_GPIO_Port, LDG_Pin);
        osDelay(100);
    }
}

//void timer1_callback(rcl_timer_t *timer, int64_t last_call_time)
//{
//    debugmsg.data.capacity = 20;
//    debugmsg.data.size = 20;
//    debugmsg.data.data = (char *)pvPortMalloc(20*sizeof(char));
//    sprintf(debugmsg.data.data,"timer%lu",xTaskGetTickCount());
//    rcl_publish(&debug_publisher, &debugmsg, NULL);
//    vPortFree(debugmsg.data.data);
//}

void UserStartDefaultTask(void *argument) {
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    if (rmw_uros_set_custom_transport(
            true,
            (void *) &huart8,
            cubemx_transport_open,
            cubemx_transport_close,
            cubemx_transport_write,
            cubemx_transport_read) == RMW_RET_ERROR)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);

    rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
    freeRTOS_allocator.allocate = microros_allocate;
    freeRTOS_allocator.deallocate = microros_deallocate;
    freeRTOS_allocator.reallocate = microros_reallocate;
    freeRTOS_allocator.zero_allocate = microros_zero_allocate;

    if (!rcutils_set_default_allocator(&freeRTOS_allocator))
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    // micro-ROS app
    allocator = rcl_get_default_allocator();
    //create support
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if (rclc_support_init(&support, 0, NULL, &allocator)
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);

    // create node
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if (rclc_node_init_default(&node, "stm32_node", "", &support)
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    std_msgs__msg__String__init(&debugmsg);

    // create publisher
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if (rclc_publisher_init_default(
            &debug_publisher,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
            "debug")
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    // create subscriber
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if (rclc_subscription_init_default(
            &servo_cmd_subscriber,
            &node,
            ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
            "joint_cmd")
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
//    if( rclc_timer_init_default(
//            &timer1,
//            &support,
//            RCL_MS_TO_NS(400),
//            timer1_callback)
//        == RCL_RET_OK)
//        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    //create executor
    executor = rclc_executor_get_zero_initialized_executor();
    HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_RESET);
    if (rclc_executor_init(&executor, &support.context, 1, &allocator)
        == RCL_RET_OK)
        HAL_GPIO_WritePin(LDR_GPIO_Port, LDR_Pin, GPIO_PIN_SET);
    //add subscriber to executor

    rclc_executor_add_subscription(
            &executor,
            &servo_cmd_subscriber,
            &_msgj,
            &servo_subscribe_callback,
            ON_NEW_DATA);
    //rclc_executor_add_timer(&executor, &timer1);
    osDelay(300);
    debugmsg.data.capacity = 5;
    debugmsg.data.size = 5;
    debugmsg.data.data = "start";
    rcl_publish(&debug_publisher, &debugmsg, NULL);
    joint_msg_init(&_msgj);
    joint_msg_init(&msgj);
    //sensor_msgs__msg__JointState__init(&msgj);
    debugmsg.data.capacity = 10;
    debugmsg.data.size = 0;
    debugmsg.data.data = (char *) pvPortMalloc(10 * sizeof(char));
    xSemaphoreGive(sync_mutex);
    rclc_executor_spin(&executor);
    for (;;) {}
}


void joint_msg_init(sensor_msgs__msg__JointState *msg) {
    msg->header.frame_id.data = (char *) pvPortMalloc(3 * sizeof(char));
    msg->header.frame_id.capacity = 3;
    msg->name.data->data = (char *) pvPortMalloc(3 * sizeof(char));
    msg->name.data->capacity = 3;
    msg->position.data = (double *) pvPortMalloc(3 * sizeof(double));
    msg->position.capacity = 3;
    msg->velocity.data = (double *) pvPortMalloc(3 * sizeof(double));
    msg->velocity.capacity = 3;
    msg->effort.data = (double *) pvPortMalloc(3 * sizeof(double));
    msg->effort.capacity = 3;
}

//void TestTask(void *argument) {
//    osDelay(300);
//    feet_servo.FEET_Servo_Init(huart7);
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//    servo1.set_trans_in2out(0, 1.57, 0, 75, 125, 75);
//    genetic_feet.set_trans_in2out(-3.14, 3.14, 0, 0, 4096, 2815);
//    osDelay(10);
//    float p = 0;
//    for (;;) {
//        HAL_GPIO_TogglePin(LDG_GPIO_Port, LDG_Pin);
//        // __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 30);
//        servo1.run(1.57);
//        osDelay(300);
//        genetic_feet.run(p);
//        printf("p:%d\n", genetic_feet.pos_out);
//        //HAL_UART_Transmit(&huart7, (uint8_t *) "hello world", 11, 1000);
//        feet_servo.Servo_Write_PosEx(genetic_feet.pos_out, 254, 254);
//        p += 0.1;
//        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 75);
//        osDelay(300);
//        //__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 75);
//
//
//    }
//}
