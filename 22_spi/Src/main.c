#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "i2c.h"
#include "spi.h"

#define PIN5				(1U<<5)

#define LED_PIN				(PIN5)

#define HISR_TCIF6			(1U<<21)
#define HIFCR_CTCIF6		(1U<<21)

char key;

int main(void){

	char message[31] = "Hello from STM32 DMA transfer\n\r";

	uart2_tx_init();
	dma1_stream6_init((uint32_t) message, (uint32_t)&USART2->DR, sizeof(message));

	/* Set PIN5 as output pin */
	GPIOA->MODER |= (1U<<10); //Set bit 10 to 1
	GPIOA->MODER &= ~(1U<<11); //Set bit 11 to 0

	while(1){}
}

void dma_cb(void){
	GPIOA->ODR |=  LED_PIN;
}

void DMA1_Stream6_IRQHandler(void){
	if(DMA1->HISR & HISR_TCIF6){
		DMA1->HIFCR |= HIFCR_CTCIF6;
		dma_cb();
	}
}
