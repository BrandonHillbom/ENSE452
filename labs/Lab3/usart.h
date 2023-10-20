#include <stdint.h>
#include "string.h"
#include <stm32f10x.h>

void usart2_open(void);
void serial_open(void);
void serial_close(void);
void USART2_IRQHandler(void);
void enable_interrupt(void);

int sendbyte(uint8_t b);
uint8_t getbyte(void);
