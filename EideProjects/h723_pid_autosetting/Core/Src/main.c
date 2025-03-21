/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fdcan.h"
#include "memorymap.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h"
#include "vofa.h"
#include "tim_delay_us.h"
#include "wtr_can.h"
#include "DJI.h"
#include "pid_param_setting.h"
#include "pid.h"
                                                                                                        

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
double lambda =2;

/* USER CODE END 0 */
//328,16 387,13
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    
    PID_AutoSetting_Buffers_t pas;
    PID_TypeDef speedpid;
    PID_TypeDef pospid;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI6_Init();
  MX_UART7_Init();
  MX_FDCAN1_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
    WS2812_Ctrl(10,0,0);
    HAL_Delay(800);
    WS2812_Ctrl(0,10,0);
    FDCAN1_RX_Filter_Init();
    FDCAN1_Interrupt_Init();
    FDCAN1_Start();
    hDJI[0].motorType = M2006;
    DJI_Init();    
    TIM_Delay_init(&htim12);

    PID_Init(&speedpid,336,14,0);//速度环pid参数已知
    PID_Set_output_limit(&speedpid,-10000,10000);
    PID_Set_integral_limit(&speedpid,500);
    PID_Init_NoParams(&pospid);
    PID_Set_output_limit(&pospid,-550,550);
    PID_Set_integral_limit(&pospid,100);
    PID_Set_deadband(&pospid,1);
    PID_AutoSetting_Init(&pas,TYPE_INTEGRAL,5,0,200,1,0.01,500);
    HAL_Delay(800);
    speedpid.setpoint = 500;
    while(PID_AutoSetting_Update(&pas,hDJI[0].AxisData.AxisAngle_inDegree) == PID_AUTOSETTING_PROCESS)
    {
        // TIM_Delay_Us_Start();
        PID_update(&speedpid,hDJI[0].AxisData.AxisVelocity);
        CanTransmit_DJI_1234(speedpid.output,1000,1000,1000);
        TIM_Delay_Us(1000);
        // TIM_Delay_Us_Until(1000);
    }
    for(int i = 0;i<2000;i++)
    {
        CanTransmit_DJI_1234(0,0,0,0);
        if(i<pas.pv_num){
        vofa_send_data(0, pas.test_output);
        vofa_send_data(1, pas.pv[i]);
        vofa_sendframetail();            
        }
        else
        {
            vofa_send_data(0, 0);
            vofa_send_data(1, 0);
            vofa_sendframetail(); 
        }
        TIM_Delay_Us(1000);
    }
    // PID_AutoSetting_Set(&pas,&pospid);
    PID_Reset(&speedpid,200,10,0);
    PID_AutoSetting_Set_Param(&pas,&pospid.kp,&pospid.ki,&pospid.kd);
    /**
     * kp=2.06,ki=0.0105,t=260ms
     */
    
    pospid.setpoint = 180*2+hDJI[0].AxisData.AxisAngle_inDegree;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        TIM_Delay_Us_Start();
        PID_update(&pospid,hDJI[0].AxisData.AxisAngle_inDegree);
        speedpid.setpoint = pospid.output;
        PID_update(&speedpid,hDJI[0].AxisData.AxisVelocity);
        CanTransmit_DJI_1234(speedpid.output,0,0,0);
        vofa_send_data(0, pospid.setpoint);
        vofa_send_data(1, hDJI[0].AxisData.AxisAngle_inDegree);
        vofa_send_data(2, pospid.output);
        vofa_send_data(3, pospid.integral);
        vofa_send_data(4, pospid.kp);
        vofa_send_data(5, pospid.ki);
        vofa_sendframetail();
        TIM_Delay_Us_Until(1000);
     

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 45;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
