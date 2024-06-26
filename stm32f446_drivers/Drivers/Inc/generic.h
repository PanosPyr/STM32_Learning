/*
 * generic.h
 *
 *  Created on: Mar 28, 2024
 *      Author: Panospyr
 */

#ifndef INC_GENERIC_H_
#define INC_GENERIC_H_

#include "stm32f4xx.h"

#define GPIO_BASEADDR_TO_CODE(x)	( (x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 :\
									(x == GPIOG) ? 6 :\
									(x == GPIOH) ? 7 :0 )

#define NVIC_ISER0	(volatile uint32_t*)0xE000E100
#define NVIC_ISER1	(volatile uint32_t*)0xE000E104
#define NVIC_ISER2	(volatile uint32_t*)0xE000E108
#define NVIC_ISER3	(volatile uint32_t*)0xE000E10C

#define NVIC_ICER0	(volatile uint32_t*)0xE000E180
#define NVIC_ICER1	(volatile uint32_t*)0xE000E184
#define NVIC_ICER2	(volatile uint32_t*)0xE000E188
#define NVIC_ICER3	(volatile uint32_t*)0xE000E18C

#define NVIC_ICER0	(volatile uint32_t*)0xE000E180
#define NVIC_ICER1	(volatile uint32_t*)0xE000E184
#define NVIC_ICER2	(volatile uint32_t*)0xE000E188
#define NVIC_ICER3	(volatile uint32_t*)0xE000E18C

#define NVIC_IPRX	(volatile uint32_t*)0xE000E400

/*
 * macros for all the possible priority levels
 */
#define NVIC_IRQ_PRI0    0
#define NVIC_IRQ_PRI15    15

#endif /* INC_GENERIC_H_ */
