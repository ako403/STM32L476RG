#include "stm32l4xx.h"                  // Device header

int main(void)
{
	RCC->AHB2ENR |= 1;  //Enable port A
	RCC->AHB2ENR |= 4;  //Enable port C
	
	GPIOA->MODER = 0x400; //Set PA5 as output
	GPIOC->MODER = 0x00000000; //Set PC13 output, default tundub olevat FFFFFFFF seega võib ka kirjutada 0xF3FFFFFF 
	
	while(1){
		if(GPIOC->IDR & 0x2000) //if PC13 is high
		{	
			GPIOA->BSRR = 0x002; //Set PA5 low(turn off led)
		}
		else
		{
			GPIOA->BSRR = 0x20; //Set PA5 high(turn on led)
		}	
	}
}
