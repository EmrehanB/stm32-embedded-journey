
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{

	    uint32_t *pClckCtrlReg=(uint32_t*) 0x40023830;
		uint32_t *pPortDModeReg=(uint32_t*) 0x40020C00;
		uint32_t *pPortDOutReg=(uint32_t*) 0x40020C14;



	    *pClckCtrlReg=*pClckCtrlReg | (1<<3);


	    *pPortDModeReg &=~(1<<24);
	    *pPortDModeReg &=~(1<<25);
	    *pPortDModeReg |=(1<<24);

	while(1){

	    *pPortDOutReg|= (1<<12);

	    for(uint32_t i =0;i<1000000;i++); //delay Şu anda 16MHz çalıştığından saniyede 16milyon işlem yapıyor biraz uzun bir delay lazım.
	                                      //Gözle görmek için

	    //şimdi ld4 kapatmak için GPIOD nin output reg ini 0 yapsam yeterli.

	    *pPortDOutReg&=~(1<<12);

	    for(uint32_t i =0;i<1000000;i++);

   }


}
