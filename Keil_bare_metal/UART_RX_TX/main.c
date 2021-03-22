

#include "stm32l4xx.h"                  // Device header
#include <stdio.h>

void USART2_Init(void);

int main(void)
{
	int n;
	char str[100];
	USART2_Init();
	
	printf("Hello from the other side\n");
	fprintf(stdout, " test for stdout\r\n");
	fprintf(stderr, " test for stderr\r\n");
	
	while(1)
	{
		printf("How old are you ?");
		scanf("%d", &n);
		printf("Your age is : %d\r\n",n);
		printf("Enter your first name :");
		gets(str);
		printf("I like your style; ");
		puts(str);
		printf("\r\n");
	}
}

void USART2_Init(void)
{
	RCC->AHB2ENR |= 1;
	RCC->APB1ENR1 |= 0x20000;
	
	GPIOA->AFR[0] |= 0x7700; //Configure PA2 as USART2 TX and configure PA3 as USART2 RX
	GPIOA->MODER = 0x00A0; //Enable alternate function for PA2, PA3
	
	USART2->BRR = 0x1A1; //9600 baud @4Mhz
	USART2->CR1 = 0x000C; //Enable Tx, Rx
	USART2->CR1 |= 1; //Enable USART2
}

int USART2_write(int ch)
{
	//Wait for Tx buffer empty
	while(!(USART2->ISR & 0x0080)){}
	USART2->TDR = (ch & 0xFF);
	return ch;
}

int USART2_read(void)
{
	while(!(USART2->ISR & 0x0020)){}
	return USART2->RDR;
}


struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr ={2};


int fgetc(FILE *f)
{
	int c;
	c = USART2_read();
	if(c == '\r'){
		USART2_write(c);
		c = '\n';
	}
	USART2_write(c);
}


int fputc(int c, FILE *f)
{
	return USART2_write(c);
}
