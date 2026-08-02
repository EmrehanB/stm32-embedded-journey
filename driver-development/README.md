# Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)

A driver development track: [Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)](https://www.udemy.com/course/mikrodenetleyici-driver-gelistirme-gpio-spi-usart-i2c/learn/) on Udemy, by **Erhan Konak**.

Register-level, no HAL (Hardware Abstraction Layer) or CMSIS peripheral drivers — same bare-metal philosophy as Course 1.

Kept outside the `course-N` numbering used for the Fastbit sequence above, since it's a separate source.

## Structure

```
driver-development/
├── Src/
│   └── main.c
└── myDrivers/
    ├── Inc/
    │   ├── stm32f407xx.h
    │   └── RCC.h
    └── Src/
        └── RCC.c
```

Driver library code (`myDrivers/`) is kept separate from application code (`Src/main.c`) — headers in `Inc/`, implementation in `Src/`.

## Contents

| File | Description |
|---|---|
| [`myDrivers/Inc/stm32f407xx.h`](./myDrivers/Inc/stm32f407xx.h) | Device-specific header for STM32F407xx — memory and peripheral base addresses (Flash/SRAM, APB1/APB2/AHB1/AHB2 bus domains), register-map structs (`GPIO_TypeDef_t`, `RCC_typDef_t`) with reserved-offset padding matched to the reference manual. |
| [`myDrivers/Inc/RCC.h`](./myDrivers/Inc/RCC.h) / [`myDrivers/Src/RCC.c`](./myDrivers/Src/RCC.c) | RCC (Reset and Clock Control) driver — first peripheral-specific driver in this track, split into header/implementation. |
| [`Src/main.c`](./Src/main.c) | Application code using the drivers above. |

More peripheral drivers (GPIO, SPI, USART, I2C) will be added under `myDrivers/` as they're completed.

---

## Türkçe

GPIO, SPI, USART, I2C sürücülerini register seviyesinde (HAL'siz) geliştirmeyi öğrendiğim bir Udemy kursunu takip ettiğim bölüm. Eğitmen **Erhan Konak** — Udemy'den takip ettiğim ve anlatımını çok sevdiğim bir eğitmen. Fastbit dizisinden ayrı bir kaynak olduğu için `course-N` numaralandırmasına dahil edilmedi.

Sürücü kütüphanesi (`myDrivers/`) uygulama kodundan (`Src/main.c`) ayrı tutuluyor — header'lar `Inc/` altında, implementasyon `Src/` altında. İlk peripheral-özel sürücü RCC (Reset and Clock Control) — GPIO, SPI, USART, I2C sürücüleri tamamlandıkça eklenecek.