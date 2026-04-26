/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ENABLE_Pin|SLEEP_Pin|IR_TX_1_Pin|IR_TX_2_Pin
                          |IR_TX_3_Pin|IR_TX_4_Pin|LED_B_Pin|LED_A_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_F_Pin|LED_E_Pin|LED_D_Pin|LED_C_Pin
                          |STEP_MR_Pin|DIR_MR_Pin|STEP_ML_Pin|DIR_ML_Pin
                          |MS2_Pin|MS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI3_NSS_GPIO_Port, SPI3_NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENABLE_Pin SLEEP_Pin IR_TX_1_Pin IR_TX_2_Pin
                           IR_TX_3_Pin IR_TX_4_Pin LED_B_Pin LED_A_Pin */
  GPIO_InitStruct.Pin = ENABLE_Pin|SLEEP_Pin|IR_TX_1_Pin|IR_TX_2_Pin
                          |IR_TX_3_Pin|IR_TX_4_Pin|LED_B_Pin|LED_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ENCC_ML_Pin */
  GPIO_InitStruct.Pin = ENCC_ML_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ENCC_ML_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BOTON_A_Pin BOTON_B_Pin BOTON_C_Pin */
  GPIO_InitStruct.Pin = BOTON_A_Pin|BOTON_B_Pin|BOTON_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_F_Pin LED_E_Pin LED_D_Pin LED_C_Pin
                           STEP_MR_Pin DIR_MR_Pin STEP_ML_Pin DIR_ML_Pin
                           MS2_Pin MS1_Pin */
  GPIO_InitStruct.Pin = LED_F_Pin|LED_E_Pin|LED_D_Pin|LED_C_Pin
                          |STEP_MR_Pin|DIR_MR_Pin|STEP_ML_Pin|DIR_ML_Pin
                          |MS2_Pin|MS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI3_NSS_Pin */
  GPIO_InitStruct.Pin = SPI3_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI3_NSS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ENCC_MR_Pin */
  GPIO_InitStruct.Pin = ENCC_MR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ENCC_MR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
