/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "circular_buffer.h"
circular_buffer_t circ_rx, circ_tx;
uint8_t tx_circ_arr[256];
uint8_t rx_circ_arr[256];
uint8_t tx_burst_arr[32]; // laseme HAL_UART_Transmit_IT korraga saata kuni 32 baiti
uint8_t tx_busy = 0; // kui 1, siis ei tohi HAL_UART_Transmit_IT uuesti kutsuda
uint8_t rx_byte; // HAL_UART_Receive_IT loeb väärtuse siia

// lisa MX_USART1_UART_Init veel:

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	circ_init(&circ_rx, tx_circ_arr, sizeof(tx_circ_arr));
	circ_init(&circ_tx, rx_circ_arr, sizeof(rx_circ_arr));
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = STLINK_RX_Pin|STLINK_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, STLINK_RX_Pin|STLINK_TX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
int USART1_send(uint8_t *data, int count) {
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	circ_push(&circ_tx, data, count);
	uint16_t size = circ_pop(&circ_tx, tx_burst_arr, sizeof(tx_burst_arr));
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	if (!tx_busy) {
		tx_busy = 1;
		HAL_UART_Transmit_IT(&huart1, tx_burst_arr, size);
	}
	return 1;

}
int USART1_receive(uint8_t *data, int count) {
	if (circ_fill_level(&circ_rx) == 0) {
		return 0;
	}
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	circ_pop(&circ_rx, &data, count);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	return 1;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	int localFillLevel = circ_fill_level(&circ_tx);
	if (localFillLevel != 0) {
		uint16_t size = circ_pop(&circ_tx, tx_burst_arr, sizeof(tx_burst_arr));
		HAL_UART_Transmit_IT(&huart1, tx_burst_arr, size);
	} else {
		tx_busy = 0;
	}
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
	circ_push_byte(&circ_rx, rx_byte);
	if(rx_byte == '\n') {
		int localFillLevel = circ_fill_level(&circ_rx);
		uint8_t localbuf[256];
		circ_pop(&circ_rx, &localbuf, localFillLevel+1);
		uint8_t sendBuf[260];
		sprintf(sendBuf, "ECHO:%s", localbuf);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		USART1_send(sendBuf, localFillLevel + 5);
		return;
	}
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}
/* USER CODE END 1 */
