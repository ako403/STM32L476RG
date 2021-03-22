#include "stm32l4xx.h"                  // Device header

void systickDelayMs(int n);

int main(void)
{
	RCC->AHB2ENR |= 1;
	GPIOA->MODER = 0x400;
	
	while(1)
	{
		systickDelayMs(100);
		GPIOA->ODR ^= 0x20;
	}
}

void systickDelayMs(int n)
{
	SysTick->LOAD = 4000; //Reload with number of clocks per millisecond
	SysTick->VAL = 0;     //Clear current value register
	SysTick->CTRL = 0x5;  //Enable Systick
	
	for(int i =0;i<n;i++)
	{
		//Wait until the COUNT flag is set
		while((SysTick->CTRL & 0x10000) ==0){}
	}
	SysTick->CTRL =0;
}
