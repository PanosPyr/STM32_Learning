//Where is the LED connected?
//Port: A
//Pin:  5
#include "stdint.h"

#define PERIPH_BASE 		(0x40000000UL)
#define AHB1PERIPH_OFFSET 	(0x00020000UL)
#define AHB1PERIPH_BASE 	(PERIPH_BASE + AHB1PERIPH_OFFSET)

#define GPIOA_OFFSET 		(0x00000000UL)
#define GPIOA_BASE			(AHB1PERIPH_BASE + GPIOA_OFFSET)

//RCC - Reseting Clock Control
#define RCC_OFFSET			(0x00003800UL)
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET)

#define GPIOAEN				(1U<<0)

#define PIN5				(1U<<5)

#define LED_PIN				(PIN5)

/* For Pin 5
 * (1U<<10) //Set bit 10 to 1
 * &=~(1U<<11) //Set bit 11 to 0
 */

typedef struct{
	volatile uint32_t MODER;	/* GPIO port mode register,						Address Offset: 0x00		*/
	volatile uint32_t OTYPER;	/* GPIO port output type register,				Address offset: 0x04		*/
	volatile uint32_t OSPEEDR;	/* GPIO port output speed register,				Address offset: 0x08		*/
	volatile uint32_t PUPDR;	/* GPIO port pull-up/pull-down register,		Address offset: 0x0C		*/
	volatile uint32_t IDR;		/* GPIO port input data register,				Address offset: 0x10		*/
	volatile uint32_t ODR;		/* GPIO port output data register,				Address offset: 0x14		*/
	volatile uint32_t BSRR;		/* GPIO port bit set/reset register,			Address offset: 0x18		*/
	volatile uint32_t LCKR;		/* GPIO port configuration lock register,		Address offset: 0x1C		*/
	volatile uint32_t AFR[2];	/* GPIO alternate function low/high register,	Address offset: 0x20/0x24	*/
}GPIO_TypeDef;

typedef struct
{
  volatile uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  volatile uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  volatile uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
  volatile uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  volatile uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  volatile uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  volatile uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint32_t      	RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  volatile uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  volatile uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint32_t      	RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  volatile uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  volatile uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      	RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  volatile uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  volatile uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint32_t      	RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  volatile uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  volatile uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  volatile uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint32_t      	RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  volatile uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  volatile uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t      	RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  volatile uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  volatile uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint32_t      	RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  volatile uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  volatile uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  uint32_t      	RESERVED7[1];  /*!< Reserved, 0x88                                                                    */
  volatile uint32_t DCKCFGR;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
} RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)
#define GPIOA ((GPIO_TypeDef*)GPIOA_BASE)

int main(void){

	/* Enable clock access to GPIOA */
	//RCC_AHB1EN_R |= GPIOAEN;
	RCC->AHB1ENR |= GPIOAEN;

	/* Set PIN5 as output pin */
	//GPIOA_MODE_R |= (1U<<10); //Set bit 10 to 1
	//GPIOA_MODE_R &= ~(1U<<11); //Set bit 11 to 0
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &= ~(1U<<11);

	while(1){
		/* Set PIN5 high */
		//GPIOA->ODR |= LED_PIN;

		/* Toggle PIN5 */
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++){
		}
	}

}
