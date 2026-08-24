
#include "EXTI.h"





//Geçerli GPIO port ve numarası için EXTI init . Paramatre olarak user tarafından konfigüre edilen structure alır.
void EXTI_Init(EXTI_InitTypedef_t *EXTI_InitStruct){

	uint32_t tempVal=0;
	tempVal = (uint32_t) EXTI_BASE_ADDR ;

	EXTI->IMR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);
	EXTI->EMR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);

	if(EXTI_InitStruct->EXTI_LineCMD != DISABLE){

		tempVal += EXTI_InitStruct->EXTI_Mode; //IMR mi EMR mi buradan anlaşılıyor.

		*((__IO uint32_t*) tempVal) |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);

		tempVal = (uint32_t) EXTI_BASE_ADDR ;

		EXTI->RTSR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);
		EXTI->FTSR &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);

		if(EXTI_InitStruct->TriggerMode == EXTI_Trigger_RF_Offset){
			//İkisinide Konfigüre et
			EXTI->RTSR |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
			EXTI->FTSR |= (0x1U << EXTI_InitStruct->EXTI_LineNumber);
		}
		else{
			//İkisinden birini konfigüre et
			tempVal += EXTI_InitStruct->TriggerMode;
			*((__IO uint32_t*) tempVal) |= (0x1U<< EXTI_InitStruct->EXTI_LineNumber);
		}



	}

	else{

		tempVal = (uint32_t) EXTI_BASE_ADDR ;

		tempVal += EXTI_InitStruct->EXTI_Mode;

		*((__IO uint32_t*) tempVal) &= ~(0x1U << EXTI_InitStruct->EXTI_LineNumber);




	}

}










//syscfg için ilgili portu konfigüre eder. // Parametreler için EXTI.h içindeki @def_group Port Values kullanılır.
void EXTI_LineConfig(uint8_t portSource , uint8_t EXTI_lineSource ){

	uint32_t tempVal;

	tempVal=SYSCFG->EXTI_CR[EXTI_lineSource>>2U];    			//Uygun EXTI_CR registerını bulduk
	tempVal &= ~((0xFU)     << ((EXTI_lineSource & 0x3U )*4));  //Pin numberın kendi registerındaki hangi 4 bitlik parçada olduğunu buldum ve onu 4 ile çarparak asıl bit başlangıç numarasını bulmuş oldum
	tempVal |= (portSource) << ((EXTI_lineSource & 0x3U )*4);   //Yukarıdaki gibi doğru bit numarasını bulma , oraya kaydırma ve set

	SYSCFG->EXTI_CR[EXTI_lineSource>>2U]=tempVal;

}

