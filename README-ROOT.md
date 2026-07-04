# Embedded C Exercises

Bare-metal embedded C exercises on STM32, written without HAL — accessing peripheral registers directly through memory addresses.

**Board:** STM32F407G-DISC1 (Discovery) · **MCU:** STM32F407VGT6 (ARM Cortex-M4) · **Toolchain:** STM32CubeIDE

Each exercise lives in its own folder with a dedicated README explaining the approach.

## Exercises

| # | Exercise | Topic | Details |
|---|----------|-------|---------|
| 01 | LED on — PD12 | GPIO output, bare-metal | [01-led-pd12](./01-led-pd12) |
| 02 | LED on — PD15 | GPIO output, register tracing | [02-led6-pd15](./02-led6-pd15) |
| 03 | LED on — PD12 (bitwise) | Bit set/clear with shift operators | [03-bitwise-shift-led](./03-bitwise-shift-led) |
| 04 | LED toggle — PD12 | Blink loop with busy-wait delay | [04-led-toggle](./04-led-toggle) |
| 05 | Input-controlled LED | GPIO input (PA0) drives output (PD12) | [05-input-controlled-led](./05-input-controlled-led) |
| 06 | Volatile pin read | O2-safe input using `volatile` | [06-volatile-pin-read](./06-volatile-pin-read) |
| 07 | const + volatile registers | Correct qualifiers for input vs output registers | [07-const-volatile-pin-read](./07-const-volatile-pin-read) |
| 08 | Struct padding | Data alignment & padding, shown byte by byte | [08-padding-test](./08-padding-test) |

---

## Türkçe

STM32 üzerinde, HAL kullanmadan register seviyesinde yazılmış bare-metal embedded C alıştırmaları. Çevre birimlerinin register'larına doğrudan bellek adresleriyle erişiyorum. Her alıştırmanın kendi klasörü ve o alıştırmayı anlatan ayrı bir README'si var.
