/*
 * buzzer.h
 *
 *  Created on: Oct 29, 2021
 *      Author: farah
 */

#ifndef BUZZER_H_
#define BUZZER_H_
#include"std_types.h"
#include"gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID    PORTA_ID
#define BUZZER_PIN_ID     PIN0_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 *  The Function responsible for setup the direction for the
 *	buzzer pin through the GPIO driver
 */
void buzzer_init(void);

/*
 * Description :
 *  The Function responsible for turning the buzzer on
 */
void buzzer_on(void);

/*
 * Description :
 *  The Function responsible for turning the buzzer off
 */
void buzzer_off(void);


#endif /* BUZZER_H_ */
