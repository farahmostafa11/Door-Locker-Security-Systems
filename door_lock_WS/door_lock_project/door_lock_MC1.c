/*
 * door_lock_MC1.c
 *
 *  Created on: Oct 29, 2021
 *      Author: farah
 */
#include<avr/io.h>
#include"lcd.h"
#include"keypad.h"
#include "uart.h"
#include"timers.h"
#include <util/delay.h> /* For the delay functions */

UART_ConfigType UARTCONFIG={DISABLED,ONE_BIT,BIT_8,9600};
Timers_ConfigType timer_config={TIMER_1,0,0,NORMAL,F_CPU_1024};
uint8 password_check=0,is_right=0,main_menu_choice;
uint8 password_input[5];
uint8 change_password=0,delay_out=0;

/*
 * Description : Function to print on the screen the order to the user and * corresponding to each
 * number on the password entered by the user
 */
void LCD_printing(const char *Str){
	LCD_sendCommand(LCD_CLEAR_COMMAND);//clear the screen to start new printing
	LCD_moveCursor(0,0);
	LCD_displayString(Str);
	LCD_moveCursor(1,0);
	for(int i=0;i<5;i++){
		password_input[i]=KEYPAD_getPressedKey();
		while((password_input[i]=='+') || (password_input[i]=='-') ||(password_input[i]=='*') \
				||(password_input[i]=='%')|| (password_input[i]==13)|| (password_input[i]=='=')){
			password_input[i]=KEYPAD_getPressedKey();
		}
		LCD_displayCharacter('*');
		_delay_ms(2000);
	}
	while(KEYPAD_getPressedKey()!=13);
	_delay_ms(2000); /* Press time */
}

/*
 * Description : Function to send the entered password by the user to the controlling MCU (MC2)
 * using UART
 */
void send_password(){
	for(int i=0;i<5;i++){
		UART_sendByte(password_input[i]);
	}
}

/*
 * Description : Function to end the delay time by making the condition of the while loop be false
 */
void call_back_funct(void){

	delay_out=1;
}
int main(void){
	Timer_setCallBack(call_back_funct,TIMER_1);
	// Enable global interrupts in MC.
	SREG  |= (1<<7);
	LCD_init();
	UART_init(&UARTCONFIG);
	while(1){
		change_password=0;	//variable used to check if the user chose to change the password
		password_check=0;
		is_right=0;
		change_password=0;
		//displaying "Please enter Password:" on the screen and take the input from the user
		LCD_printing("Please enter Password:");
		//sending the password to MC2
		send_password();
		//displaying "Please re_enter Password:" on the screen and take the input from the user
		LCD_printing("Please re_enter Password:");
		//sending the re-entered password to MC2
		send_password();
		/*receive byte sent by MC2 which indicates if the 2 passwords are identical or not
		password_check==0 -->identical		password_check==0 -->not identical*/
		password_check=UART_recieveByte();

		while(password_check==0 && change_password==0){
			//printing the main menu options
			LCD_sendCommand(LCD_CLEAR_COMMAND);
			LCD_moveCursor(0,0);
			LCD_displayString("+: Open Door");
			LCD_moveCursor(1,0);
			LCD_displayString("-: Change Password");
			//getting the user choice
			main_menu_choice=KEYPAD_getPressedKey();
			_delay_ms(200); /* Press time */
			//wait till the user enters + of - from the main menu
			while(main_menu_choice!='+' && main_menu_choice!='-'){
				main_menu_choice=KEYPAD_getPressedKey();
				_delay_ms(200); /* Press time */
			}
			//sending the user choice to MC2
			UART_sendByte(main_menu_choice);//-->S3
			if(main_menu_choice=='+'){
				//if the user chose to open the door ('+') then we ask him to enter the password
				//displaying "Please enter Password:" on the screen and take the input from the user
				LCD_printing("Please enter Password:");
				//sending the password to MC2
				send_password();
				/*if the user entered the password right "welcome :)" will be printed on the screen till
				the door ends it's opening as the byte is sent by MC2 after opening the door*/
				LCD_sendCommand(LCD_CLEAR_COMMAND);
				LCD_moveCursor(0,0);
				LCD_displayString("welcome :)");
				is_right=UART_recieveByte();//-->R2
				while(is_right=='w'){
					/*
					 * if the user entered wrong password we will ask him to re enter the password
					 * till MC2 confirms the users password by sending 'r' byte
					 * or the user writs the password wrongly for the limited times decided by MC2
					 * so MC2 sends 'x'*/
					LCD_printing("Please enter Password:");
					send_password();
					is_right=UART_recieveByte();//-->R3
				}
				if(is_right!='r'){
					/*
					 * MC2 sends 'x' so an alert message is printed on the screen for 1 minute*/
					LCD_sendCommand(LCD_CLEAR_COMMAND);
					LCD_moveCursor(0,0);
					LCD_displayString("HARD LUCK:)");
					/*
					 * the timer is set to work on normal (overflow) mood so every 65536 counts
					 * it makes an interrupt so we need 7 counts to make 1 minute
					 * 1minute -->480000 counts*/
					for(int i=0;i<7;i++){
						Timer_init(&timer_config);
						while(delay_out==0);
						delay_out=0;
					}
				}
				else{

				}
			}
			else{
				/*if the user chose to change the password ('-') then we ask him to repeat the initial
				 *steps*/
				change_password=1;
			}
		}
	}
}
