#include "stm32l4xx.h"                  // Device header

//Configure systick interrupt to toggle LED
// at 1Hz, system clock is 4Mhz

int main(void)
{
	__disable_irq();
	RCC->AHB2ENR |= 1;
	GPIOA->MODER = 0x400;
	
	SysTick->LOAD = 4000000 -1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;
	
	__enable_irq();
	
	while(1){}
	
}

void SysTick_Handler(void)
{
	GPIOA->ODR ^= 0x20;
}
