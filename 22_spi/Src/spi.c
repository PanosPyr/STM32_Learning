/*
 * spi.c
 *
 *  Created on: Jan 18, 2024
 *      Author: Panospyr
 */

#include "spi.h"

#define SPI1ENR				(1U<<12)
#define GPIOAENR			(1U<<0)
#define SR_TXE				(1U<<1)
#define SR_BSY				(1U<<7)
#define SR_RXNE				(1U<<0)

/*
 * PA5 -> SCK
 * PA6 -> MISO
 * PA7 -> MOSI
 * PA9 -> SS (Slave Select)
 */

void spi_gpio_init(void){
	RCC->AHB1ENR |= GPIOAENR; //Enable access to GPIOA
	GPIOA->MODER &= ~(1U<<10); //Set PA5 to alternate mode
	GPIOA->MODER |= (1U<<11);
	GPIOA->MODER &= ~(1U<<12); //Set PA6 to alternate mode
	GPIOA->MODER |= (1U<<13);
	GPIOA->MODER &= ~(1U<<14); //Set PA7 to alternate mode
	GPIOA->MODER |= (1U<<15);
	GPIOA->MODER |= (1U<<18); //Set PA9 to output mode
	GPIOA->MODER &= ~(1U<<19);
	GPIOA->AFR[0] |=(1U<<20); //Set PA5 alternate function type to SPI1
	GPIOA->AFR[0] &= ~(1U<<21);
	GPIOA->AFR[0] |=(1U<<22);
	GPIOA->AFR[0] &= ~(1U<<23);
	GPIOA->AFR[0] |=(1U<<24); //Set PA6 alternate function type to SPI1
	GPIOA->AFR[0] &= ~(1U<<25);
	GPIOA->AFR[0] |=(1U<<26);
	GPIOA->AFR[0] &= ~(1U<<27);
	GPIOA->AFR[0] |=(1U<<28); //Set PA7 alternate function type to SPI1
	GPIOA->AFR[0] &= ~(1U<<29);
	GPIOA->AFR[0] |=(1U<<30);
	GPIOA->AFR[0] &= ~(1U<<31);
}

void spi1_config(void){
	RCC->APB2ENR |= SPI1ENR; //Enable access to SPI1
	SPI1->CR1 |= (1U<<3); //Set clock (BaudRate) to fPCLK/4
	SPI1->CR1 &= ~(1U<<4);
	SPI1->CR1 &= ~(1U<<5);
	SPI1->CR1 |= (1U<<0); //Set CPHA to 1
	SPI1->CR1 |= (1U<<1); //Set CPOL to 1
	SPI1->CR1 &= ~(1U<<10); //Enable full-duplex mode
	SPI1->CR1 &= ~(1U<<7); //MSB transmitted first
	SPI1->CR1 |= (1U<<2); //Set STM32 as master
	SPI1->CR1 &= ~(1U<<11); //Set 8-bit data mode
	SPI1->CR1 |= (1U<<9); //Enable slave management
	SPI1->CR1 |= (1U<<8);
	SPI1->CR1 |= (1U<<6); //Enable SPI1
}

void spi1_transmit(uint8_t *data, uint32_t size){
	uint32_t i=0;
	uint8_t temp;

	while(i<size){
		while(!(SPI1->SR & SR_TXE)){} //Wait until TXE is set
		SPI1->DR = *data++; //Write the data to the data register
		i++;
	}

	while(!(SPI1->SR & SR_TXE)){}//Wait until TXE is set
	while(SPI1->SR & SR_BSY){}//Wait for BUSY flag to reset
	temp = SPI1->DR; //Weird way to clear OVR fLag
	temp = SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t size){
	while(size){
		SPI1->DR = 0; //Send dummy data
		while(!((SPI1->SR & SR_RXNE))){} //Wait for the receive buffer to be full
		*data++ = SPI1->DR; //Get data from the data register
		size--;
	}
}

void cs_enable(void){
	GPIOA->ODR &= ~(1U<<9);
}

//Pull high to disable
void cs_disable(void){
	GPIOA->ODR |= (1U<<9);
}
