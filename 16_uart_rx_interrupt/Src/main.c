#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"

#define PIN5				(1U<<5)

#define LED_PIN				(PIN5)

char key;

int main(void){

	uart2_tx_rx_interrupt_init();

	/* Set PIN5 as output pin */
	GPIOA->MODER |= (1U<<10); //Set bit 10 to 1
	GPIOA->MODER &= ~(1U<<11); //Set bit 11 to 0

	while(1){}
}

void uart_cb(void){
	key = USART2->DR;
	if(key == '1'){
		GPIOA->ODR |= LED_PIN;
	}
	else if(key == '2'){
		GPIOA->ODR &= ~LED_PIN;
	}
}

void USART2_IRQHandler(void){
	//Check if RXNE is set
	if(USART2->SR & SR_RXNE){
		uart_cb();
	}
}
