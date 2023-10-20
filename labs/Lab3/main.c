/*
	ENSE 452 Lab3
	Brandon Hillbom
	200426340
	The objective is to improve the Command-Line Interface (CLI) through which you can communicate
	with the target board (STM32F103RB). From lab 2, I have abandoned the polling method used in USART communications
	and it will now use interrupts. I also created safer code by incorporating a timer in my USART
	driver code. 
	
	TODO: there is a bug where sometimes you must reneter a command (the second time it will register)

*/

#include "cli.h"
#include <stdint.h>

extern int charReceivedFlag;
extern int exitProgram;
extern uint8_t charReceived;
extern char message[20]; // Declare message to be arbitrary size

int main()
{
	serial_open();
	enable_interrupt();
	
	const char* str = "\r\nType 'help' for a full list of commands\r\n"; 
	uint16_t length = (uint16_t)strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length); //send opening message to the screen
	exitProgram = 0; //initialize, set to 1 when exit is entered
	
	while(exitProgram != 1)
	{
			if(charReceivedFlag == 1)	//If the user types a value, enter
			{
				if (charReceived == 0x08) { //check for backspace
					charReceived = 0x7f; // backspace character, need to send as delete
				} 
				sendbyte(charReceived);	//send received character
				CLI_Receive(&charReceived);	//append to the overall message/check message value
				charReceivedFlag= 0; //reset flag	
			}		
	}
	return 0;
}
