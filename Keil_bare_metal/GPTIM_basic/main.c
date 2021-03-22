#include "stm32l4xx.h"                  // Device header

int main(void)
{
	RCC->AHB2ENR |= 1;
	GPIOA->MODER = 0x400;
	
	//TIM2 Config
	RCC->APB1ENR1 |= 1;   //Enable clock to TIM2
	TIM2->PSC = 400-1;    //4 000 000 fivided by 400 = 10 000
	TIM2->ARR = 10000-1;  //10 000 divided by 10 000 = 1
	TIM2->CNT = 0;        //Clear timer counter
	TIM2->CR1 = 1;        //Enable TIM2
	
	while(1)
	{
		//Wait until timer flag is set
		while(!(TIM2->SR & 1)){} //When timer flag is NOT set do nothing
		TIM2->SR &=~1;           //Toggle led when timer flag is set
		GPIOA->ODR ^= 0x20;
	}
}
