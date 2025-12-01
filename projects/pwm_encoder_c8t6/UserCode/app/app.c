/**
 * @file    app.c
 * @author  syhanjin
 * @date    2025-09-03
 * @brief   Brief description of the file
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
#include "app.h"
#include "cmsis_os2.h"


/**
 * @brief Function implementing the initTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Init */
void Init(void* argument)
{
    /* 执行初始化 */

    // 运行实例将不在此处，请见 app/*_example.c

    /* 初始化完成后退出线程 */
    osThreadExit();
}
