#include "SPI.h"


static void SPI_CloseISR_TX(SPI_HandleTypeDef_t *SPI_Handle){

	SPI_Handle->Instance->CR2 &= ~ (0x1U << 7); // TXEIE clearlama işlemi
	SPI_Handle->TxDataSize  = 0           ;
	SPI_Handle->pTxDataAddr = NULL		  ;
	SPI_Handle->busStateTX  = SPI_BUS_FREE;
}


static void SPI_TransmitHelper_16Bits (SPI_HandleTypeDef_t *SPI_Handle) {

	SPI_Handle->Instance->DR = *((uint16_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint16_t);
	SPI_Handle->TxDataSize  -= 2 ;

	if(SPI_Handle->TxDataSize == 0){
			SPI_CloseISR_TX(SPI_Handle);
		}
}


static void SPI_TransmitHelper_8Bits (SPI_HandleTypeDef_t *SPI_Handle) {

	SPI_Handle->Instance->DR = *((uint8_t*)(SPI_Handle->pTxDataAddr));
	SPI_Handle->pTxDataAddr += sizeof(uint8_t);
	SPI_Handle->TxDataSize  -- ;

	if(SPI_Handle->TxDataSize == 0){
		SPI_CloseISR_TX(SPI_Handle);
	}


}


//SPI Init yapar
void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle){

	uint32_t tempVal=0;

	tempVal = SPI_Handle->Instance->CR1; // Instance zaten bizim SPI base adresimiz (SPI1/2/3/4) ve CR1 ise konfigüre ettiğimiz registerımız.

	tempVal |= (SPI_Handle->Init.BaudRate   ) | (SPI_Handle->Init.CPHA     ) | (SPI_Handle->Init.CPOL   ) | (SPI_Handle->Init.DFF) | (SPI_Handle->Init.Mode) |                 \
			   (SPI_Handle->Init.FrameFormat) | (SPI_Handle->Init.BusConfig) | (SPI_Handle->Init.SSM_Cmd) ;

	SPI_Handle->Instance->CR1 = tempVal;

}


//SPI enable/disable
void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle,FunctionalState_t stateOfSPI){

	if(stateOfSPI == ENABLE ){

		 SPI_Handle->Instance->CR1 |= (0x1U << 6); // CR1 registerında SPE bit'i 6. bit

	}
	else{

		SPI_Handle->Instance->CR1 &= ~(0x1U << 6);

	}

}


//transmit data to the slave . Size of data yı byte cinsinden yaz
void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle , uint8_t *pData , uint16_t sizeOfData){

	//Benim shift ve data registerlarım 16 bitlik yani istersem 1 byte istersem 2 byte lık veri yollayabilirim. Bunu spı_handle dan data frame format ı kontrol edelim.,

	if(SPI_Handle->Init.DFF == SPI_DFF_16BITS )  // 16 bitlik veri
	{

		while(sizeOfData > 0)   // 0 dan büyükse hala yazacak verim vardır
		{

			if(SPI_GetFlagStatus(SPI_Handle, SPI_TxE_FLAG)){   //SR registerındaki 1. bit'inde (TXE Transmit buffer empty) veri var mı yok mu ?

				SPI_Handle->Instance->DR = *((uint16_t *) pData);
				pData += sizeof(uint16_t);          //a drese 2 byte eklendi yani 2. datayı gösterdi
				sizeOfData -=2 ;


			}

		}

	}
	else  // 8 bitlik veri
	{

		while(sizeOfData > 0)
		{

			if(SPI_Handle->Instance->SR & (0x1U << 1)){        // 47. satırla tamamen aynı şey farklı kullanımlar görülebilsin diye bunu böyle bıraktım.

				SPI_Handle->Instance->DR = *pData;  // data registeri zaten 16 bit casting işlemine gerek yok.
				pData += sizeof(uint8_t); 						    // adrese 1byte eklendi yani 2. datayı gösterdi
				sizeOfData--;                       // yollanacak veri sayısı 1 azaldı

			}

		}

	}

	while(SPI_GetFlagStatus(SPI_Handle, SPI_BSY_FLAG));			//SR registerının 7. bitinde busy flag var . Bu while ın içi 0 gelmiyorsa yani true ise o zaman busy flag hala sıfır olmamıştır ve beklemeliyiz.



}


void SPI_TransmitData_IT(SPI_HandleTypeDef_t *SPI_Handle , uint8_t *pData , uint16_t sizeOfData){

	SPI_BusStatus_t busState = SPI_Handle->busStateTX;

	if(busState != SPI_BUS_BUSY_TX){  //Birnevi race condition engelliyoruz.

		SPI_Handle->busStateTX = SPI_BUS_BUSY_TX;
		SPI_Handle->pTxDataAddr= pData;
		SPI_Handle->TxDataSize = sizeOfData;

		if(SPI_Handle->Instance->CR1 & (0x1U << 11)) {         //SPI --> CR1 --> DFF biti kontrolü 1 mi 0 mı ? 1 ise 16 bit veri 0 ise 8 bit veri aktaracağız.

			SPI_Handle->TxISRFunction = SPI_TransmitHelper_16Bits;


		}
		else{

			SPI_Handle->TxISRFunction = SPI_TransmitHelper_8Bits;

		}


		SPI_Handle->Instance->CR2 |= (0x1U << 7);
	}

	else{




	}


}


void SPI_InterruptHandler(SPI_HandleTypeDef_t *SPI_Handle){

	uint8_t interruptSource=0 ;
	uint8_t interruptFlag  =0 ;

	interruptSource	 = SPI_Handle->Instance->CR2 & (0x1U << 7); //CR2 registerının TXEIE bit'i enable mı? 1 ya da 0 yazıcaz.
	interruptFlag    = SPI_Handle->Instance->SR  & (0x1U << 1); //SR registerının TXE bit'i enable mı? 1 ya da 0 yazıcaz.

	if(interruptSource!=0 && interruptFlag !=0) { //interrupt gerçekleşmiş yani fonksiyon çağırması yapılabilir

		SPI_Handle->TxISRFunction(SPI_Handle);

	}

}



void SPI_ReceiveData(SPI_HandleTypeDef_t *SPI_Handle , uint8_t *pBuffer , uint16_t sizeOfData){

	if(SPI_Handle->Init.DFF == SPI_DFF_16BITS){

		while(sizeOfData > 0){

			if(SPI_GetFlagStatus( SPI_Handle , SPI_RxNE_FLAG)){

				//içerisi not empty ise veri okur
				*((uint16_t*) pBuffer) = SPI_Handle->Instance->DR;
				pBuffer += sizeof(uint16_t);
				sizeOfData -= 2 ;



			}

		}

	}
	else{

		while(sizeOfData > 0){

		if(SPI_GetFlagStatus( SPI_Handle , SPI_RxNE_FLAG)){

			    //içerisi not empty ise veri okur
				*pBuffer    =  *((__IO uint8_t*) &SPI_Handle->Instance->DR);  //Önce adresini volatile 8 bit pointera typcast ettim (çünkü reg structta 32 bit volatile ) ardından dereference ettim.
				pBuffer    +=  sizeof(uint8_t);
				sizeOfData -=  1 ;


					}
		}
	}

}





SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle , uint16_t SPI_Flag){

	return(SPI_Handle->Instance->SR & SPI_Flag) ? SPI_FLAG_SET : SPI_FLAG_RESET ;

}
