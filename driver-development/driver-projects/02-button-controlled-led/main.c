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

	RCC_GPIOD_CLK_ENABLE();
    RCC_GPIOA_CLK_ENABLE();//Clock aktif etme makrolarım

	GPIO_InitTtypedef InitConfigStruct = { 0 }; //Garbage değerlerle gelmesin diye tüm struct elemanlarını sıfır yaptım.

	InitConfigStruct.pinNumber  = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	InitConfigStruct.Mode       = GPIO_MODE_OUTPUT;
	InitConfigStruct.Speed 	    = GPIO_SPEED_LOW;                                      //Fazla hız fazla güç tüketimi
	InitConfigStruct.Otype      = GPIO_OTYPE_PP ;
	InitConfigStruct.PuPd       = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD,           &InitConfigStruct);

	memset(&InitConfigStruct , 0 ,sizeof(InitConfigStruct)); // Hali hazırda var olan structımı sıfırladım yani konfigürasyon iptal edildi iş bittikten sonra

	InitConfigStruct.pinNumber  = GPIO_PIN_0;
	InitConfigStruct.Mode       = GPIO_MODE_INPUT;
	InitConfigStruct.PuPd 		= GPIO_PUPD_PULLDOWN;        // Bu konfigürasyonu PA0 için yapıyorum ve schematic e baktığımda user button yani PA0 pull-down modda

	GPIO_Init(GPIOA, &InitConfigStruct);

}
