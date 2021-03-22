#include "stm32l4xx.h"                  // Device header

//Create TIM2 to toggle LED at 1Hz
//System clock is 16Mhz

int main(void)
{
	__disable_irq();
	RCC->AHB2ENR |= 1;
	GPIOA->MODER = 0x400;
	
	//TIM2
	RCC->APB1ENR1 |= 1;
	TIM2->PSC = 4000-1;
	TIM2->ARR = 1000-1;
	TIM2->CR1 = 1;
	TIM2->DIER |= 1;  //Update interrupt enable(UIE)
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	__enable_irq();
	
	while(1){}

}

void TIM2_IRQHandler(void)
{
	TIM2->SR = 0;  //Clear update interrupt flag(UIF)
	GPIOA->ODR ^= 0x20;
}
