

#ifndef INC_USART_H_
#define INC_USART_H_


#include "stm32f407xx.h"


//@def_group Mode_Types
#define USART_MODE_Rx	 ((uint32_t) (0x00000004))
#define USART_MODE_Tx 	 ((uint32_t) (0x00000008)) //transmit enable yani Cr1 registerı için 3. biti 1 olan sayı
#define USART_MODE_Tx_Rx ((uint32_t) (0x0000000C))



//@def_group WordLength_Types
#define USART_WORDLENGTH_8Bit ((uint32_t) (0x00000000))
#define USART_WORDLENGTH_9Bit ((uint32_t) (0x00001000))



// @def_group Parity_Modes
#define USART_PARITY_NONE ((uint32_t) (0x00000000))
#define USART_PARITY_Even ((uint32_t) (0x00000400))  //PSE enable ve PS 0 yani CR1 registerının 10. bit'i 1 ve 9. bit'i 0 dır. 9. bit ancak 10. bit aktif edilirse işe yarar.
#define USART_PARITY_Odd ((uint32_t)  (0x00000600))  //10. bit'i 1 ve 9. bit'i 1 dir



//@def_group StopBits
#define USART_STOPBITS_1    		((uint32_t) (0x00000000))
#define USART_STOPBITS_Half 		((uint32_t) (0x00001000))  // Bunlar cr2 registerlarında
#define USART_STOPBITS_2 			((uint32_t) (0x00002000))
#define USART_STOPBITS_1andHalf 	((uint32_t) (0x00003000))



//@def_group OverSampling_modes
#define USART_OVERSAMPLE_16 		((uint32_t) (0x00000000))
#define USART_OVERSAMPLE_8 		    ((uint32_t) (0x00008000))



// @def_group HardwareFlowControl_modes
#define USART_HW_NONE				((uint32_t) (0x00000000))
#define USART_HW_CTS				((uint32_t) (0x00000200))
#define USART_HW_RTS				((uint32_t) (0x00000100))
#define USART_HW_CTS_RTS			((uint32_t) (0x00000300))







typedef struct{

	uint32_t Mode			; 			//transmission and reception modları @def_group Mode_Types
	uint32_t BaudRate		;
	uint32_t OverSampling	;			//@def_group OverSampling_modes
	uint32_t WordLength 	;  			//8 veya 9 bitlik mod seçimi @def_group WordLength_Types
	uint32_t Parity			;  			//Çift ya da tek parity @def_group Parity_Modes
	uint32_t StopBits		;  			// @def_group StopBits -->USART CR2 STOP[1:0] : cerceve sonunda hatti bosta (1) seviyesine dondurur, sonraki start bitinin kenarini ayirt edilebilir kilar
	uint32_t HardwareFlowControl;       // @def_group HardwareFlowControl_modes

}USART_InitTypeDef_t;


typedef struct{

 USART_TypeDef_t     *Instance;
 USART_InitTypeDef_t  Init 	  ;

}USART_HandleTypeDef_t;










#endif /* INC_USART_H_ */
