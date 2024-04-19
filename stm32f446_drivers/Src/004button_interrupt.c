/*
 * 004button_interrupt.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Panospyr
 */

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f446xx_gpio_driver.h"


IRQn_Type Exti12IrqNumber = EXTI15_10_IRQn;

void delay(void)
{
	for(int i = 0; i < 250000; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn; //PA5, PC13
	memset(&GpioLed, 0, sizeof(GpioLed));
	memset(&GpioBtn, 0, sizeof(GpioBtn));

	GpioLed.pGPIOx = GPIOC;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOC,1);
	GPIO_Init(&GpioLed);

	GpioBtn.pGPIOx = GPIOB;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOB,1);
	GPIO_Init(&GpioBtn);

	//IRQ Config
	GPIO_IRQPriorityConfig(Exti12IrqNumber, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(Exti12IrqNumber, 1);

	while(1);

	return 0;
}

void EXTI15_10_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_12);
	GPIO_ToggleOutputPin(GPIOC,GPIO_PIN_NO_10);
}
