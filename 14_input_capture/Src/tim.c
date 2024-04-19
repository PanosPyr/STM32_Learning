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
#define TIM3EN 				(1U<<1)
#define CR1_CEN 			(1U<<0)
#define CCMR1_TOGGLE 		(((1U<<4) | (1U<<5)) & ~(1U<<6))
#define CCER_CC1E 			(1U<<0)
#define AFR5_TIM2_CH1		((1U<<20) & ~(1U<<21) & ~(1U<<22) & ~(1U<<23))
#define AFR6_TIM3_CH1		((1U<<25) & ~(1U<<24) & ~(1U<<26) & ~(1U<<27))
#define CCER_CC1S			((1U<<0) & ~(1U<<1))

void tim2_1hz_init(void){
	RCC->APB1ENR |= TIM2EN; //Enable clock access to tim2

	TIM2->PSC = 1600 - 1; //Set prescaler value //16 000 000 / 1 600 = 10 000
	TIM2->ARR = 10000 - 1; //Set Auto-Reload value //10 000 / 10 000 = 1
	TIM2->CNT = 0; //Clear counter
	TIM2->CR1 |= CR1_CEN; //Enable timer
}


void tim2_pa5_output_compare(void){

	if((RCC->AHB1ENR & GPIOAEN) != GPIOAEN){
		RCC->AHB1ENR |= GPIOAEN; //Enable Clock access to GPIOA
	}

	GPIOA->MODER &= ~(1U<<10); //Set PA5 mode to alternate function mode
	GPIOA->MODER |= (1U<<11);

	GPIOA->AFR[0] |= AFR5_TIM2_CH1; //Set PA5 alternate function type to TIM2_CH1 (AF1)

	RCC->APB1ENR |= TIM2EN; //Enable clock access to tim2
	TIM2->PSC = 1600 - 1; //Set prescaler value //16 000 000 / 1 600 = 10 000
	TIM2->ARR = 10000 - 1; //Set Auto-Reload value //10 000 / 10 000 = 1
	TIM2->CCMR1 = CCMR1_TOGGLE; //Set output compare toggle mode
	TIM2->CCER |= CCER_CC1E;//Enable TIM2 CH1 in compare mode
	TIM2->CNT = 0; //Clear counter
	TIM2->CR1 |= CR1_CEN; //Enable timer
}

void tim3_pa6_input_capture(void){

	if((RCC->AHB1ENR & GPIOAEN) != GPIOAEN){
		RCC->AHB1ENR |= GPIOAEN; //Enable Clock access to GPIOA
	}

	GPIOA->MODER &= ~(1U<<12); //Set PA5 mode to alternate function mode
	GPIOA->MODER |= (1U<<13);

	GPIOA->AFR[0] |= AFR6_TIM3_CH1; //Set PA6 alternate function type to TIM3_CH1 (AF1)

	RCC->APB1ENR |= TIM3EN; //Enable clock access to tim3
	TIM3->PSC = 16000 - 1; //Set prescaler value //16 000 000 / 16 000
	TIM3->CCMR1 = CCER_CC1S; //Set CH1 to input capture
	TIM3->CCER |= CCER_CC1E; //Set CH1 to capture at rising edge
	TIM3->CR1 |= CR1_CEN; //Enable timer
}

