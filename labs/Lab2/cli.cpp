#include "cli.h"

using namespace std;

void CLI_Transmit(uint8_t *pData, uint16_t Size) {
	for (uint16_t i = 0; i < Size; i++) {
			send_byte(pData[i]); //send each byte in the message
	}
}

void CLI_Receive(uint8_t *pData, uint16_t Size) {
	string message = ""; //keep track of the typed message
	CLI_Transmit(pData, Size); //transmit initial message
	
	while (1) { //while program has not been terminated
		uint8_t input = get_byte(); //wait for user input
		if (input == 0x08 || input == 0x7f) { //if the input is the backspace or delete char
			if (!message.empty()) { //if there is a message to delete chars
				message.pop_back();
				uint8_t deleteChar = 0x7f; //delete ascii
				CLI_Transmit(&deleteChar, 1); //send delete char to USART
			}
		} else {
			CLI_Transmit(&input, 1); //send the input char
			
			if (input != '\r') { //if the input is not a return then add it to the message
				message.push_back(input);
			}
			if (input == '\r')	//enter, check command
			{
				if (message == "on") {
					GPIOA->BSRR |= GPIO_BSRR_BS5; //turn led on
					const char* str = "\r\nLED has been turned on."; 
					uint16_t length = static_cast<uint16_t>(strlen(str));
					uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str));

					CLI_Transmit(data, length);
				} 
				else if (message == "off") {
					GPIOA->BSRR |= GPIO_BSRR_BR5; //turn led off
					const char* str = "\r\nLED has been turned off."; 
					uint16_t length = static_cast<uint16_t>(strlen(str));
					uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str));
					CLI_Transmit(data, length);
				} else if (message == "help") {
					
					const char* str = "\r\nSelect an option: \r\nTurn the led on by typing 'on' \r\nTurn the LED off by typing 'off'\r\nExit the program by typing 'exit'"; 
					uint16_t length = static_cast<uint16_t>(strlen(str));
					uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str));
						CLI_Transmit(data, length);
				} else if (message == "exit") {
						const char* str = "\r\nProgram terminated..."; 
						uint16_t length = static_cast<uint16_t>(strlen(str));
						uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str));
						CLI_Transmit(data, length);
						break;
				}
				else {
					const char* str = "\r\nUnknown command. Nothing done."; 
					uint16_t length = static_cast<uint16_t>(strlen(str));
					uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str));
					CLI_Transmit(data, length);
				}
				message = ""; //after return clear the message
				CLI_Transmit(pData, Size); //resend the initial message
			}
			
		}
			
	}
}
