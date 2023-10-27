#include "cli.h"

char message[20] = ""; // full message that the user has typed, arbitrary size
int exitProgram; //exit program flag

void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int i = 0; i  <size; i++)
	{
			sendbyte(pData[i]); //send by usart
	}
}


void CLI_Receive(uint8_t *input) {
		char* str;
		int messageLength = strlen(message);
		if (input[0] == 0x08 || input[0] == 0x7f) //enter if backspace or delete has been pressed
		{
				if (messageLength > 0) //if the user has a message
					{
								message[--messageLength] = '\0'; //null at the last character spot
					}		
		}		
		else if (input[0] == 0x0D) //message is to be evaluated (enter)
		{            
			if (strcmp(message, (const char *)"on") == 0)	//check if total string is a command
			{
				GPIOA->BSRR |= GPIO_BSRR_BS5; //turn led on
				str = "\r\nLED has been turned on.\r\n";  
				uint16_t length = strlen(str);
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				
				//status window
				CLI_Transmit(ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR)); //save where the cursor is
				CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP)); //move cursor to the top
				CLI_Transmit(ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE)); //clear the status line
				str = "\r\nStatus:\e[32m LED is on \e[0m\r\n";		
				length = strlen(str);
				uint8_t* dataSt = (uint8_t*)str; //send the new status 
				CLI_Transmit(dataSt, length);
				CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
				CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
			}
			else if (strcmp(message, (const char *)"off") == 0)
			{
				GPIOA->BSRR |= GPIO_BSRR_BR5;	//turn led off
				str = "\r\nLED has been turned off.\r\n"; 
				uint16_t length = strlen(str); 
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				
				//status window
				CLI_Transmit(ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR)); //save where the cursor is
				CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP)); //move cursor to the top
				CLI_Transmit(ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE)); //clear the status line
				str = "\r\nStatus:\e[31m LED is off\e[0m\r\n";		
				length = strlen(str);
				uint8_t* dataSt = (uint8_t*)str; //send the new status 
				CLI_Transmit(dataSt, length);
				CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
				CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
			}
			else if (strcmp(message, (const char *)"status") == 0)
			{
				if (GPIOA->ODR & (1 << 5)) //check if bit 5 is set
				{
					str = "\r\nStatus: LED is on \r\n"; 
					uint16_t length = strlen(str);
					uint8_t* data = (uint8_t*)str;
					CLI_Transmit(data, length);
				}
				else
				{
					str = "\r\nStatus: LED is off\r\n"; 
					uint16_t length = strlen(str);
					uint8_t* data = (uint8_t*)str;
					CLI_Transmit(data, length);
				}
			}
			else if (strcmp(message, (const char *)"help") == 0)
			{
				str = "\r\nSelect an option:  \r\nSee the status of the LED by typing 'status'\r\nTurn the led on by typing 'on' \r\nTurn the LED off by typing 'off'\r\nExit the program by typing 'exit'\r\n"; 
				uint16_t length = strlen(str);
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length); //display commands
			}
			else if (strcmp(message, (const char *)"exit") == 0)
			{
				str = "\r\nProgram terminated...";
				uint16_t length = strlen(str);
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				serial_close(); //exit program
				exitProgram = 1; //end the while loop in main
			}
			else {
				//enter clicked and no command registered
				str = "\r\nSelect an option: \r\nTurn the led on by typing 'on' \r\nTurn the LED off by typing 'off'\r\nExit the program by typing 'exit'\r\n"; 
				uint16_t length = strlen(str);
				uint8_t* data = (uint8_t*)str;	
				CLI_Transmit(data, length);
				}
			memset(message, 0, sizeof(message)); //clear message value
			}	
		
		else 
			if (messageLength < sizeof(message) - 1) {
                message[messageLength++] = input[0]; // append char to message
                message[messageLength] = '\0'; 
            }
} 
