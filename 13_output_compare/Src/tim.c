/*
 * tim.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Panospyr
 */

/*
 *
 * Update Event = SYSCLK / (PSC - 1) * (ARR - 1)
 *
 */


#include "stm32f4xx.h"
#include "tim.h"

#define GPIOAEN				(1U<<0)
#define TIM2EN 				(1U<<0)
#define CR1_CEN 			(1U<<0)
#define CCMR1_TOGGLE 		(((1U<<4) | (1U<<5)) & ~(1U<<6))
#define CCER_CCIE 			(1U<<0)
#define AFR5_TIM2_CH1		((1U<<20) & ~(1U<<21) & ~(1U<<22) & ~(1U<<23))

void tim2_1hz_init(void){
	RCC->APB1ENR |= TIM2EN; //Enable clock access to tim2

	TIM2->PSC = 1600 - 1; //Set prescaler value //16 000 000 / 1 600 = 10 000
	TIM2->ARR = 10000 - 1; //Set Auto-Reload value //10 000 / 10 000 = 1
	TIM2->CNT = 0; //Clear counter
	TIM2->CR1 |= CR1_CEN; //Enable timer
}


void tim2_pa5_output_compare(void){

	RCC->AHB1ENR = GPIOAEN; //Enable Clock access to GPIOA

	GPIOA->MODER &= ~(1U<<10); //Set PA5 mode to alternate function mode
	GPIOA->MODER |= (1U<<11);

	GPIOA->AFR[0] |= AFR5_TIM2_CH1; //Set PA5 alternate function type to TIM2_CH1 (AF1)

	RCC->APB1ENR |= TIM2EN; //Enable clock access to tim2
	TIM2->PSC = 1600 - 1; //Set prescaler value //16 000 000 / 1 600 = 10 000
	TIM2->ARR = 10000 - 1; //Set Auto-Reload value //10 000 / 10 000 = 1
	TIM2->CCMR1 = CCMR1_TOGGLE; //Set output compare toggle mode
	TIM2->CCER |= CCER_CCIE;//Enable TIM2 CH1 in compare mode
	TIM2->CNT = 0; //Clear counter
	TIM2->CR1 |= CR1_CEN; //Enable timer
}

