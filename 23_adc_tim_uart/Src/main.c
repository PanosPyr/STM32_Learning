#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define PIN5				(1U<<5)
#define LED_PIN				(PIN5)

#define V25 0.76
#define AVG_SLOPE 2.5
// Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
//V25 = 0.76
//Avg_Slope = 2.5
uint16_t sensor_value, cpu_temp;
float  VSENSE;
int main(void){

	uart2_tx_init();


	adc_temp_sensor_init();

	tim2_1hz_interrupt_init();

	while(1){}
}

static void tim2_cb(void){
	start_convertion();
}

void TIM2_IRQHandler(void){
		TIM2->SR &= ~SR_UIF; //Clear UIF
		tim2_cb();
}

static void adc1_cb(void){
	sensor_value = ADC1->DR;
	VSENSE = sensor_value/4095 * 3.3;
	cpu_temp = (uint16_t)(((VSENSE - V25) / AVG_SLOPE) + 25);
	printf("CPU TEMP: %d \n\r",cpu_temp);
}

void ADC_IRQHandler(void){
	if(ADC1->SR & SR_EOC){
		adc1_cb();
	}
}
