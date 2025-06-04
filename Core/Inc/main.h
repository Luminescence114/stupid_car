/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define DATA_SIZE 64
#define FORWORDSPEED 400
#define COMPEN 22
typedef enum{
    control = 1,
    trace = 2,
    avoid = 3
}car_mode;
extern car_mode Car_Mode;
extern int speed;
extern int ccr_l;
extern int ccr_r;
extern int count;
extern char ReceiveData[DATA_SIZE];
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
#define IRIN_Pin GPIO_PIN_10
#define IRIN_GPIO_Port GPIOD
#define IRIN_EXTI_IRQn EXTI15_10_IRQn
#define TRACE_R_Pin GPIO_PIN_4
#define TRACE_R_GPIO_Port GPIOG
#define TRACE_L_Pin GPIO_PIN_6
#define TRACE_L_GPIO_Port GPIOG
#define TRACE_M_Pin GPIO_PIN_8
#define TRACE_M_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
