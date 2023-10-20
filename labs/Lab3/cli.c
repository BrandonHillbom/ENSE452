#include "cli.h"

char message[20] = ""; // full message that the user has typed
int exitProgram; //exit program flag

void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int i = 0; i  <size; i++)
	{
			sendbyte(pData[i]); //send by usart
	}
}


void CLI_Receive(uint8_t *input) {
		
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
				char* str = "\r\nLED has been turned on.\r\n"; 
				uint16_t length = strlen(str);  
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
			}
			else if (strcmp(message, (const char *)"off") == 0)
			{
				GPIOA->BSRR |= GPIO_BSRR_BR5;	//turn led off
				char* str = "\r\nLED has been turned off.\r\n"; 
				uint16_t length = strlen(str); 
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
			}
			else if (strcmp(message, (const char *)"help") == 0)
			{
				char* str = "\r\nSelect an option: \r\nTurn the led on by typing 'on' \r\nTurn the LED off by typing 'off'\r\nExit the program by typing 'exit'\r\n"; 
				uint16_t length = strlen(str);  
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length); //display commands
			}
			else if (strcmp(message, (const char *)"exit") == 0)
			{
				char* str = "\r\nProgram terminated...\r\n";
				uint16_t length = strlen(str);
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				serial_close(); //exit program
				exitProgram = 1; //end the while loop in main
			}
			else {
				//default case, message was not understood/is invalid
				memset(message, 0, sizeof(message));
				const char* str = "\r\nSelect an option: \r\nTurn the led on by typing 'on' \r\nTurn the LED off by typing 'off'\r\nExit the program by typing 'exit'\r\n"; 
				uint16_t length = (uint16_t)strlen(str);
				uint8_t* data = (uint8_t*)str;	
				CLI_Transmit(data, length);
				}
			}	
		
		else 
			if (messageLength < sizeof(message) - 1) {
                message[messageLength++] = input[0]; // append char to message
                message[messageLength] = '\0'; 
            }
} 
