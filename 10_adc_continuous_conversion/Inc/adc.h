/*
 * adc.h
 *
 *  Created on: Jan 12, 2024
 *      Author: Panospyr
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void pa1_adc_init(void);
void start_convertion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
