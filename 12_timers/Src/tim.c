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

#define TIM2EN 			(1U<<0)
#define CR1_CEN 		(1U<<0)

void tim2_1hz_init(void){
	RCC->APB1ENR |= TIM2EN; //Enable clock access to tim2

	TIM2->PSC = 1600 - 1; //Set prescaler value //16 000 000 / 1 600 = 10 000
	TIM2->ARR = 10000 - 1; //Set Auto-Reload value //10 000 / 10 000 = 1
	TIM2->CNT = 0; //Clear counter
	TIM2->CR1 |= CR1_CEN; //Enable timer
}
