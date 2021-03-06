/*
 * DC_motor.c
 *
 *  Created on: Oct 9, 2021
 *      Author: farah
 */

#include"DC_motor.h"
#include"gpio.h"
#include"PWM_TIMER0.h"

/*
 * Description :
 *  The Function responsible for setup the direction for the two
 *	motor pins through the GPIO driver
 */
 void DcMotor_Init(void){
	 /* Configure the direction for PB0 and PB1 pins as output pins */
	 GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN0_ID,PIN_OUTPUT);
	 GPIO_setupPinDirection(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,PIN_OUTPUT);
	 GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN0_ID,LOGIC_LOW);
	 GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,LOGIC_LOW);
 }

 /*
  * Description :
  *  This function responsible for rotate the DC Motor CW/ or A-CW or
  *	 stop the motor based on the state input state value.
  *  Send the required duty cycle to the PWM driver based on the
  *  required speed value.
  */
 void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	 /*
	  * writing the state entered STOP/CW/A_CW to PB0 and PB1 pins
	  */
	 GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN0_ID,((uint8)state&(1<<0)));
	 GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_PIN1_ID,(((uint8)state&(1<<1))==2)?LOGIC_HIGH:LOGIC_LOW);
	 /* Generate PWM with speed entered */
	 PWM_Timer0_Start(speed);
 }
