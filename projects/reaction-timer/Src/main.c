#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
    //USER buttın PA0 üzerinden okunuyor.
    //GPIOx MODER offset 0x00
    //GPIOx IDR offset 0x10
    //GPIOx ODR offset 0x14
    //GPIOA 0x40020000 adresinden başlıyor.
    //GPIOD 0x40020C00 adresinden başlıyor.
    //RCCAHB1 0x40023830 --> GPIOA için 0. bit ve GPIOD için 3. bit değerleri 1 yapılır

    // A ve D portuna clock verelim.
    volatile uint32_t *pRCC_AHB1_reg = (uint32_t*) 0x40023830;

    *pRCC_AHB1_reg |= (1<<0);
    *pRCC_AHB1_reg |= (1<<3);

    volatile uint32_t *pGPIOA_IDR_reg= (uint32_t*) 0x40020010;
    volatile uint32_t *pGPIOA_MOD_reg= (uint32_t*) 0x40020000;

    //GPIOA Modunu inputa çekmek için ilgili bit işlemlerini yapalım

    *pGPIOA_MOD_reg &= ~(3<<0); // ilgili bitlerin 00 olması zaten input mod

    //PA0 Reset sonrası zaten input modda oluyormuş ancak ben yine de yazdım.

    volatile uint32_t *pGPIOD_MOD_reg= (uint32_t*) 0x40020C00;

    *pGPIOD_MOD_reg &= ~(3<<24);
    *pGPIOD_MOD_reg |=  (1<<24);

    volatile uint32_t *pGPIOD_ODR_reg= (uint32_t*) 0x40020C14;

    *pGPIOD_ODR_reg &= ~(1<<12);


    volatile uint32_t reactionTimer = 0;
    volatile uint32_t bekleme = 2000000;

    while(1)
    {

        bekleme = 2000000 + (reactionTimer % 6000000);
        reactionTimer = 0;

        // Bekleme döngüsü (sınırı bekleme olarak değiştirildi)

        for(volatile int i=0; i<bekleme; i++);

        // Sırasıyla LED yakma, buton bekleme ve LED söndürme satırları

        *pGPIOD_ODR_reg |= (1<<12);

        while((*pGPIOA_IDR_reg & 0x1) == 0){
            reactionTimer++;
        }

        *pGPIOD_ODR_reg &= ~(1<<12);
    }


}
