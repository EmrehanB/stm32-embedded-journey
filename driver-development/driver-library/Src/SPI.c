#include "SPI.h"


//SPI Init yapar
void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle){

	uint32_t tempVal=0;

	tempVal = SPI_Handle->Instance->CR1; // Instance zaten bizim SPI base adresimiz (SPI1/2/3/4) ve CR1 ise configüre ettiğimiz register registerımız.

	tempVal |= (SPI_Handle->Init.BaudRate   ) | (SPI_Handle->Init.CPHA     ) | (SPI_Handle->Init.CPOL   ) | (SPI_Handle->Init.DFF) | (SPI_Handle->Init.Mode) |                 \
			   (SPI_Handle->Init.FrameFormat) | (SPI_Handle->Init.BusConfig) | (SPI_Handle->Init.SSM_Cmd) ;

	SPI_Handle->Instance->CR1 = tempVal;

}
