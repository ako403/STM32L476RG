//PA2 USART2_TX
//PA3 USART2_RX

#include "stm32l4xx.h"                  // Device header

void LED_play(int value);
void systickDelayMs(int n);
void UART2_Init(void);


int main(void)
{
	__disable_irq();  //disable global interrupt for configuring
	
	RCC->AHB2ENR |= 1;  //Enable PA clock for LED(PA5)
	GPIOA->MODER = 0x00000000; //Initialize GPIOA MODER register to all 0(by default all F)
	GPIOA->MODER |= 0x400;  //PA5 as output
	
	UART2_Init();
	USART2->CR1 |= 0X0020; //RXNEIE, RX interrupt enable
	NVIC_EnableIRQ(USART2_IRQn);
	__enable_irq();  //enable global interrupt
	
	while(1){
	}
}


void UART2_Init(void)
{
	RCC->AHB2ENR |= 1; //PA clock enable for UART
	RCC->APB1ENR1 |= 0x20000;  //Enable UART2 clock
	
	GPIOA->AFR[0] |= 0x7000;  //Configure PA3 as USART2 RX
	GPIOA->MODER |= 0x0080;  //PA3 alternate function
	
	//Configure USART2
	USART2->BRR = 0x23;      //115200 baudrate @4Mhz
	USART2->CR1 = 0x0004;    //Enable RX
	USART2->CR1 |= 1;        //Enable USART2
	
}

void LED_play(int value)
{
	value %=16;
	for(; value>0;value--)
	{
		GPIOA->BSRR = 0x20;   //Turn on LED
		systickDelayMs(50);
		GPIOA->BSRR = 0x200000;  //Turn off LED
		systickDelayMs(50);
	}
	systickDelayMs(200);
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

void USART2_IRQHandler(void)
{
	char c;
	if(USART2->ISR & 0x0020)
	{
		c = USART2->RDR;
		LED_play(c);
	}
}
