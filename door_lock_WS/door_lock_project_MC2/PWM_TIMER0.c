/*
 * PWM_TIMER0.c
 *
 *  Created on: Oct 10, 2021
 *      Author: farah
 */
#include"PWM_TIMER0.h"
#include "avr/io.h" /* To use the TIMER0 Registers */
#include "gpio.h"
#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*
 *  Description:
 * The function responsible for trigger the Timer0 with the PWM Mode.
 * Setup the PWM mode with Non-Inverting.
 * Setup the prescaler with F_CPU/8.
 * Setup the compare value based on the required input duty cycle
 * Setup the direction for OC0 as output pin through the GPIO driver.
 * The generated PWM signal frequency will be 500Hz to control the DC
 * Motor speed.
 */
void PWM_Timer0_Start(uint8 duty_cycle){
	/*
	 * to set the speed of the fan we change the value of the PWM of the  used timer in this task
	 * i used TIMER0 which is 8 bits timer,therefore it has values from 0->255 by which we change
	 *  the duty cycle to change the value of the output speed therefore the motor speed
	 *  0% ->0	100% ->255	50% ->128	*/
	TCNT0=0;
	/*
	 * Configure OCR0 register to the value 0->255 according to the duty cycle entered
	 */
	OCR0=(uint8)(PWM_TIMER0_MAX_VALUE*((float)duty_cycle/100));
	/* Configure the direction for PB3/OC0 pin as output pin
	 * where the PWM signal is generated from MC*/
	/* Configure PB3/ OC0 as output pin --> pin where the PWM signal is generated from Timer0 */
	GPIO_setupPinDirection(TIMER0_OCO_PORT_ID,TIMER0_OCO_PIN_ID,PIN_OUTPUT);

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS01);

}
