/*
 * DC_motor.h
 *
 *  Created on: Oct 9, 2021
 *      Author: farah
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
#include"std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID PORTB_ID
#define DC_MOTOR_PIN0_ID PIN0_ID
#define DC_MOTOR_PIN1_ID PIN1_ID
typedef enum{
	STOP,CW,ACW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 *  The Function responsible for setup the direction for the two
 *	motor pins through the GPIO driver
 */
 void DcMotor_Init(void);

 /*
  * Description :
  *  This function responsible for rotate the DC Motor CW/ or A-CW or
  *	 stop the motor based on the state input state value.
  *  Send the required duty cycle to the PWM driver based on the
  *  required speed value.
  */
 void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
