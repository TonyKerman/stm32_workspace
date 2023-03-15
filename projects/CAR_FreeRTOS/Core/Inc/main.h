/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#define IN2_Pin GPIO_PIN_9
#define IN2_GPIO_Port GPIOI
#define IN1_Pin GPIO_PIN_10
#define IN1_GPIO_Port GPIOF
#define HALL_RB_Pin GPIO_PIN_15
#define HALL_RB_GPIO_Port GPIOD
#define IN4_Pin GPIO_PIN_1
#define IN4_GPIO_Port GPIOC
#define HALL_RA_Pin GPIO_PIN_14
#define HALL_RA_GPIO_Port GPIOD
#define HALL_RA_EXTI_IRQn EXTI15_10_IRQn
#define HALL_LB_Pin GPIO_PIN_13
#define HALL_LB_GPIO_Port GPIOD
#define ENB_Pin GPIO_PIN_1
#define ENB_GPIO_Port GPIOA
#define ENA_Pin GPIO_PIN_0
#define ENA_GPIO_Port GPIOA
#define VCC2_Pin GPIO_PIN_4
#define VCC2_GPIO_Port GPIOA
#define HALL_LA_Pin GPIO_PIN_12
#define HALL_LA_GPIO_Port GPIOD
#define HALL_LA_EXTI_IRQn EXTI15_10_IRQn
#define VCC1_Pin GPIO_PIN_5
#define VCC1_GPIO_Port GPIOA
#define IN3_Pin GPIO_PIN_5
#define IN3_GPIO_Port GPIOC
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
