#include <stdint.h>
#include <string.h>
#include "stm32f407xx.h"

static void GPIO_LedConfig();
static void GPIO_ButtonInterruptConfig();
static void SPI_Config();
static void SPI_GPIO_Config();


SPI_HandleTypeDef_t SPI_Handle = { 0 } ; //SPI , USART gibi birimler çeşitli yerlerde kullanıldığ için handleları global tanımlanır mesela ben gidip bunu interruptımda kullanabileceğim.


void EXTI0_IRQHandler(){

	static char messageToSend[]= "Hello Embedded!\n";

	if(EXTI->PR & 0x1 )

	{

  		EXTI->PR |= (0x1U << 0U);

  		SPI_TransmitData_IT(&SPI_Handle, (uint8_t *)messageToSend, strlen(messageToSend));




	}

}


void SPI1_IRQHandler(){ //SPI dan interrupt geldiğinde kod buraya atlaaycak. Hiçbir şekilde başka yere atlama şansı yok.

	SPI_InterruptHandler(&SPI_Handle);

}









int main(void)
{





 	GPIO_LedConfig();

	GPIO_ButtonInterruptConfig();

	SPI_GPIO_Config();

	SPI_Config();




	for(;;);






}






void GPIO_ButtonInterruptConfig(){

	//Şu ana kadar interrupt ile ilgili (SYSCFG - EXTI) yaptıklarımı test edip registerları izleme amaçlı bir fonksiyondur. Daha NVIC ile ilgili geliştirme yapmadım.

	RCC_SYSCFG_CLK_ENABLE();
	//EXTI için harici bir clock enable yok.
	//Dinlediğim portun clockunu da enable etmem lazım.
	RCC_GPIOA_CLK_ENABLE() ;
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

static void SPI_Config(){

	RCC_SPI1_CLK_ENABLE();



	SPI_Handle.Instance = SPI1;

	SPI_Handle.Init.BaudRate    = SPI_BAUDRATE_DIV8   ; //logic analyzer ' da daha rahat incelemek için 2 Mhz e düşüreyim. (PLL konfigüre değil kart 16 MHz çalışıyor)
	SPI_Handle.Init.BusConfig   = SPI_BUS_FullDuplex  ;
	SPI_Handle.Init.CPHA        = SPI_CPHA_FirstEdge  ;
	SPI_Handle.Init.CPOL        = SPI_CPOL_LOW        ;
	SPI_Handle.Init.DFF         = SPI_DFF_8BITS       ;
	SPI_Handle.Init.FrameFormat = SPI_FrameFormat_MSB ;
	SPI_Handle.Init.Mode        = SPI_MODE_MASTER	  ;
	SPI_Handle.Init.SSM_Cmd     = SPI_SSM_ENABLE      ;


	SPI_Init(&SPI_Handle);

	NVIC_EnableInterrupt(SPI1_IRQNumber);

	SPI_PeriphCmd(&SPI_Handle, ENABLE);



}


static void SPI_GPIO_Config(){


	GPIO_InitTtypedef GPIO_Init_Struct = { 0 };

	GPIO_Init_Struct.pinNumber = GPIO_PIN_5 | GPIO_PIN_7 ;  //(PA5 = SPI1_SCK ve PA7=SPI1_MOSI)---> AF5'e göre konfigüre edildiklerinde böyle.Bunlara datasheet "Alternate function mapping" kısmından ulaşılabilir.
	GPIO_Init_Struct.Mode      = GPIO_MODE_ALTFUN    ;
	GPIO_Init_Struct.Otype     = GPIO_OTYPE_PP       ;
	GPIO_Init_Struct.PuPd      = GPIO_PUPD_NOPULL    ;
	GPIO_Init_Struct.Speed     = GPIO_SPEED_VERYHIGH ;
	GPIO_Init_Struct.Alternate = GPIO_AF5 ;

	GPIO_Init(GPIOA,&GPIO_Init_Struct);



}



