

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



#include "main.h"

int main(void)

{

	volatile RCC_AHB1ENR_t *pRCCenableReg=(RCC_AHB1ENR_t*) 0x40023830;
	volatile GPIOx_MODE_t *pPortD_ModerReg= (GPIOx_MODE_t *) 0x40020C00;
	volatile GPIOx_ODR_t *pPortD_ODRreg=( GPIOx_ODR_t *) 0x40020C14;




	//LD4 için GPIOD nin 12. pini ile ilgileniyoruz.


	pRCCenableReg->gpio_d_en=1;
	pPortD_ModerReg->pin_12=1;

	while(1){

		pPortD_ODRreg->pin_12=1;

		for(int i=0;i<1000000;i++);

		pPortD_ODRreg->pin_12=0;

		for(int i=0;i<1000000;i++);
	}

	for(;;);
}
