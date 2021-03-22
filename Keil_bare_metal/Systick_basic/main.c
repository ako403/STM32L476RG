#include "stm32l4xx.h"                  // Device header

int main(void)
{
	RCC->AHB2ENR |=1;
	GPIOA->MODER = 0x400;
	
	//Configure Systick(200ms)
	SysTick->LOAD = 800000 - 1; // 4Mhz*0.2s = 800000
	SysTick->VAL = 0;
	SysTick->CTRL = 5;
	
	while(1)
	{
		//Check if count flag is set
		if(SysTick->CTRL & 0x10000){
			GPIOA->ODR ^= 0x20; //Toggle LED
		}
	}
}
