# STM32 Embedded Journey

Bare-metal'den ileri seviyeye, STM32 üzerinde embedded sistem programlama yolculuğu.

**Board:** STM32F407G-DISC1 (Discovery) · **MCU:** STM32F407VGT6 (ARM Cortex-M4) · **Toolchain:** STM32CubeIDE

Her alıştırma kendi klasöründe, o alıştırmayı anlatan ayrı bir README ile birlikte bulunur. Kurslar ilerledikçe repo yeni bölümlerle genişler.

---

## Course 1 — Bare-metal Embedded C

Register seviyesinde, HAL kullanmadan bare-metal C. GPIO, `volatile`, struct/union, bit-field ve register modelleme.

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

Cortex-M3/M4 işlemci mimarisi: çalışma modları, register set, MSP/PSP stack, exception/interrupt yönetimi (NVIC), memory map, bus arayüzleri, bit-banding, bootloader/IAP.

| # | Exercise | Topic | Details |
|---|----------|-------|---------|
| — | _(devam ediyor)_ | | [course-2-cortex-m](./course-2-cortex-m) |

---

## Türkçe

STM32 üzerinde bare-metal'den ileri seviyeye uzanan embedded sistem programlama yolculuğu. Her alıştırmanın kendi klasörü ve o alıştırmayı anlatan ayrı bir README'si var. Repo iki bölümden oluşuyor: **Course 1** bare-metal embedded C (register seviyesi, HAL'siz), **Course 2** ARM Cortex-M işlemci mimarisi. Kurslar ilerledikçe yeni bölümler eklenecek.