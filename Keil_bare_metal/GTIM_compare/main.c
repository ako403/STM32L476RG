#include "stm32l4xx.h"                  // Device header

//Toggle LED at 1 Hz using TIM2 Compare mode.
int main(void)
{
	RCC->AHB2ENR |=1;
	GPIOA->MODER = 0x800;      //Set PA5 to Alternate function
	GPIOA->AFR[0] |=0x00100000; //Set pin AF1 for TIM2 CH1(PA5 corresponds to TIM2_CH1)
	
	RCC->APB1ENR1 |=1; //Enable TIM2 clock
	TIM2->PSC = 400-1;
	TIM2->ARR = 10000-1;
	TIM2->CCMR1 = 0x30; //Set output to toggle on match
	TIM2->CCR1 =0;      //Set match mode
	TIM2->CCER |=1;     //Enable CH1 compare mode
	TIM2->CNT =0;       //Clear the counter
	TIM2->CR1 =1;       //Enable TIM2
	
	while(1){}
	
}
