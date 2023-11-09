#include "cli.h"

char message[20] = ""; // full message that the user has typed, arbitrary size
QueueHandle_t xFrequencyQueue;

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
				if (strcmp(message, (const char *)"f1") == 0)	//check if total string is a command
			{

				str = "\r\nLED frequency changed to 200 ms\r\n"; 
				uint16_t length = strlen(str); 
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				int frequency = 200;
				xQueueSendToFrontFromISR(xFrequencyQueue, &frequency, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
				
				//status window
				CLI_Transmit(ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR)); //save where the cursor is
				CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP)); //move cursor to the top
				CLI_Transmit(ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE)); //clear the status line
				str = "\r\nFrequency of flashing light: 0.2 seconds\r\n";		
				length = strlen(str);
				uint8_t* dataSt = (uint8_t*)str; //send the new frequency
				CLI_Transmit(dataSt, length);
				CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
				CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
				
			}
			else if (strcmp(message, (const char *)"f2") == 0)	//check if total string is a command
			{
				str = "\r\nLED frequency changed to 1s.\r\n"; 
				uint16_t length = strlen(str); 
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				int frequency = 1000;
				xQueueSendToFrontFromISR(xFrequencyQueue, &frequency, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
				
				//status window
				CLI_Transmit(ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR)); //save where the cursor is
				CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP)); //move cursor to the top
				CLI_Transmit(ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE)); //clear the status line
				str = "\r\nFrequency of flashing light: 1 second   \r\n";		
				length = strlen(str);
				uint8_t* dataSt = (uint8_t*)str; //send the new frequency
				CLI_Transmit(dataSt, length);
				CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
				CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
				
			}
			else if (strcmp(message, (const char *)"f3") == 0)	//check if total string is a command
			{
				str = "\r\nLED frequency changed to 5s.\r\n"; 
				uint16_t length = strlen(str); 
				uint8_t* data = (uint8_t*)str;
				CLI_Transmit(data, length);
				int frequency = 5000;
				xQueueSendToFrontFromISR(xFrequencyQueue, &frequency, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
				
				//status window
				CLI_Transmit(ANSI_SAVE_CURSOR, sizeof(ANSI_SAVE_CURSOR)); //save where the cursor is
				CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP)); //move cursor to the top
				CLI_Transmit(ANSI_CLEAR_LINE, sizeof(ANSI_CLEAR_LINE)); //clear the status line
				str = "\r\nFrequency of flashing light: 5 seconds  \r\n";		
				length = strlen(str);
				uint8_t* dataSt = (uint8_t*)str; //send the new frequency
				CLI_Transmit(dataSt, length);
				CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
				CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
				
			}
			else {
				//enter clicked and no command registered
				str = "\r\nSelect an option: \r\nChange frequency of blinking light to 0.2 seconds by typing 'f1' \r\nChange frequency of blinking light to 1 second by typing 'f2' \r\nChange frequency of blinking light to 5 seconds by typing 'f3' \r\n"; 
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