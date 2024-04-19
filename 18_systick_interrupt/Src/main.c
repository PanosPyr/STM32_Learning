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

	systick1HzInterrupt();

	while(1){}
}

static void systick_cb(void){
	printf("A second passed \n\r");
	GPIOA->ODR ^= LED_PIN;
}

void SysTick_Handler(void){
	systick_cb();
}
