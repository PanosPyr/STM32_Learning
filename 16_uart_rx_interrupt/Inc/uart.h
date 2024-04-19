/*
 * uart.h
 *
 *  Created on: Jan 11, 2024
 *      Author: Panospyr
 */

#ifndef UART_H_
#define UART_H_

#define SR_RXNE				(1U<<5)

void uart2_tx_init(void);
char uart2_read(void);
void uart2_tx_rx_init(void);
void uart2_tx_rx_interrupt_init(void);

#endif /* UART_H_ */
