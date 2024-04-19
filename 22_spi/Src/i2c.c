/*
 * i2c.c
 *
 *  Created on: Jan 18, 2024
 *      Author: Panospyr
 */
#include "stm32f4xx.h"
#include "i2c.h"

#define GPIOBEN					(1U<<1)
#define I2C1EN					(1U<<21)
#define MODER_PB8_AF			((1U<<17) & ~(1U<<16))
#define MODER_PB9_AF			((1U<<19) & ~(1U<<18))
#define AFR_PB8_AF4				((1U<<2) & ~(1U<<0) & ~(1U<<1) & ~(1U<<3))
#define AFR_PB9_AF4				((1U<<6) & ~(1U<<4) & ~(1U<<5) & ~(1U<<7))
#define OTYPER_PB8				(1U<<8)
#define OTYPER_PB9				(1U<<9)
#define PUPDR_PB8				((1U<<16) & ~(1U<<17))
#define PUPDR_PB9				((1U<<18) & ~(1U<<19))
#define I2C_CR1_PE				(1U<<0)
#define I2C_CR1_SWRST			(1U<<15)
#define I2C_CR2_FREQ			16 //(~(1U<<0) & ~(1U<<1) & ~(1U<<2) & ~(1U<<3) & (1U<<4) & ~(1U<<5)) //16Mhz
#define I2C_100KHZ				80
#define SD_MODE_MAX_RISE_TIME	17
#define I2C_SR2_BUSY			(1U<<1)
#define I2C_CR1_START			(1U<<8)
#define I2C_SR1_SB				(1U<<0)
#define I2C_SR1_ADDR			(1U<<1)
#define I2C_SR1_TXE				(1U<<7)
#define I2C_CR1_ACK				(1U<<10)
#define I2C_CR1_STOP			(1U<<9)
#define I2C_SR1_RXNE			(1U<<6)
#define I2C_SR1_BTF				(1U<<2)

/*
 * Pinout: 	PB8 -> SCL
 * 			PB9 -> SDA
 */

void i2c_init(void){
	RCC->AHB1ENR |= GPIOBEN; //Enable clock access to GPIOB
	GPIOB->MODER |= MODER_PB8_AF; //Set alternate mode and function AF4 to PB8 and PB9
	GPIOB->MODER |= MODER_PB9_AF;
	GPIOB->AFR[1] |= AFR_PB8_AF4;
	GPIOB->AFR[1] |= AFR_PB9_AF4;
	GPIOB->OTYPER |= OTYPER_PB8; //Set output type of PB8 and PB9 to open drain
	GPIOB->OTYPER |= OTYPER_PB9;
	GPIOB->PUPDR |= PUPDR_PB8; //Enable pull-up for PB8 and PB9
	GPIOB->PUPDR |= PUPDR_PB9;

	RCC->AHB1ENR |= I2C1EN; //Enable clock access to I2C1
	I2C1->CR1 |= I2C_CR1_SWRST; //Enter reset mode
	I2C1->CR1 &= ~I2C_CR1_SWRST; //Come out of reset mode
	I2C1->CR2 |= I2C_CR2_FREQ; //Set I2C frequency
	I2C1->CCR |= I2C_100KHZ; //SCL frequency
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME; //Set rise time
	I2C1->CR1 |= I2C_CR1_PE; //Enable I2C
}

void i2c1_byte_read(char saddr, char maddr, char* data){
	volatile int tmp;

	while(I2C1->SR2 & I2C_SR2_BUSY){} //Wait until bus is not busy
	I2C1->CR1 |= I2C_CR1_START; //Generate start condition
	while(!(I2C1->SR1 & I2C_SR1_SB)){} //Wait until start flag is set
	I2C1->DR = saddr << 1; //Transmit slave address + Write
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){} //Wait until address flag is set
	tmp = I2C1->SR1; //Clear address flag
	tmp = I2C1->SR2;
	while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
	I2C1->DR = maddr; //Send memory address
	while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
	I2C1->CR1 |= I2C_CR1_START; //Generate restart
	while(!(I2C1->SR1 & I2C_SR1_SB)){} //Wait until start flag is set
	I2C1->DR = saddr << 1 | 1; //Send address + Read
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){} //Wait until address flag is set
	I2C1->CR1 &= ~I2C_CR1_ACK; //Disable ACK
	tmp = I2C1->SR1; //Clear address flag
	tmp = I2C1->SR2;
	I2C1->CR1 |= I2C_CR1_STOP; //Generate stop after data received
	while(!(I2C1->SR1 & I2C_SR1_RXNE)){} //Wait until RXNE flag is set
	*data++ = I2C1->DR; //Read data from the data register
}

void i2c1_burst_read(char saddr, char maddr, int n, char* data){
	volatile int tmp;

	while(I2C1->SR2 & I2C_SR2_BUSY){} //Wait until bus is not busy
	I2C1->CR1 |= I2C_CR1_START; //Generate start condition
	while(!(I2C1->SR1 & I2C_SR1_SB)){} //Wait until start flag is set
	I2C1->DR = saddr << 1; //Transmit slave address + Write
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){} //Wait until address flag is set
	tmp = I2C1->SR1; //Clear address flag
	tmp = I2C1->SR2;
	while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
	I2C1->DR = maddr; //Send memory address
	while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
	I2C1->CR1 |= I2C_CR1_START; //Generate restart
	while(!(I2C1->SR1 & I2C_SR1_SB)){} //Wait until start flag is set
	I2C1->DR = saddr << 1 | 1; //Send address + Read
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){} //Wait until address flag is set
	tmp = I2C1->SR1; //Clear address flag
	tmp = I2C1->SR2;
	I2C1->CR1 |= I2C_CR1_ACK; //Enable ACK
	while(n > 0U){
		//If one byte
		if(n == 1U){
			I2C1->CR1 &= ~I2C_CR1_ACK; //Disable ACK
			I2C1->CR1 |= I2C_CR1_STOP; //Generate stop after data received
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){} //Wait until RXNE flag is set
			*data++ = I2C1->DR; //Read data from the data register
			break;
		}
		else{
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){} //Wait until RXNE flag is set
			*data++ = I2C1->DR; //Read data from the data register
			n--;
		}
	}
}

void i2c1_burst_write(char saddr, char maddr, int n, char* data){
	volatile int tmp;

	while(I2C1->SR2 & I2C_SR2_BUSY){} //Wait until bus is not busy
	I2C1->CR1 |= I2C_CR1_START; //Generate start condition
	while(!(I2C1->SR1 & I2C_SR1_SB)){} //Wait until start flag is set
	I2C1->DR = saddr << 1; //Transmit slave address + Write
	while(!(I2C1->SR1 & I2C_SR1_ADDR)){} //Wait until address flag is set
	tmp = I2C1->SR1; //Clear address flag
	tmp = I2C1->SR2;
	while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
	I2C1->DR = maddr; //Send memory address

	for(int i = 0; i < n; i++){
		while(!(I2C1->SR1 & I2C_SR1_TXE)){} //Wait until data register is empty
		I2C1->DR = *data++;
	}
	while(!(I2C1->SR1 & I2C_SR1_BTF)){} //Wait until transfer is finished
	I2C1->CR1 |= I2C_CR1_STOP; //Generate stop
}
