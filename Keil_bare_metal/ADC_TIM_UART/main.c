#include "stm32l4xx.h"                  // Device header
#include <stdio.h>

void USART2_Init(void);
int USART2_write(int ch);
void systickDelayMs(int n);

int data;  //ADC real value
double voltage;
double celcius;

int main(void)
{
	RCC->AHB2ENR |= 1;    //Enable clock to Port A
	
	//TIM2 Config
	/*RCC->APB1ENR1 |= 1;   //Enable clock to TIM2
	TIM2->PSC = 400-1;    //4 000 000 fivided by 400 = 10 000
	TIM2->ARR = 10000-1;  //10 000 divided by 10 000 = 1
	TIM2->CNT = 0;        //Clear timer counter
	TIM2->CCMR1 = 0x6800;
	TIM2->CCER = 0x0010;
	TIM2->CCR2 = 50-1;
	TIM2->CR1 = 1;*/        //Enable TIM2
	
	//ADC Setup(default sampling time is too fast even with 4Mhz internal clock)
	RCC->CCIPR |= 0x30000000;  //Select system clock as ADC clock source
	RCC->AHB2ENR |= 0x2000;  //ADC clock enable
	ADC1->CR = 0x00000000;   //Disable deep-power-down by setting DEEPPWD to 0
	ADC1->CR |= 0x10000000;  //Enable ADC voltage regulator by setting ADVREGEN to 1
	systickDelayMs(10);             //Wait 10 ms to start-up ADC voltage regulator
	
	//ADC calibration(make sure ADCALDIF is in correct state, write 1 when needed calibration in differential inputs mode)
	ADC1->CR |= 0x80000000;  //Start ADC calibration by setting ADCAL to 1
	systickDelayMs(100);     //Wait 100	ms to be sure ADC calibration is finished
	
	ADC1->SMPR2 = 0x00800000;  //Sampling time minimum 10us
	ADC1->SQR1 = 0x440;      //First conversion is ADC1_IN17 because temperature sensor is connected to ADC1_IN17
	
	ADC1->CR |= 1;           //Enable ADC by setting ADEN to 1
	ADC123_COMMON ->CCR |= 0x00C00000;  //Enable temperature sensor
	ADC123_COMMON ->CCR &= ~0x01000000;  //Disable VBAT
	systickDelayMs(10);     //Wait 10	ms to let ADC stabilize
	
	USART2_Init();
	printf("STM32f4 Temperature \r\n");
	
	while(1)
	{
		ADC1->CR |= 0x4;                  //Start ADC conversion
		while(!(ADC1->ISR & 0x4)){}  //Wait for ADC conversion to be complete
		data = ADC1->DR;
		voltage = (double)data/4095*3.3;
		celcius = (voltage - 0.76)/0.0025+25;
		
		printf("%d\r\n",data);
		//printf("%d,%.2f\370C\r\n",data,celcius);
		systickDelayMs(100);
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

struct __FILE{int handle;};
FILE __stdout = {1};
int fputc(int c, FILE *f)
{
	return USART2_write(c);
}

int USART2_write(int ch)
{
	//Wait for Tx buffer empty
	while(!(USART2->ISR & 0x0080)){}
	USART2->TDR = ch;
	return ch;
}

void systickDelayMs(int n)
{
	SysTick->LOAD = 4000; //Reload with number of clocks per millisecond
	SysTick->VAL = 0;     //Clear current value register
	SysTick->CTRL = 0x5;  //Enable Systick
	
	for(int i=0;i<n;i++)
	{
		//Wait until the COUNT flag is set
		while((SysTick->CTRL & 0x10000) ==0){}
	}
	SysTick->CTRL =0;
}
