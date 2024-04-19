#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define PIN5				(1U<<5)
#define LED_PIN				(PIN5)


int main(void){

	uart2_tx_init();

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	tim2_1hz_init();

	while(1){

		while(!(TIM2->SR & SR_UIF)){} //Wait for UIF
		TIM2->SR &= ~SR_UIF; //Clear UIF

		printf("Blink LED \n\r");
		GPIOA->ODR ^= LED_PIN;
	}
}
