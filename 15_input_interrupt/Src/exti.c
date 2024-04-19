/*
 * exti.c
 *
 *  Created on: Jan 16, 2024
 *      Author: Panospyr
 */

#include "stm32f4xx.h"
#include "exti.h"

#define GPIOCEN						(1U<<2)
#define SYSCFGEN					(1U<<14)

void pc13_exti_init(void){
	__disable_irq();//Disable global interrupts (not necessary but good practice)
	RCC->AHB1ENR |= GPIOCEN; //Enable clock access to GPIOC
	GPIOC->MODER &= ~(1U<<26); //Set PC13 as input
	GPIOC->MODER &= ~(1U<<27);
	RCC->APB2ENR |= SYSCFGEN; //Enable clock access to SYSCFG
	SYSCFG->EXTICR[3] |= (1U<<5); //Select PORTC for EXTI13
	EXTI->IMR |= (1U<<13); //Unmask EXTI13
	EXTI->FTSR |= (1U<<13);//Select falling edge trigger
	NVIC_EnableIRQ(EXTI15_10_IRQn);//Enable EXTI line in NVIC
	__enable_irq();//Enable global interrupts
}
