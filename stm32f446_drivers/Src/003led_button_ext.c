/*
 * 003led_button_ext.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Panospyr
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f446xx_gpio_driver.h"

void delay(void)
{
	for(int i = 0; i < 250000; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GpioBtn; //PA5, PC13

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
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOB,1);
	GPIO_Init(&GpioBtn);


	while(1)
	{
		if(!GPIO_ReadInputPin(GPIOB,GPIO_PIN_NO_12))
		{
			delay();
			GPIO_ToggleOutputPin(GPIOC,GPIO_PIN_NO_10);
		}
	}
	return 0;
}
