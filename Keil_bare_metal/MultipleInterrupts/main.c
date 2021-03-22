#include "stm32l4xx.h"                  // Device header

void systickDelayMs(int n);

volatile int pb10_counter;
volatile int pc13_counter;

int main(void)
{
	
	__disable_irq();
	
	RCC->AHB2ENR |= 1;
	RCC->AHB2ENR |= 2;
	RCC->AHB2ENR |= 4;
	RCC->APB2ENR |= 1;  //Enable SYSCFG clock
	
	GPIOA->MODER = 0x400;  //PA5 as output
	GPIOB->MODER = 0x00000000;  //PB10 as input
	GPIOC->MODER = 0x00000000;  //PC13 as input
	
	SYSCFG->EXTICR[3] |= 0x0020; //Select Port C for EXT13
	EXTI->IMR1 |= 0x2000;  //Unmask EXTI13
	EXTI->FTSR1 |=0x2000;  //Select falling edge trigger
	
	GPIOB->PUPDR = 0x00100001;  //Enable pull-up resistor for PB10; don't use |(or), won't work
	
	SYSCFG->EXTICR[2] |= 0x0100;  //Select PORTB for EXT10(EXT10 is located in EXTICR 3 register)
	
	EXTI->IMR1 |= 0x0400;  //Unmask EXT10
	EXTI->FTSR1 |= 0x0400;  //Select falling edge trigger
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();
	
	while(1){}
		
}


void EXTI15_10_IRQHandler(void)
{
	//pc13
	if(EXTI->PR1 == 0x2000){
		pc13_counter++;
		
	  GPIOA->BSRR = 0x20;
	  systickDelayMs(100);
	  GPIOA->BSRR = 0x00200000;
	  systickDelayMs(100);
	
	  GPIOA->BSRR = 0x20;
	  systickDelayMs(100);
	  GPIOA->BSRR = 0x00200000;
	  systickDelayMs(100);
		
		EXTI->PR1 = 0x2000;  //Clear interrupt pending flag
	}
	else if(EXTI->PR1 == 0x0400){
		pb10_counter++;
		
	  GPIOA->BSRR = 0x20;
	  systickDelayMs(200);
	  GPIOA->BSRR = 0x00200000;
	  systickDelayMs(200);
	
		GPIOA->BSRR = 0x20;
		systickDelayMs(200);
		GPIOA->BSRR = 0x00200000;
		systickDelayMs(200);
		
		EXTI->PR1 = 0x0400;  //Clear interrupt pending flag
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
