#include <stdint.h>
#include "stm32f407xx.h"

static void GPIO_LedConfig();


int main(void)
{
	GPIO_LedConfig();
	GPIO_WritePin(GPIOD,GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15,GPIO_Pin_Set);


	for(;;);
}



static void GPIO_LedConfig(){

    RCC_GPIOD_CLK_ENABLE();  				   //Clock aktif etme makrom

	GPIO_InitTtypedef ledConfigStruct = { 0 }; //Garbage değerlerle gelmesin diye tüm struct elemanlarını sıfır yaptım.

	ledConfigStruct.pinNumber = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	ledConfigStruct.Mode      = GPIO_MODE_OUTPUT;
	ledConfigStruct.Speed 	  = GPIO_SPEED_LOW;                                      //Fazla hız fazla güç tüketimi
	ledConfigStruct.Otype     = GPIO_OTYPE_PP ;
	ledConfigStruct.PuPd      = GPIO_PUPD_NOPULL;

	GPIO_Init(GPIOD,&ledConfigStruct);

}
