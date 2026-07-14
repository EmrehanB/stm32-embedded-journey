
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{

	    uint32_t volatile *pClckCtrlReg=(uint32_t*) 0x40023830;
		uint32_t volatile *pPortDModeReg=(uint32_t*) 0x40020C00;
		uint32_t volatile *pPortDOutReg=(uint32_t*) 0x40020C14;



	    *pClckCtrlReg=*pClckCtrlReg | (1<<3);
	    *pClckCtrlReg=*pClckCtrlReg | (1<<0); //GPIOA ye clock vermek için


	    *pPortDModeReg &=~(1<<24);
	    *pPortDModeReg &=~(1<<25); // Bu iki satır *pPortDModeReg &=~(3<<24); şeklinde tek satırda da yapılabilirdi.
	    *pPortDModeReg |=(1<<24);


	    //GPIOA adresi:  0x40020000

	    uint32_t volatile *pPortAModeReg=(uint32_t*) 0x40020000;
	    uint32_t volatile *pPortAInReg=(uint32_t*) 0x40020010;

	    //GPIOA yi input moda konfigüre edelim.
	    *pPortAModeReg&=~(0x03);

        while(1){

        	uint8_t pinDurumu = (uint8_t) (*pPortAInReg&0x01);

        	if(pinDurumu){

	    	*pPortDOutReg|= (1<<12);
        	}

        	else{
	    	*pPortDOutReg&= ~(1<<12);
	    	}


        }



}
