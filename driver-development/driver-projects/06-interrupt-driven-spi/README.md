# 06 — Interrupt-Driven SPI Transmit

The same task as [project 05](../05-button-triggered-spi), rewritten so that the SPI transfer itself is interrupt-driven. Pressing the user button still raises an EXTI interrupt, but the handler no longer waits for anything — it hands the buffer to the driver and returns immediately.

Comparing the two projects is the point. In 05 the processor spends the whole transfer spinning on a status bit inside an interrupt handler; here it spends microseconds per byte and is free in between.

## What changed

| | 05 — polled | 06 — interrupt-driven |
|---|---|---|
| Who waits for `TXE` | The `while` loop in `SPI_TransmitData` | Nobody — the peripheral raises IRQ 35 |
| Time spent in `EXTI0_IRQHandler` | The full transfer (~64 µs for 16 bytes at 2 MHz) | A few instructions |
| Where the next byte is written | Inside the polling loop | Inside `SPI1_IRQHandler` |
| Who owns the buffer | The caller's stack frame, alive for the whole call | The handle, alive until the transfer completes |
| Ends when | `BSY` clears | `TxDataSize` reaches zero and `TXEIE` is cleared |

## Flow

| Step | Where |
|---|---|
| PA0 rises, EXTI line 0 latches the edge, NVIC dispatches IRQ 6 | Hardware |
| `EXTI0_IRQHandler` clears the pending bit and calls `SPI_TransmitData_IT` | `main.c` |
| The driver stores the buffer pointer and size in the handle, picks an ISR helper, sets `TXEIE` | [`SPI.c`](../../driver-library/Src/SPI.c) |
| Setting `TXEIE` while `TXE` is already 1 raises IRQ 35 immediately | Hardware |
| `SPI1_IRQHandler` → `SPI_InterruptHandler` → the helper writes one byte to `DR` | `main.c` → `SPI.c` |
| Writing `DR` clears `TXE`; the buffer drains and `TXE` rises again → next interrupt | Hardware |
| Last byte written: `SPI_CloseISR_TX` clears `TXEIE` and releases the handle | `SPI.c` |

Verified on a logic analyser on SCK (PA5) and MOSI (PA7). The waveform is identical to project 05 — the difference is not on the wire, it is in what the processor was doing while the wire was busy.

## Design Notes

**The first interrupt is not triggered by hand.** There is no "send the first byte to prime the pump" step. `TXE` is already 1 when the transmit buffer is empty, so the moment `TXEIE` is set the condition `TXEIE && TXE` becomes true and the peripheral raises its interrupt. Setting the enable bit *is* the start of the transfer.

**The per-byte handler is chosen once, not per byte.** The data frame format decides whether one byte or two goes into `DR`. Testing that inside the ISR would repeat the same `if` for every byte of every transfer, for a value that cannot change mid-transfer. Instead `SPI_TransmitData_IT` reads the DFF bit once and stores the matching function in the handle:

```c
if (SPI_Handle->Instance->CR1 & (0x1U << 11))
    SPI_Handle->TxISRFunction = SPI_TransmitHelper_16Bits;
else
    SPI_Handle->TxISRFunction = SPI_TransmitHelper_8Bits;
```

The interrupt handler then has no branch of its own:

```c
SPI_Handle->TxISRFunction(SPI_Handle);
```

Beyond saving a comparison, this is the shape a state machine takes in C: the handle carries *what to do next*, and the ISR only carries it out. Receive support, or a transfer that changes behaviour partway through, extends this without touching the handler.

**The struct needs a tag to point at itself.** `SPI_HandleTypeDef_t` does not exist yet while the struct is being defined — the `typedef` name only comes into scope at the closing semicolon. The struct tag `__SPI_HandleTypeDef_t` lives in a separate namespace and is usable from the opening line, which is what lets the function-pointer member take a pointer to the type it belongs to. Leading double underscores are technically reserved for the implementation; ST's own HAL does the same thing, and the alternative is a forward declaration for very little gain.

**`busStateTX` is a busy flag, not a lock.** It stops a second `SPI_TransmitData_IT` from overwriting a transfer already in flight — without it, the pointer and size in the handle would be replaced mid-stream and the first buffer would be sent half-finished. There is still a window between reading the flag and setting it; closing it properly means disabling interrupts around the pair. With one caller that window is unreachable, so it is left open deliberately rather than by omission.

**`busStateTX` must be `volatile`.** It is written from interrupt context by `SPI_CloseISR_TX` and read from thread context. Without the qualifier the compiler is free to cache the value in a register, and any future `while (handle.busStateTX != SPI_BUS_FREE);` would hang forever at optimisation levels above `-O0` — the loop would never re-read memory. This is the general rule for anything shared across execution contexts: the compiler cannot see that an interrupt exists.

**The transmit buffer must outlive the function call.** This is the difference that makes an asynchronous API a different thing to use, not just a faster one:

```c
void EXTI0_IRQHandler(void)
{
    static char msg[] = "Hello Embedded!\n";     // static, not automatic
    ...
    SPI_TransmitData_IT(&SPI_Handle, (uint8_t *)msg, strlen(msg));
}
```

`SPI_TransmitData_IT` does not send the data — it records the *address* of the data and returns. The bytes are read later, from a different interrupt. An automatic array would live in the handler's stack frame, and that frame is gone the instant the handler returns; the SPI interrupt would then read memory that belongs to nobody. The bug is invisible in testing, because nothing has overwritten that stack region yet.

The polled version had no such requirement — it finished with the buffer before returning. Moving to interrupts moved the ownership boundary, and that is the part that does not show up in the register configuration.

**`EXTI->PR` is written, not read-modified-written.** The pending register is `rc_w1`: writing 1 to a bit clears it, writing 0 does nothing. `EXTI->PR |= (1 << 0)` first reads the register, so any other line that happened to become pending in that instant is written back as 1 and silently cleared too. `EXTI->PR = (1 << 0)` touches exactly one bit. Same argument as BSRR over ODR.

## Known limitations

- **The 16-bit path does not terminate.** `SPI_TransmitHelper_16Bits` decrements `TxDataSize` but does not check it against zero or call `SPI_CloseISR_TX`. With `SPI_DFF_16BITS` selected, `TXEIE` would stay set, the counter would wrap around, and the interrupt would repeat indefinitely. The current application uses 8-bit frames, so the path is unreachable here — it is left as an open item rather than a hidden one.

- **Received bytes are still discarded.** SPI is full duplex: every byte sent produces a byte received. Nothing reads `DR`, so `RXNE` stays set and `OVR` (Overrun) eventually raises. Harmless for transmit-only traffic, but it means the peripheral is in an error state that no code inspects. Receive-side interrupts (`RXNEIE`) come next.

- **No error handling.** `MODF`, `OVR` and `CRCERR` all have interrupt enables in `CR2` (`ERRIE`) and none of them are enabled or checked. `SPI_InterruptHandler` only tests the `TXEIE`/`TXE` pair.

- **No completion callback.** The application has no way to learn that a transfer finished other than polling `busStateTX`. A weak callback invoked from `SPI_CloseISR_TX` is the usual next step.

- **`SPI_Init` still ORs into `CR1`.** The register is read and OR-ed rather than cleared and written, so a second call on the same instance keeps bits from the first configuration. Single-init applications are unaffected.

## Hardware

| | |
|---|---|
| Board | [STM32F407G-DISC1 (Discovery)](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) |
| Input | PA0 — user button, external pull-down, rising edge (EXTI line 0, IRQ 6) |
| SPI1 | PA5 (SCK) · PA7 (MOSI) — alternate function 5, IRQ 35 |
| Bus | SPI1 sits on APB2 |
| Clock | 16 MHz, PLL not configured — `SPI_BAUDRATE_DIV8` gives 2 MHz on the bus |
| Toolchain | [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html), `arm-none-eabi-gcc` |

---

## Türkçe

[Proje 05](../05-button-triggered-spi) ile aynı iş, bu kez SPI aktarımının kendisi kesme tabanlı. Butona basılınca yine EXTI kesmesi oluşuyor, ancak işleyici artık hiçbir şey beklemiyor — tamponu sürücüye devredip hemen dönüyor.

Asıl amaç ikisini karşılaştırmak. 05'te işlemci aktarımın tamamı boyunca bir kesme işleyicisinin içinde durum biti bekliyordu; burada bayt başına birkaç komut harcıyor ve aralarda serbest.

### Akış

Buton PA0'ı yükseltiyor, EXTI hat 0 kenarı yakalıyor, NVIC IRQ 6'yı çağırıyor. `EXTI0_IRQHandler` pending bitini temizleyip `SPI_TransmitData_IT` çağırıyor. Sürücü tampon adresini ve boyutunu handle'a yazıyor, uygun ISR yardımcısını seçiyor ve `TXEIE` bitini set ediyor.

`TXE` zaten 1 olduğu için `TXEIE` set edildiği anda IRQ 35 tetikleniyor. `SPI1_IRQHandler` her tetiklenmede `DR`'ye bir bayt yazıyor; `DR`'ye yazmak `TXE`'yi temizliyor, tampon boşalınca tekrar 1 oluyor ve döngü kendiliğinden devam ediyor. Son bayttan sonra `SPI_CloseISR_TX` `TXEIE`'yi temizleyip handle'ı serbest bırakıyor.

Logic analizörle SCK ve MOSI üzerinde doğrulandı. Dalga şekli 05 ile aynı — fark hatta değil, hat meşgulken işlemcinin ne yaptığında.

### Tasarım kararları

**İlk kesme elle tetiklenmiyor.** "İlk baytı gönder ki döngü başlasın" gibi bir adım yok. Gönderme tamponu boşken `TXE` zaten 1; `TXEIE` set edildiği anda `TXEIE && TXE` koşulu sağlanıyor ve çevre birimi kesmesini üretiyor. Enable bitini set etmek aktarımı **başlatmanın kendisi**.

**Bayt başına çalışacak fonksiyon bir kez seçiliyor.** DFF (Data Frame Format) biti `DR`'ye bir bayt mı iki bayt mı yazılacağını belirliyor. Bunu ISR'ın içinde test etmek, aktarım boyunca değişmeyecek bir değer için her baytta aynı `if`'i tekrarlamak demek. Bunun yerine `SPI_TransmitData_IT` DFF'i bir kez okuyup uygun fonksiyonu handle'a yazıyor; işleyici sadece `SPI_Handle->TxISRFunction(SPI_Handle);` diyor.

Kazanç bir karşılaştırmadan fazlası: bu, C'de durum makinesinin aldığı biçim. Handle "sırada ne yapılacağını" taşıyor, ISR yalnızca uyguluyor. Alma tarafı veya ortada davranış değiştiren bir aktarım, işleyiciye dokunmadan eklenebiliyor.

**Struct'ın kendini gösterebilmesi için etiket gerekiyor.** `SPI_HandleTypeDef_t` ismi struct tanımlanırken henüz mevcut değil — `typedef` adı ancak kapanış noktalı virgülünde kapsama giriyor. `__SPI_HandleTypeDef_t` struct etiketi ayrı bir isim alanında yaşıyor ve açılış satırından itibaren kullanılabiliyor; fonksiyon pointer'ının kendi tipine işaret edebilmesini bu sağlıyor.

**`busStateTX` bir meşgul bayrağı, kilit değil.** Devam eden bir aktarımın üzerine ikinci bir `SPI_TransmitData_IT` çağrısının yazmasını engelliyor; olmasaydı handle'daki pointer ve boyut aktarım ortasında değişir, ilk tampon yarım giderdi. Bayrağı okumakla set etmek arasında hâlâ bir pencere var; tam kapatmak için kesmelerin o çift etrafında kapatılması gerekir. Tek çağıran varken o pencereye ulaşılamadığı için bilerek açık bırakıldı.

**`busStateTX` `volatile` olmalı.** Kesme bağlamından (`SPI_CloseISR_TX`) yazılıyor, thread bağlamından okunuyor. Niteleyici olmadan derleyici değeri bir register'da tutabilir; ileride yazılacak bir `while (handle.busStateTX != SPI_BUS_FREE);` `-O0` üstü optimizasyonda sonsuza kadar asılır, çünkü döngü belleği bir daha okumaz. Yürütme bağlamları arasında paylaşılan her değişken için geçerli genel kural bu: derleyici kesmenin varlığını göremez.

**Gönderme tamponu fonksiyon çağrısından uzun yaşamalı.** Asenkron API'yi sadece "daha hızlı" değil, **farklı** yapan nokta bu:

```c
static char msg[] = "Hello Embedded!\n";   // static, otomatik değil
```

`SPI_TransmitData_IT` veriyi göndermiyor — verinin **adresini** kaydedip dönüyor. Baytlar daha sonra, başka bir kesmeden okunuyor. Otomatik bir dizi işleyicinin stack çerçevesinde yaşar ve o çerçeve işleyici döner dönmez ölür; SPI kesmesi artık kimseye ait olmayan belleği okur. Hata testte görünmez, çünkü o stack bölgesinin henüz üzerine yazılmamıştır.

Yoklama tabanlı sürümde böyle bir gereklilik yoktu — tamponla işi dönmeden bitiriyordu. Kesmeye geçmek **sahiplik sınırını** taşıdı; register yapılandırmasında görünmeyen kısım tam olarak bu.

**`EXTI->PR` okunmadan yazılıyor.** Pending register `rc_w1`: 1 yazılan bit temizlenir, 0 yazılan bit etkilenmez. `EXTI->PR |= (1 << 0)` önce registerı okur; o anda pending olan başka bir hat varsa geri yazılırken o da sessizce temizlenir. `EXTI->PR = (1 << 0)` tek bite dokunur. BSRR'ı ODR'ye tercih etme gerekçesiyle aynı.

### Bilinen sınırlamalar

- **16 bitlik yol sonlanmıyor.** `SPI_TransmitHelper_16Bits` sayacı azaltıyor ama sıfır kontrolü yapıp `SPI_CloseISR_TX` çağırmıyor. `SPI_DFF_16BITS` seçilirse `TXEIE` açık kalır, sayaç alttan taşar ve kesme sonsuza kadar tekrarlar. Mevcut uygulama 8 bit kullandığı için bu yola girilmiyor — gizli değil, açık bir eksik olarak bırakıldı.
- **Gelen baytlar hâlâ atılıyor.** SPI full duplex; gönderilen her bayt karşılığında bir bayt geliyor. `DR` okunmadığı için `RXNE` set kalıyor ve `OVR` (Overrun) yükseliyor. Yalnızca gönderim yapılırken zararsız, ama çevre birimi kimsenin bakmadığı bir hata durumunda.
- **Hata yönetimi yok.** `MODF`, `OVR` ve `CRCERR` için `CR2`'de `ERRIE` var; ne açılıyor ne kontrol ediliyor.
- **Tamamlanma geri çağrısı yok.** Uygulama aktarımın bittiğini `busStateTX`'i yoklamaktan başka yolla öğrenemiyor. Sıradaki adım `SPI_CloseISR_TX` içinden çağrılan zayıf (weak) bir callback.
- **`SPI_Init` hâlâ `CR1`'e OR'luyor.** Aynı instance üzerinde ikinci bir init çağrısı ilk yapılandırmadan kalan bitleri taşır.
