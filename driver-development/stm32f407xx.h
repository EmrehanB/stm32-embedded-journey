

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_




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
















#endif /* INC_STM32F407XX_H_ */
