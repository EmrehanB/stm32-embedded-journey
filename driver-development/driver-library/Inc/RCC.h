
#ifndef INC_RCC_H_
#define INC_RCC_H_

#include "stm32f407xx.h"



//RCC AHB1 CLOCK CONTROL MACRO DEFs






#define RCC_GPIOA_CLK_ENABLE()       do{  		uint32_t tempVal=0;											\
												SET_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOAEN)	;				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOAEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)


#define RCC_GPIOB_CLK_ENABLE()       do{  		uint32_t tempVal=0;											\
												SET_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOBEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOBEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)

#define RCC_GPIOC_CLK_ENABLE()       do{  		uint32_t tempVal=0;											\
												SET_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOCEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOCEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)

#define RCC_GPIOD_CLK_ENABLE()       do{  		uint32_t tempVal=0;											\
												SET_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIODEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIODEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)


#define RCC_GPIOA_CLK_DISABLE()       do{  		uint32_t tempVal=0;											\
												CLEAR_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOAEN)	;				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOAEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)


#define RCC_GPIOB_CLK_DISABLE()       do{  		uint32_t tempVal=0;											\
												CLEAR_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOBEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOBEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)

#define RCC_GPIOC_CLK_DISABLE()       do{  		uint32_t tempVal=0;											\
												CLEAR_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOCEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIOCEN);		\
												UNUSED (tempVal);											\
																											\
									  } while (0)

#define RCC_GPIOD_CLK_DISABLE()       do{  		uint32_t tempVal=0;											\
												CLEAR_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIODEN);				\
												tempVal= READ_BIT(RCC->AHB1ENR , RCC_AHB1ENR_GPIODEN);		\
												UNUSED (tempVal);	                                        \
									  } while (0)


#define RCC_SYSCFG_CLK_ENABLE()      do{  		uint32_t tempVal=0;											\
												SET_BIT(RCC->APB2ENR , RCC_APB2ENR_SYSCFG )	;				\
												tempVal= READ_BIT(RCC->APB2ENR , RCC_APB2ENR_SYSCFG );		\
												UNUSED (tempVal);											\
									  } while (0)


#define RCC_SPI1_CLK_ENABLE()        do{        uint32_t tempVal=0;											\
                                                SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1);                \
                                                tempVal= READ_BIT(RCC->APB2ENR , RCC_APB2ENR_SPI1 ) ;       \
												UNUSED(tempVal);                                            \
									  } while (0)


#define RCC_SPI2_CLK_ENABLE()        do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2);                \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2);      \
                                                UNUSED(tempVal);                                        \
                                     } while (0)

#define RCC_SPI3_CLK_ENABLE()        do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3);                \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI3);      \
                                                UNUSED(tempVal);                                        \
                                     } while (0)

#define RCC_SPI4_CLK_ENABLE()        do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4);                \
                                                tempVal= READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4);      \
                                                UNUSED(tempVal);                                        \
                                     } while (0)



#define RCC_USART2_CLK_ENABLE()      do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2);              \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2);    \
                                                UNUSED(tempVal);                                        \
                                     } while (0)

#define RCC_USART3_CLK_ENABLE()      do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3);              \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3);    \
                                                UNUSED(tempVal);                                        \
                                     } while (0)

#define RCC_UART4_CLK_ENABLE()       do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4);               \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4);     \
                                                UNUSED(tempVal);                                        \
                                     } while (0)

#define RCC_UART5_CLK_ENABLE()       do{        uint32_t tempVal=0;                                     \
                                                SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5);               \
                                                tempVal= READ_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5);     \
                                                UNUSED(tempVal);                                        \
                                     } while (0)












#endif /* INC_RCC_H_ */
