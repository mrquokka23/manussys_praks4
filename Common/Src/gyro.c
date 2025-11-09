/*
 * gyro.c
 *
 *  Created on: Nov 9, 2025
 *      Author: armin
 */
#include "gyro.h"
#include "main.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "log.h"
#include "queue.h"

static Gyro_xyz_data dma_xyz;
static QueueHandle_t queue = NULL;
uint8_t pTx[7] = { READ_SLAVE_CONTINUOUS | 0x28, 0, 0, 0, 0, 0, 0 };
uint8_t pRx[7] = { 0, 0, 0, 0, 0, 0, 0 };

void Gyro_Init() {
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	Gyro_Write(0x20, 0b11111111);
	Gyro_Write(0x22, 0b00001000);
	Gyro_Write(0x23, 0b01000000);

	queue = xQueueCreate(1, sizeof(Gyro_xyz_data));
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	Gyro_xyz_data xyz;
	Gyro_ReadXYZ(&xyz);
}

uint8_t Gyro_CanReadWHOAMI() {
	uint8_t pTxData[2] = { READ_SLAVE_OPERATION | REG_WHOAMI, 0 };
	uint8_t pRxData[2] = { 0, 0 };
	CS_START(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	HAL_SPI_TransmitReceive(&hspi5, pTxData, pRxData, 2, HAL_MAX_DELAY);
	CS_END(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	return pRxData[1] == REG_WHOAMI_DEFAULT_VALUE;
}

void Gyro_Write(uint8_t address, uint8_t data) {
	uint8_t pTxData[2] = { WRITE_SLAVE_OPERATION | address, data };
	CS_START(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	HAL_SPI_Transmit(&hspi5, pTxData, 2, HAL_MAX_DELAY);
	CS_END(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
}

uint8_t Gyro_Read(uint8_t address) {
	uint8_t pTxData[2] = { READ_SLAVE_OPERATION | address, 0 };
	uint8_t pRxData[2] = { 0, 0 };
	CS_START(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	HAL_SPI_TransmitReceive(&hspi5, pTxData, pRxData, 2, HAL_MAX_DELAY);
	CS_END(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	return pRxData[1];
}

void CS_START() {
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);
}

void CS_END() {
	HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
}

uint8_t Gyro_HasData() {
	if (HAL_GPIO_ReadPin(MEMS_INT2_GPIO_Port, MEMS_INT2_Pin)) {
		return 1;
	} else {
		return 0;
	}
}

void Gyro_ReadXYZ(Gyro_xyz_data *xyz) {
	uint8_t pTxData[7] = { READ_SLAVE_CONTINUOUS | 0x28, 0, 0, 0, 0, 0, 0 };
	uint8_t pRxData[7] = { 0, 0, 0, 0, 0, 0, 0 };
	CS_START();
	HAL_SPI_TransmitReceive(&hspi5, pTxData, pRxData, 7, HAL_MAX_DELAY);
	CS_END();
	xyz->x = pRxData[1] << 8 | pRxData[2];
	xyz->y = pRxData[3] << 8 | pRxData[4];
	xyz->z = pRxData[5] << 8 | pRxData[6];
}

void Gyro_handleInt2IRQ() {
	CS_START();
	HAL_SPI_TransmitReceive_DMA(&hspi5, pTx, pRx, 7);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi) {
	if (hspi == &hspi5) {
		dma_xyz.x = (int16_t)((pRx[1] << 8) | pRx[2]);
		dma_xyz.y = (int16_t)((pRx[3] << 8) | pRx[4]);
		dma_xyz.z = (int16_t)((pRx[5] << 8) | pRx[6]);
		CS_END();

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xQueueOverwriteFromISR(queue, &dma_xyz, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

int Gyro_rtosGet(Gyro_xyz_data* xyz, int timeout_ms) {
	TickType_t timeout_ticks = timeout_ms / portTICK_PERIOD_MS;
	if (xQueueReceive(queue, xyz, timeout_ticks)) {
		return 1;
	}
	return 0;
}

