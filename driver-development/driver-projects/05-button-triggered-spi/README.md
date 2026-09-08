# 05 — Button-Triggered SPI Transmit

Two peripherals working together for the first time: pressing the user button raises an EXTI interrupt, and the handler sends a string over SPI1.

**The interrupt here is EXTI, not SPI.** The button is interrupt-driven; the SPI transfer itself is polled — the driver spins on `TXE` and `BSY` inside the handler. Interrupt-driven SPI (`TXEIE` / `RXNEIE`) is a separate project.

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

## Known limitation: a polled transfer inside an ISR

`SPI_TransmitData` waits on `TXE` before every byte and on `BSY` at the end. Those waits are `while` loops, and here they run **inside the interrupt handler**.

At 2 MHz, sixteen bytes take roughly 64 µs. For that entire window no interrupt of equal or lower priority can run — the handler is holding the processor while doing nothing but waiting on a status bit.

Two ways out, both planned:

**Set a flag, transfer in `main`.** The handler does nothing but record that the button was pressed; the main loop performs the transfer. The blocking still exists but no longer happens in interrupt context.

**Interrupt-driven SPI.** Enabling `TXEIE` in `CR2` makes the peripheral raise its own interrupt whenever the transmit buffer drains. Each byte is written from a short handler and the processor is free in between — no waiting at all.

DMA removes even that, letting the peripheral move the whole block without the processor.

This project is deliberately the blocking version: comparing it against the interrupt-driven one is the point.

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

**Buradaki kesme EXTI'den geliyor, SPI'dan değil.** Buton kesme tabanlı; SPI aktarımının kendisi yoklama (polling) ile yapılıyor — sürücü işleyicinin içinde `TXE` ve `BSY` bayraklarını bekliyor. Kesme tabanlı SPI (`TXEIE` / `RXNEIE`) ayrı bir proje olacak.

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

### Bilinen sınırlama: ISR içinde yoklama tabanlı aktarım

`SPI_TransmitData` her bayttan önce `TXE`, sonunda `BSY` bekliyor. Bu beklemeler `while` döngüsü ve burada **kesme işleyicisinin içinde** çalışıyorlar.

2 MHz'de on altı bayt yaklaşık 64 µs sürüyor. Bu süre boyunca aynı veya daha düşük öncelikli hiçbir kesme çalışamıyor — işleyici, bir durum bitini beklemekten başka bir şey yapmadan işlemciyi tutuyor.

İki çıkış yolu var, ikisi de planlı:

**Bayrak set et, aktarımı `main`'de yap.** İşleyici yalnızca butona basıldığını kaydeder; aktarımı ana döngü yapar. Bloklama devam eder ama artık kesme bağlamında değildir.

**Kesme tabanlı SPI.** `CR2`'deki `TXEIE` etkinleştirildiğinde çevre birimi, gönderme tamponu boşaldıkça kendi kesmesini üretir. Her bayt kısa bir işleyiciden yazılır ve aralarda işlemci serbesttir — hiç bekleme yoktur.

DMA bunu da ortadan kaldırır; çevre birimi tüm bloğu işlemci karışmadan taşır.

Bu proje bilerek bloklayan sürüm: kesme tabanlı olanla karşılaştırmak asıl amaç.