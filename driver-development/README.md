# STM32F407 Bare-Metal Driver Library

A reusable peripheral driver library for the STM32F407, written from scratch at register level using only the reference manual and datasheet.

No HAL (Hardware Abstraction Layer) or CMSIS (Cortex Microcontroller Software Interface Standard) peripheral drivers are used. Every register access is written and verified against [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation).

This track follows the Udemy course *Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)* by Erhan Konak. It is intentionally kept outside the `course-N` numbering used for the [Fastbit sequence](../) because it is a separate learning resource with a different structure.

## Status

| Component | Status | Scope |
|---|---|---|
| [`stm32f407xx.h`](driver-library/Inc/stm32f407xx.h) | Working | Base addresses, register structs (GPIO, RCC, SYSCFG, EXTI, SPI), peripheral pointers, bit definitions |
| [`RCC`](driver-library/Inc/RCC.h) | Working | Peripheral clock enable / disable for GPIO ports, SYSCFG and SPI1–SPI4 |
| [`GPIO`](driver-library/Inc/GPIO.h) | In progress | Init, read, write, toggle, lock — alternate function (AFR) not yet implemented |
| [`EXTI`](driver-library/Inc/EXTI.h) | Working | SYSCFG line routing, mask and edge configuration, NVIC interrupt enable |
| [`SPI`](driver-library/Inc/SPI.h) | In progress | Init, peripheral enable, polled transmit, flag status — receive and interrupt-driven transfer pending |
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
│   │   ├── EXTI.h           # external interrupt interface
│   │   └── SPI.h            # SPI interface
│   └── Src/
│       ├── RCC.c
│       ├── GPIO.c
│       ├── EXTI.c
│       └── SPI.c
│
├── driver-projects/         # applications built on top of the library
│   ├── 01-four-led-on/
│   ├── 02-button-controlled-led/
│   ├── 03-exti-configuration/
│   └── 04-button-interrupt/
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

Configuring SPI1 as a master and sending a buffer:

```c
RCC_SPI1_CLK_ENABLE();

SPI_HandleTypeDef_t spi = {0};
spi.Instance         = SPI1;
spi.Init.Mode        = SPI_MODE_MASTER;
spi.Init.CPOL        = SPI_CPOL_LOW;
spi.Init.CPHA        = SPI_CPHA_FirstEdge;
spi.Init.BaudRate    = SPI_BAUDRATE_DIV16;
spi.Init.DFF         = SPI_DFF_8BITS;
spi.Init.FrameFormat = SPI_FrameFormat_MSB;
spi.Init.BusConfig   = SPI_BUS_FullDuplex;
spi.Init.SSM_Cmd     = SPI_SSM_ENABLE;

SPI_Init(&spi);
SPI_PeriphCmd(&spi, ENABLE);

uint8_t buf[] = {0x0A, 0x0B, 0x0C};
SPI_TransmitData(&spi, buf, sizeof(buf));
```

Setting up a rising-edge interrupt on PA0:

```c
RCC_SYSCFG_CLK_ENABLE();
RCC_GPIOA_CLK_ENABLE();

EXTI_InitTypedef_t exti = {0};
exti.EXTI_LineCMD    = ENABLE;
exti.EXTI_LineNumber = EXTI_LineSource_0;
exti.EXTI_Mode       = EXTI_Mode_Interrupt;
exti.TriggerMode     = EXTI_Trigger_Rising_Offset;

EXTI_LineConfig(EXTI_PortSource_GPIOA, EXTI_LineSource_0);
EXTI_Init(&exti);
NVIC_EnableInterrupt(EXTI0_IRQNumber);
```

The handler is a plain C function whose name must match the startup file's vector table entry:

```c
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & 0x1U) {
        EXTI->PR = 0x1U;          // rc_w1: writing 1 clears
        GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_Pin_Set);
    }
}
```

## Design Notes

The library is organised in layers, and the layer boundary is the point of the exercise:

**1. Hardware description** — `stm32f407xx.h` answers *how is the silicon laid out?* Base addresses, register maps as structs, bit positions and masks. Nothing here knows what a driver is.

**2. Interface** — `RCC.h`, `GPIO.h`, `EXTI.h`, `SPI.h` answer *what does a driver user get to call?* Symbolic pin masks, state enums, function prototypes.

**3. Implementation** — `RCC.c`, `GPIO.c`, `EXTI.c`, `SPI.c` answer *how is that interface realised in registers?*

**4. Application** — code under `driver-projects/`. It should not know GPIOD's base address, BSRR's offset, or which half of BSRR resets a pin.

Decisions worth recording:

- **Register structs include explicit `RESERVED` members.** The compiler lays struct members out contiguously, so gaps in the peripheral memory map must be padded or every member after the gap resolves to the wrong address. RCC has six such gaps; SYSCFG has one, between `EXTICR[4]` and `CMPCR`.

- **Clock-enable macros read the register back after writing it.** On this bus architecture the write may not have taken effect by the time the next instruction accesses the peripheral; the read-back forces completion. The value is discarded through `UNUSED()`.

- **`GPIO_WritePin` uses BSRR, not ODR.** Writing to BSRR is a single, indivisible store: it expresses the change as a delta rather than an absolute value, so no read-modify-write is needed and no interrupt can corrupt it.

- **Bit-field width drives the shift arithmetic.** OTYPER gives each pin 1 bit (shift by `position`), MODER/OSPEEDR/PUPDR give 2 bits (shift by `2 * position`), and SYSCFG's EXTICR gives 4 bits spread across four registers (index by `line >> 2`, shift by `(line & 3) * 4`). Same pattern, different partitioning.

- **EXTI has no clock enable bit of its own.** It is part of the processor's interrupt fabric rather than a vendor peripheral, so `RCC_APB2ENR` has no EXTI entry. What does need enabling is SYSCFG — because the `EXTICR` registers that route a port to a line belong to SYSCFG — and the clock of the GPIO port being watched.

- **An interrupt needs two independent enables.** `EXTI->IMR` releases the line from the EXTI block; `NVIC->ISER` allows the IRQ to reach the core. Setting only one produces no interrupt and no diagnostic.

- **`EXTI->PR` and `NVIC->ISER` are written directly, not read-modify-write.** Both are write-1-to-act registers: bits written as 1 take effect, bits written as 0 are ignored. A read adds nothing and widens the window for losing a concurrent event — the same argument that favours BSRR over ODR.

- **SPI configuration values are stored pre-shifted.** `SPI_BAUDRATE_DIV16` is `0x18`, not `3` — the value already sits at bit 3 where `CR1` expects it. This is ST's SPL convention and it keeps `SPI_Init` free of per-field shift arithmetic, since every `CR1` field has a fixed position. GPIO could not do this: there the shift depends on the pin number and is not constant.

- **Configuration must finish before the peripheral is enabled.** `SPI_Init` writes `CR1` while SPE is still 0; `SPI_PeriphCmd` sets SPE afterwards. Changing CPOL, CPHA, baud rate or MSTR while SPE is set is undefined behaviour, so the two steps are deliberately separate functions.

- **`TXE` does not mean the byte has left the wire.** SPI is double-buffered: data goes to a transmit buffer first and only then into the shift register. `TXE` reports that the buffer drained, not that transmission finished — that is what `BSY` is for. Disabling SPI on `TXE` alone truncates the last byte.

- **A two-level handle separates the instance from the configuration.** `SPI_HandleTypeDef_t` holds a pointer to the peripheral (`SPI1`–`SPI4`) alongside the configuration struct, so one driver call carries both *which* SPI and *how* it should behave.

- **A peripheral's bus determines which clock-enable register it uses.** GPIO sits on AHB1 (`AHB1ENR`), SYSCFG on APB2 (`APB2ENR`). SPI is split: SPI1 and SPI4 are on APB2 and run at up to 84 MHz, while SPI2 and SPI3 are on APB1 at 42 MHz. The first question when adding any peripheral is which bus it hangs off.

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

Mevcut durum: RCC (Reset and Clock Control) clock enable/disable çalışıyor. GPIO sürücüsünde init, read, write, toggle ve lock tamamlandı; alternatif fonksiyon (AFR) desteği henüz yok. EXTI (External Interrupt/Event Controller) tarafında SYSCFG hat yönlendirmesi, maske ve kenar yapılandırması ile NVIC (Nested Vectored Interrupt Controller) kesme etkinleştirme tamamlandı; kesme işleyicileri uygulama tarafında yazılıyor. SPI (Serial Peripheral Interface) tarafında init, çevre birimi etkinleştirme, yoklama (polling) tabanlı veri gönderme ve bayrak okuma tamamlandı; veri alma ve kesme tabanlı aktarım bekliyor. USART ve I2C kurs ilerledikçe gelecek.

`driver-projects/` klasörü, bu kütüphaneyi kullanan küçük uygulamalar için ayrıldı. Repository kökündeki `projects/` klasörü ise kütüphaneden bağımsız genel projeler için kalmaya devam ediyor.