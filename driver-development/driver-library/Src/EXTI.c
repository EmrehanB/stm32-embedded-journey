
#include "EXTI.h"


//syscfg için ilgili portu konfigüre eder. // Parametreler için EXTI.h içindeki @def_group Port Values kullanılır.
void EXTI_LineConfig(uint8_t portSource , uint8_t EXTI_lineSource ){

	uint32_t tempVal;

	tempVal=SYSCFG->EXTI_CR[EXTI_lineSource>>2U];    			//Uygun EXTI_CR registerını bulduk
	tempVal &= ~((0xFU)     << ((EXTI_lineSource & 0x3U )*4));  //Pin numberın kendi registerındaki hangi 4 bitlik parçada olduğunu buldum ve onu 4 ile çarparak asıl bit başlangıç numarasını bulmuş oldum
	tempVal |= (portSource) << ((EXTI_lineSource & 0x3U )*4);   //Yukarıdaki gibi doğru bit numarasını bulma , oraya kaydırma ve set

	SYSCFG->EXTI_CR[EXTI_lineSource>>2U]=tempVal;

}

