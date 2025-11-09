/*
 * led.h
 *
 *  Created on: Sep 22, 2025
 *      Author: armin
 */

#ifndef LED_H_
#define LED_H_


#define LED_Green_Toggle() HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
#define LED_Green_Write(GPIO_PinState) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PinState)
#define LED_Green_Read() ({GPIO_PinState state; state = HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_13); state;})

#define LED_Red_Toggle() HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
#define LED_Red_Write(GPIO_PinState) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PinState)
#define LED_Red_Read() ({GPIO_PinState state; state = HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_14); state;})

#endif /* LED_H_ */
