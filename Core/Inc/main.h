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
#define NTC_Pin GPIO_PIN_4
#define NTC_GPIO_Port GPIOC
#define SPK_SD_Pin GPIO_PIN_5
#define SPK_SD_GPIO_Port GPIOC
#define SPK_SD_EXTI_IRQn EXTI9_5_IRQn
#define SPK_GAIN_Pin GPIO_PIN_0
#define SPK_GAIN_GPIO_Port GPIOB
#define RTC_DS_Pin GPIO_PIN_1
#define RTC_DS_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_7
#define SD_CS_GPIO_Port GPIOE
#define CTP_INT_Pin GPIO_PIN_8
#define CTP_INT_GPIO_Port GPIOE
#define CTP_INT_EXTI_IRQn EXTI9_5_IRQn
#define CTP_RST_Pin GPIO_PIN_9
#define CTP_RST_GPIO_Port GPIOE
#define CTP_RST_EXTI_IRQn EXTI9_5_IRQn
#define LCD_RST_Pin GPIO_PIN_10
#define LCD_RST_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_11
#define LCD_RS_GPIO_Port GPIOE
#define CTP_SCL_Pin GPIO_PIN_10
#define CTP_SCL_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_14
#define KEY3_GPIO_Port GPIOD
#define KEY2_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_6
#define KEY1_GPIO_Port GPIOC
#define KEY1_EXTI_IRQn EXTI9_5_IRQn
#define LCD_LED_Pin GPIO_PIN_7
#define LCD_LED_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
