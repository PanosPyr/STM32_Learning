/*
 * uart.c
 *
 *  Created on: Jan 11, 2024
 *      Author: Panospyr
 */
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOAEN				(1U<<0)
#define UART2EN				(1U<<17)
#define CR1_TE				(1U<<3)
#define CR1_RE				(1U<<2)
#define CR1_UE				(1U<<13)

#define SR_TXE				(1U<<7)
#define SR_RXNE				(1U<<5)


#define SYS_FREQ 			16000000 //16 MHz
#define APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_write(int ch);


int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

void uart2_tx_rx_init(void){
	RCC->AHB1ENR = GPIOAEN; //Enable Clock access to GPIOA

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

