/*
 * tim.h
 *
 *  Created on: Jan 15, 2024
 *      Author: Panospyr
 */

#ifndef TIM_H_
#define TIM_H_

#define SR_UIF			(1U<<0)

void tim2_1hz_init(void);
void tim2_1hz_interrupt_init(void);

#endif /* TIM_H_ */
