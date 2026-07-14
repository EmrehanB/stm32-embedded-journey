

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	//-----------------Algoritmam------------------

	//LED6 D portunun 15 inci pininde olduğunu şematikten öğrendim.
	//Şimdi referans manuelden GPIOD nin bellek adresinin başlangıcını bulmak var. --> 0x40020C00 (GPIOD başlangıç adresi)
	//Clock vermem lazım gidip clock kısmına bakacağım.GPIOD nin AHB1 Bus üzerinde olduğunu gördüm.O zaman o Bus ı Etkileyen CLock registerına bakacağım.
	//AHB1ENR registerının 3 bitini aktif ettiğimde (logic 1) GPIOD ye clock verdiğimi gördüm.
	//Şimdi D portunun 15. pinini out olarak ayarlamam ve onu aktif yapmam lazım ki ışığımız yansın.
	//İlk olarak GPIOx_MODER registerine gidiyorum. Burada her registerin 2 bit ile temsil edildiğine ve 2y,2y+1 kuralına dikkat ediyorum.
	//GPIOx_MODER 31 ve 30. bitlerini 01 yapmalıyım (burada 0 olan 31. bit yani MSB)
	//Ardından GPIOx_ODR registerinin 15. bitini 1 yaparsam işlem tamamlanacaktır.

	//-------------------ADRESLER------------------

	//GPIOD Başlangıç : 0x40020C00
	//RCC Base adresi : 0x40023800    ve  AHB1ENR offseti eklenmiş hali : 0x40023830
	//GPIOx_MODER     : 0x40020C00
	//GPIOx_ODR       : 0x40020C14

	uint32_t *pClockVermeReg = (uint32_t*) 0x40023830 ;
	uint32_t *pModerReg=(uint32_t*) 0x40020C00;
	uint32_t *OutDataReg=(uint32_t*) 0x40020C14;

	//--------------Maskeleme değerlerini bulmalıyım--------------------

	//AHB1ENR registerının 0x08 yani 1000 ile OR lanması lazım ki 3. bit aktif olsun.

	*pClockVermeReg |= 0x08;

	//*pModerReg i önce 0x3FFFFFFF ile and işlemine sokacağım ardından 0x40000000 ile OR işlemine sokacağım.

	*pModerReg &= 0x3FFFFFFF;  *pModerReg |= 0x40000000;

	//*OutDataReg inin 15. bitini 1 yapmak için 0x8000 ile OR işkemine sokacağım

	*OutDataReg |=0x8000;

	//Debug esnasında SFRs arayüzünden tüm register değerlerini her adımda tek tek izledim.









    /* Loop forever */
	for(;;);
}
