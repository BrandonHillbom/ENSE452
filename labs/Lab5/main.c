/*
	ENSE 452 Lab5
	Brandon Hillbom
	200426340
	This lab modifies the previous labs to implement FreeRTOS instead of using global variables. 
	The user can modify the frequency of the blinking light on the onboard STM32F103RB Microcontroller.
*/

#include "cli.h"
#include <stdint.h>

#define mainBLINKY_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

static void vBlinkTask(void * parameters);
static void vCLITask(void * parameters);

extern QueueHandle_t xCLIQueue;
extern QueueHandle_t xFrequencyQueue;
extern char message[20]; // Declare message to be arbitrary size


int main()
{
	serial_open();
	enable_interrupt();
	
	xCLIQueue = xQueueCreate(1, sizeof(char));
	xFrequencyQueue = xQueueCreate(1, sizeof(int)); //queue length, item size
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE, NULL, mainBLINKY_TASK_PRIORITY, NULL);
	xTaskCreate(vCLITask, "CLI", configMINIMAL_STACK_SIZE, NULL, mainCLI_TASK_PRIORITY, NULL);
	vTaskStartScheduler(); // start the scheduler
	
	return 0;
}

static void vBlinkTask(void * parameters) {
int frequency = 1000;
	for (;;) {
		BaseType_t status = xQueueReceive(xFrequencyQueue, &frequency, 100); 
		if( uxQueueMessagesWaiting( xFrequencyQueue ) != 0 )		
		{			
			xQueueReceive(xFrequencyQueue, &frequency, 100);
		}	
		GPIOA->BSRR |= GPIO_BSRR_BS5; //turn led on
		vTaskDelay((TickType_t)frequency);
		GPIOA->BSRR |= GPIO_BSRR_BR5; //turn led off
		vTaskDelay((TickType_t)frequency);
		
	}
}

static void vCLITask(void * parameters) {
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION));	//set the scroll window
  CLI_Transmit(ANSI_CLEAR_SCREEN, sizeof(ANSI_CLEAR_SCREEN));	//clear the screen 
	CLI_Transmit(ANSI_MOVE_CURSOR_TOP, sizeof(ANSI_MOVE_CURSOR_TOP));	//move cursor to the status window
	const char* statusWin = "\r\nFrequency of flashing light: 1 second\r\n";		
	uint16_t statusWinLength = strlen(statusWin);
	uint8_t* dataSt = (uint8_t*)statusWin; //send the new frequency
	CLI_Transmit(dataSt, statusWinLength);
	CLI_Transmit(ANSI_SCROLL_REGION, sizeof(ANSI_SCROLL_REGION)); //scroll
	CLI_Transmit(ANSI_RESTORE_CURSOR, sizeof(ANSI_RESTORE_CURSOR)); //move cursor back to messgae 
	
	CLI_Transmit(ANSI_MOVE_CURSOR_MIDDLE, sizeof(ANSI_MOVE_CURSOR_MIDDLE));	//move the cursor to the message window
	const char* str = "\r\nSelect an option: \r\nChange frequency of blinking light to 0.2 seconds by typing 'f1' \r\nChange frequency of blinking light to 1 second by typing 'f2' \r\nChange frequency of blinking light to 5 seconds by typing 'f3' \r\n"; 
	uint16_t length = (uint16_t)strlen(str);
	uint8_t* array = (uint8_t*)str;	
	CLI_Transmit(array, length); //send opening message to the screen
	
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