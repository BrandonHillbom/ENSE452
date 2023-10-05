//Brandon Hillbom
//SID: 200426340
//October 5, 2023
//Lab2 ENSE452

//NOTE: compile with c++11/c11, default compiler 6, and optimization set as -O0 in target

/*
 This lab allows you to turn on an LED and turn it off by interacting 
 with a command line interface. 
*/

 #include "stm32f10x.h"
 #include "cli.h"
 
using namespace std; 

 int main() {
	serial_open();

	const char* str = "\r\nEnter command. Type help for options:"; 
	uint16_t length = static_cast<uint16_t>(strlen(str)); // get length of string
	uint8_t* data = reinterpret_cast<uint8_t*>(const_cast<char*>(str)); //convert from char to pointer
	 
	CLI_Receive(data, length); //enter receive function
	serial_close(); //close the connection
	
	return 0;
 }
