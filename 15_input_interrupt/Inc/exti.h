/*
 * exti.h
 *
 *  Created on: Jan 16, 2024
 *      Author: Panospyr
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"

#define LINE13					(1U<<13)

void pc13_exti_init(void);

#endif /* EXTI_H_ */
