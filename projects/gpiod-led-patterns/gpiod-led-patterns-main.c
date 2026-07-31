#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __IO volatile

#define PERIPH_BASE_ADDR (0x40000000UL)

#define AHB1_BASE_ADDR  (PERIPH_BASE_ADDR + 0x20000UL)      // AHB1 bus domain base addr

#define GPIOD_BASE_ADDR (AHB1_BASE_ADDR + 0x0C00UL)			//GPIO_D Base addr

#define RCC_BASE_ADDR   (AHB1_BASE_ADDR + 0x3800UL)			//RCC base addr

typedef struct
{
	__IO  uint32_t MODER;
	__IO  uint32_t OTYPER;
	__IO  uint32_t OSPEEDR;
	__IO  uint32_t PUPDR;
	__IO  uint32_t IDR;
	__IO  uint32_t ODR;

}GPIO_TypeDef_t;

#define GPIOD ( (GPIO_TypeDef_t*) (GPIOD_BASE_ADDR) )

#endif /* INC_STM32F407XX_H_ */


int main(void)
{
    volatile uint32_t *pRCC = (volatile uint32_t*) (RCC_BASE_ADDR+0x030);
    *pRCC |= (1<<3);

	GPIOD->MODER &=~(3<<26);// sırasıyla 13 12 14 15 . pinlerdeki ledler yanacak.
	GPIOD->MODER &=~(3<<24);
	GPIOD->MODER &=~(3<<28);
	GPIOD->MODER &=~(3<<30);

	GPIOD->MODER |= (1<<26);
	GPIOD->MODER |= (1<<24);
	GPIOD->MODER |= (1<<28);
	GPIOD->MODER |= (1<<30);

	while(1){

    for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR |= (1<<13);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR &= ~(1<<13);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR |= (1<<12);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR &= ~(1<<12);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR |= (1<<14);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR &= ~(1<<14);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR |= (1<<15);

	for(volatile int i=0 ; i<500000 ; i++);

	GPIOD->ODR &= ~(1<<15);

	for(volatile int i =0 ; i<4;i++){

		for(volatile int i=0 ; i<500000 ; i++);
		    GPIOD->ODR |= (1<<13);
			GPIOD->ODR |= (1<<12);
			GPIOD->ODR |= (1<<14);
			GPIOD->ODR |= (1<<15);

			for(volatile int i=0 ; i<500000 ; i++);

			GPIOD->ODR &= ~(1<<13);
			GPIOD->ODR &= ~(1<<12);
			GPIOD->ODR &= ~(1<<14);
			GPIOD->ODR &= ~(1<<15);

	}

	}

}
