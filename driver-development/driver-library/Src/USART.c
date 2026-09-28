
#include "USART.h"


void USART_Init(USART_HandleTypeDef_t *USART_Handle){

	uint32_t periphClock  	 = 0;
	uint32_t mantissaPart 	 = 0;
	uint32_t fractionPart 	 = 0;
	uint32_t USART_DIV_Value = 0;
	uint32_t tempValue 		 = 0;

	/************************************CR1*********************************************/

	uint32_t tempReg;

	tempReg  = USART_Handle->Instance->CR1;

	tempReg |= USART_Handle->Init.OverSampling | USART_Handle->Init.OverSampling | USART_Handle->Init.Mode | USART_Handle->Init.Parity;

	USART_Handle->Instance->CR1 = tempReg;

	/************************************CR2**********************************************/

    tempReg  = USART_Handle->Instance->CR2;

    tempReg |= ~(0x3U << 12 ); //CR2 registerının stop bitleri sıfırlandı

    tempReg |= (USART_Handle->Init.StopBits);

    USART_Handle->Instance->CR2 = tempReg;

    /************************************CR3**********************************************/

    tempReg = USART_Handle->Instance->CR3;

    tempReg = USART_Handle->Init.HardwareFlowControl;

    USART_Handle->Instance->CR3 = tempReg;

    /**************************Baud Rate Configuration************************************/

    if(USART_Handle->Instance == USART1 || USART6){

    	//APB2 CLOCK hattına göre baudrate config yapılır
    	periphClock = RCC_GetPClock2();

    }
    else{

    	periphClock = RCC_GetPClock1();

    }


    if(USART_Handle->Init.OverSampling == USART_OVERSAMPLE_8){

    	USART_Handle->Instance->BRR = __USART_BRR_OVERSAMPLING_8(periphClock,USART_Handle->Init.BaudRate);

    	USART_DIV_Value=__USART_DIV_VALUE_8(periphClock,USART_Handle->Init.BaudRate);
    	mantissaPart = USART_DIV_Value / 100U;
    	fractionPart = USART_DIV_Value - (mantissaPart*100U);
    	fractionPart = ((( fractionPart * 8U)+50U)/100U) & (0x7U);

    }
    else{

    	USART_Handle->Instance->BRR = __USART_BRR_OVERSAMPLING_16(periphClock,USART_Handle->Init.BaudRate);

    	USART_DIV_Value = __USART_DIV_VALUE_16(periphClock,USART_Handle->Init.BaudRate);
    	mantissaPart = USART_DIV_Value / 100U;
    	fractionPart = USART_DIV_Value - (mantissaPart*100U);

    	fractionPart = ((( fractionPart * 16U)+50U)/100U) & (0xFU);

    }


    USART_Handle->Instance->BRR = tempValue;

}






void USART_TransmitData(USART_HandleTypeDef_t *USART_Handle , uint8_t *pDataAddr , uint16_t dataSize  ){


	uint16_t *data16Bits ; // 9 bitlik frame ve 9 u da data yani parity yok. Bunundışındaki diğer olasılıklarda 8 bitlik data yeterli yani direkt pDataAddr değerini basarız.


	if( (USART_Handle->Init.WordLength == USART_WORDLENGTH_9Bit) && (USART_Handle->Init.Parity==USART_PARITY_NONE) ){  // bu durumda frame'in 9 biti de datadır.

		data16Bits = (uint16_t *) pDataAddr;

	}else{

		data16Bits=NULL;

	}

	while (dataSize>0){


		while(! (USART_GetFlagStatus(USART_Handle , USART_TXE_FLAG) ) ); //SR registerındaki TXE flag 1 olana kadar bekle

		if(data16Bits == NULL){

			USART_Handle->Instance->DR = (*pDataAddr & 0x0FFU);
			pDataAddr ++;
			dataSize  --;

		}
		else{

			USART_Handle->Instance->DR = (uint16_t) (*data16Bits & 0x01FFU ); // sadece 9 bitle ilgilendiğimiz için ilk 9 biti alıyoruz kalanlar sıfırlanıyor .
			data16Bits ++ ; //uint16_t olduğunda 2 byte 2 byte atar ztn. Bu bir adres
			dataSize   -=2; // bu bir value

		}


	}


	while( !(USART_GetFlagStatus(USART_Handle , USART_TC_FLAG  ))); //Transmission complete bit'i (flag) 1 olana kadar bekle



}



void USART_PeriphCMD (USART_HandleTypeDef_t *USART_Handle , FunctionalState_t stateOfUSART){

	if(stateOfUSART == ENABLE){

		USART_Handle->Instance->CR1 |= (0x1U << 13U ); //CR1 registerında usart enable bitini 1 yapıyoruz.

	}
	else{

		USART_Handle->Instance->CR1 &= ~(0x1U << 13U );

	}

}





USART_FlagStatus_t USART_GetFlagStatus (USART_HandleTypeDef_t *USART_Handle , uint16_t flagName ) {

	return ( (USART_Handle->Instance->SR & flagName) ? USART_FLAG_SET : USART_FLAG_RESET );

}

















