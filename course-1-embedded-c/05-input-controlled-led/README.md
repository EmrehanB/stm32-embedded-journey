# 05 — Input-controlled LED (PA0 -> PD12)

Reading a digital **input** on **PA0** and using it to drive the on-board **green LED (LD4, PD12)**: when PA0 reads high, the LED turns on; when it reads low, the LED turns off.

This is the first exercise that reads a GPIO **input** — the previous ones only drove outputs. Two ports are used at once (GPIOA as input, GPIOD as output), so both peripheral clocks are enabled.

## How it works

```c
// Enable clocks for both ports
*pClckCtrlReg |= (1 << 3);   // RCC_AHB1ENR: GPIOD clock (bit 3)
*pClckCtrlReg |= (1 << 0);   // RCC_AHB1ENR: GPIOA clock (bit 0)

// PD12 as output (MODER bits 24-25 = 01)
*pPortDModeReg &= ~(1 << 24);
*pPortDModeReg &= ~(1 << 25);
*pPortDModeReg |=  (1 << 24);

// PA0 as input (MODER bits 0-1 = 00)
*pPortAModeReg &= ~(0x03);

while (1) {
    uint8_t pinState = (uint8_t)(*pPortAInReg & 0x01);   // read PA0 (IDR bit 0)
    if (pinState)
        *pPortDOutReg |=  (1 << 12);   // PA0 high -> LED on
    else
        *pPortDOutReg &= ~(1 << 12);   // PA0 low  -> LED off
}
```

## Key points

- **Input vs output direction:** PA0 is set to input mode (`00` in MODER) and read from the **input data register (IDR)**; PD12 is output mode (`01`) and written to the **output data register (ODR)**.
- **Reading a single bit:** `*pPortAInReg & 0x01` masks out everything except bit 0, giving the state of PA0.
- **Continuous polling:** the read/decide/write happens inside `while(1)`, so the LED follows PA0 continuously rather than reading it only once.
- **Two ports, two clocks:** GPIOA and GPIOD each need their clock enabled in `RCC_AHB1ENR` (bits 0 and 3).

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

İlk kez bir GPIO **girişi** okuyorum: **PA0** pinini okuyup ona göre **yeşil LED'i (LD4, PD12)** kontrol ediyorum. PA0 yüksekse LED yanıyor, alçaksa sönüyor. Önceki alıştırmalar yalnızca output sürüyordu; bu, girdi tarafına geçtiğim ilk alıştırma.

İki port aynı anda kullanılıyor (GPIOA giriş, GPIOD çıkış), bu yüzden ikisinin de clock'u `RCC_AHB1ENR`'de açılıyor (bit 0 ve bit 3). PA0 input moduna (MODER `00`) alınıp **input data register (IDR)**'den okunuyor; PD12 ise output (`01`) olarak **output data register (ODR)**'ye yazılıyor. `*pPortAInReg & 0x01` ile sadece 0. bit maskelenerek PA0'ın durumu alınıyor. Okuma/karar/yazma `while(1)` içinde olduğu için LED, PA0'ı sürekli takip ediyor — tek seferlik değil.
