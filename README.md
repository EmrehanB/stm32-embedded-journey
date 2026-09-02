# STM32 Embedded Journey

Embedded systems programming on STM32, from bare-metal upwards.

**Board:** [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) · **MCU:** [STM32F407VGT6](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html) (ARM Cortex-M4) · **Toolchain:** [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

Everything here is written at register level against [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — no HAL (Hardware Abstraction Layer), no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral drivers. The aim is to understand what vendor libraries do underneath rather than to call them.

Each exercise lives in its own folder with a dedicated README explaining the approach. The repo grows with new sections as the courses progress.

See [RESOURCES.md](./RESOURCES.md) for courses, reference documents, and articles used throughout this repo.

## Contents

| Section | Focus | Status |
|---|---|---|
| [Course 1](#course-1--bare-metal-embedded-c) | Bare-metal embedded C | Completed — 11 exercises |
| [Course 2](#course-2--arm-cortex-m-architecture) | Cortex-M processor architecture | Paused at memory map & bus interfaces |
| [Driver Development](#driver-development) | Reusable peripheral driver library | Active |
| [Projects](#projects) | Standalone applications | Ongoing |

---

## Course 1 — Bare-metal Embedded C

Register-level bare-metal C, no HAL. GPIO, `volatile`, structs/unions, bit-fields, and register modeling.

| # | Exercise | Topic | Details |
|---|----------|-------|---------|
| 01 | LED on — PD12 | GPIO output, bare-metal | [01-led-pd12](./course-1-embedded-c/01-led-pd12) |
| 02 | LED on — PD15 | GPIO output, register tracing | [02-led6-pd15](./course-1-embedded-c/02-led6-pd15) |
| 03 | LED on — PD12 (bitwise) | Bit set/clear with shift operators | [03-bitwise-shift-led](./course-1-embedded-c/03-bitwise-shift-led) |
| 04 | LED toggle — PD12 | Blink loop with busy-wait delay | [04-led-toggle](./course-1-embedded-c/04-led-toggle) |
| 05 | Input-controlled LED | GPIO input (PA0) drives output (PD12) | [05-input-controlled-led](./course-1-embedded-c/05-input-controlled-led) |
| 06 | Volatile pin read | I/O-safe input using `volatile` | [06-volatile-pin-read](./course-1-embedded-c/06-volatile-pin-read) |
| 07 | const + volatile registers | Correct qualifiers for input vs output registers | [07-const-volatile-pin-read](./course-1-embedded-c/07-const-volatile-pin-read) |
| 08 | Struct padding | Data alignment & padding, shown byte by byte | [08-padding-test](./course-1-embedded-c/08-padding-test) |
| 09 | Packet bit-field parsing | Decode a 32-bit packet with shift & mask | [09-packet-parsing](./course-1-embedded-c/09-packet-parsing) |
| 10 | Union-based packet parsing | Same packet via union + struct; portability trade-off | [10-union-packet-parsing](./course-1-embedded-c/10-union-packet-parsing) |
| 11 | Register struct model | Registers as bit-field structs in a header, accessed via `->` | [11-register-struct-model](./course-1-embedded-c/11-register-struct-model) |

---

## Course 2 — ARM Cortex-M Architecture

Cortex-M3/M4 processor internals: operational modes, register set, MSP/PSP banked stack, exception & interrupt handling via NVIC (Nested Vectored Interrupt Controller), memory map, bus interfaces, bit-banding, bootloader/IAP.

Reference: [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/).

| # | Exercise | Topic | Details |
|---|----------|-------|---------|
| 01 | Inline asm: add in memory | Inline assembly, LDR/STR, load-store architecture | [01-inline-asm-add](./course-2-cortex-m/01-inline-asm-add) |
| 02 | Inline asm: constraints & MRS | Constraint strings (r/=r/i), reading CONTROL with MRS | [02-inline-asm-constraints](./course-2-cortex-m/02-inline-asm-constraints) |

Currently paused at the memory map and bus interface section; resumes after the driver development track.

---

## Driver Development

A reusable STM32F407 peripheral driver library built from scratch, following the Udemy course *Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)* by Erhan Konak. Kept outside the `course-N` numbering since it's a separate source, not part of the Fastbit sequence above.

Split into two layers: `driver-library/` holds the drivers themselves, `driver-projects/` holds applications that consume them — so application code never touches a register directly.

| Component | Status |
|---|---|
| `stm32f407xx.h` — device header, base addresses, register structs | Working |
| `RCC` — peripheral clock enable/disable | Working |
| `GPIO` — init, read, write, toggle, lock | In progress |
| `EXTI` — line routing, edge config, NVIC enable | Working |
| `SPI` — register struct, clock enable, init | In progress |
| `USART` · `I2C` | Planned |
Details: [driver-development](./driver-development)

---

## Projects

Standalone projects combining skills from completed exercises — not tied to a specific course.

| Project | Description | Details |
|---|---|---|
| Reaction Timer | Bare-metal reaction time game: random wait, button input, busy-wait timing | [reaction-timer](./projects/reaction-timer) |
| GPIOD LED Patterns | Onboard LED (PD12-PD15) blink sequence, written during a power outage | [gpiod-led-patterns](./projects/gpiod-led-patterns) |

---

## Türkçe

STM32 üzerinde bare-metal'den ileri seviyeye uzanan embedded sistem programlama yolculuğu. Tüm kod register seviyesinde, RM0090 referans kılavuzuna karşı yazılıyor; HAL ve CMSIS çevre birimi sürücüleri kullanılmıyor.

Repo dört bölümden oluşuyor: **Course 1** bare-metal embedded C (tamamlandı, 11 alıştırma), **Course 2** ARM Cortex-M işlemci mimarisi (bellek haritası bölümünde duraklatıldı), **Driver Development** kendi sürücü kütüphanem — GPIO/EXTI/SPI/USART/I2C (aktif olarak geliştiriliyor), **Projects** ise kurs alıştırmalarından bağımsız, kazanılan becerileri birleştiren kendi projelerim.

Her alıştırma klasörü kendi README dosyasıyla birlikte, yaklaşımı açıklayarak duruyor.