/*
 * timers.c
 *
 *  Created on: Oct 26, 2021
 *      Author: farah
 */

#include"timers.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_timer_0)(void) = NULL_PTR;
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_timer_1)(void) = NULL_PTR;
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr_timer_2)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_timer_0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_OVF_vect){
	if(g_callBackPtr_timer_0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_timer_1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPB_vect)
{
	if(g_callBackPtr_timer_1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect){
	if(g_callBackPtr_timer_1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_timer_2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect){
	if(g_callBackPtr_timer_2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr_timer_2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description : Function to initialize the Timer driver
 *  1. Select the required Timer (0,1 or 2)
 *  2. Set the required mode (Normal-CTC)
 * 	3. Set the required clock prescaler.
 * 	4. Set the required initial value
 * 	5. Set the required compare value (in case of CTC mode)
 */
void Timer_init(const Timers_ConfigType * Config_PTR){
	if(Config_PTR->timer_id==TIMER_0){
		OCR0  = Config_PTR->COMP_value; // Set Compare Value in case of compare mode
		TCNT0 = Config_PTR->initial_value; // Set Timer initial value to 0

		/* configure the timer
		 * 1. Non PWM mode FOC0=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. Normal Mode COM00=0 & COM01=0
		 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
		 */
		TCCR0 = (1<<FOC0);

		if(Config_PTR->timer_mode==NORMAL){
			TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt
		}
		else{
			TIMSK |= (1<<OCIE0); // Enable Timer0 Compare match Interrupt
			TCCR0 |= (1<<WGM01);
		}

		TCCR0 = (TCCR0 & 0XF8) | (Config_PTR->timer_prescaler & 0X07); //Setting the prescaler
	}
	else if(Config_PTR->timer_id==TIMER_1){
		TCNT1 = Config_PTR->initial_value; // Set Timer initial value to 0
		OCR1A  = Config_PTR->COMP_value; // Set Compare Value in case of compare mode
		if(Config_PTR->timer_mode==NORMAL){
			TIMSK |= (1<<TOIE1);
		}
		else{
			TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */
			TCCR1B = (1<<WGM12);	/*selecting the CTC mode*/
		}
		/* Configure timer control register TCCR1A
		 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
		 * 2. FOC1A=1 FOC1B=0
		 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4)
		 */
		TCCR1A = (1<<FOC1A) | (1<<FOC1B);
		/* Configure timer control register TCCR1B
		 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
		 * 2. Prescaler = F_CPU/8 CS10=0 CS11=1 CS12=0
		 */
		TCCR1B = (TCCR1B & 0XF8) | (Config_PTR->timer_prescaler & 0X07);
	}
	else{
		OCR2  = Config_PTR->COMP_value; // Set Compare Value in case of compare mode
		TCNT2 = Config_PTR->initial_value; // Set Timer initial value to 0
		/* configure the timer
		 * 1. Non PWM mode FOC0=1
		 * 2. Normal Mode WGM01=0 & WGM00=0
		 * 3. Normal Mode COM00=0 & COM01=0
		 * 4. clock = F_CPU/256 CS00=0 CS01=0 CS02=1
		 */
		TCCR2 = (1<<FOC2);
		if(Config_PTR->timer_mode==NORMAL){
			TIMSK |= (1<<TOIE2); // Enable Timer0 Overflow Interrupt
		}
		else{
			TIMSK |= (1<<OCIE2); // Enable Timer0 Compare match Interrupt
			TCCR2 |= (1<<WGM21);
		}
		TCCR2 = (TCCR2 & 0XF8) | (Config_PTR->timer_prescaler & 0X07); //Setting the prescaler
	}
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(void(*a_ptr)(void),const Timer_ID timer_num){
	if(timer_num==TIMER_0){
		g_callBackPtr_timer_0=a_ptr;
	}
	else if(timer_num==TIMER_1){
		g_callBackPtr_timer_1=a_ptr;
	}
	else{
		g_callBackPtr_timer_2=a_ptr;
	}
}


void Timer_DeInit(const Timer_ID timer_num){
	if(timer_num==TIMER_0){
		TCCR0=0;
	}
	else if(timer_num==TIMER_1){
		TCCR1A=0;
		TCCR1B=0;
	}
	else{
		TCCR2=0;
	}
}
