
#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "stm32f407xx.h"


//@def_group Port Values


#define EXTI_PortSource_GPIOA  ((uint8_t) (0x0))
#define EXTI_PortSource_GPIOB  ((uint8_t) (0x1))
#define EXTI_PortSource_GPIOC  ((uint8_t) (0x2))
#define EXTI_PortSource_GPIOD  ((uint8_t) (0x3))
#define EXTI_PortSource_GPIOE  ((uint8_t) (0x4))
#define EXTI_PortSource_GPIOF  ((uint8_t) (0x5))
#define EXTI_PortSource_GPIOG  ((uint8_t) (0x6))
#define EXTI_PortSource_GPIOH  ((uint8_t) (0x7))
// Reference manual de olan tüm portlar benim kartımda olmadığından hepsini yapmadım.


#define EXTI_LineSource_0       ((uint8_t) (0x0))  // Pin numaraları aslında
#define EXTI_LineSource_1       ((uint8_t) (0x1))
#define EXTI_LineSource_2       ((uint8_t) (0x2))
#define EXTI_LineSource_3       ((uint8_t) (0x3))
#define EXTI_LineSource_4       ((uint8_t) (0x4))
#define EXTI_LineSource_5       ((uint8_t) (0x5))
#define EXTI_LineSource_6       ((uint8_t) (0x6))
#define EXTI_LineSource_7       ((uint8_t) (0x7))
#define EXTI_LineSource_8       ((uint8_t) (0x8))
#define EXTI_LineSource_9       ((uint8_t) (0x9))
#define EXTI_LineSource_10      ((uint8_t) (0xA))
#define EXTI_LineSource_11      ((uint8_t) (0xB))
#define EXTI_LineSource_12      ((uint8_t) (0xC))
#define EXTI_LineSource_13      ((uint8_t) (0xD))
#define EXTI_LineSource_14      ((uint8_t) (0xE))
#define EXTI_LineSource_15      ((uint8_t) (0xF))



void EXTI_LineConfig(uint8_t portSource , uint8_t EXTI_lineSource );























#endif /* INC_EXTI_H_ */
