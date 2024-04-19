#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


int timestamp = 0;
//SET UP: Connect with a jumper wire pa5 and pa6
int main(void){

	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1){
		while(!(TIM3->SR & SR_CC1IF)){} //Wait until edge is captured

		timestamp = TIM3->CCR1;
	}
}
