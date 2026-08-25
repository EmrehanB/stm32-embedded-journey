#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"

static void GPIO_LedConfig();
void GPIO_ButtonInterruptConfig();


void EXTI0_IRQHandler(){

	if(EXTI->PR & 0x1 ){

  		EXTI->PR |= (0x1U << 0U);

		GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_Pin_Set);


	}

}









int main(void)
{





 	GPIO_LedConfig();

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
	myInitStruct.EXTI_LineNumber = EXTI_LineSource_0;
	myInitStruct.EXTI_Mode       = EXTI_Mode_Interrupt;
	myInitStruct.TriggerMode 	 = EXTI_Trigger_Rising_Offset;

	EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);
	EXTI_Init(&myInitStruct);

	NVIC_EnableInterrupt(EXTI0_IRQNumber);

    }



static void GPIO_LedConfig(){

	RCC_GPIOD_CLK_ENABLE() ;
    RCC_GPIOA_CLK_ENABLE() ;
    RCC_SYSCFG_CLK_ENABLE();//Clock aktif etme makrolarım

	GPIO_InitTtypedef GPIO_InitConfigStruct = { 0 }; //Garbage değerlerle gelmesin diye tüm struct elemanlarını sıfır yaptım.

	GPIO_InitConfigStruct.pinNumber  = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitConfigStruct.Mode       = GPIO_MODE_OUTPUT;
	GPIO_InitConfigStruct.Speed 	    = GPIO_SPEED_LOW;                                      //Fazla hız fazla güç tüketimi
	GPIO_InitConfigStruct.Otype      = GPIO_OTYPE_PP ;
	GPIO_InitConfigStruct.PuPd       = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD,           &GPIO_InitConfigStruct);

	memset(&GPIO_InitConfigStruct , 0 ,sizeof(GPIO_InitConfigStruct)); // Hali hazırda var olan structımı sıfırladım yani konfigürasyon iptal edildi iş bittikten sonra

	GPIO_InitConfigStruct.pinNumber  = GPIO_PIN_0;
	GPIO_InitConfigStruct.Mode       = GPIO_MODE_INPUT;
	GPIO_InitConfigStruct.PuPd 		= GPIO_PUPD_PULLDOWN;        // Bu konfigürasyonu PA0 için yapıyorum ve schematic e baktığımda user button yani PA0 pull-down modda

	GPIO_Init(GPIOA, &GPIO_InitConfigStruct);

	EXTI_LineConfig(EXTI_PortSource_GPIOA,EXTI_LineSource_0);


}
