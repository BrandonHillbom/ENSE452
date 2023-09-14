#include <stdint.h>

void delay(uint32_t delay); // A delay loop which blocks while a value is decremented to 0
void led_IO_init (void); // Configuration for PA5 as a push pull output with peripheral clocks for port A and port C turned on
void led_flash(void); // Use the delay routine to toggle the state of the Green LED on PA5
