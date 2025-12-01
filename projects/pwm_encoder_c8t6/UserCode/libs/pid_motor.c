/**
 * @file    pid_motor.c
 * @author  syhanjin
 * @date    2025-09-04
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
#include "pid_motor.h"
#include <string.h>

void MotorPID_Calculate(MotorPID_t* hpid)
{
    hpid->cur_error = hpid->ref - hpid->fdb;
    hpid->output +=
        hpid->Kp * (hpid->cur_error - hpid->prev_error1) +
        hpid->Ki * hpid->cur_error +
        hpid->Kd * (hpid->cur_error - 2 * hpid->prev_error1 + hpid->prev_error2);
    if (hpid->output > hpid->abs_output_max)
        hpid->output = hpid->abs_output_max;
    if (hpid->output < -hpid->abs_output_max)
        hpid->output = -hpid->abs_output_max;

    hpid->prev_error2 = hpid->prev_error1;
    hpid->prev_error1 = hpid->cur_error;
}

void MotorPID_Init(MotorPID_t* hpid, const MotorPID_Config_t pid_config)
{
    /* reset pid */
    memset(hpid, 0, sizeof(MotorPID_t));

    /* set pid arguments */
    hpid->Kp             = pid_config.Kp;
    hpid->Ki             = pid_config.Ki;
    hpid->Kd             = pid_config.Kd;
    hpid->abs_output_max = pid_config.abs_output_max;
}
