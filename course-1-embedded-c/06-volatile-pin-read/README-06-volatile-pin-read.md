# 06 — Volatile pin read (O2-safe input)

The same input-controlled LED as exercise 05 (PA0 drives PD12), but made **safe under compiler optimization** by declaring the register pointers as `volatile`.

Exercise 05 works at `-O0` (no optimization). This one is written to work correctly at `-O2` as well — and was verified on hardware at `-O2`.

## The problem this solves

The compiler optimizes code it thinks is redundant. It only sees the C code — it does **not** know that a pointer targets a hardware register whose value changes on its own.

In the polling loop, PA0 is read every iteration but nothing in the code writes to it. At `-O2` the compiler reasons: *"this value never changes between reads, so read it once and reuse it."* The loop collapses to a single read, and the LED stops responding to PA0 — even though the code looks correct.

## The fix: `volatile`

Declaring the register pointers `volatile` tells the compiler: *this data can change unexpectedly (from hardware) — never optimize its reads or writes; perform every access for real.*

```c
volatile uint32_t *pPortAInReg = (volatile uint32_t*) 0x40020010;
```

With this, the read inside `while(1)` happens on every iteration at any optimization level, so the LED keeps following PA0.

## Where volatile is needed

`volatile` is used here because the pointers target **memory-mapped peripheral registers** — one of the three classic cases (the others being RTOS shared globals, and variables shared between main code and an ISR). The common thread: the value can change outside the flow of the code reading it.

## Verified

- Built and tested on hardware at **-O2** — PA0 correctly controls PD12.
- Without `volatile`, the same code breaks at `-O2` (read gets optimized away); with it, it works.

## Register addresses

```
RCC_AHB1ENR     : 0x40023830
GPIOD MODER     : 0x40020C00
GPIOD ODR       : 0x40020C14
GPIOA MODER     : 0x40020000
GPIOA IDR       : 0x40020010
```

---

## Türkçe

05. alıştırmadaki aynı input-kontrollü LED (PA0, PD12'yi sürüyor), ama register pointer'ları `volatile` olarak tanımlanarak **derleyici optimizasyonuna karşı güvenli** hale getirildi. 05 yalnızca `-O0`'da (optimizasyon kapalı) çalışıyordu; bu ise `-O2`'de de doğru çalışacak şekilde yazıldı ve donanımda `-O2`'de test edildi.

**Çözdüğü sorun:** Derleyici sadece C kodunu görür, bir pointer'ın kendi kendine değişen bir donanım register'ını gösterdiğini bilmez. Polling döngüsünde PA0 her turda okunuyor ama kod ona hiç yazmıyor. `-O2`'de derleyici "bu değer okumalar arasında değişmiyor, bir kez oku yeter" diye düşünüp okumayı tek sefere indirger; LED PA0'a tepki vermez olur — kod doğru görünse bile.

**Çözüm:** Register pointer'larını `volatile` yapmak, derleyiciye "bu veri beklenmedik şekilde (donanımdan) değişebilir, okuma/yazmalarını optimize etme, her erişimi gerçekten yap" der. Böylece `while(1)` içindeki okuma her optimizasyon seviyesinde her turda gerçekleşir.

Burada `volatile` gerekiyor çünkü pointer'lar **bellek eşlemeli çevre birimi register'larını** gösteriyor — volatile'ın üç klasik kullanım durumundan biri (diğerleri: RTOS'ta paylaşılan global'ler ve ana kod ile ISR arasında paylaşılan değişkenler). Ortak nokta: değer, onu okuyan kodun akışı dışında değişebilir.
