# 03 — LED on (PD12) with bitwise shift operators

Same on-board **green LED (LD4, PD12)** as exercise 01, but the register bits are now set and cleared using **bitwise shift operators** (`<<`, `|`, `&`, `~`) instead of hand-written hex masks.

The hardware result is identical — this exercise is about writing the register manipulation more clearly and with less room for error.

## What changed vs. exercise 01

In exercise 01 the mode bits were cleared with a hand-computed mask (`0xFCFFFFFF`). Here the same operations are expressed directly from the bit positions:

```c
*pClckCtrlReg |= (1 << 3);     // RCC_AHB1ENR: enable GPIOD clock (bit 3)

*pPortDModeReg &= ~(1 << 24);  // MODER: clear bit 24
*pPortDModeReg &= ~(1 << 25);  // MODER: clear bit 25
*pPortDModeReg |=  (1 << 24);  // MODER: set PD12 to output (01)

*pPortDOutReg  |=  (1 << 12);  // ODR: drive PD12 high -> LED on
```

The two clear lines could also be written as a single `*pPortDModeReg &= ~(3 << 24);`, since `3` is `0b11` and masks both bits at once.

## Why this is better than hex masks

- The intent reads straight from the code — `(1 << 24)` says "bit 24", no mental hex decoding.
- Less error-prone — no need to hand-compute long masks like `0xFCFFFFFF`.
- `&= ~(1 << n)` is the standard idiom for clearing a single bit without touching the others.

## Register addresses

```
RCC_AHB1ENR  : 0x40023830
GPIOx_MODER  : 0x40020C00
GPIOx_ODR    : 0x40020C14
```

---

## Türkçe

01. alıştırmadaki aynı yeşil LED'i (LD4, PD12) yakıyorum, ama bu sefer register bitlerini elle yazılmış hex maskeler yerine **bitwise shift operatörleriyle** (`<<`, `|`, `&`, `~`) set edip temizliyorum. Donanım sonucu birebir aynı; bu alıştırmanın amacı register işlemlerini daha okunur ve daha az hata payıyla yazmak.

01'de mode bitlerini temizlemek için elle `0xFCFFFFFF` maskesi hesaplamıştım. Burada aynı işi doğrudan bit pozisyonlarından ifade ediyorum: `&= ~(1 << 24)` ile 24. biti temizliyorum, `|= (1 << 24)` ile set ediyorum. İki temizleme satırı `&= ~(3 << 24)` ile tek satırda da yapılabilirdi (`3` = `0b11`, iki biti birden maskeler).

Avantajı: niyet koddan doğrudan okunuyor (`(1 << 24)` = "24. bit"), uzun maskeleri elle hesaplamaya gerek kalmıyor ve `&= ~(1 << n)` tek bir biti diğerlerine dokunmadan temizlemenin standart yolu.
