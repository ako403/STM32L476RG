#include "stm32l4xx.h"                  // Device header

int timeStamp=0;

int main(void)
{
	RCC->AHB2ENR |=1;
	GPIOA->MODER =0x800;         //Set PA5 as alternate function
	GPIOA->AFR[0] |=0x00100000;  //Set pin to AF1 for TIM2 CH1
	
	RCC->APB1ENR1 |=1;           //Enable TIM2 clock
	TIM2->PSC = 400-1;
	TIM2->ARR = 10000-1;
	TIM2->CCMR1 = 0x330;         //Set output to toggle on match
	TIM2->CCR1 =0;               //Set match value
	TIM2->CCER =1;              //Enable CH1 compare mode
	TIM2->CNT =0;                //Clear counter
	TIM2->CR1 =1;                //Enable TIM2
	
	//Configure PA6 as input of TIM3 CH1
	RCC->AHB2ENR |=1;            //Not needed second time(doing it second time just in case wanna copy and paste...)
	GPIOA->MODER |=0x2000;       //Set PA6 to alternate function
	GPIOA->AFR[0] |= 0x02000000; //Set pin to AF2 for TIM3 CH1
	
	
	//Config TIM3
	RCC->APB1ENR1 |=2;           //Enable TIM3 clock
	TIM3->PSC = 4000;            //Divide by 4000
	TIM3->CCMR1 = 0x41;          //Set CH1 to capture at every edge(BIT 0 also sets CH1 as input)
	TIM3->CCER =1;               //Enable capture on rising edge
	TIM3->CR1 =1;                //Enable TIM3
	
	while(1){
		while(!(TIM3->SR &2)){}
	timeStamp = TIM3->CCR1;
	}
}
