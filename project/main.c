/*
	ENSE 452 Term Project
	Brandon Hillbom
	200426340
	This project...
*/

#include "cli.h"
#include <stdint.h>

#define mainMOVE_ELEVATOR_TASK_PRIORITY (tskIDLE_PRIORITY + 3)
#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

static void vMoveElevatorTask(void * parameters);
static void vCLITask(void * parameters);

extern QueueHandle_t xCLIQueue;
extern QueueHandle_t xMoveElevatorQueue;
extern char message[20]; // Declare message to be arbitrary size

int main()
{
	serial_open();
	enable_interrupt();
	
	xCLIQueue = xQueueCreate(1, sizeof(char));
	xMoveElevatorQueue = xQueueCreate(1, sizeof(int)); //queue length, item size
	
	xTaskCreate(vMoveElevatorTask, "MOVE", configMINIMAL_STACK_SIZE, NULL, mainMOVE_ELEVATOR_TASK_PRIORITY, NULL);
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	vTaskStartScheduler(); // start the scheduler
	
	return 0;
}

static void vMoveElevatorTask(void * parameters) {
	int currentFloor = 1;
  int targetFloor = 1;
	for (;;) {
		BaseType_t status = xQueueReceive(xMoveElevatorQueue, &targetFloor, 100); 
//		if( uxQueueMessagesWaiting( xMoveElevatorQueue ) != 0 )		
//		{			
//			xQueueReceive(xMoveElevatorQueue, &targetFloor, 100);
//		}	
		//move to the next floor
		if (status == pdTRUE) {
            // Check the direction to move (up or down)
            int direction = (targetFloor > currentFloor) ? 1 : -1;

            // Move the elevator to the target floor
            while (currentFloor != targetFloor) {
                // Simulate the elevator moving to the next floor
                vTaskDelay(pdMS_TO_TICKS(2500)); // Adjust the delay as needed

                // Update the current floor based on the direction
                currentFloor += direction;
									CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
									CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
									CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window

							    const char* statusWin = "\r\nFloor: ";

									// Calculate the length of the concatenated string
									size_t totalLength = strlen(statusWin) + 1; // +1 for the null terminator
									char buffer[50]; // Adjust the size based on your needs

									// Convert the current floor to a string and concatenate
									snprintf(buffer, sizeof(buffer), "%s%d\r\n", statusWin, currentFloor);

									uint16_t statusWinLength = strlen(buffer);
									uint8_t* dataSt = (uint8_t*)buffer; //send the new frequency
									CLI_Transmit(dataSt, statusWinLength);
									CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
									CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
									
									CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	

									CLI_Transmit(elevatorPanel, sizeof(elevatorPanel)); //send opening message to the screen

            }
		
		}
	}
}

static void vCLITask(void * parameters) {
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
  CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
	CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
	const char* statusWin = "\r\nFloor: 1\r\n";		
	uint16_t statusWinLength = strlen(statusWin);
	uint8_t* dataSt = (uint8_t*)statusWin; //send the new frequency
	CLI_Transmit(dataSt, statusWinLength);
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
	CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
	
	CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	//move the cursor to the message window	
	CLI_Transmit(signalPanel, sizeof(signalPanel)); //send opening message to the screen
	
	uint8_t charReceived;
	
	for (;;) {
		BaseType_t status = xQueueReceive(xCLIQueue, &charReceived, 100); //block until there is something in the queue
		if (status == pdTRUE) { 
			if(charReceived == 0x08 || charReceived == 0x7f) { //backspace or delete has been pressed
					
							charReceived = 0x7f; //ensure delete is sent
				}	
				sendbyte(charReceived);	//send received character
				CLI_Receive(&charReceived);	//append to the overall message/check message value
		}

	}


}
