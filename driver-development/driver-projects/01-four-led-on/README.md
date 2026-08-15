# 01 — Four LEDs On

The first application built on top of my own driver library.

All four onboard LEDs of the STM32F407G-DISC1 (green PD12, orange PD13, red PD14, blue PD15) are turned on simultaneously — without a single line of vendor code.

## What "no HAL" means here

No HAL (Hardware Abstraction Layer) and no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral driver is used. Every layer under `main.c` was written by me from the [RM0090 reference manual](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation):

| Layer | What it does | Where |
|---|---|---|
| `stm32f407xx.h` | Peripheral base addresses, register maps as structs, bit definitions | [driver-library/Inc](../../driver-library/Inc) |
| `RCC_GPIOD_CLK_ENABLE()` | Sets the GPIOD enable bit in RCC (Reset and Clock Control) `AHB1ENR` | [RCC.h](../../driver-library/Inc/RCC.h) |
| `GPIO_Init()` | Walks the pin mask and writes MODER, OTYPER, OSPEEDR, PUPDR | [GPIO.c](../../driver-library/Src/GPIO.c) |
| `GPIO_WritePin()` | Atomic set/reset through BSRR (Bit Set/Reset Register) | [GPIO.c](../../driver-library/Src/GPIO.c) |

`main.c` itself never touches a register. It doesn't know GPIOD's base address, MODER's bit layout, or which half of BSRR resets a pin. That separation is the point of the exercise.

## Configuration

| Setting | Value | Reason |
|---|---|---|
| Mode | Output | LEDs are driven, not read |
| Output type | Push-pull | The pin actively drives both high and low |
| Pull-up / pull-down | None | The output stage already defines the level |
| Speed | Low | An LED needs no fast edges; higher speed only costs power and adds noise |

Pin selection uses bit masks rather than pin indices, so a single call configures and drives all four pins at once:

```c
GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15   // 0xF000
```

The config struct is zero-initialised with `{0}` so that no member is left holding an indeterminate value.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| LEDs | PD12 green · PD13 orange · PD14 red · PD15 blue |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

Pin assignments are taken from the board user manual, not assumed.

---

## Türkçe

Kendi yazdığım sürücü kütüphanesinin üzerine kurulan ilk uygulama.

STM32F407G-DISC1 kartındaki dört LED (PD12 yeşil, PD13 turuncu, PD14 kırmızı, PD15 mavi) aynı anda yakılıyor — tek satır hazır kütüphane kodu kullanmadan.

### "HAL'siz" ne demek

HAL (Hardware Abstraction Layer) ve CMSIS (Cortex Microcontroller Software Interface Standard) çevre birimi sürücüleri kullanılmıyor. `main.c` altındaki her katmanı RM0090 referans kılavuzuna bakarak kendim yazdım:

- `stm32f407xx.h` — çevre birimi base adresleri, register haritalarının struct karşılıkları, bit tanımları
- `RCC_GPIOD_CLK_ENABLE()` — RCC (Reset and Clock Control) `AHB1ENR` registerındaki GPIOD clock bitini set eder
- `GPIO_Init()` — pin maskesini dolaşıp MODER, OTYPER, OSPEEDR ve PUPDR registerlarını yazar
- `GPIO_WritePin()` — BSRR (Bit Set/Reset Register) üzerinden atomik set/reset yapar

`main.c` hiçbir registera dokunmuyor. GPIOD'nin base adresini, MODER'in bit yerleşimini veya BSRR'nin hangi yarısının reset yaptığını bilmiyor. Bu ayrım çalışmanın asıl amacı.

### Konfigürasyon

Çıkış modu, push-pull, pull-up/pull-down yok, düşük hız. Düşük hız tercih edildi çünkü LED sürmek hızlı kenar gerektirmiyor; yüksek hız yalnızca güç tüketimini ve gürültüyü artırır.

Pin seçimi indeks yerine bit maskesiyle yapılıyor, bu sayede dört pin tek çağrıda hem konfigüre ediliyor hem sürülüyor. Konfigürasyon struct'ı `{0}` ile sıfırlanıyor ki hiçbir üye tanımsız değer taşımasın.