#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"


uint32_t sensor_value;
int main(void){

	uart2_tx_init();
	pa1_adc_interrupt_init();
	start_convertion();

	while(1){}
}

static void adc1_cb(void){
	sensor_value = ADC1->DR;
	printf("Sensor Value: %d \n\r",(int)sensor_value);

}

void ADC_IRQHandler(void){
	if(ADC1->SR & SR_EOC){
		ADC1->SR &= ~SR_EOC;
		adc1_cb();
	}

}
