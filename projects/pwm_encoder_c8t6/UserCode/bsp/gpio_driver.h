/**
 * @file    gpio_driver.h
 * @author  syhanjin
 * @date    2025-09-10
 * @brief   gpio driver
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
#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H
#include "main.h"

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIO_t;

static inline void GPIO_WritePin(GPIO_t* hgpio, const GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(hgpio->port, hgpio->pin, PinState);
}

static inline void GPIO_SetPin(GPIO_t* hgpio) { GPIO_WritePin(hgpio, GPIO_PIN_SET); }

static inline void GPIO_ResetPin(GPIO_t* hgpio) { GPIO_WritePin(hgpio, GPIO_PIN_RESET); }

static inline void GPIO_TogglePin(GPIO_t* hgpio) { HAL_GPIO_TogglePin(hgpio->port, hgpio->pin); }

#endif // GPIO_DRIVER_H
