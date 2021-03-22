#include "stm32l4xx.h"                  // Device header
#include <stdio.h>
#include <string.h>

void systickDelayMs(int n);
void USART2_Init(void);
int USART2_write(int ch);
void SystemClock_Config(void);

//PA1 connected to ADC12_IN6(ADC1 and ADC2 share same input)
//tADCVREG_STUP(ADC voltage regulator start-up time = 20µs)

uint16_t analogValue;
int data;  //ADC real value
double voltage;
double celcius;

int main(void)
{
	//HAL_Init();
	//SystemClock_Config();
	
	RCC->AHB2ENR |= 1;       //Port A clock enable
	RCC->CCIPR |= 0x30000000;  //Select system clock as ADC clock source
	GPIOA->MODER = 0xABFFF000; //Initialize GPIOA MODER register to all 0(by default all F)
	GPIOA->MODER |= 0xC;     //Set PA1 to analog state
	GPIOA->ASCR |= 0x2;      //Connect analog switch to the ADC input(PORT A input 1), only necessary on STM32L47x/L48x devices
	
	RCC->AHB2ENR |= 0x2000;  //ADC clock enable
	ADC1->CR = 0x00000000;   //Disable deep-power-down by setting DEEPPWD to 0
	//ADC1->CR |= 0x2;         //ADDIS(ADC disable command); disable the ADC
	//ADC1->SQR1 = 0x180;      //First conversion is ADC1_IN6 because PA1 is connected to ADC1_IN6
	ADC1->CR |= 0x10000000;  //Enable ADC voltage regulator by setting ADVREGEN to 1
	systickDelayMs(10);             //Wait 10 ms to start-up ADC voltage regulator
	
	//ADC calibration(make sure ADCALDIF is in correct state, write 1 when needed calibration in differential inputs mode)
	ADC1->CR |= 0x80000000;  //Start ADC calibration by setting ADCAL to 1
	systickDelayMs(100);     //Wait 100	ms to be sure ADC calibration is finished
	
	//Enable ADC
	//ADC1->ISR |= 1;          //Clear ADC ready by setting ADRDY to 1
	ADC1->CR |= 1;           //Enable ADC by setting ADEN to 1
	systickDelayMs(10);     //Wait 10	ms to let ADC stabilize
	ADC1->SQR1 = 0x180;      //First conversion is ADC1_IN6 because PA1 is connected to ADC1_IN6
	//ADC1->SMPR1 |= 0x80000;
	systickDelayMs(10);
	
	USART2_Init();
	
	while(1)
	{
		ADC1->CR |= 0x4;                  //Start ADC conversion
		while(!(ADC1->ISR & 0x4)){}     //Wait for ADC conversion to be complete
		//systickDelayMs(100);
		analogValue = ADC1->DR;
		//analogValue = 1997;
		systickDelayMs(100);
		//data = ADC1->DR;
		//while(!(USART2->ISR & 0x0080)){}  //Wait for Tx buffer empty
		//USART2->TDR = 0x61;
		printf("STM32f4 Temperature \r\n");
		//data = 0x42d;
		printf("%d\r\n",analogValue);
		//printf("%d\r\n",data);
		systickDelayMs(100);
	}
}


void USART2_Init(void)
{
	RCC->AHB2ENR |= 1;
	RCC->APB1ENR1 |= 0x20000;
	
	GPIOA->AFR[0] |= 0x7700; //Configure PA2 as USART2 TX and configure PA3 as USART2 RX
	GPIOA->MODER |= 0x00A0; //Enable alternate function for PA2, PA3
	
	USART2->BRR = 0x1A1; //9600 baud @4Mhz
	USART2->CR1 = 0x000C; //Enable Tx, Rx
	USART2->CR1 |= 1; //Enable USART2
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
