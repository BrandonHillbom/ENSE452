/*
	ENSE 452 Term Project - Elevator Control System
	Brandon Hillbom
	200426340
	
	This project implements FreeRTOS to simulate an elevator control system using the STMF103RB microcontroller. 
	The user is able to select which direction they would like to go and the elevator comes to the user upon signalling it. 
	The user is then able to select which floor they would like to go to. If the blue button from the microcontroller is
	pressed, the elevator will be stopped immediately. Pressing it again will resume operation. 
	
*/

#include "cli.h"
#include <stdint.h>

//emergency button interrupts all, signal elevator interrupts moving, moving interrupts cli
#define mainEMERGENCY_STOP_TASK_PRIORITY (tskIDLE_PRIORITY + 3) //interrupts cli task. signal the elevator when it is moving 
#define mainSIGNAL_ELEVATOR_TASK_PRIORITY (tskIDLE_PRIORITY + 3) //interrupts cli task. signal the elevator when it is moving 
#define mainMOVE_ELEVATOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2) //interrupts cli task
#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY + 1)

static void vMoveElevatorTask(void * parameters);
static void vCLITask(void * parameters);

//4 queues: cli, moving the elevator, stoping the elevator, and signalling the elevator
extern QueueHandle_t xCLIQueue; //handle user input, 
extern QueueHandle_t xMoveElevatorQueue; //move the elevator up/down, this handles the target floor
extern QueueHandle_t xEmergencyStopQueue; //stop the elevator immediately. Do nothing until released
extern QueueHandle_t xSignalElevatorQueue; //call the elevator to come to us

int main()
{
	serial_open();
	enable_interrupt();
	
	//define queues
	xCLIQueue = xQueueCreate(1, sizeof(char));
	xMoveElevatorQueue = xQueueCreate(1, sizeof(ElevatorState)); //queue length, item size
	xSignalElevatorQueue = xQueueCreate(1, sizeof(OutsidePanelState)); //queue length, item size represents the floor that we are calling from and what direction we want to go
	xEmergencyStopQueue = xQueueCreate(1, sizeof(int));

	// 2 tasks: one for elevator control and one for cli
	xTaskCreate(vMoveElevatorTask, "MOVE", configMINIMAL_STACK_SIZE, NULL, mainMOVE_ELEVATOR_TASK_PRIORITY, NULL);
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);

	vTaskStartScheduler(); // start the scheduler
	
	return 0;
}


//This function handles the state of the elevator. It contains the queues for emergency stop, signal elevator, and move elevator. 
static void vMoveElevatorTask(void * parameters) {
	ElevatorState elevator; //init elevator state
	int elevatorCurrentFloor = 3; //local to function
	elevator.currentFloor = elevatorCurrentFloor; //initial current floor will be 3. (could be any)
	elevator.direction = 'i'; //initial direction of elevator is idle
	int displayElevatorPanel = 0; // 0 is do not show the elevator panel, 1 is show it
	
	int stopFlag = 0; //continue if 0, 1 will indicate an emergency stop
	OutsidePanelState user; //intialize the state of the outside panel
	int userCurrentFloor = 1; //initial current floor of user
	
	for (;;) {
		
		//if the status is true, an emergency stop has been pressed
		BaseType_t status2 = xQueueReceive(xEmergencyStopQueue, &stopFlag, 100); 
		if (status2 == pdTRUE) { 
			if (stopFlag == 1) {
					const char* message = "\r\nEmergency Button Pressed! Press again to exit maintenance mode.\r\n";		
					uint16_t messageLength = strlen(message);
					uint8_t* dataSt = (uint8_t*)message; //send the new frequency
					CLI_Transmit(dataSt, messageLength);
				xQueueReceive(xEmergencyStopQueue, &stopFlag, portMAX_DELAY); //block until we receive another.
				stopFlag = 0;
					message = "\r\nResuming operations...\r\n";		
					messageLength = strlen(message);
					dataSt = (uint8_t*)message; 
					CLI_Transmit(dataSt, messageLength); //continue operation message
			}

		}

		//If the status is true, the elevator must be moved
		BaseType_t status = xQueueReceive(xMoveElevatorQueue, &elevator, 100); 
		if (status == pdTRUE) {
            // If the current floor has not been specified in the queue, default to the past recorded current floor
						if (elevator.currentFloor > 4 || elevator.currentFloor < 0) {
							//no value passed with elevator for current floor. Therefore, current floor value of currentFloor (local)
							elevator.currentFloor = elevatorCurrentFloor;
						}
				
						//if we aren't at the target floor, move floors
						if (elevator.currentFloor != elevator.targetFloor) {
							 int incrementFloor = (elevator.targetFloor > elevator.currentFloor) ? 1 : -1; //go down or up

                vTaskDelay(pdMS_TO_TICKS(2500)); //elevator floor to floor delay

                // Update the current floor based on the direction
									elevator.currentFloor += incrementFloor;
									elevatorCurrentFloor += incrementFloor; //failsafe
							
									//specify the elevator direction
									if (elevator.currentFloor == elevator.targetFloor) {
											elevator.direction = 'i'; //idle
									} else {
										elevator.direction = incrementFloor > 0 ? 'u' : 'd'; //going up or down
									}
									
									//Status window
									CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
									CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
									CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
									
							    const char* elevatorStatusWin = "\r\nElevator on floor: ";
									size_t totalLength = strlen(elevatorStatusWin) + 1; // +1 for the null terminator
									char buffer[50]; 
									snprintf(buffer, sizeof(buffer), "%s%d\r\n", elevatorStatusWin, elevator.currentFloor); // Convert the current floor to a string and concatenate
									uint16_t statusWinLength = strlen(buffer);
									uint8_t* dataSt = (uint8_t*)buffer; //send the new frequency
									CLI_Transmit(dataSt, statusWinLength);
									
									//main window
									CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
									CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
									CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	
									
									//if we are on the current floor
									if (elevator.currentFloor == elevator.targetFloor) {
										
										//if 
										if (user.currentFloor != elevator.currentFloor) {
											
												user.currentFloor = elevator.currentFloor; //update the user's floor
												userCurrentFloor = elevator.currentFloor; //failsafe
												CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
												CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
												CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
											
												//status window, what floor the elvator is currently on
												const char* statusWin = "\r\nElevator on floor: ";
												size_t totalLength = strlen(statusWin) + 1; // +1 for the null terminator
												char buffer[50]; 
												snprintf(buffer, sizeof(buffer), "%s%d\r\n", statusWin, elevator.currentFloor);
												uint16_t statusWinLength = strlen(buffer);
												uint8_t* dataSt = (uint8_t*)buffer; //send the new frequency
												CLI_Transmit(dataSt, statusWinLength);
												
												//status window, what floor the user is currently on
												statusWin = "\r\nYou are on floor: ";
												totalLength = strlen(statusWin) + 1; // +1 for the null terminator
												snprintf(buffer, sizeof(buffer), "%s%d\r\n", statusWin, user.currentFloor);
												statusWinLength = strlen(buffer);
												dataSt = (uint8_t*)buffer; //send the new frequency
												CLI_Transmit(dataSt, statusWinLength);
									
												//prompt exit
												const char* feedback = "exit Elevator? y/n \r\n";
												uint16_t length = (uint16_t)strlen(feedback);
												uint8_t* array = (uint8_t*)feedback;	
												CLI_Transmit(array, length);
												user.currentFloor = elevator.currentFloor;
										} else {
												//elevator has been signalled and is now on the target floor
												const char* feedback = "Elevator has arrived. Opening doors...\r\n";
												uint16_t length = (uint16_t)strlen(feedback);
												uint8_t* array = (uint8_t*)feedback;	
												CLI_Transmit(array, length);
												vTaskDelay(pdMS_TO_TICKS(2000));	
										}
										displayElevatorPanel = 1;
										elevator.direction = 'i';
									}
									if (displayElevatorPanel == 1) {
										CLI_Transmit(elevatorPanel, sizeof(elevatorPanel)); //send opening message to the screen
									}
									//update the queue with the new elevator floor, loop again
									xQueueSendToFront(xMoveElevatorQueue, &elevator, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
            }
		}
		
		//if the user has signalled the elvator
		BaseType_t status3 = xQueueReceive(xSignalElevatorQueue, &user, 100); 
		if (status3 == pdTRUE) {
				if (user.currentFloor > 4 || user.currentFloor < 0) {
							//no value passed with user for current floor. Therefore, current floor value of currentFloor (local)
							user.currentFloor = userCurrentFloor;
						}
			if (elevator.currentFloor == user.currentFloor) {
				const char* feedback = "Elevator has arrived. Opening doors...\r\n";
				uint16_t length = (uint16_t)strlen(feedback);
				uint8_t* array = (uint8_t*)feedback;	
				CLI_Transmit(array, length);
				vTaskDelay(pdMS_TO_TICKS(2000));
				CLI_Transmit(elevatorPanel, sizeof(elevatorPanel)); //send opening message to the screen
			}
			//if elevator is idle, come imediately
			//if we are going down
			//if elevator is going down and above us, stop at our floor by pushing to front of stop floors queue
			//if the elevator is below us and going up, stop at our floor, push to front of the stop floors queue
			else if (elevator.direction == 'i' || (elevator.currentFloor >= user.currentFloor && elevator.direction == 'd' && user.direction == 'd') || (elevator.currentFloor <= user.currentFloor && elevator.direction == 'u' && user.direction == 'u')) {
				//elevator must come to us
				elevator.targetFloor = user.currentFloor;
				xQueueSendToFront(xMoveElevatorQueue, &elevator, NULL); 
			}
		}
	}
}

// This function handles the receiving of values from the cli queue and updates the CLI display
static void vCLITask(void * parameters) {
	//initial message 
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
  CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
	CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
	const char* statusWin = "\r\nElevator on floor: 3\r\nYou are on floor: 1\r\n";	//initial placement 	
	uint16_t statusWinLength = strlen(statusWin);
	uint8_t* dataSt = (uint8_t*)statusWin; //send the new frequency
	CLI_Transmit(dataSt, statusWinLength);
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
	CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
	
	CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	//move the cursor to the message window	
	CLI_Transmit(signalPanel, sizeof(signalPanel)); //send opening message to the screen
	
	uint8_t charReceived;
	
	for (;;) {
		//if the user has typed in their CLI
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
