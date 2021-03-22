//PA5
//AHB2
//RCC->AHB2ENR
//GPIOx_MODER
//GPIOx_ODR

#include "stm32l4xx.h"                  // Device header

void delayMs(int delay);

int main(void)
{
	RCC->AHB2ENR |= 1;                    // enable GPIOA clock, 00000000000000000000000000000001
	
	GPIOA->MODER = 0x400;                // set PA5 as output, 0b 0000 0000 0000 0000 0000 0100 0000 0000, default value //0xABFFF7FF?
		
	while(1){
			
		GPIOA->ODR = 0x20;                  // set PA5 output data register 1, 0b .... 0010 0000
		delayMs(100);
		GPIOA->ODR &=~ 0x20;
		delayMs(100);
	}
}
//Taking into account 16Mhz system clock

void delayMs(int delay)
{
	int i;
	for(; delay>0; delay--){
		for(i = 0; i<3195; i++);
	}
}
