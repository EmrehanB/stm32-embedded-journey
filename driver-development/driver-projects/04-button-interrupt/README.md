# 04 — Button Interrupt

The first project in this repository where the processor is **interrupted** rather than polled. Pressing the user button on PA0 raises an EXTI line, the NVIC dispatches to a handler, and the handler lights the red LED on PD14.

No polling loop. `main()` ends in an empty `for(;;)` and the work happens entirely in interrupt context.

## No HAL

No HAL (Hardware Abstraction Layer) and no CMSIS (Cortex Microcontroller Software Interface Standard) peripheral driver is used. The SYSCFG, EXTI and NVIC register access, the IRQ number table and every driver function were written by hand from [RM0090](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) and the [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/latest/).

| Layer | Responsibility |
|---|---|
| [`stm32f407xx.h`](../../driver-library/Inc/stm32f407xx.h) | Register structs, base addresses, `NVIC_ISER0` pointer, IRQ number enum |
| [`RCC.h`](../../driver-library/Inc/RCC.h) | SYSCFG and GPIO port clock enable |
| [`GPIO.c`](../../driver-library/Src/GPIO.c) | Pin configuration, BSRR write |
| [`EXTI.c`](../../driver-library/Src/EXTI.c) | `EXTI_LineConfig()`, `EXTI_Init()`, `NVIC_EnableInterrupt()` |

## The signal path

Six stages, four of them configured by software:

```
PA0 pin
   │
   ▼
Edge detector          ← RTSR selects rising edge
   │
   ▼
Pending register (PR)  ← latches the event until cleared
   │
   ▼
Interrupt mask (IMR)   ← must be set for the line to escape EXTI
   │
   ▼
NVIC ISER              ← must be set for the IRQ to reach the core
   │
   ▼
EXTI0_IRQHandler()
```

The important part is that **two separate enables are required**. `EXTI->IMR` lets the line out of the EXTI block; `NVIC->ISER` lets the IRQ reach the processor. Setting only one produces no interrupt and no error — a silent failure that is easy to spend an afternoon on.

## Configuration order

| Step | What | Why |
|---|---|---|
| 1 | SYSCFG clock | The `EXTICR` routing registers live in SYSCFG |
| 2 | GPIOA clock | Without it the pin state never reaches the edge detector |
| 3 | PA0 as input, pull-down | Matches the board's external pull-down (R39 to ground) |
| 4 | `EXTICR[0]` bits 3:0 → port A | Line 0 can be driven by PA0, PB0, PC0… only one at a time |
| 5 | `IMR` bit 0, `RTSR` bit 0 | Unmask the line, detect rising edges |
| 6 | `NVIC->ISER[0]` bit 6 | EXTI0 is IRQ 6 |

EXTI itself has no clock enable bit anywhere in RCC — it belongs to the processor's interrupt fabric rather than the vendor peripheral set.

## Why rising edge

The button reads **1 when pressed**: R39 (220 kΩ) ties PA0 to ground, and the button connects it to VDD through R38 (100 Ω). Pressing therefore produces a low-to-high transition — a rising edge.

Falling-edge triggering here would fire on release instead.

## Clearing the pending bit

The handler's first job is to clear `EXTI->PR`:

```c
EXTI->PR = (0x1U << 0U);
```

Two things about this line.

**Writing 1 clears the bit.** `PR` is an `rc_w1` register — read/clear by writing one. Writing zero does nothing.

**Direct assignment, not `|=`.** Because a write of 1 clears and a write of 0 is ignored, only the bits actually written are affected. Read-modify-write would add a pointless read and open a window in which another line could go pending and be cleared unintentionally. Same reasoning as using BSRR instead of ODR.

Forgetting this line is the classic EXTI mistake: `PR` stays set, the NVIC re-fires the handler immediately, and the program locks in an interrupt loop.

## Debouncing

None. The button's mechanical bounce produces several rising edges per press, so the handler runs multiple times.

It is invisible here because the handler only sets the LED — running it five times has the same effect as running it once. A handler that toggled or counted would need debouncing, in software or with the C38 capacitor the board leaves unpopulated.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| Input | PA0 — user / wake-up button, external pull-down |
| Output | PD14 — red LED |
| IRQ | EXTI0, IRQ number 6 |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

The handler name `EXTI0_IRQHandler` must match the symbol in the startup file exactly — that is the name placed in the vector table. A misspelling compiles cleanly and never runs.

---

## Türkçe

Bu depoda işlemcinin **kesildiği** ilk proje — yoklama (polling) yok. PA0'daki kullanıcı butonuna basılınca EXTI hattı tetikleniyor, NVIC (Nested Vectored Interrupt Controller) kesme işleyicisine dallanıyor, işleyici PD14'teki kırmızı LED'i yakıyor.

`main()` boş bir `for(;;)` ile bitiyor; iş tamamen kesme bağlamında yapılıyor.

HAL ve CMSIS çevre birimi sürücüleri kullanılmıyor. SYSCFG, EXTI ve NVIC register erişimleri, IRQ numara tablosu ve tüm sürücü fonksiyonları RM0090 ile Cortex-M4 Technical Reference Manual'e bakılarak elle yazıldı.

### İki ayrı izin

En kritik nokta bu: bir kesmenin işlemciye ulaşması için **iki farklı yerde** izin verilmesi gerekiyor.

`EXTI->IMR` hattın EXTI bloğundan çıkmasına izin verir. `NVIC->ISER` ise IRQ'nun çekirdeğe ulaşmasına. Yalnızca birini ayarlarsan hiçbir kesme gelmez ve hata mesajı da alamazsın — sessiz bir başarısızlık.

### Neden yükselen kenar

Buton basılıyken **1** okuyor: R39 (220 kΩ) PA0'ı toprağa çekiyor, buton ise R38 (100 Ω) üzerinden VDD'ye bağlıyor. Basma işlemi düşükten yükseğe geçiş, yani yükselen kenar üretiyor.

Düşen kenar seçilseydi kesme bırakma anında gelirdi.

### Pending bitinin temizlenmesi

İşleyicinin ilk işi `EXTI->PR` bitini temizlemek.

`PR` bir **rc_w1** registerı: 1 yazınca temizleniyor, 0 yazınca hiçbir şey olmuyor. Bu yüzden `|=` yerine doğrudan atama kullanılıyor — okuma gereksiz ve okuma ile yazma arasında başka bir hattın pending olması durumunda onu da temizleme riski var. BSRR'yi ODR'ye tercih etme gerekçesinin aynısı.

Bu satır unutulursa `PR` set kalır, NVIC işleyiciyi hemen yeniden çağırır ve program sonsuz kesme döngüsüne girer.

### Debounce

Yok. Butonun mekanik zıplaması basış başına birkaç yükselen kenar üretiyor, dolayısıyla işleyici birden çok kez çalışıyor.

Burada görünmüyor çünkü işleyici sadece LED'i set ediyor — beş kez çalışması ile bir kez çalışması aynı sonucu veriyor. Toggle yapan veya sayan bir işleyici debounce gerektirirdi.

### İsimlendirme

`EXTI0_IRQHandler` ismi startup dosyasındaki sembolle **birebir** aynı olmak zorunda; vektör tablosuna o isim yerleştiriliyor. Yanlış yazılırsa kod sorunsuz derlenir ve fonksiyon hiç çağrılmaz.