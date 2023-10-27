/*
	ENSE 452 Lab4
	Brandon Hillbom
	200426340
	The objective is to improve the Command-Line Interface (CLI) by introducing a 
	status window to display the status of the LED.

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
	
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
  CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
	CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
	strcpy(message, "status");
	uint8_t enter = 0x0D;
	CLI_Receive(&enter);	
	
	CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	//move the cursor to the message window
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
