/*
 * uart.c
 *
 *  Created on: Jan 11, 2024
 *      Author: Panospyr
 */
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOAEN					(1U<<0)
#define UART2EN					(1U<<17)
#define CR1_TE					(1U<<3)
#define CR1_RE					(1U<<2)
#define CR1_UE					(1U<<13)
#define CR1_RXNEIE				(1U<<5)
#define UART2_CR3_DMAT			(1U<<7)

#define SR_TXE					(1U<<7)


#define SYS_FREQ 				16000000 //16 MHz
#define APB1_CLK				SYS_FREQ

#define UART_BAUDRATE			115200

#define DMA1EN					(1U<<21)
#define DMA_CR_EN				(1U<<0)
#define CHSEL4					(1U<<27)
#define DMA_MEM_INC				(1U<<10)
#define DMA_DIR_MEM_TO_PERIPH	(1U<<6)
#define DMA_CR_TCIE				(1U<<4)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_tx_init(void);
void uart2_write(int ch);


int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len){
	RCC->AHB1ENR |= DMA1EN; //Enable clock access to dma1
	DMA1_Stream6->CR &= ~DMA_CR_EN; //Disable DMA1 stream 6
	while(DMA1_Stream6->CR & DMA_CR_EN){} //Wait until DMA1 stream 6 is disabled
	DMA1->HIFCR |= (1U<<16); //Clean all interrupt flags of stream 6
	DMA1->HIFCR |= (1U<<18);
	DMA1->HIFCR |= (1U<<19);
	DMA1->HIFCR |= (1U<<20);
	DMA1->HIFCR |= (1U<<21);
	DMA1_Stream6->PAR = dst; //Set the destination buffer
	DMA1_Stream6->M0AR = src; //Set the source buffer
	DMA1_Stream6->NDTR = len; //Set length
	DMA1_Stream6->CR = CHSEL4; //Select stream 6 CH4
	DMA1_Stream6->CR |= DMA_MEM_INC; //Enable memory increment
	DMA1_Stream6->CR |=  DMA_DIR_MEM_TO_PERIPH; //Configure transfer direction (memory -> peripheral)
	DMA1_Stream6->CR |=  DMA_CR_TCIE; //Enable DMA transfer complete interrupt
	DMA1_Stream6->FCR = 0; //Disable FIFO mode and enable direct mode
	DMA1_Stream6->CR |= DMA_CR_EN; //Enable dma1 stream 6
	USART2->CR3 = UART2_CR3_DMAT; //Enable UART2 transmitter DMA
	NVIC_EnableIRQ(DMA1_Stream6_IRQn); //Enable DMA1_Stream6 interrupt in NVIC
}

void uart2_tx_rx_init(void){
	RCC->AHB1ENR |= GPIOAEN; //Enable Clock access to GPIOA

	GPIOA->MODER &= ~(1U<<4); //Set PA2 mode to alternate function mode
	GPIOA->MODER |= (1U<<5);
	GPIOA->AFR[0] |= (1U<<8); //Set PA2 alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	GPIOA->MODER &= ~(1U<<6); //Set PA3 mode to alternate function mode
	GPIOA->MODER |= (1U<<7);
	GPIOA->AFR[0] |= (1U<<12); //Set PA3 alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	RCC->APB1ENR = UART2EN; //Enable Clock access to UART2

	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE); //Configure baudrate

	USART2->CR1 = (CR1_TE | CR1_RE); //Configure transfer direction (Tx/Rx)
	USART2->CR1 |= CR1_UE; //Enable UART module

}

void uart2_tx_init(void){
	RCC->AHB1ENR |= GPIOAEN; //Enable Clock access to GPIOA
	GPIOA->MODER &= ~(1U<<4); //Set PA2 mode to alternate function mode
	GPIOA->MODER |= (1U<<5);
	GPIOA->AFR[0] |= (1U<<8); //Set PA2 alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	RCC->APB1ENR = UART2EN; //Enable Clock access to UART2

	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE); //Configure baudrate

	USART2->CR1 = CR1_TE; //Configure transfer direction (Tx)
	USART2->CR1 |= CR1_UE; //Enable UART module

}

void uart2_tx_rx_interrupt_init(void){
	RCC->AHB1ENR |= GPIOAEN; //Enable Clock access to GPIOA

	GPIOA->MODER &= ~(1U<<4); //Set PA2 mode to alternate function mode
	GPIOA->MODER |= (1U<<5);
	GPIOA->AFR[0] |= (1U<<8); //Set PA2 alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	GPIOA->MODER &= ~(1U<<6); //Set PA3 mode to alternate function mode
	GPIOA->MODER |= (1U<<7);
	GPIOA->AFR[0] |= (1U<<12); //Set PA3 alternate function type to UART_RX (AF7)
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	RCC->APB1ENR = UART2EN; //Enable Clock access to UART2

	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE); //Configure baudrate

	USART2->CR1 = (CR1_TE | CR1_RE); //Configure transfer direction (Tx/Rx)
	USART2->CR1 |= CR1_RXNEIE; //Enable Rx interrupt
	NVIC_EnableIRQ(USART2_IRQn); //Enable UART2 interrupt in NVIC
	USART2->CR1 |= CR1_UE; //Enable UART module

}

char uart2_read(void){
	while(!(USART2->SR & SR_RXNE)){}//Make sure the receive data register is not empty

	return USART2->DR;
}

void uart2_write(int ch){

	while(!(USART2->SR & SR_TXE)){} //Make sure the transmit data register is empty

	USART2->DR = (ch & 0xFF); //Transmit data
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

