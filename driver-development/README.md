# MCU Driver Development (GPIO, SPI, USART, I2C)

A driver development track: [Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)](https://www.udemy.com/course/mikrodenetleyici-driver-gelistirme-gpio-spi-usart-i2c/learn/) on Udemy, by **Erhan Konak**.

Register-level, no HAL (Hardware Abstraction Layer) or CMSIS peripheral drivers — same bare-metal philosophy as Course 1.

Kept outside the `course-N` numbering used for the Fastbit sequence above, since it's a separate source.

## Contents

| File | Description |
|---|---|
| [`stm32f407xx.h`](./stm32f407xx.h) | Device-specific header for STM32F407xx — memory and peripheral base addresses (Flash/SRAM, APB1/APB2/AHB1/AHB2 bus domains, GPIO/TIM/SPI/USART/I2C/SYSCFG/EXTI/RCC). Scoped to what this course covers, not a full CMSIS-equivalent header. |

More files (driver source per peripheral) will be added here as they're completed.

---

## Türkçe

GPIO, SPI, USART, I2C sürücülerini register seviyesinde (HAL'siz) geliştirmeyi öğrendiğim bir Udemy kursunu takip ettiğim bölüm. Eğitmen **Erhan Konak** — Udemy'den takip ettiğim ve anlatımını çok sevdiğim bir eğitmen. Fastbit dizisinden ayrı bir kaynak olduğu için `course-N` numaralandırmasına dahil edilmedi.

İlk dosya `stm32f407xx.h` — STM32F407xx için cihaza özel başlık dosyası, bellek ve çevre birimi taban adreslerini içeriyor. Alıştırmalar tamamlandıkça buraya eklenecek.