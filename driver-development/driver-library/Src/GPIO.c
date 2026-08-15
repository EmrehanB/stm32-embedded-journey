
#include "GPIO.h"


void GPIO_Init (GPIO_TypeDef_t *GPIOx , GPIO_InitTtypedef *GPIO_ConfigStruct){

	uint32_t position;
	uint32_t fakePosition = 0;
	uint32_t lastPosition = 0;

	for(position=0 ; position<16 ; position++)
	{
		fakePosition= (0x1<<position);
		lastPosition= (uint32_t) (GPIO_ConfigStruct->pinNumber) & fakePosition;

		if(fakePosition == lastPosition)

		{

			//Mode Konfigürasyonu
			uint32_t tempVal= GPIOx->MODER;
			tempVal &= ~( 0x3U << (2*position) );
			tempVal |= (GPIO_ConfigStruct->Mode <<(2*position) );
			GPIOx->MODER=tempVal;



			if(GPIO_ConfigStruct->Mode == GPIO_MODE_INPUT || GPIO_ConfigStruct->Mode == GPIO_MODE_ANALOG)
			{

				//Otype Konfigürasyonu
				tempVal=GPIOx->OTYPER;
				tempVal&=~(0x1U<<position);
				tempVal|=(GPIO_ConfigStruct->Otype << position);
				GPIOx->OTYPER=tempVal;

				//Ospeed Konfigürasyonu
				tempVal=GPIOx->OSPEEDR;
				tempVal&=~(0x3U << (2*position));
				tempVal|= (GPIO_ConfigStruct->Speed << (2*position));
				GPIOx->OSPEEDR=tempVal;
			}

			//PUPD Konfigürasyonu
			tempVal=GPIOx->PUPDR;
			tempVal&=~(0x3U << (2*position));
			tempVal|= (GPIO_ConfigStruct->PuPd << (2*position));
			GPIOx->PUPDR=tempVal;

		}


	}



}



void GPIO_WritePin(GPIO_TypeDef_t *GPIOx , uint16_t pinNumber , GPIO_PinState_t pinState )
{

	if(pinState==GPIO_Pin_Set)
	{

		GPIOx->BSRR=pinNumber;

	}

	else
	{

		GPIOx->BSRR=(pinNumber << 16U);

	}



}



GPIO_PinState_t GPIO_ReadPin(GPIO_TypeDef_t *GPIOx , uint16_t pinNumber ){

	GPIO_PinState_t bitStatus= GPIO_Pin_Reset;

	if( (GPIOx->IDR & pinNumber) != GPIO_Pin_Reset ){                              //pin numberları sadece ilgili bit 1 olacak şekilde ayarladığımızdan burada büyük kolaylık sağladı.
		bitStatus=GPIO_Pin_Set;
	}

	return bitStatus;
}



void GPIO_LockPin(GPIO_TypeDef_t *GPIOx, uint16_t pinNumber ){

	/*
	  Bu fonksiyon biraz alışılmadık daha doğrusu LCKR registerı ı biraz alışılmadık. Reference Manual deki tarif:

	  LOCK key write sequence:
      WR LCKR[16] = ‘1’ + LCKR[15:0]  satır 1
      WR LCKR[16] = ‘0’ + LCKR[15:0]  satır 2
      WR LCKR[16] = ‘1’ + LCKR[15:0]  satır 3

    */


	uint32_t tempValue = (0x1U <<16 ) | pinNumber ;
	GPIOx->LCKR = tempValue;                            //satır 1     ---   WR LCKR[16] = ‘1’ + LCKR[15:0]  = myDATA
	GPIOx->LCKR = pinNumber;                            //satır 2     ---   WR LCKR[16] = ‘0’ + LCKR[15:0]  = myDATA    Pin numaramın zaten 16. biti 0
	GPIOx->LCKR = tempValue;                            //satır 3     ---   WR LCKR[16] = ‘1’ + LCKR[15:0]  = myDATA

	//Ardından kontrol amaçlı olan okumamızı tempValue üzerinden yapabiliriz.

	tempValue = GPIOx->LCKR;                            //Opsiyonel Read işlemi

}





