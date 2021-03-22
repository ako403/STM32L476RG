//APB1 BIT 17 :USART2
//USART2 TX connected to PA2
//default clock is 4 Mhz

#include "stm32l4xx.h"                  // Device header

void USART2_Init(void);
void USART_write(int ch);
void delayMs(int delay);

int main(void)
{
	USART2_Init();
	
	while(1){
		USART_write('H');
		USART_write('i');
		USART_write('\r');
		USART_write('\n');
		delayMs(100);
	}
}


void USART2_Init(void){
	
	RCC->APB1ENR1 |= 0x20000;  // enable USART2 clock(bit 17)
	RCC->AHB2ENR |= 1; //enable port A clock
	GPIOA->AFR[0] = 0x0700; // enable port A pin number 2 alternate function USART2(AF7 = 0111)
	GPIOA->MODER = 0x0020; //Set PA2 to alternate function
	
	//Initialize UART
	USART2->BRR = 0x1A1; //9600 bps @4 Mhz // 9600 bps @8 Mhz = 0x341 //9600 bps @16 Mhz
	USART2->CR1 = 0x0008; //Enable Tx
	//First initialize UART then Enable UART(recommended)
	USART2->CR1 |= 1; //Enable USART2
	
}

void USART_write(int ch){
	// wait while TX buffer is empty
	while(!(USART2->ISR & 0x0080)){}
		USART2->TDR = (ch & 0xFF);
}

void delayMs(int delay){
	int i;
	for(;delay>0;delay--){
		for(i=0;i<3195;i++);
	}
}
