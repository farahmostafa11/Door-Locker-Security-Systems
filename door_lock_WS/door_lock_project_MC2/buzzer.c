/*
 * buzzer.c
 *
 *  Created on: Oct 29, 2021
 *      Author: farah
 */

#include"buzzer.h"
#include"gpio.h"
#include"std_types.h"

/*
 * Description :
 *  The Function responsible for setup the direction for the
 *	buzzer pin through the GPIO driver
 */
void buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
}

/*
 * Description :
 *  The Function responsible for turning the buzzer on
 */
void buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}

/*
 * Description :
 *  The Function responsible for turning the buzzer off
 */
void buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
