#include "usart.h"

void serial_open(void) {
	  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //usart2 clock 
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN; //GPIO port a
	 
	 GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); //PA2 as AFIO (TX)
	 GPIOA->CRL |= (GPIO_CRL_MODE2 | GPIO_CRL_CNF2_1);
	 
	 GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3); //PA3 as input float (RX)
	GPIOA->CRL |= (GPIO_CRL_CNF3_1);
	
	//pin PA5 as a general-purpose output push-pull
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
	GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	 
	 USART2->BRR = (19 << 4) | (9 & 0xf); //baud rate set for 115200
	 
	 USART2->CR1 &= ~(USART_CR1_M); //8 data bits
	 USART2->CR1 &= ~(USART_CR1_PCE); //no parity
	 USART2->CR2 &= ~(USART_CR2_STOP); // 1 stop bit
	
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // enable
	 
 }
 
/**
Undo whatever serial_open() did, setting the peripheral back to
its reset configuration.
2
*/
void serial_close(void) {
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);

	//USART2->CR1 &= ~USART_CR1_UE; // Disable USART2
	
}

/**
Send an 8-bit byte to the serial port, using the configured
bit-rate, # of bits, etc. Returns 0 on success and non-zero on
failure.
@param b the 8-bit quantity to be sent.
@pre must have already called serial_open()
*/
int send_byte(uint8_t b) {
	 while (!(USART2->SR & USART_SR_TXE)) {
				}
 
        // Transmit character
    USART2->DR = b & 0xff;
		while (!(USART2->SR & USART_SR_TC)) {}
	return 1;
}

/**
Gets an 8-bit character from the serial port, and returns it.
@pre must have already called serial_open()
*/
uint8_t get_byte(void) {
	  // Wait until a character is received
    while (!(USART2->SR & USART_SR_RXNE)) {}

    // Return received data
    return USART2->DR & 0xff;
}
