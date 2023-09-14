 #include "stm32f10x.h"
 #include "util.h"

void delay(uint32_t delay)
{
	volatile  uint32_t i;
  	 for (i = 0; i < delay; i++)
		{
		//delay
		}
}

//**************************** I/O ************************************************************

void led_IO_init (void)
{
 
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;

    //Set the config and mode bits for  
		GPIOA->CRL |= GPIO_CRL_MODE5;
    GPIOA->CRL &= ~GPIO_CRL_CNF5;//Port A bit 5 so it will be a push-pull output (up to 50 MHz)
   
}	
	
void led_flash(void)
{			
			GPIOA->BSRR |= GPIO_BSRR_BS5;
			delay(7200000);
	    GPIOA->BRR |= GPIO_BRR_BR5;
			delay(7200000);
}
