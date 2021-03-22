#include "stm32l4xx.h"                  // Device header

void delayMs(int delay);
void LED_play(int value);
char USART2_read(void);
void USART2_Init(void);

int main(void)
{
	RCC->AHB2ENR |= 1;    //Enable Clock to Port A
	GPIOA->MODER = 0x00000000; //Initialize GPIOA MODER register to all 0(by default all F)
	GPIOA->MODER |= 0x400; //Set PA5 as output
	USART2_Init();
	while(1)
	{
		char ch;
		ch = USART2_read();
		LED_play(ch);
	}
}

void USART2_Init(void)
{
	RCC->AHB2ENR |=1;        //Enable GPIOA Clock
	RCC->APB1ENR1 |=0x20000; //Enable USART2 Clock
	
	//Configure PA3 as USART2 RX
	GPIOA->AFR[0] |= 0x7000; //Alternate function 7 for USART2
	GPIOA->MODER |= 0x0080;   //Enable alternate function at PA3
	
	//Configure USART2
	USART2->BRR = 0x23;      //115200 baudrate @4Mhz
	USART2->CR1 = 0x0004;    //Enable RX
	USART2->CR1 |= 1;        //Enable USART2
}

char USART2_read(void)
{
	while(!(USART2->ISR & 0x0020)){} //wait until char arrives
	return USART2->RDR;
}

void LED_play(int value)
{
	value %=16;
	for(; value>0;value--)
	{
		GPIOA->BSRR = 0x20;   //Turn on LED
		delayMs(50);
		GPIOA->BSRR = 0x200000;  //Turn off LED
		delayMs(50);
	}
	delayMs(200);
}

void delayMs(int delay){
	int i = 0;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++);
	}
}
