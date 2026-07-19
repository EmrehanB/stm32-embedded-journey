
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	__asm volatile ("LDR R1,=#0x20001000");
	__asm volatile ("LDR R2,=#0x20001004");
	__asm volatile ("LDR R0,[R1]");
	__asm volatile ("LDR R1,[R2]");
	__asm volatile ("ADD R0,R0,R1");
	__asm volatile ("STR R0,[R2]");  //LDR nin tersi registerdan bellege yazar.Adrese (0x20001004) R0 degerini yazdirdik.





    /* Loop forever */
	for(;;);
}
