# README.md Kaynak Metni

````markdown
# STM32F407 Bare-Metal Driver Development

A bare-metal driver development track based on the Udemy course **Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)** by **Erhan Konak**.

The goal of this section is to build a reusable STM32F407 driver library from scratch at register level and then use that library in separate embedded projects.

No HAL (Hardware Abstraction Layer) peripheral drivers are used.

This track is kept outside the `course-N` numbering used for the Fastbit sequence because it follows a separate learning resource.

## Structure

```text
driver-development/
├── driver-library/
│   ├── Inc/
│   │   ├── stm32f407xx.h
│   │   ├── RCC.h
│   │   └── GPIO.h
│   └── Src/
│       ├── RCC.c
│       └── GPIO.c
│
├── driver-projects/
│
└── README.md
```

## Driver Library

`driver-library/` contains the reusable low-level driver code.

### Current Components

- `stm32f407xx.h`
  - Device-specific header for the STM32F407xx family
  - Memory and peripheral base addresses
  - APB1, APB2, AHB1 and AHB2 bus domains
  - Peripheral register structures
  - Peripheral pointer definitions
  - Register bit definitions
  - Common register manipulation macros

- `RCC.h / RCC.c`
  - RCC (Reset and Clock Control) driver infrastructure
  - GPIO peripheral clock enable/disable support

- `GPIO.h / GPIO.c`
  - GPIO (General Purpose Input/Output) driver
  - GPIO pin definitions
  - GPIO pin state abstraction
  - Register-level GPIO operations

Additional drivers will be added as the course progresses:

- SPI
- USART
- I2C

## Driver Projects

`driver-projects/` will contain small embedded applications developed using **my own driver library**.

The purpose of these projects is to use and test the reusable driver layer without placing low-level register operations directly inside application code.

Projects will be added as they are completed.

The separation is:

```text
driver-library/
→ reusable low-level driver implementation

driver-projects/
→ applications built using the driver library
```

The repository-level `projects/` directory remains separate and contains standalone embedded projects that are not specifically built to demonstrate this driver library.

## Purpose

The main objective of this track is to understand what happens underneath vendor libraries by implementing peripheral support directly from the STM32F407 reference manual and datasheet.

The project currently covers concepts such as:

- Memory-mapped I/O
- Peripheral memory maps
- Register structures
- Peripheral base addresses
- Clock control
- Bit masks and register manipulation
- GPIO pin abstraction
- Reusable driver interfaces

---

## Türkçe

Bu bölümde STM32F407 için kendi bare-metal sürücü kütüphanemi register seviyesinde geliştiriyorum.

Çalışma, Erhan Konak'ın Udemy'deki **Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)** kursunu temel alıyor.

Amaç yalnızca kurs kodlarını tekrar etmek değil; tekrar kullanılabilir bir driver altyapısı oluşturmak ve daha sonra bu driver'ları ayrı projelerde kullanmak.

HAL (Hardware Abstraction Layer) çevre birimi sürücüleri kullanılmıyor.

### Yapı

`driver-library/`, geliştirdiğim tekrar kullanılabilir düşük seviyeli sürücüleri içerir.

Şu anda:

- STM32F407 device-specific header
- RCC (Reset and Clock Control) altyapısı
- GPIO (General Purpose Input/Output) sürücüsü

bulunmaktadır.

Kurs ilerledikçe SPI, USART ve I2C sürücüleri de aynı kütüphaneye eklenecektir.

`driver-projects/` ise yalnızca kendi geliştirdiğim driver kütüphanesini kullanarak oluşturduğum uygulamaları içerecektir.

Böylece:

```text
driver-library/
→ sürücünün kendisi

driver-projects/
→ sürücüyü kullanan projeler
```

birbirinden ayrılmış olur.

Repository kökündeki `projects/` klasörü ise driver kütüphanesinden bağımsız genel embedded projeler için kullanılmaya devam eder.
````
