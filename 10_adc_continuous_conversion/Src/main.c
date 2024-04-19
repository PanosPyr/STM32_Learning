#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"


uint32_t sensor_value;
int main(void){

	uart2_tx_init();
	pa1_adc_init();
	start_convertion();

	while(1){
		sensor_value = adc_read();
		printf("Sensor Value: %d \n\r",(int)sensor_value);
	}
}
