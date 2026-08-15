
#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f407xx.h"



//@def_group GPIO_Pins
#define GPIO_PIN_0    (uint16_t) (0x0001) // 0000 0000 0000 0001
#define GPIO_PIN_1    (uint16_t) (0x0002) // 0000 0000 0000 0010
#define GPIO_PIN_2    (uint16_t) (0x0004) // 0000 0000 0000 0100
#define GPIO_PIN_3    (uint16_t) (0x0008) // ...
#define GPIO_PIN_4    (uint16_t) (0x0010)
#define GPIO_PIN_5    (uint16_t) (0x0020)
#define GPIO_PIN_6    (uint16_t) (0x0040)
#define GPIO_PIN_7    (uint16_t) (0x0080)
#define GPIO_PIN_8    (uint16_t) (0x0100)
#define GPIO_PIN_9    (uint16_t) (0x0200) // GPIO Pin 9 seçildi
#define GPIO_PIN_10   (uint16_t) (0x0400)
#define GPIO_PIN_11   (uint16_t) (0x0800)
#define GPIO_PIN_12   (uint16_t) (0x1000)
#define GPIO_PIN_13   (uint16_t) (0x2000)
#define GPIO_PIN_14   (uint16_t) (0x4000)
#define GPIO_PIN_15   (uint16_t) (0x8000) // 1000 0000 0000 0000
#define GPIO_PIN_ALL  (uint16_t) (0xFFFF) // 1111 1111 1111 1111



//@def_group GPIO_Modes
#define GPIO_MODE_INPUT    (0X0U)
#define GPIO_MODE_OUTPUT   (0X1U)
#define GPIO_MODE_ALTFUN   (0X2U)
#define GPIO_MODE_ANALOG   (0X3U)



//@def_group GPIO_Otype_Modes
#define GPIO_OTYPE_PP (0x0U)
#define GPIO_OTYPE_OD (0x1U)



//@def_group GPIO_PUPDR
#define GPIO_PUPD_NOPULL   (0x0U)
#define GPIO_PUPD_PULLUP   (0x1U)
#define GPIO_PUPD_PULLDOWN (0x2U)      //0x3 yazmama gerek yok o kısım zaten RESERVED durumda.



//@def_group GPIO_Speed_Modes
#define GPIO_SPEED_LOW    		(0x0U)
#define GPIO_SPEED_MEDIUM    	(0x1U)
#define GPIO_SPEED_HIGH    		(0x2U)
#define GPIO_SPEED_VERYHIGH   	(0x3U)










//Amacımız soyutlama
typedef enum{

	GPIO_Pin_Reset = 0x0U,
	GPIO_Pin_Set   = !GPIO_Pin_Reset

}GPIO_PinState_t;









//GPIO pini ya da pinlerinin konfigürasyonu:

typedef struct{

	uint32_t pinNumber;  	//@def_group GPIO_Pins e göre numaralamalıyız.
	uint32_t Mode;	     	//@def_group GPIO_Modes e göre konfigürasyon sağlamalıyız.
	uint32_t Otype;      	//@def_group GPIO_Otype_Modes e göre konfigürasyon sağlamalıyız.
	uint32_t PuPd;       	//@def_group GPIO_PUPDR e göre konfigürasyon sağlamalıyız.
	uint32_t Speed;      	//@def_group GPIO_Speed_Modes e göre konfigürasyon sağlamalıyız.
	uint32_t Alternate;

	//IDR ye gerek yok ondan veri okuyoruz konfigürasyon manasız.
	//ODR de aynı şekilde
	//BSSR yazıldığında ilgili ODR bitlerini atomik biçimde kurar veya sıfırlar. konfigürasyon değildir.
	//LCKR ye gerek yok şu an



}GPIO_InitTtypedef;

void GPIO_Init (GPIO_TypeDef_t *GPIOx , GPIO_InitTtypedef *GPIO_ConfigStruct) ;   //GPIO_ConfigStruct'ı ben call by value ile getirseydim stack i çok şimdi ise sadece 4 byte(adres boyutu) kadar şişti

void GPIO_WritePin(GPIO_TypeDef_t *GPIOx , uint16_t pinNumber , GPIO_PinState_t pinState  );

GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx , uint16_t pinNumber );

void GPIO_LockPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber );





#endif /* INC_GPIO_H_ */
