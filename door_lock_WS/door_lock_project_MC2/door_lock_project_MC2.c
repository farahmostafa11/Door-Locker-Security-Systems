/*
 * door_lock_project_MC2.c
 *
 *  Created on: Oct 29, 2021
 *      Author: farah
 */
#include<avr/io.h>
#include"DC_motor.h"
#include"external_eeprom.h"
#include"buzzer.h"
#include"uart.h"
#include"timers.h"
#include"twi.h"
#include <util/delay.h> /* For the delay functions */
#define MAX_TIMER1_COUNT	(uint16)65535
uint8 delay_out=0;
uint8 motor_speed=100;
Timers_ConfigType timer_config={TIMER_1,0,MAX_TIMER1_COUNT,CTC,F_CPU_1024};///------------>
TWI_ConfigType twi_config={0b00000010,0x02};
uint8 counter=0;
uint8 change_pass=0;
UART_ConfigType uart_config={DISABLED,ONE_BIT,BIT_8,9600};
uint16 mem_loc=0X000000A0;
uint8 choice;
uint8 first_password[5];
uint8 password_from_eeprom[5];
uint8 password_eeprom=0;
uint8 password_check=1,password_recheck=1;

/*
 * Description : Function to turning the buzzer ON for 1 minute then turning it OFF again
 */
void alert_enable(){
	buzzer_on();
	timer_config.COMP_value=MAX_TIMER1_COUNT;
	for(int i=0;i<7;i++){
		Timer_init(&timer_config);
		while(delay_out==0);
		delay_out=0;
	}
	buzzer_off();
	Timer_DeInit(TIMER_1);
}
void open_door_fn(void){
	/*
	 * the timer is set to work on CTC mood
	 * to delay the timer for 15 second-->117187 counts
	 * 117187-MAX_TIMER1_COUNT=51652*/
	//moving the dc-motor in Clock-wise direction at max speed
	DcMotor_Rotate(CW,motor_speed);
	timer_config.COMP_value=MAX_TIMER1_COUNT;
	Timer_init(&timer_config);
	while(delay_out==0);
	delay_out=0;
	timer_config.COMP_value=51652;
	Timer_init(&timer_config);
	while(delay_out==0);
	delay_out=0;
	/*
	 * to delay the timer for 3 second--> 23437 counts
	 */
	timer_config.COMP_value=23437;
	//stopping the motor motion
	DcMotor_Rotate(STOP,motor_speed);
	Timer_init(&timer_config);
	while(delay_out==0);
	delay_out=0;
	timer_config.COMP_value=MAX_TIMER1_COUNT;
	//moving the dc-motor in Anti-Clock-wise direction at max speed
	DcMotor_Rotate(ACW,motor_speed);
	Timer_init(&timer_config);
	while(delay_out==0);
	delay_out=0;
	timer_config.COMP_value=51652;
	Timer_init(&timer_config);
	while(delay_out==0);
	delay_out=0;
	DcMotor_Rotate(STOP,motor_speed);
	Timer_DeInit(TIMER_1);
	timer_config.COMP_value=MAX_TIMER1_COUNT;
}
void timer_adjust(void){
	delay_out=1;
}
int main(void){
	Timer_setCallBack(timer_adjust,TIMER_1);
	// Enable global interrupts in MC.
	SREG  |= (1<<7);
	TWI_init(& twi_config);
	buzzer_init();
	UART_init(& uart_config);
	DcMotor_Init();
	while(1){
		counter=0;
		password_check=1;
		password_recheck=1;
		change_pass=0;
		//receiving the initial password byte by byte from MC1 and save it to first_password
		for(int i=0;i<5;i++){
			first_password[i]=UART_recieveByte();
		}
		/*receiving the re-entered password byte by byte from MC1 and compare it to the values
		 * saved in first_password*/
		for(int i=0;i<5;i++){
			/*
			 * if a byte is not the same to it's corresponding it sets password_check=0*/
			if(first_password[i]!=UART_recieveByte()){
				password_check=0;
			}
		}

		if(password_check==1){
			/*
			 * the 2 passwords are identical we save the password to eeprom*/
			UART_sendByte(0);
			for(int i=0;i<5;i++){
				EEPROM_writeByte(mem_loc+i,first_password[i]);
				_delay_ms(100);
			}
			while(change_pass==0){
				//receive the user choice for the main menu options
				choice=UART_recieveByte();
				//if the user chose to open the door ('+') then we ask him to enter the password
				if(choice=='+'){
					counter=0;	//this counter counts the times the user entered the password
					counter++;	//the first chance to enter the right password
					password_recheck=1;
					/*
					 * check if the password entered by the user is identical to password saved to
					 * that saved in the eeprom*/
					for(int i=0;i<5;i++){
						first_password[i]=UART_recieveByte();
						EEPROM_readByte(mem_loc+i,&password_eeprom);
						password_from_eeprom[i]=password_eeprom;
						/*
						 * if a byte is not the same to it's corresponding it sets password_recheck=0*/
						if(first_password[i]!=password_eeprom){
							password_recheck=0;
						}
						else{
						}
					}
					/*
					 * if the user entered the password wrongly he/she have 2 more chances to enter it
					 * right*/
					while(password_recheck==0 && counter!=3){
						UART_sendByte('w');//-->S2
						counter++;
						password_recheck=1;
						for(int i=0;i<5;i++){
							/*
							 * if the received byte is not the same to it's corresponding it sets
							 * password_recheck=0*/
							if(password_from_eeprom[i]!=UART_recieveByte()){
								password_recheck=0;
							}
						}
					}
					if(password_recheck==1){
						/*
						 * if the user entered the password right before exceeding the limit
						 * the door opens*/
						open_door_fn();
						/*
						 * informing MC1 the the password is correct by sending 'r'*/
						UART_sendByte('r');
					}
					else{
						/*
						 * if the user didn't enter the password right
						 * an alert is enabled for 1 minute*/
						UART_sendByte('x');
						alert_enable();
					}
				}
				else{
					/*if the user chose to change the password ('-') then we ask him to repeat the initial
					 *steps*/
					change_pass=1;
				}
			}
		}
		else{
			/*
			 * if the 2 passwords are different sends 1 to MC1*/
			UART_sendByte(1);
		}
	}
}
