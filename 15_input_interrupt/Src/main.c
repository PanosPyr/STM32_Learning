#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN				(1U<<0)
#define PIN5				(1U<<5)
#define LED_PIN				(PIN5)

static void exti_cb(void);

int main(void){

	pc13_exti_init();
	uart2_tx_init();

	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1){}
}

static void exti_cb(void){
	printf("BTN Pressed....\n\r");
	GPIOA->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler(void){
	if((EXTI->PR & LINE13) != 0){
		//Clear PR flag
		EXTI->PR |= LINE13;
		//Do something
		exti_cb();
	}

}
