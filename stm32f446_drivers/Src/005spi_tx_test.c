/*
 * 005spi_tx_test.c
 *
 *  Created on: Mar 29, 2024
 *      Author: Panospyr
 */

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"

#define SR_TXE				(1U<<1)
#define SR_BSY				(1U<<7)

/*
 * PB12 --> SPI2_NSS
 * PB13 --> SPI2_SCLK
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * AF --> 5
 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//NSS
	/*SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);*/

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MISO
	/*SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);*/

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2; //generates sclk of 8MHz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;


	SPI_Init(&SPI2Handle);
}

int main(void)
{
	char user_data[] = "Hello World";

	//Init gpio pins to spi
	SPI2_GPIOInits();

	//Init SPI2 periph params
	SPI2_Inits();

	//Set SSI to 1 since SSM is enabled
	SPI_SSIConfig(SPI2, 1);

	SPI_SSOEConfig(SPI2, 1);

	//Enable SPI2 peripheral
	SPI_PeripheralControl(SPI2, 1);

	//Send data
	SPI_Send(SPI2, (uint8_t*)user_data, strlen(user_data));

	uint8_t temp;

	while(!(SPI2->SR & SR_TXE)){}//Wait until TXE is set
	while(SPI2->SR & SR_BSY){}//Wait for BUSY flag to reset
	temp = SPI2->DR; //Weird way to clear OVR fLag
	temp = SPI2->SR;

	//SPI_PeripheralControl(SPI2, 0);

	while(1);

	return 0;
}
