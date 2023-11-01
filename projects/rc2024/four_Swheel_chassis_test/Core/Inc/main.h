/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SWHEEL_3_LS_Pin GPIO_PIN_15
#define SWHEEL_3_LS_GPIO_Port GPIOD
#define SWHEEL_3_LS_EXTI_IRQn EXTI15_10_IRQn
#define SWHEEL_2_LS_Pin GPIO_PIN_14
#define SWHEEL_2_LS_GPIO_Port GPIOD
#define SWHEEL_2_LS_EXTI_IRQn EXTI15_10_IRQn
#define SWHEEL_1_LS_Pin GPIO_PIN_13
#define SWHEEL_1_LS_GPIO_Port GPIOD
#define SWHEEL_1_LS_EXTI_IRQn EXTI15_10_IRQn
#define SWHEEL_0_LS_Pin GPIO_PIN_12
#define SWHEEL_0_LS_GPIO_Port GPIOD
#define SWHEEL_0_LS_EXTI_IRQn EXTI15_10_IRQn
#define LDR_Pin GPIO_PIN_11
#define LDR_GPIO_Port GPIOE
#define LDG_Pin GPIO_PIN_14
#define LDG_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
