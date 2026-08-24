#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"

void GPIO_ButtonInterruptConfig();




int main(void)
{

	GPIO_ButtonInterruptConfig();

	for(;;);

}


void GPIO_ButtonInterruptConfig(){

	//Şu ana kadar interrupt ile ilgili (SYSCFG - EXTI) yaptıklarımı test edip registerları izleme amaçlı bir fonksiyondur. Daha NVIC ile ilgili geliştirme yapmadım.

	RCC_SYSCFG_CLK_ENABLE();
	//EXTI için harici bir clock enable yok.
	//Dinlediğim portun clockunu da enable etmem lazım.
	RCC_GPIOC_CLK_ENABLE() ;

	EXTI_InitTypedef_t myInitStruct={0};

	myInitStruct.EXTI_LineCMD    = ENABLE;
	myInitStruct.EXTI_LineNumber = EXTI_LineSource_10;
	myInitStruct.EXTI_Mode       = EXTI_Mode_Interrupt;
	myInitStruct.TriggerMode 	 = EXTI_Trigger_Rising_Offset;

	EXTI_LineConfig(EXTI_PortSource_GPIOC, EXTI_LineSource_10);
	EXTI_Init(&myInitStruct);

    }
