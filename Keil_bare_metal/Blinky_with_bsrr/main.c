#include "stm32l4xx.h"                  // Device header


void delayMs(int delay);

int main(void)
{
	RCC->AHB2ENR |= 1;
	
	GPIOA->MODER = 0x400;
	
	while(1)
	{
		GPIOA->BSRR = 0x20; //Set PA5 high
		delayMs(50);
		
		GPIOA->BSRR = 0x00200000; //Set PA5 low //0b 0000 0000 0010 0000 0000 0000 0000 0000
		delayMs(50);
	}
}
//Taking into account 16Mhz system clock

void delayMs(int delay)
{
	int i;
	for(; delay>0; delay--){
		for(i=0; i<3195; i++);
	}
}
