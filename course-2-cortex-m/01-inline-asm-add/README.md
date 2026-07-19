# 01 — Inline Assembly: Add Two Values in Memory

Bare-metal ARM inline assembly exercise on STM32F407. Reads two 32-bit
values from consecutive SRAM addresses, adds them, and stores the result back.

## What it does
- Loads address `0x20001000` into R1 and `0x20001004` into R2
- Dereferences both (`LDR R0,[R1]` and `LDR R1,[R2]`) to fetch the stored values
- Adds them (`ADD R0,R0,R1`)
- Stores the result to `0x20001004` (`STR R0,[R2]`)

## Key concepts
- **`=addr` vs `[reg]`**: `LDR R1,=0x...` loads an address; `LDR R0,[R1]` dereferences it (like `*ptr` in C).
- **Load/store architecture**: ARM arithmetic (`ADD`) works only on registers. Memory must be brought in with `LDR` and written back with `STR`.
- **`__asm volatile`**: inline assembly, `volatile` prevents the compiler from optimizing the instructions away.
- **Register reuse**: R1 holds an address first, then a data value — registers have no fixed role.

## Verification
Debugged on target (STM32F407G-DISC1). Traced R0/R1/R2 in the Registers view,
confirmed the sum written to `0x20001004` in the Memory Browser.

## Türkçe
STM32F407 üzerinde bare-metal ARM inline assembly alıştırması. İki ardışık SRAM
adresindeki 32-bit değeri okuyup toplar, sonucu geri yazar. `=adres` ile `[register]`
farkı (adres yükleme vs dereference) ve load/store mimarisi (aritmetik sadece
register'da, belleğe LDR/STR ile erişilir) pratik olarak gösterilir. Hedef board
üzerinde debug edilerek register hareketleri ve bellekteki sonuç doğrulandı.