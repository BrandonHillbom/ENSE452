//Brandon Hillbom
//SID: 200426340
//September 14,2023
//LAB 1: Flash green led on and off in 1 second intervals

 #include "stm32f10x.h"
 #include "util.h"
 
 int main() {

	 led_IO_init ();
	 while (1) {
		led_flash();
	 }
 }
 


