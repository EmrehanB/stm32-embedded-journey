# 02 — Button-Controlled LEDs

Reading a physical input and driving outputs from it — the first project in this repository where the board reacts to the outside world.

The user button on PA0 controls all four onboard LEDs (PD12–PD15). Press and hold: all four light up. Release: all four go dark.

## No HAL

No HAL (Hardware Abstraction Layer) and no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral driver is used. Every layer below `main.c` was written by hand from the [RM0090 reference manual](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) and the board schematic.

| Layer | Responsibility | Source |
|---|---|---|
| `stm32f407xx.h` | Peripheral base addresses, register maps as structs, bit definitions | [driver-library/Inc](../../driver-library/Inc) |
| `RCC_GPIOx_CLK_ENABLE()` | Sets the port enable bit in RCC (Reset and Clock Control) `AHB1ENR` | [RCC.h](../../driver-library/Inc/RCC.h) |
| `GPIO_Init()` | Walks the pin mask, configures MODER / OTYPER / OSPEEDR / PUPDR | [GPIO.c](../../driver-library/Src/GPIO.c) |
| `GPIO_ReadPin()` | Reads a pin from IDR (Input Data Register) | [GPIO.c](../../driver-library/Src/GPIO.c) |
| `GPIO_WritePin()` | Atomic set/reset through BSRR (Bit Set/Reset Register) | [GPIO.c](../../driver-library/Src/GPIO.c) |

`main.c` never touches a register. It does not know GPIOA's base address, where IDR sits, or which half of BSRR clears a pin.

## Configuration

Two different port configurations are performed in the same function, using one struct that is cleared with `memset()` in between — so no field leaks from the first configuration into the second.

**PD12–PD15 — outputs**

| Setting | Value | Reason |
|---|---|---|
| Mode | Output | LEDs are driven |
| Output type | Push-pull | The pin must actively source current to light the LED |
| Pull-up / pull-down | None | The output stage already defines the level |
| Speed | Low | An LED needs no fast edges; higher speed only costs power and adds noise |

**PA0 — input**

| Setting | Value | Reason |
|---|---|---|
| Mode | Input | The pin is read, not driven |
| Pull-up / pull-down | Pull-down | Matches the external circuit — see below |

## Why pull-down and not pull-up

This is not a guess; it comes from the board schematic.

On the STM32F4DISCOVERY, PA0 sits between two resistors. R39 (220 kΩ) ties the net to ground; the button connects the same net to VDD through R38 (100 Ω). The resistor's far end goes to **ground**, which makes it a pull-down.

Consequences:

- Button released — the switch is open, no path to VDD exists, R39 holds the pin low → reads **0**
- Button pressed — VDD reaches the pin through 100 Ω and easily overcomes the 220 kΩ pull-down → reads **1**

So the logic is *active high*: pressed means 1. That is why `main.c` lights the LEDs on `GPIO_Pin_Set` rather than on reset.

The internal pull-down configured in software reinforces the external one. It is not strictly required here, but it is the correct choice: configuring an internal pull-**up** instead would fight R39 and leave the pin at an indeterminate voltage.

The schematic also shows C38, a 100 nF capacitor intended as a hardware debounce filter — marked **Not Fitted**. It is not populated on the board, so any debouncing has to happen in software. This project does not debounce, because holding a level (rather than counting edges) is insensitive to contact bounce.

## Polling, not interrupts

The main loop reads PA0 on every iteration and writes the LED state accordingly. The processor does nothing else, so busy-waiting costs nothing here.

This is deliberately the simple version. An interrupt-driven equivalent using EXTI (External Interrupt/Event Controller) comes later, once the driver library covers NVIC (Nested Vectored Interrupt Controller) configuration. Comparing the two is the point of doing this one first.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| Input | PA0 — user / wake-up button, external pull-down |
| Outputs | PD12 green · PD13 orange · PD14 red · PD15 blue |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

Pin assignments and the button circuit are taken from the board schematic and user manual, not assumed.

---

## Türkçe

Fiziksel bir girişi okuyup çıkışları ona göre sürmek — bu depodaki, kartın dış dünyaya tepki verdiği ilk proje.

PA0'daki kullanıcı butonu, karttaki dört LED'i (PD12–PD15) kontrol ediyor. Basılı tutulduğunda dördü yanıyor, bırakıldığında sönüyor.

### HAL yok

HAL (Hardware Abstraction Layer) ve CMSIS (Cortex Microcontroller Software Interface Standard) çevre birimi sürücüleri kullanılmıyor. `main.c` altındaki her katman RM0090 referans kılavuzuna ve kart şemasına bakılarak elle yazıldı.

`main.c` hiçbir registera dokunmuyor: GPIOA'nın base adresini, IDR'nin nerede olduğunu veya BSRR'nin hangi yarısının pini sıfırladığını bilmiyor.

### Neden pull-down

Bu bir tahmin değil, kart şemasından geliyor.

PA0 hattında R39 (220 kΩ) direnci var ve bu direncin diğer ucu **toprağa** bağlı — bu onu pull-down yapıyor. Buton ise aynı hattı R38 (100 Ω) üzerinden VDD'ye bağlıyor.

Sonuç:

- Buton bırakıldığında anahtar açık, VDD'ye yol yok, R39 pini aşağı çekiyor → **0** okunur
- Butona basıldığında VDD 100 Ω üzerinden pine ulaşıyor ve 220 kΩ'luk pull-down'ı kolayca yeniyor → **1** okunur

Yani mantık *aktif yüksek*: basılı olmak 1 demek. `main.c` bu yüzden LED'leri `GPIO_Pin_Set` durumunda yakıyor.

Yazılımda ayarlanan dahili pull-down, harici olanı destekliyor. Zorunlu değil ama doğru tercih: dahili pull-**up** ayarlanmış olsaydı R39 ile çekişip pini belirsiz bir gerilimde bırakırdı.

Şemada C38 adında 100 nF'lık bir kondansatör de görünüyor — donanımsal buton zıplaması (debounce) filtresi olarak düşünülmüş ama **Not Fitted**, yani karta lehimlenmemiş. Bu projede debounce yapılmıyor; çünkü kenar saymak yerine seviye okunuyor ve seviye okuma kontak zıplamasına duyarsız.

### Polling

Ana döngü her turda PA0'ı okuyup LED durumunu yazıyor. İşlemci başka bir iş yapmadığı için burada meşgul bekleme (busy-wait) bir maliyet oluşturmuyor.

Bu bilerek basit sürüm. EXTI (External Interrupt/Event Controller) ile kesme tabanlı karşılığı, sürücü kütüphanesi NVIC (Nested Vectored Interrupt Controller) yapılandırmasını kapsadıktan sonra gelecek. İkisini karşılaştırabilmek için önce bunun yazılması gerekiyordu.

### Konfigürasyon notu

Aynı fonksiyonda iki farklı port yapılandırılıyor ve arada struct `memset()` ile sıfırlanıyor — böylece ilk konfigürasyondan hiçbir alan ikinciye sızmıyor.