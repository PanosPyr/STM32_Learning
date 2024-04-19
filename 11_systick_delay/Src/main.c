#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define PIN5				(1U<<5)
#define LED_PIN				(PIN5)


int main(void){

	uart2_tx_init();

	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1){
		printf("Blink LED \n\r");
		GPIOA->ODR ^= LED_PIN;
		systickDelayMs(2000);
	}
}
