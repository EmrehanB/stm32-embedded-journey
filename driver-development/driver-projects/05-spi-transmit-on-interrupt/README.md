# 05 — SPI Transmit on Interrupt

Two peripherals working together for the first time: pressing the user button raises an EXTI interrupt, and the handler sends a string over SPI1.

Everything below `main.c` — the GPIO, EXTI, NVIC and SPI drivers — was written from scratch. No HAL (Hardware Abstraction Layer), no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral driver.

## What happens

| Step | Where |
|---|---|
| PA0 rises when the button is pressed | Hardware |
| EXTI line 0 latches the edge, NVIC dispatches IRQ 6 | [`EXTI.c`](../../driver-library/Src/EXTI.c) |
| `EXTI0_IRQHandler` clears the pending bit | `main.c` |
| The handler calls `SPI_TransmitData()` | [`SPI.c`](../../driver-library/Src/SPI.c) |
| 16 bytes go out on PA7 (MOSI), clocked by PA5 (SCK) | Hardware |

Output is verified with a logic analyser on SCK and MOSI. The baud rate is deliberately set to `SPI_BAUDRATE_DIV8` — the board runs at 16 MHz without PLL configuration, giving 2 MHz on the bus, which is comfortable to read on a capture.

## Alternate function configuration

This is the first project where GPIO pins are handed over to a peripheral rather than driven directly.

Setting `MODER` to alternate function only says *"this pin belongs to some peripheral"*. Which peripheral is decided by AFR (Alternate Function Register). On the STM32F407, PA5 and PA7 reach SPI1 through **AF5** — the mapping table is in the datasheet, not the reference manual.

AFR gives each pin 4 bits. Sixteen pins × 4 bits = 64 bits, which does not fit one register, so the driver indexes into a two-element array:

```c
tempVal  = GPIOx->AFR[position >> 3];                       // AFRL or AFRH
tempVal &= ~(0xFU << ((position & 0x7U) * 4));              // clear the nibble
tempVal |= cfg->Alternate << ((position & 0x7U) * 4);       // place the value
GPIOx->AFR[position >> 3] = tempVal;
```

The same pattern appears in SYSCFG's `EXTICR`, where four lines share one register instead of eight pins.

The pins are configured at `GPIO_SPEED_VERYHIGH`. Unlike the LED projects, where low speed was chosen to save power, SPI needs sharp edges — a slow slew rate would blur the clock and corrupt the data.

## SPI configuration

| Setting | Value | Reason |
|---|---|---|
| Mode | Master | The board generates the clock |
| CPOL / CPHA | Low / first edge | SPI mode 0, the most common default |
| Data frame | 8-bit | Sending a character string |
| Frame format | MSB first | Standard bit order |
| Bus config | Full duplex | Both data lines active, even though only MOSI matters here |
| SSM | Enabled | See below |
| Baud rate | PCLK / 8 | Slow enough to inspect on a logic analyser |

### Why SSM must be enabled

With software slave management off, the peripheral reads NSS from its physical pin. That pin is unused here and left floating, so noise can pull it low — and in master mode a low NSS is interpreted as *"another master has taken the bus"*.

The hardware then clears MSTR and SPE by itself and raises MODF (Mode Fault). SPI stops dead, with nothing visible in the code.

Setting `SSM = 1` together with `SSI = 1` tells the peripheral that NSS is high and no other master exists. Chip select, when a real slave is attached, is handled by a separate GPIO pin.

## The handle is global

```c
SPI_HandleTypeDef_t SPI_Handle = {0};
```

Deliberately outside `main()`. The interrupt handler needs the same handle that `SPI_Config()` filled in, and an ISR takes no parameters — so the two have to meet somewhere in global scope.

This is the standard arrangement for peripherals that are driven from more than one execution context, and it is the same reason ST's own drivers keep handles global.

## Known limitation

The transfer runs inside the interrupt handler and blocks until the last bit leaves the wire. At 2 MHz, sixteen bytes take roughly 64 µs, during which no interrupt of equal or lower priority can run.

For this project the cost is invisible, but the correct pattern is for the handler to set a flag and let the main loop do the work. Moving to interrupt-driven SPI (`TXEIE` in `CR2`) or to DMA removes the blocking entirely — that is what comes next.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| Input | PA0 — user button, external pull-down, rising edge |
| SPI1 | PA5 (SCK) · PA7 (MOSI) — alternate function 5 |
| Bus | SPI1 sits on APB2 |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

---

## Türkçe

İki çevre biriminin ilk kez birlikte çalıştığı proje: kullanıcı butonuna basılınca EXTI kesmesi oluşuyor, kesme işleyicisi SPI1 üzerinden bir metin gönderiyor.

`main.c` altındaki her katman — GPIO, EXTI, NVIC ve SPI sürücüleri — sıfırdan yazıldı. HAL ve CMSIS çevre birimi sürücüleri kullanılmıyor.

Çıktı, SCK ve MOSI hatlarına bağlanan logic analizör ile doğrulanıyor. Baud rate bilerek `SPI_BAUDRATE_DIV8` seçildi: kart PLL yapılandırılmadığı için 16 MHz'de çalışıyor, bu da hatta 2 MHz veriyor ve yakalamada rahat okunuyor.

### Alternate function

Bu, GPIO pinlerinin doğrudan sürülmek yerine bir çevre birimine **devredildiği** ilk proje.

`MODER`'ı alternate function'a almak yalnızca "bu pin bir çevre birimine ait" demek. Hangi çevre birimi olduğunu AFR (Alternate Function Register) belirliyor. STM32F407'de PA5 ve PA7, **AF5** üzerinden SPI1'e bağlanıyor — bu eşleme tablosu reference manual'da değil, datasheet'te.

AFR her pin için 4 bit ayırıyor. 16 pin × 4 bit = 64 bit, tek registera sığmıyor; bu yüzden sürücü iki elemanlı bir diziyi indeksliyor. Aynı kalıp SYSCFG'nin `EXTICR` registerında da var — orada sekiz pin yerine dört hat bir registeri paylaşıyor.

Pinler `GPIO_SPEED_VERYHIGH` ile yapılandırıldı. LED projelerinde güç tasarrufu için düşük hız seçilmişti; SPI'da tersi geçerli — yavaş kenar saat sinyalini bulanıklaştırır ve veriyi bozar.

### SSM neden etkin

Yazılım slave yönetimi kapalıyken çevre birimi NSS'i fiziksel pininden okur. O pin burada kullanılmıyor ve havada kalıyor; gürültü onu aşağı çekebilir. Master modda düşük NSS ise "başka bir master hattı ele geçirdi" olarak yorumlanıyor.

Donanım bunun üzerine MSTR ve SPE bitlerini kendisi temizliyor ve MODF (Mode Fault) bayrağını kaldırıyor. SPI aniden duruyor ve kodda hiçbir belirti görünmüyor.

`SSM = 1` ile `SSI = 1` birlikte, çevre birimine "NSS yüksek, başka master yok" demiş oluyor. Gerçek bir slave bağlandığında chip select ayrı bir GPIO pini ile yönetilir.

### Handle neden global

Kesme işleyicisi, `SPI_Config()` tarafından doldurulan aynı handle'a ihtiyaç duyuyor ve bir ISR parametre almıyor — ikisinin global kapsamda buluşması gerekiyor.

Birden fazla yürütme bağlamından sürülen çevre birimlerinde standart düzen budur; ST'nin kendi sürücüleri de handle'ları global tutar.

### Bilinen sınırlama

Aktarım kesme işleyicisinin içinde çalışıyor ve son bit hattan çıkana kadar bloke ediyor. 2 MHz'de on altı bayt yaklaşık 64 µs sürüyor; bu süre boyunca aynı veya daha düşük öncelikli hiçbir kesme çalışamıyor.

Bu projede maliyeti görünmüyor, ama doğru kalıp işleyicinin bir bayrak set etmesi ve işi ana döngünün yapmasıdır. Kesme tabanlı SPI (`CR2`'deki `TXEIE`) veya DMA bu bloklamayı tamamen ortadan kaldırır — sıradaki adım bu.