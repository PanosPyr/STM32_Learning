/*
 * adc.c
 *
 *  Created on: Jan 12, 2024
 *      Author: Panospyr
 */
#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOAEN				(1U<<0)
#define ADC1EN				(1U<<8)
#define ADC_CH1				(1U<<0)
#define ADC_SEQ_LEN_1		(0x00)
#define CR2_ADON			(1U<<0)
#define CR2_SWSTART			(1U<<30)
#define SR_EOC				(1U<<1)



void pa1_adc_init(void){
	//Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//Set PA1 mode to analog
	GPIOA->MODER |= (1U<<2);
	GPIOA->MODER |= (1U<<3);

	//Enable clock access to ADC1
	RCC->APB2ENR |= ADC1EN;

	//Conversion sequence start
	ADC1->SQR3 |= ADC_CH1;

	//Conversion sequence length
	ADC1->SQR1 = ADC_SEQ_LEN_1; //all register bit are set to zero but we don't care since we don't use any other bits
	//or
	//ADC1->SQR1 &= ~(1U<<20);
	//ADC1->SQR1 &= ~(1U<<21);
	//ADC1->SQR1 &= ~(1U<<22);
	//ADC1->SQR1 &= ~(1U<<23);

	//Enable ACD1 module
	ADC1->CR2 |= CR2_ADON;
}

void start_convertion(void){
	//Start ADC1 conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void){
	//Wait for conversion to be complete
	while(!(ADC1->SR & SR_EOC)){}

	//Read results
	return ADC1->DR;

}
