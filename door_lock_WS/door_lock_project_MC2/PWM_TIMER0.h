/*
 * PWM_TIMER0.h
 *
 *  Created on: Oct 10, 2021
 *      Author: farah
 */

#ifndef PWM_TIMER0_H_
#define PWM_TIMER0_H_
#include"std_types.h"
#include"PWM_TIMER0.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PWM_TIMER0_MAX_VALUE 255
#define TIMER0_OCO_PORT_ID    1
#define TIMER0_OCO_PIN_ID     3
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

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
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_TIMER0_H_ */
