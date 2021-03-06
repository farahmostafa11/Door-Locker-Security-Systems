/*
 * timers.h
 *
 *  Created on: Oct 26, 2021
 *      Author: farah Mostafa
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum
{
	TIMER_0,TIMER_1,TIMER_2
}Timer_ID;

typedef enum
{
	NO_CLK,CLK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EXTERNAL_CLK_FALLING,EXTERNAL_CLK_RISING
}Timer_Prescaler;

typedef enum
{
	NORMAL,CTC=0X02
}Timer_Mode;

typedef struct{
	Timer_ID timer_id;
	uint16 initial_value;
	uint16 COMP_value;
	Timer_Mode timer_mode;
	Timer_Prescaler timer_prescaler;
}Timers_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 *  1. Select the required Timer (0,1 or 2)
 *  2. Set the required mode (Normal-CTC)
 * 	3. Set the required clock prescaler.
 * 	4. Set the required initial value
 * 	5. Set the required compare value (in case of CTC mode)
 */
void Timer_init(const Timers_ConfigType * Config_PTR);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(void(*a_ptr)(void),const Timer_ID timer_num);

/*
 * Description: Function to disable the Timer
 */
void Timer_DeInit(const Timer_ID timer_num);

#endif /* TIMERS_H_ */
