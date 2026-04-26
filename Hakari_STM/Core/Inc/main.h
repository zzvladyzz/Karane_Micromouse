/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define ENABLE_Pin GPIO_PIN_13
#define ENABLE_GPIO_Port GPIOC
#define SLEEP_Pin GPIO_PIN_14
#define SLEEP_GPIO_Port GPIOC
#define IR_TX_1_Pin GPIO_PIN_0
#define IR_TX_1_GPIO_Port GPIOC
#define IR_TX_2_Pin GPIO_PIN_1
#define IR_TX_2_GPIO_Port GPIOC
#define IR_TX_3_Pin GPIO_PIN_2
#define IR_TX_3_GPIO_Port GPIOC
#define IR_TX_4_Pin GPIO_PIN_3
#define IR_TX_4_GPIO_Port GPIOC
#define ENCA_ML_Pin GPIO_PIN_0
#define ENCA_ML_GPIO_Port GPIOA
#define ENCB_ML_Pin GPIO_PIN_1
#define ENCB_ML_GPIO_Port GPIOA
#define ENCC_ML_Pin GPIO_PIN_2
#define ENCC_ML_GPIO_Port GPIOA
#define IR_38KHZ_Pin GPIO_PIN_3
#define IR_38KHZ_GPIO_Port GPIOA
#define IR_RX_1_Pin GPIO_PIN_4
#define IR_RX_1_GPIO_Port GPIOA
#define IR_RX_2_Pin GPIO_PIN_5
#define IR_RX_2_GPIO_Port GPIOA
#define IR_RX_3_Pin GPIO_PIN_6
#define IR_RX_3_GPIO_Port GPIOA
#define IR_RX_4_Pin GPIO_PIN_7
#define IR_RX_4_GPIO_Port GPIOA
#define VOLTAJE_Pin GPIO_PIN_0
#define VOLTAJE_GPIO_Port GPIOB
#define BOTON_A_Pin GPIO_PIN_1
#define BOTON_A_GPIO_Port GPIOB
#define BOTON_B_Pin GPIO_PIN_2
#define BOTON_B_GPIO_Port GPIOB
#define BOTON_C_Pin GPIO_PIN_10
#define BOTON_C_GPIO_Port GPIOB
#define LED_F_Pin GPIO_PIN_12
#define LED_F_GPIO_Port GPIOB
#define LED_E_Pin GPIO_PIN_13
#define LED_E_GPIO_Port GPIOB
#define LED_D_Pin GPIO_PIN_14
#define LED_D_GPIO_Port GPIOB
#define LED_C_Pin GPIO_PIN_15
#define LED_C_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_6
#define LED_B_GPIO_Port GPIOC
#define LED_A_Pin GPIO_PIN_7
#define LED_A_GPIO_Port GPIOC
#define SPI3_NSS_Pin GPIO_PIN_12
#define SPI3_NSS_GPIO_Port GPIOA
#define ENCA_MR_Pin GPIO_PIN_15
#define ENCA_MR_GPIO_Port GPIOA
#define ENCC_MR_Pin GPIO_PIN_2
#define ENCC_MR_GPIO_Port GPIOD
#define ENCB_MR_Pin GPIO_PIN_3
#define ENCB_MR_GPIO_Port GPIOB
#define STEP_MR_Pin GPIO_PIN_4
#define STEP_MR_GPIO_Port GPIOB
#define DIR_MR_Pin GPIO_PIN_5
#define DIR_MR_GPIO_Port GPIOB
#define STEP_ML_Pin GPIO_PIN_6
#define STEP_ML_GPIO_Port GPIOB
#define DIR_ML_Pin GPIO_PIN_7
#define DIR_ML_GPIO_Port GPIOB
#define MS2_Pin GPIO_PIN_8
#define MS2_GPIO_Port GPIOB
#define MS1_Pin GPIO_PIN_9
#define MS1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
