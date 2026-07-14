# 04 — LED toggle (PD12)

Blinking the on-board **green LED (LD4, PD12)** by toggling it on and off inside an infinite loop, with a busy-wait delay between states.

Setup (clock + mode) is identical to the previous exercises; the new part is the `while(1)` loop that turns the LED on, waits, turns it off, waits, and repeats.

## How it works

```c
while (1) {
    *pPortDOutReg |=  (1 << 12);   // PD12 high  -> LED on
    for (uint32_t i = 0; i < 1000000; i++);   // delay
    *pPortDOutReg &= ~(1 << 12);   // PD12 low   -> LED off
    for (uint32_t i = 0; i < 1000000; i++);   // delay
}
```

## Note on the delay

The board runs at ~16 MHz by default (~16 million operations/second), so a short loop finishes in microseconds and the LED appears permanently on. The loop count has to be large (~1,000,000+) for the blink to be visible to the eye.

This `for`-loop delay is a **busy-wait** — fine for learning, but not how real timing is done:
- It is not accurate (depends on clock speed and compiler optimization; the compiler may even remove an "empty" loop).
- It wastes the CPU, which spins doing nothing just to pass time.

The proper approach is a hardware **timer** peripheral, which gives precise delays without blocking the CPU. (Coming later — Fastbit MCU2 / TIMERS.)

## Register addresses

```
RCC_AHB1ENR  : 0x40023830
GPIOx_MODER  : 0x40020C00
GPIOx_ODR    : 0x40020C14
```

---

## Türkçe

STM32F407G-DISC1 üzerindeki yeşil LED'i (LD4, PD12) sonsuz döngü içinde yakıp söndürüyorum; iki durum arasında busy-wait gecikme var. Clock ve mode ayarı önceki alıştırmalarla aynı; yeni olan kısım `while(1)` döngüsü: LED'i yak, bekle, söndür, bekle, tekrarla.

Kart varsayılan olarak ~16 MHz'de çalışıyor (saniyede ~16 milyon işlem), bu yüzden kısa bir döngü mikrosaniyede biter ve LED sürekli yanıkmış gibi görünür. Yanıp sönmenin gözle görülebilmesi için döngü sayısının büyük olması gerekiyor (~1.000.000+).


