

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __IO volatile




#define NVIC_ISER0 (uint32_t*) (0xE000E100)

#define NVIC_ISER1 (uint32_t*) (0xE000E104)

#define NVIC_ISER2 (uint32_t*) (0xE000E108)

#define NVIC_ISER3 (uint32_t*) (0xE000E10C)

#define NVIC_ISER4 (uint32_t*) (0xE000E110)

#define NVIC_ISER5 (uint32_t*) (0xE000E114)

#define NVIC_ISER6 (uint32_t*) (0xE000E118)

#define NVIC_ISER7 (uint32_t*) (0xE000E11C)








#define SET_BIT(REG,BIT)       ( (REG) |=  (BIT) )

#define CLEAR_BIT(REG,BIT)     ( (REG) &= ~(BIT) )

#define READ_BIT(REG,BIT)      ( (REG)  &  (BIT) )

#define UNUSED(x)              (void)x  //Kullanmama uyarısı vermesin diye tanımladım.


typedef enum{  //Makro tanımlaması gibi de düşünülebilir.

	DISABLE  = 0x0U,
 	ENABLE = !DISABLE

}FunctionalState_t;





/*
 * Memory Base addrs
 *
 */

#define FLASH_BASE_ADDR (0x08000000UL) // Flash Base addr (1 MB )
#define SRAM1_BASE_ADDR (0x20000000UL) // SRAM1 Base addr (112 KB)
#define SRAM2_BASE_ADDR (0x2001C000UL) // SRAM2 Base adrr (16 KB)
//...



/*
 * Peripheral Base addrs
 *
 */

#define PERIPH_BASE_ADDR (0x40000000UL)                     // Base address for all peripherals

#define APB1_BASE_ADDR  (PERIPH_BASE_ADDR + 0x00000UL)                 // APB1 bus domain base addr
#define APB2_BASE_ADDR  (PERIPH_BASE_ADDR + 0x10000UL)      // APB2 bus domain base addr  (0x00010000 şeklinde yazmak okunaklılığı arttırır.)
#define AHB1_BASE_ADDR  (PERIPH_BASE_ADDR + 0x20000UL)      // AHB1 bus domain base addr
#define AHB2_BASE_ADDR  (PERIPH_BASE_ADDR + 0x10000000UL )  // AHB2 bus domain base addr
//...




/*
 * APB1 Peripherals Base Addresses
 *
 */

#define TIM2_BASE_ADDR   (APB1_BASE_ADDR + 0x0000UL)                   // Timer 2 base addr
#define TIM3_BASE_ADDR   (APB1_BASE_ADDR + 0x0400UL)        // Timer 3 base addr
#define TIM4_BASE_ADDR   (APB1_BASE_ADDR + 0x0800UL)        // Timer 4 base addr
#define TIM5_BASE_ADDR   (APB1_BASE_ADDR + 0x0C00UL)        // Timer 5 base addr
#define TIM6_BASE_ADDR   (APB1_BASE_ADDR + 0x1000UL)        // Timer 6 base addr
#define TIM7_BASE_ADDR   (APB1_BASE_ADDR + 0x1400UL)        // Timer 7 base addr
// ... (APB1_BASE_ADDR == PERIPH_BASE_ADDR )

#define SPI2_BASE_ADDR   (APB1_BASE_ADDR + 0x3800UL)        // SPI 2 base addr
#define SPI3_BASE_ADDR   (APB1_BASE_ADDR + 0x3C00UL)        // SPI 3 base addr

#define USART2_BASE_ADDR (APB1_BASE_ADDR + 0x4400UL)        // USART2 base addr
#define USART3_BASE_ADDR (APB1_BASE_ADDR + 0x4800UL)        // USART3 base addr

#define UART4_BASE_ADDR  (APB1_BASE_ADDR + 0x4C00UL)        // UART4 base addr
#define UART5_BASE_ADDR  (APB1_BASE_ADDR + 0x5000UL)        // UART5 base addr

#define I2C1_BASE_ADDR   (APB1_BASE_ADDR + 0x5400UL)        //I2C1 base addr
#define I2C2_BASE_ADDR   (APB1_BASE_ADDR + 0x5800UL)        //I2C2 base addr
#define I2C3_BASE_ADDR   (APB1_BASE_ADDR + 0x5C00UL)        //I2C3 base addr




/*
 * APB2 Peripherals Base Addresses
 *
 */

#define TIM1_BASE_ADDR   (APB2_BASE_ADDR + 0x0000UL)		//Timer 1 base addr
#define TIM8_BASE_ADDR   (APB2_BASE_ADDR + 0x0400UL)		//Timer 8 base addr

#define USART1_BASE_ADDR (APB2_BASE_ADDR + 0x1000UL)		//USART 1 base addr
#define USART6_BASE_ADDR (APB2_BASE_ADDR + 0x1400UL)		//USART 6 base addr

#define SPI1_BASE_ADDR   (APB2_BASE_ADDR + 0x3000UL)		//SPI 1 base addr
#define SPI4_BASE_ADDR   (APB2_BASE_ADDR + 0x3400UL)		//SPI 4 base addr

#define SYSCFG_BASE_ADDR (APB2_BASE_ADDR + 0x3800UL)        //SYSCFG base addr

#define EXTI_BASE_ADDR   (APB2_BASE_ADDR + 0x3C00UL)		//EXTI base addr





/*
 * AHB1 Peripherals Base Addresses
 *
 */

#define GPIOA_BASE_ADDR (AHB1_BASE_ADDR + 0x0000UL)			//GPIO_A Base addr
#define GPIOB_BASE_ADDR (AHB1_BASE_ADDR + 0x0400UL)			//GPIO_B Base addr
#define GPIOC_BASE_ADDR (AHB1_BASE_ADDR + 0x0800UL)			//GPIO_C Base addr
#define GPIOD_BASE_ADDR (AHB1_BASE_ADDR + 0x0C00UL)			//GPIO_D Base addr
#define GPIOE_BASE_ADDR (AHB1_BASE_ADDR + 0x1000UL)			//GPIO_E Base addr
#define GPIOF_BASE_ADDR (AHB1_BASE_ADDR + 0x1400UL)			//GPIO_F Base addr
#define GPIOG_BASE_ADDR (AHB1_BASE_ADDR + 0x1800UL)			//GPIO_G Base addr
#define GPIOH_BASE_ADDR (AHB1_BASE_ADDR + 0x1C00UL)			//GPIO_H Base addr
#define GPIOI_BASE_ADDR (AHB1_BASE_ADDR + 0x2000UL)			//GPIO_I Base addr
#define GPIOJ_BASE_ADDR (AHB1_BASE_ADDR + 0x2400UL)			//GPIO_J Base addr
#define GPIOK_BASE_ADDR (AHB1_BASE_ADDR + 0x2800UL)			//GPIO_K Base addr

#define RCC_BASE_ADDR   (AHB1_BASE_ADDR + 0x3800UL)			//RCC base addr




/*
 * Peripheral Structure Definitions
 *
 */

typedef struct
{
	__IO  uint32_t MODER;
	__IO  uint32_t OTYPER;
	__IO  uint32_t OSPEEDR;
	__IO  uint32_t PUPDR;
	__IO  uint32_t IDR;
	__IO  uint32_t ODR;
	__IO  uint32_t BSRR;
	__IO  uint32_t LCKR;
	__IO  uint32_t AFR[2];



}GPIO_TypeDef_t;


//struct yapımız 4 byte değerlerden oluştuğundan dolayı ve cast yaptığımzdan dolayı 4 byte 4 byte struct gezilecek ve istenen hedefe ulaşılabilecek.

typedef struct{

	__IO  uint32_t CR;
	__IO  uint32_t PLLCFGR;
	__IO  uint32_t CFGR;
	__IO  uint32_t CIR;
	__IO  uint32_t AHB1RSTR;
	__IO  uint32_t AHB2RSTR;
	__IO  uint32_t AHB3RSTR;
	__IO  uint32_t RESERVED0; // RESERVED LARI EKLEMEZSEM KODUM BOZULUR 4 BYTE DEĞİLDE 8 BYTE ATLAMAM GEREKİR BU DA STRUCT YAPIMDAN DOLAYI İMKANSIZ
	__IO  uint32_t APB1RSTR;
	__IO  uint32_t APB2RSTR;
	__IO  uint32_t RESERVED1[2];
	__IO  uint32_t AHB1ENR;
	__IO  uint32_t AHB2ENR;
	__IO  uint32_t AHB3ENR;
	__IO  uint32_t RESERVED2;
	__IO  uint32_t APB1ENR;
	__IO  uint32_t APB2ENR;
	__IO  uint32_t RESERVED3[2];
	__IO  uint32_t AHB1LPENR;
	__IO  uint32_t AHB2LPENR;
	__IO  uint32_t AHB3LPENR;
	__IO  uint32_t RESERVED4;
	__IO  uint32_t APB1LPENR;
	__IO  uint32_t APB2LPENR;
	__IO  uint32_t RESERVED5[2];
	__IO  uint32_t BDCR;
	__IO  uint32_t CSR;
	__IO  uint32_t RESERVED6[2];
	__IO  uint32_t SSCGR;
	__IO  uint32_t PLLI2SCFGR;


}RCC_typDef_t;




typedef struct{

	__IO uint32_t MEMRMP;       //SYSCFG memory remap register ve                       adres offset = 0x00
	__IO uint32_t PMC; 			//SYSCFG peripheral mode configuration register         adres offset = 0x04
	__IO uint32_t EXTI_CR[4];	//SYSCFG external interrupt configuration registers     adres offset = 0x08
	__IO uint32_t RESERVED[2];
	__IO uint32_t CMPCR;		//Compensation cell control register                    adres offset = 0x20


}SYSCFG_typeDef_t;




typedef struct{

	__IO uint32_t IMR;			//Interrupt mask register								adres offset = 0x00
	__IO uint32_t EMR;			//Event mask register									adres offset = 0x04
	__IO uint32_t RTSR;			//Rising trigger selection register						adres offset = 0x08
	__IO uint32_t FTSR;			//Falling trigger selection register					adres offset = 0x0C
	__IO uint32_t SWIER;		//Software interrupt event register						adres offset = 0x10
	__IO uint32_t PR;			//Pending register										adres offset = 0x14


}EXTI_typeDef_t;





#define GPIOA 	( (GPIO_TypeDef_t*) GPIOA_BASE_ADDR )
#define GPIOB 	( (GPIO_TypeDef_t*) GPIOB_BASE_ADDR )
#define GPIOC 	( (GPIO_TypeDef_t*) GPIOC_BASE_ADDR )
#define GPIOD 	( (GPIO_TypeDef_t*) GPIOD_BASE_ADDR )
#define GPIOE 	( (GPIO_TypeDef_t*) GPIOE_BASE_ADDR )

#define RCC   	( (RCC_typDef_t*  ) RCC_BASE_ADDR   )

#define SYSCFG 	( (SYSCFG_typeDef_t *) SYSCFG_BASE_ADDR )

#define EXTI    ( (EXTI_typeDef_t *) EXTI_BASE_ADDR)


//Bit tanımlamaları (bit definitions)



#define RCC_AHB1ENR_GPIOAEN_POSITION  (0U)                                      // RCC AHB1ENRegister GPIOAEN bit konumu
#define RCC_AHB1ENR_GPIOAEN_MASK      (0x1 << RCC_AHB1ENR_GPIOAEN_POSITION )    // RCC AHB1ENRegister GPIOAEN bit mask (0x01<<1)
#define RCC_AHB1ENR_GPIOAEN           RCC_AHB1ENR_GPIOAEN_MASK                  // RCC AHB1ENRegister GPIOAEN macro

#define RCC_AHB1ENR_GPIOBEN_POSITION  (1U)										// RCC AHB1ENRegister GPIOBEN bit konumu
#define RCC_AHB1ENR_GPIOBEN_MASK      (0x1 << RCC_AHB1ENR_GPIOBEN_POSITION )	// RCC AHB1ENRegister GPIOBEN bit mask (0x01<<1)
#define RCC_AHB1ENR_GPIOBEN           RCC_AHB1ENR_GPIOBEN_MASK					// RCC AHB1ENRegister GPIOBEN macro

#define RCC_AHB1ENR_GPIOCEN_POSITION  (2U)                                      // RCC AHB1ENRegister GPIOCEN bit konumu
#define RCC_AHB1ENR_GPIOCEN_MASK      (0x1 << RCC_AHB1ENR_GPIOCEN_POSITION )    // RCC AHB1ENRegister GPIOCEN bit mask (0x01<<1)
#define RCC_AHB1ENR_GPIOCEN           RCC_AHB1ENR_GPIOCEN_MASK                  // RCC AHB1ENRegister GPIOCEN macro

#define RCC_AHB1ENR_GPIODEN_POSITION  (3U)                                      // RCC AHB1ENRegister GPIODEN bit konumu
#define RCC_AHB1ENR_GPIODEN_MASK      (0x1 << RCC_AHB1ENR_GPIODEN_POSITION )    // RCC AHB1ENRegister GPIODEN bit mask (0x01<<1)
#define RCC_AHB1ENR_GPIODEN           RCC_AHB1ENR_GPIODEN_MASK                  // RCC AHB1ENRegister GPIODEN macro




#define RCC_APB2ENR_SYSCFG_POSITION   (14U)
#define RCC_APB2ENR_SYSCFG_MASK    	  (0x1 << RCC_APB2ENR_SYSCFG_POSITION  )
#define RCC_APB2ENR_SYSCFG            RCC_APB2ENR_SYSCFG_MASK








//Bu bölüm sayesinde maine sadece "stm32f407xx.h" include etmem yeterli olacaktır.

#include "RCC.h"
#include "GPIO.h"
#include "EXTI.h"

#endif /* INC_STM32F407XX_H_ */
