#include "stm32l4xx.h"                  // Device header

//Led : PA5
//Button : PC13

void systickDelayMs(int n);

int main(void)
{
	__disable_irq();   //Disable interrupt to configure
	
	RCC->AHB2ENR |=1;  //Enable clock access to PORT A
	RCC->AHB2ENR |=4;  //Enable clock access to PORT C
	RCC->APB2ENR |=1;  //Enable SYSCFG clock
	
	
	GPIOA->MODER = 0x400;  //PA5 as output
	GPIOC->MODER = 0x00000000;  //PC13 as input
	SYSCFG->EXTICR[3] = 0x0020;  //Select port C for EXTI13
	
	EXTI->IMR1|= 0x2000;  //Unmask EXTI13
	EXTI->FTSR1 |= 0x2000;  //Select falling edge trigger
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);  //Enable EXTI15-10
	
	__enable_irq();     //Enable interrupt after configuring

	while(1){}

}

void EXTI15_10_IRQHandler(void)
{
	GPIOA->BSRR = 0x20;
	systickDelayMs(100);
	GPIOA->BSRR = 0x00200000;
	systickDelayMs(100);
	
	GPIOA->BSRR = 0x20;
	systickDelayMs(100);
	GPIOA->BSRR = 0x00200000;
	systickDelayMs(100);
	
	EXTI->PR1 = 0x2000;  //Clear pending interrupt flag
}

void systickDelayMs(int n)
{
	int i = 0;
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
