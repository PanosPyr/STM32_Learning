/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Mar 29, 2024
 *      Author: Panospyr
 */

#include "stm32f446xx_spi_driver.h"

/*
 * Peripheral Clock setup
 */
void SPI_PeriClockControl(SPI_TypeDef *pSPIx, uint8_t En)
{
	if(En)
	{
		if(pSPIx == SPI1) RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		else if(pSPIx == SPI2) RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
		else if(pSPIx == SPI3) RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
		else if(pSPIx == SPI4) RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	}
	else
	{
		if(pSPIx == SPI1)
		{
			RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
			RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
		}
		else if(pSPIx == SPI2)
		{
			RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;
			RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
		}
		else if(pSPIx == SPI3)
		{
			RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;
			RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;
		}
		else if(pSPIx == SPI4)
		{
			RCC->APB2RSTR |= RCC_APB2RSTR_SPI4RST;
			RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI4RST;
		}
	}
}

/*
 * Init and De-init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Configure SPI_CR1 reg

	uint32_t tempreg = 0;

	//Enable preipheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, 1);

	//Config device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	//Config bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//clear BIDIMODE
		tempreg &= ~(1<<15);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//set BIDIMODE
		tempreg |= (1<<15);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//clear BIDIMODE
		tempreg &= ~(1<<15);
		//set RXONLY
		tempreg |= (1<<10);
	}
	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;

	//4.  Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;

	//5. configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

	//6 . configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << 9;

	pSPIHandle->pSPIx->CR1 = tempreg;
}

void SPI_DeInit(SPI_TypeDef *pSPIx)
{
	if(pSPIx == SPI1)
	{
		RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
		RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	}
	else if(pSPIx == SPI2)
	{
		RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
	}
	else if(pSPIx == SPI3)
	{
		RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST;
		RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;
	}
	else if(pSPIx == SPI4)
	{
		RCC->APB2RSTR |= RCC_APB2RSTR_SPI4RST;
		RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI4RST;
	}
}

/*
 * Data read and write
 */
//This is blocking call due to whiles
void SPI_Send(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
	while(len > 0)
	{
		//Wait until TXE is set
		while(!(pSPIx->SR & (1 << 1)));

		//Check DFF
		if((pSPIx->CR1 & (1 << 11)))
		{
			//16-bit DFF
			//load data to DR reg
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			len--;
			len--;
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			//8-bit DFF
			pSPIx->DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}

void SPI_Receive(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{

}

/*
 * IRQ Configuration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t En)
{

}

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{

}

void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

}

void SPI_PeripheralControl(SPI_TypeDef *pSPIx, uint8_t En)
{
	if(En) pSPIx->CR1 |= (1 << 6);
	else pSPIx->CR1 &= ~(1 << 6);
}

void SPI_SSIConfig(SPI_TypeDef *pSPIx, uint8_t En)
{
	if(En) pSPIx->CR1 |= (1 << 8);
	else pSPIx->CR1 &= ~(1 << 8);
}

void SPI_SSOEConfig(SPI_TypeDef *pSPIx, uint8_t En)
{
	if(En) pSPIx->CR2 |= (1 << 2);
	else pSPIx->CR2 &= ~(1 << 2);
}
