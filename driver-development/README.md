# STM32F407 Bare-Metal Driver Library

A reusable peripheral driver library for the STM32F407, written from scratch at register level using only the reference manual and datasheet.

No HAL (Hardware Abstraction Layer) or CMSIS (Cortex Microcontroller Software Interface Standard) peripheral drivers are used. Every register access is written and verified against [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation).

This track follows the Udemy course *Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)* by Erhan Konak. It is intentionally kept outside the `course-N` numbering used for the [Fastbit sequence](../) because it is a separate learning resource with a different structure.

## Status

| Component | Status | Scope |
|---|---|---|
| [`stm32f407xx.h`](driver-library/Inc/stm32f407xx.h) | Working | Base addresses, register structs, peripheral pointers, bit definitions |
| [`RCC`](driver-library/Inc/RCC.h) | Working | GPIO peripheral clock enable / disable |
| [`GPIO`](driver-library/Inc/GPIO.h) | In progress | Pin masks, pin state type, `GPIO_WritePin` done — `GPIO_Init` in development |
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
│   │   └── GPIO.h           # GPIO interface
│   └── Src/
│       ├── RCC.c
│       └── GPIO.c
│
├── driver-projects/         # applications built on top of the library
│
└── README.md
```

`driver-library/` is the reusable layer. `driver-projects/` will hold small applications that consume it, so that no application file ever touches a register directly.

The repository-level [`projects/`](../projects) directory is separate — it holds standalone embedded projects not built around this library.

`GPIO_WritePin` writes to BSRR (Bit Set/Reset Register) rather than performing a read-modify-write on ODR (Output Data Register), which makes the operation atomic and interrupt-safe.

## Design Notes

The library is organised in layers, and the layer boundary is the point of the exercise:

**1. Hardware description** — `stm32f407xx.h` answers *how is the silicon laid out?* Base addresses, register maps as structs, bit positions and masks. Nothing here knows what a driver is.

**2. Interface** — `RCC.h`, `GPIO.h` answer *what does a driver user get to call?* Symbolic pin masks, state enums, function prototypes.

**3. Implementation** — `RCC.c`, `GPIO.c` answer *how is that interface realised in registers?*

**4. Application** — code under `driver-projects/`. It should not know GPIOD's base address, BSRR's offset, or which half of BSRR resets a pin.

Two specific decisions worth recording:

- Register structs include explicit `RESERVED` members. The compiler lays struct members out contiguously, so gaps in the peripheral memory map must be padded or every member after the gap resolves to the wrong address.
- Clock-enable macros read the register back after writing it. On this bus architecture the write may not have taken effect by the time the next instruction accesses the peripheral; the read-back forces completion. The value is discarded through `UNUSED()`.

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

Kütüphane katmanlı bir yapıda: `stm32f407xx.h` donanımı tarif eder, `GPIO.h` / `RCC.h` kullanıcıya sunulan arayüzü tanımlar, `.c` dosyaları bu arayüzü register seviyesinde gerçekler, uygulama kodu ise register bilmez.

Mevcut durum: RCC (Reset and Clock Control) clock enable/disable çalışıyor, GPIO sürücüsü geliştirme aşamasında (`GPIO_WritePin` tamamlandı, `GPIO_Init` sürüyor). SPI, USART ve I2C kurs ilerledikçe eklenecek.

`driver-projects/` klasörü, bu kütüphaneyi kullanan küçük uygulamalar için ayrıldı. Repository kökündeki `projects/` klasörü ise kütüphaneden bağımsız genel projeler için kalmaya devam ediyor.