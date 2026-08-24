# STM32F407 Bare-Metal Driver Library

A reusable peripheral driver library for the STM32F407, written from scratch at register level using only the reference manual and datasheet.

No HAL (Hardware Abstraction Layer) or CMSIS (Cortex Microcontroller Software Interface Standard) peripheral drivers are used. Every register access is written and verified against [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation).

This track follows the Udemy course *Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)* by Erhan Konak. It is intentionally kept outside the `course-N` numbering used for the [Fastbit sequence](../) because it is a separate learning resource with a different structure.

## Status

| Component | Status | Scope |
|---|---|---|
| [`stm32f407xx.h`](driver-library/Inc/stm32f407xx.h) | Working | Base addresses, register structs (GPIO, RCC, SYSCFG, EXTI), peripheral pointers, bit definitions |
| [`RCC`](driver-library/Inc/RCC.h) | Working | Peripheral clock enable / disable for GPIO ports and SYSCFG |
| [`GPIO`](driver-library/Inc/GPIO.h) | In progress | Init, read, write, toggle, lock — alternate function (AFR) not yet implemented |
| [`EXTI`](driver-library/Inc/EXTI.h) | In progress | SYSCFG line routing, mask and edge configuration — NVIC setup and IRQ handlers pending |
| `SPI` | Planned | — |
| `USART` | Planned | — |
| `I2C` | Planned | — |

## Hardware and Toolchain

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| MCU | [STM32F407VGT6](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html) — ARM Cortex-M4F, 168 MHz |
| IDE | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) |
| Toolchain | GNU Arm Embedded (`arm-none-eabi-gcc`) |
| Language | C, bare-metal |

## Structure

```text
driver-development/
├── driver-library/          # the drivers themselves
│   ├── Inc/
│   │   ├── stm32f407xx.h    # device header — hardware description
│   │   ├── RCC.h            # clock control interface
│   │   ├── GPIO.h           # GPIO interface
│   │   └── EXTI.h           # external interrupt interface
│   └── Src/
│       ├── RCC.c
│       ├── GPIO.c
│       └── EXTI.c
│
├── driver-projects/         # applications built on top of the library
│   ├── 01-four-led-on/
│   ├── 02-button-controlled-led/
│   └── 03-exti-configuration/
│
└── README.md
```

`driver-library/` is the reusable layer. `driver-projects/` holds small applications that consume it, so that no application file ever touches a register directly.

The repository-level [`projects/`](../projects) directory is separate — it holds standalone embedded projects not built around this library.

## Usage

Driving an LED:

```c
#include "stm32f407xx.h"

RCC_GPIOD_CLK_ENABLE();

GPIO_InitTtypedef cfg = {0};
cfg.pinNumber = GPIO_PIN_12;
cfg.Mode      = GPIO_MODE_OUTPUT;
cfg.Otype     = GPIO_OTYPE_PP;
cfg.Speed     = GPIO_SPEED_LOW;
cfg.PuPd      = GPIO_PUPD_NOPULL;
GPIO_Init(GPIOD, &cfg);

GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_Pin_Set);
```

Routing a pin to an EXTI line and configuring the trigger:

```c
RCC_SYSCFG_CLK_ENABLE();
RCC_GPIOC_CLK_ENABLE();

EXTI_InitTypedef_t exti = {0};
exti.EXTI_LineCMD    = ENABLE;
exti.EXTI_LineNumber = EXTI_LineSource_10;
exti.EXTI_Mode       = EXTI_Mode_Interrupt;
exti.TriggerMode     = EXTI_Trigger_Rising_Offset;

EXTI_LineConfig(EXTI_PortSource_GPIOC, EXTI_LineSource_10);
EXTI_Init(&exti);
```

## Design Notes

The library is organised in layers, and the layer boundary is the point of the exercise:

**1. Hardware description** — `stm32f407xx.h` answers *how is the silicon laid out?* Base addresses, register maps as structs, bit positions and masks. Nothing here knows what a driver is.

**2. Interface** — `RCC.h`, `GPIO.h`, `EXTI.h` answer *what does a driver user get to call?* Symbolic pin masks, state enums, function prototypes.

**3. Implementation** — `RCC.c`, `GPIO.c`, `EXTI.c` answer *how is that interface realised in registers?*

**4. Application** — code under `driver-projects/`. It should not know GPIOD's base address, BSRR's offset, or which half of BSRR resets a pin.

Decisions worth recording:

- **Register structs include explicit `RESERVED` members.** The compiler lays struct members out contiguously, so gaps in the peripheral memory map must be padded or every member after the gap resolves to the wrong address. RCC has six such gaps; SYSCFG has one, between `EXTICR[4]` and `CMPCR`.

- **Clock-enable macros read the register back after writing it.** On this bus architecture the write may not have taken effect by the time the next instruction accesses the peripheral; the read-back forces completion. The value is discarded through `UNUSED()`.

- **`GPIO_WritePin` uses BSRR, not ODR.** Writing to BSRR is a single, indivisible store: it expresses the change as a delta rather than an absolute value, so no read-modify-write is needed and no interrupt can corrupt it.

- **Bit-field width drives the shift arithmetic.** OTYPER gives each pin 1 bit (shift by `position`), MODER/OSPEEDR/PUPDR give 2 bits (shift by `2 * position`), and SYSCFG's EXTICR gives 4 bits spread across four registers (index by `line >> 2`, shift by `(line & 3) * 4`). Same pattern, different partitioning.

- **EXTI has no clock enable bit of its own.** It is part of the processor's interrupt fabric rather than a vendor peripheral, so `RCC_APB2ENR` has no EXTI entry. What does need enabling is SYSCFG — because the `EXTICR` registers that route a port to a line belong to SYSCFG — and the clock of the GPIO port being watched.

- **`EXTI_Mode` and `TriggerMode` hold register offsets, not enum-like codes.** `EXTI_Mode_Interrupt` is `0x00` (IMR) and `EXTI_Mode_Event` is `0x04` (EMR); the trigger values are the offsets of RTSR, FTSR and a sentinel for "both". The driver adds the offset to the EXTI base address and writes through the resulting pointer. This mirrors ST's older SPL style; the alternative — plain `if`/`else` on `EXTI->IMR` and `EXTI->EMR` — avoids offset arithmetic entirely but was not used here in order to follow the course's structure.

## References

- [RM0090 Reference Manual](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — register-level source of truth
- [STM32F407VG datasheet](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — pinout, electrical characteristics
- [STM32F4DISCOVERY user manual and schematic](https://www.st.com/en/evaluation-tools/stm32f4discovery.html#documentation) — board wiring, LED and button pin assignments
- [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/) — core architecture
- Further reading is collected in [`RESOURCES.md`](../RESOURCES.md)

---

## Türkçe Özet

STM32F407 için register seviyesinde, sıfırdan yazılmış tekrar kullanılabilir bir sürücü kütüphanesi. HAL ve CMSIS çevre birimi sürücüleri kullanılmıyor; tüm register erişimleri RM0090 referans kılavuzuyla doğrulanıyor.

Erhan Konak'ın *Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)* Udemy kursunu temel alıyor. Fastbit serisinin `course-N` numaralandırmasından ayrı tutulmasının sebebi farklı bir kaynak olması.

Kütüphane katmanlı bir yapıda: `stm32f407xx.h` donanımı tarif eder, `GPIO.h` / `RCC.h` / `EXTI.h` kullanıcıya sunulan arayüzü tanımlar, `.c` dosyaları bu arayüzü register seviyesinde gerçekler, uygulama kodu ise register bilmez.

Mevcut durum: RCC (Reset and Clock Control) clock enable/disable çalışıyor. GPIO sürücüsünde init, read, write, toggle ve lock tamamlandı; alternatif fonksiyon (AFR) desteği henüz yok. EXTI (External Interrupt/Event Controller) tarafında SYSCFG hat yönlendirmesi, maske ve kenar yapılandırması yazıldı; NVIC (Nested Vectored Interrupt Controller) ayarı ve kesme işleyicileri bekliyor. SPI, USART ve I2C kurs ilerledikçe eklenecek.

`driver-projects/` klasörü, bu kütüphaneyi kullanan küçük uygulamalar için ayrıldı. Repository kökündeki `projects/` klasörü ise kütüphaneden bağımsız genel projeler için kalmaya devam ediyor.