/*
 * button.h
 *
 *  Created on: Sep 22, 2025
 *      Author: armin
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "gpio.h"

typedef enum {
    Button_Up,
    Button_Rising,
    Button_Falling,
    Button_Down,
} Button_State;

Button_State Button_GetState();
void Button_Update();


#endif /* BUTTON_H_ */
