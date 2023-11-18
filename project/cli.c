#include "cli.h"

char message[20] = ""; // full message that the user has typed, arbitrary size
QueueHandle_t xMoveElevatorQueue;

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
				if (strcmp(message, (const char *)"b") == 0)	//check if total string is a command
			{

				//move to floor 1
				//temp
				const char* feedback = "Moving to basement floor...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				
				int targetFloor = 0;
				xQueueSendToFrontFromISR(xMoveElevatorQueue, &targetFloor, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			else if (strcmp(message, (const char *)"1") == 0)	//check if total string is a command
			{
				//move to floor 2
								//temp
				const char* feedback = "Moving to floor 1...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				
				int targetFloor = 1;
				xQueueSendToFrontFromISR(xMoveElevatorQueue, &targetFloor, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			else if (strcmp(message, (const char *)"2") == 0)	//check if total string is a command
			{
				//move to floor 3
				//temp
				const char* feedback = "Moving to floor 2...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				int targetFloor = 2;
				xQueueSendToFrontFromISR(xMoveElevatorQueue, &targetFloor, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			else if (strcmp(message, (const char *)"3") == 0)	//check if total string is a command
			{
				//move to floor 3
				//temp
				const char* feedback = "Moving to floor 3...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				int targetFloor = 3;
				xQueueSendToFrontFromISR(xMoveElevatorQueue, &targetFloor, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			else if (strcmp(message, (const char *)"4") == 0)	//check if total string is a command
			{
				//move to floor 3
				//temp
				const char* feedback = "Moving to floor 4...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				int targetFloor = 4;
				xQueueSendToFrontFromISR(xMoveElevatorQueue, &targetFloor, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			if (strcmp(message, (const char *)"up") == 0)	//check if total string is a command
			{

				//move to floor 1
				//temp
				const char* feedback = "Elevator signalled...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
			}
			if (strcmp(message, (const char *)"down") == 0)	//check if total string is a command
			{

				//move to floor 1
				//temp
				const char* feedback = "Elevator signalled...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
			}
			if (strcmp(message, (const char *)"leave") == 0)	//check if total string is a command
			{

				//move to floor 1
				//temp
				const char* feedback = "Exited elevator.\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
	
				CLI_Transmit(signalPanel, sizeof(signalPanel));
			}
			else {
				//have to have cases for out of eleavtor and inside elevator
					uint16_t length = (uint16_t)strlen(elevatorPanel);
					uint8_t* array = (uint8_t*)elevatorPanel;	
					CLI_Transmit(array, length); //send opening message to the screen
				}
			memset(message, 0, sizeof(message)); //clear message value
			}	
		
		else 
			if (messageLength < sizeof(message) - 1) {
                message[messageLength++] = input[0]; // append char to message
                message[messageLength] = '\0'; 
            }
} 
