/*
 * gyro.h
 *
 *  Created on: Nov 9, 2025
 *      Author: armin
 */

#ifndef INC_GYRO_H_
#define INC_GYRO_H_

#include "stdint.h"

#define READ_SLAVE_OPERATION 0b10000000
#define WRITE_SLAVE_OPERATION 0
#define REG_WHOAMI 0x0F
#define REG_WHOAMI_DEFAULT_VALUE 0b11010100

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Gyro_xyz_data;

void Gyro_Init();
uint8_t Gyro_CanReadWHOAMI();
void Gyro_Write(uint8_t addrses, uint8_t data);
uint8_t Gyro_Read(uint8_t address);
void CS_START();
void CS_END();
uint8_t Gyro_HasData();
void Gyro_ReadXYZ(Gyro_xyz_data* xyz);

#endif /* INC_GYRO_H_ */
