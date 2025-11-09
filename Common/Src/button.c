/*
 * button.c
 *
 *  Created on: Sep 22, 2025
 *      Author: armin
 */

#include "button.h"
Button_State state = Button_Up;

Button_State Button_GetState() {
	return state;
}

void Button_Update() {
	GPIO_PinState newState = HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_2);
	if ((state == Button_Up || state == Button_Rising) && newState == GPIO_PIN_SET) {
		state = Button_Up;
	} else if ((state == Button_Down || state == Button_Falling) && newState == GPIO_PIN_SET) {
		state = Button_Rising;
	} else if ((state == Button_Up || state == Button_Rising) && newState == GPIO_PIN_RESET) {
		state = Button_Falling;
	} else if ((state == Button_Down || state == Button_Falling) && newState == GPIO_PIN_RESET) {
		state = Button_Down;
	}
}
