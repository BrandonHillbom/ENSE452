#include "stm32f10x.h"
#include "usart.h"

QueueHandle_t xCLIQueue;

void serial_open(void){
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//GPIO port A
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable USART2 clock
		
		//USART2 TX PA2 as output and RX PA3 as input
		GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
		GPIOA->CRL |= (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3_1);
		
		//PA5 as a general-purpose output push-pull (LED)
		GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1);
		GPIOA->CRL |= (GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	
		// Set baud rate to 115200 bps for clock at 36mhz
		USART2->BRR = (19 << 4) | (9 & 0xF);

		USART2->CR1 &= ~USART_CR1_M; // 8 data bits
		USART2->CR1 &= ~USART_CR1_PCE; // No parity
		USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit
	
		NVIC_EnableIRQ(USART2_IRQn); //enavle usart in nvic
		NVIC_EnableIRQ(TIM2_IRQn); //enable timer2 in nvic

		USART2->CR1 |= USART_CR1_RXNEIE; //interrupt
		USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; //enable
				
}

void enable_interrupt(void) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //enable timer 2 clock

		TIM2->PSC = 36000; // 1 ms delay
		TIM2->ARR = 50000; // 1 second delay

		TIM2->DIER |= TIM_DIER_UIE; //update interrupt
		TIM2->CR1 |= TIM_CR1_CEN;//enable timer
}

int sendbyte(uint8_t b) {
	
    TIM2->SR &= ~TIM_SR_UIF;  // Clear update flag
	
    TIM2->CR1 |= TIM_CR1_CEN; //start timer
		TIM2->CNT = 0; // Reset count

    while (!(USART2->SR & USART_SR_TXE)) {
        // Check for timeout
        if (TIM2->SR & TIM_SR_UIF) {
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
        }
    }

    USART2->DR = b & 0xFF; // Load the character to be transmitted into the data register

    while (!(USART2->SR & USART_SR_TC)) { //transmission
        // if timeout
        if (TIM2->SR & TIM_SR_UIF) {
            TIM2->SR &= ~TIM_SR_UIF;  // Clear the update flag
            TIM2->CR1 &= ~TIM_CR1_CEN;  // Stop the timer
            return 1;  // Timeout error
        }
    }
    // Stop timer
    TIM2->CR1 &= ~TIM_CR1_CEN;
		TIM2->CNT = 0; // Reset the timer counter

    return 0; // Success
}

void serial_close(void)
{
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz).
	// Configure USART2 settings
	USART2->CR1 &= ~USART_CR1_UE; // Disable USART2

}
// USART2 interrupt service routine
void USART2_IRQHandler(void) {
	uint8_t characterReceived;
    if (USART2->SR & USART_SR_RXNE) {
        // USART2 received data interrupt

        // Read the received data from USART2_DR register
      characterReceived = USART2->DR;
			xQueueSendToFrontFromISR(xCLIQueue, &characterReceived, NULL); //queue, *pvItemToQueue, *pxHigherPriorityTaskWoken
//when its not from isr, third param should be TickType_t ticksToWait
		

        // Clear the RXNE flag (optional, but recommended)
        USART2->SR &= ~USART_SR_RXNE;
    }

}