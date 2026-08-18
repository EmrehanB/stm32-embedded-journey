#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"

static void GPIO_LedConfig();


int main(void)
{
	GPIO_LedConfig();



	for(;;)
	{
		if(GPIO_ReadPin(GPIOA, GPIO_PIN_0)== GPIO_Pin_Set)
		{
			GPIO_WritePin(GPIOD,GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_Pin_Set);
		}
		else
		{
			GPIO_WritePin(GPIOD,GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_Pin_Reset);
		}

	}
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
