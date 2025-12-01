/**
 * @file    pid_motor.h
 * @author  syhanjin
 * @date    2025-09-04
 * @brief   用于电机控制的 PID 算法库
 *
 * 本库只包含基本的 比例 - 积分 - 微分 控制以及抗饱和算法，不包括其他复杂算法，用于电机控制
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
#ifndef PID_MOTOR_H
#define PID_MOTOR_H

typedef struct
{
    /* Arguments */
    float Kp;             //< 比例系数
    float Ki;             //< 积分系数
    float Kd;             //< 微分系数
    float abs_output_max; //< 输出限幅

    /* Runtime Data */
    float fdb;         //< 反馈量
    float cur_error;   //< 当前误差
    float prev_error1; //< 上一次误差
    float prev_error2; //< 上上次误差
    float output;      //< 输出量

    /**
     * 本人认为 target 表意更合适，但是战队传承的代码使用的是 ref
     * 处于对前人的尊重，此处沿用 ref 表示 PID 控制目标值
     */
    float ref; //< 目标值

} MotorPID_t;

typedef struct
{
    float Kp;             //< 比例系数
    float Ki;             //< 积分系数
    float Kd;             //< 微分系数
    float abs_output_max; //< 输出限幅
} MotorPID_Config_t;

void MotorPID_Init(MotorPID_t* hpid, MotorPID_Config_t pid_config);
void MotorPID_Calculate(MotorPID_t* hpid);


#endif // PID_MOTOR_H
