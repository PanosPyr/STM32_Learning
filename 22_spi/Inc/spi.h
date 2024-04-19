/*
 * spi.h
 *
 *  Created on: Jan 18, 2024
 *      Author: Panospyr
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "stm32f4xx.h"

void cs_enable(void);
void cs_disable(void);
void spi_gpio_init(void);
void spi1_config(void);
void spi1_transmit(uint8_t *data, uint32_t size);
void spi1_receive(uint8_t *data, uint32_t size);

#endif /* SPI_H_ */