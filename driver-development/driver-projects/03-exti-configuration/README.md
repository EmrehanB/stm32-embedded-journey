# 03 — EXTI Configuration

Routing a GPIO pin to an external interrupt line and configuring its trigger — the register-level groundwork that has to exist before an interrupt can ever fire.

This project does **not** produce a working interrupt yet. It configures SYSCFG and EXTI and stops there, so the resulting register contents can be inspected in the debugger. NVIC (Nested Vectored Interrupt Controller) setup and the IRQ handler come next.

## No HAL

No HAL (Hardware Abstraction Layer) and no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral driver is used. The SYSCFG and EXTI register maps, the bit definitions and both driver functions were written by hand from [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation).

| Layer | Responsibility |
|---|---|
| [`stm32f407xx.h`](../../driver-library/Inc/stm32f407xx.h) | SYSCFG and EXTI register structs, base addresses, bit definitions |
| [`RCC.h`](../../driver-library/Inc/RCC.h) | SYSCFG and GPIO port clock enable |
| [`EXTI.c`](../../driver-library/Src/EXTI.c) | `EXTI_LineConfig()` — port-to-line routing; `EXTI_Init()` — mask and edge selection |

## What gets configured

PC10 is routed to EXTI line 10, rising-edge triggered, interrupt mode.

| Step | Register | What happens |
|---|---|---|
| 1 | `RCC->APB2ENR` | SYSCFG clock enabled |
| 2 | `RCC->AHB1ENR` | GPIOC clock enabled |
| 3 | `SYSCFG->EXTICR[2]` | Bits 11:8 set to port C, binding PC10 to line 10 |
| 4 | `EXTI->IMR` | Bit 10 set — the line may reach the NVIC |
| 5 | `EXTI->RTSR` | Bit 10 set — rising edges are detected |

## Why SYSCFG, and why no EXTI clock

EXTI has no clock enable bit anywhere in RCC. It belongs to the processor's interrupt fabric rather than to the vendor peripheral set, so `RCC_APB2ENR` simply has no EXTI entry.

Two clocks *are* required, for different reasons:

**SYSCFG** — because the `EXTICR` registers live in SYSCFG, not in EXTI. Line 10 can be driven by PA10, PB10, PC10 and so on, but only one at a time; `EXTICR` is what makes the choice.

**The GPIO port** — because without its clock the pin's state never reaches the edge detector at all.

## The EXTICR index arithmetic

Each line needs 4 bits to name its port. Sixteen lines × 4 bits = 64 bits, which does not fit in one 32-bit register, so ST provides four registers holding four lines each.

```c
register index = line >> 2          // 10 >> 2 = 2   → EXTICR[2]
bit position   = (line & 3) * 4     // (10 & 3) * 4 = 8
```

So PC10 lives in bits 11:8 of `EXTICR[2]`. The same pattern reappears with different widths across the GPIO registers: 1 bit per pin in OTYPER, 2 bits in MODER, 4 bits in EXTICR and AFR.

## Verifying it

Set a breakpoint after `EXTI_Init()` and read the SFR view:

| Register | Expected |
|---|---|
| `SYSCFG->EXTICR[2]` | `0x0200` — port C in bits 11:8 |
| `EXTI->IMR` | bit 10 set |
| `EXTI->RTSR` | bit 10 set |
| `EXTI->FTSR` | bit 10 clear |
| `EXTI->EMR` | bit 10 clear |

Register inspection is the only meaningful test at this stage — with no NVIC configuration and no handler, nothing observable happens on the board.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| Pin | PC10 |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

---

## Türkçe

Bir GPIO pinini harici kesme hattına yönlendirmek ve tetikleme kenarını yapılandırmak — bir kesmenin oluşabilmesi için önce var olması gereken register seviyesi altyapı.

Bu proje **henüz çalışan bir kesme üretmiyor.** SYSCFG ve EXTI yapılandırılıyor, orada duruluyor; böylece registerların son hali debugger'da incelenebiliyor. NVIC (Nested Vectored Interrupt Controller) ayarı ve kesme işleyicisi bir sonraki adım.

HAL ve CMSIS çevre birimi sürücüleri kullanılmıyor. SYSCFG ve EXTI register haritaları, bit tanımları ve her iki sürücü fonksiyonu RM0090'a bakılarak elle yazıldı.

### Yapılandırma

PC10, EXTI hattı 10'a bağlanıyor; yükselen kenar, kesme modu.

### Neden SYSCFG, neden EXTI için clock yok

EXTI'nin RCC'de hiçbir clock enable biti yoktur. İşlemcinin kesme altyapısının parçasıdır, üretici çevre birimi değildir — bu yüzden `RCC_APB2ENR` içinde EXTI satırı bulunmaz.

Buna karşılık iki clock gerekiyor:

**SYSCFG** — çünkü `EXTICR` registerları EXTI'ye değil SYSCFG'ye aittir. Hat 10'u PA10, PB10, PC10 sürebilir ama aynı anda yalnızca biri; seçimi `EXTICR` yapar.

**GPIO portu** — çünkü clock kapalıyken pinin durumu kenar algılayıcıya hiç ulaşmaz.

### EXTICR indeks hesabı

Her hat, portunu belirtmek için 4 bit istiyor. 16 hat × 4 bit = 64 bit; tek 32-bit registera sığmıyor. ST bu yüzden dörder hat tutan dört register koymuş.

```
register indeksi = hat >> 2          // 10 >> 2 = 2   → EXTICR[2]
bit pozisyonu    = (hat & 3) * 4     // (10 & 3) * 4 = 8
```

Yani PC10, `EXTICR[2]` registerının 11:8 bitlerinde. Aynı kalıp farklı genişliklerle tekrar ediyor: OTYPER'da pin başına 1 bit, MODER'de 2 bit, EXTICR ve AFR'de 4 bit.

### Doğrulama

`EXTI_Init()` sonrasına breakpoint koyup SFR penceresinden `SYSCFG->EXTICR[2]`, `EXTI->IMR` ve `EXTI->RTSR` registerlarını oku. Bu aşamada tek anlamlı test budur; NVIC yapılandırması ve işleyici olmadan kartta gözlemlenebilir bir şey olmaz.