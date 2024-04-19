/*
 * systick.c
 *
 *  Created on: Jan 12, 2024
 *      Author: Panospyr
 */
#include "stm32f4xx.h"
#include "systick.h"

#define SYSTICK_LOAD_VAL				16000 //Since sys clock is 16MHz, 16000 is 1 ms
#define CTRL_ENABLE						(1U<<0)
#define CTRL_CLKSRC						(1U<<2)
#define CTRL_COUNTFLAG					(1U<<16)



void systickDelayMs(int delay){
	//Reload with number of clocks per ms
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	//Clear systick current value register
	SysTick->VAL = 0;

	//Enable systick and select internal clock source
	SysTick->CTRL |= CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++){
		//Wait until countflag is set
		while(!(SysTick->CTRL & CTRL_COUNTFLAG)){
		}
	}

	SysTick->CTRL = 0;
}

