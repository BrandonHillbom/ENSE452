#include "cli.h"

char message[20] = ""; // full message that the user has typed
QueueHandle_t xMoveElevatorQueue; //define queues 
QueueHandle_t xSignalElevatorQueue;
QueueHandle_t xEmergencyStopQueue;

// Loop through the message and send each byte to the CLI 
void CLI_Transmit(uint8_t *pData, uint16_t size) {
	for (int i = 0; i  <size; i++)
	{
			sendbyte(pData[i]); //send by usart
	}
}


// This function receives the value that was typed into the CLI. If it is a valid command, 
// a prompt will be sent to the CLI and a value sent to the correct queue
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
				//move to floor 0
				const char* feedback = "Moving to basement floor...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				ElevatorState target;
				target.targetFloor = 0;
				xQueueSendToFront(xMoveElevatorQueue, &target, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
			}
			else if (strcmp(message, (const char *)"1") == 0)	//check if total string is a command
			{
				//move to floor 1
				const char* feedback = "Moving to floor 1...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				
				ElevatorState target;
				target.targetFloor = 1;
				xQueueSendToFront(xMoveElevatorQueue, &target, NULL); 
			}
			else if (strcmp(message, (const char *)"2") == 0)	//check if total string is a command
			{
				//move to floor 2
				const char* feedback = "Moving to floor 2...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				ElevatorState target;
				target.targetFloor = 2;
				xQueueSendToFront(xMoveElevatorQueue, &target, NULL); 
			}
			else if (strcmp(message, (const char *)"3") == 0)	//check if total string is a command
			{
				//move to floor 3
				const char* feedback = "Moving to floor 3...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				ElevatorState target;
				target.targetFloor = 3;
				xQueueSendToFront(xMoveElevatorQueue, &target, NULL); 
			}
			else if (strcmp(message, (const char *)"4") == 0)	//check if total string is a command
			{
				//move to floor 4
				const char* feedback = "Moving to floor 4...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length); //send opening message to the screen
				
				ElevatorState target;
				target.targetFloor = 4;
				xQueueSendToFront(xMoveElevatorQueue, &target, NULL); 
			}
			else if (strcmp(message, (const char *)"up") == 0)	//check if total string is a command
			{
				//user selects to go up from the outside panel
				const char* feedback = "Elevator signalled...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				OutsidePanelState target;
				target.direction = 'u';
				xQueueSendToFront(xSignalElevatorQueue, &target, NULL);

			}
			else if (strcmp(message, (const char *)"down") == 0)	//check if total string is a command
			{
				//user selects to go down from the outside panel
				const char* feedback = "Elevator signalled...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				OutsidePanelState target;
				target.direction = 'd';
				xQueueSendToFront(xSignalElevatorQueue, &target, NULL); 
				
			}
			else if (strcmp(message, (const char *)"<") == 0)	//check if total string is a command
			{
				//open the elevator door
				const char* openDoor = "\r\nOpening door. Would you like to leave? y/n.\r\n";
				uint16_t length = (uint16_t)strlen(openDoor);
				uint8_t* array = (uint8_t*)openDoor;	
				CLI_Transmit(array, length);
	
			}
			else if (strcmp(message, (const char *)">") == 0)	//check if total string is a command
			{
				//close the elevator door
				const char* openDoor = "\r\nClosing door...\r\n";
				uint16_t length = (uint16_t)strlen(openDoor);
				uint8_t* array = (uint8_t*)openDoor;	
				CLI_Transmit(array, length);

			}
			else if (strcmp(message, (const char *)"y") == 0)	//check if total string is a command
			{

				//User wants to leave the elevator
				const char* openDoor = "\r\nUser exited elevator.\r\n";
				uint16_t length = (uint16_t)strlen(openDoor);
				uint8_t* array = (uint8_t*)openDoor;	
				CLI_Transmit(array, length);
				//display the outside panel
				CLI_Transmit(signalPanel, sizeof(signalPanel));
			}
			else if (strcmp(message, (const char *)"n") == 0)	//check if total string is a command
			{
				//User does not want to leave the elevator.
				const char* openDoor = "\r\nClosing doors...\r\n";
				uint16_t length = (uint16_t)strlen(openDoor);
				uint8_t* array = (uint8_t*)openDoor;	
				CLI_Transmit(array, length);
			}
			else {
				//invalid command entered
					const char* message = "\r\nPlease enter a valid command...\r\n";
					uint16_t length = (uint16_t)strlen(message);
					uint8_t* array = (uint8_t*)message;	
					CLI_Transmit(array, length);
				}
			memset(message, 0, sizeof(message)); //clear message value
			}	
		
		else 
			if (messageLength < sizeof(message) - 1) {
                message[messageLength++] = input[0]; // append char to message
                message[messageLength] = '\0'; 
            }
} 

//ISR to read the button press from the blue button on the STM32f103 board 
// Upon pressing it once, the elevator will stop. Presssing it twice will resume action
void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & EXTI_PR_PR13) { // Check if the interrupt is from PC13
      EXTI->PR |= EXTI_PR_PR13; 
			int stopFlag = 1;
			xQueueSendToFrontFromISR(xEmergencyStopQueue, &stopFlag, NULL);	
    }
}
