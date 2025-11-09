/*
 * gyro.c
 *
 *  Created on: Nov 9, 2025
 *      Author: armin
 */
#include "gyro.h"
#include "main.h"
#include "spi.h"

void Gyro_Init() {
	Gyro_Write(0x20, 0b11111111);
	Gyro_Write(0x22, 0b00001000);
}

uint8_t Gyro_CanReadWHOAMI() {
    uint8_t pTxData[2] = {READ_SLAVE_OPERATION | REG_WHOAMI, 0};
    uint8_t pRxData[2] = {0, 0};
    CS_START(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
    HAL_SPI_TransmitReceive(&hspi5, pTxData, pRxData, 2, HAL_MAX_DELAY);
    CS_END(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
    return pRxData[1] == REG_WHOAMI_DEFAULT_VALUE;
}

void Gyro_Write(uint8_t address, uint8_t data) {
	uint8_t pTxData[2] = {WRITE_SLAVE_OPERATION | address, data};
	CS_START(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
	HAL_SPI_Transmit(&hspi5, pTxData, 2, HAL_MAX_DELAY);
	CS_END(); // Kasuta ja implementeeri, kui puudub riistvaraline CS
}

uint8_t Gyro_Read(uint8_t address) {
    uint8_t pTxData[2] = {READ_SLAVE_OPERATION | address, 0};
    uint8_t pRxData[2] = {0, 0};
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
	if(HAL_GPIO_ReadPin(MEMS_INT2_GPIO_Port, MEMS_INT2_Pin)) {
		return 1;
	} else {
		return 0;
	}
}

void Gyro_ReadXYZ(Gyro_xyz_data* xyz) {
	xyz->x = Gyro_Read(0x29) << 8;
	xyz->x |= Gyro_Read(0x28);
	xyz->y = Gyro_Read(0x2B) << 8;
	xyz->y |= Gyro_Read(0x2A);
	xyz->z = Gyro_Read(0x2D) << 8;
	xyz->z |= Gyro_Read(0x2C);
}
