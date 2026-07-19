# 07 — const + volatile register pointers

The same input-controlled LED (PA0 -> PD12), now with the register pointers declared using the **correct combination of `const` and `volatile` qualifiers** — so each pointer expresses exactly what it is: a fixed-address register whose value changes from hardware, and (for the input) one that must not be written to.

Functionally identical to exercise 06. The point here is precise, self-documenting register declarations.

## The two qualifier positions

`const` and `volatile` around a pointer mean different things depending on which side of the `*` they sit:

- **before `*`** -> applies to the **pointed-to data**
- **after `*`** -> applies to the **pointer itself**

## Output/clock/mode registers

```c
uint32_t volatile * const pPortDOutReg = (uint32_t*) 0x40020C14;
```

- `volatile` (before `*`) -> the register value can change unexpectedly (hardware).
- `const` (after `*`) -> the pointer is fixed; it will always point to this one address.

No `const` on the data side, because these registers **are written** (driving the LED).

## Input register — the key difference

```c
uint32_t volatile const * const pPortAInReg = (uint32_t*) 0x40020010;
```

Three qualifiers, each with a purpose:

- `volatile` (data) -> value changes from hardware (the pin's input signal).
- `const` (data, before `*`) -> **read-only**: the code must not write to it. Attempting `*pPortAInReg = x` becomes a compile error.
- `const` (pointer, after `*`) -> the pointer is fixed to this address.

This is the correct description of an **input data register (IDR)**: read-only, externally changing, fixed address. The `const` on the data side is what distinguishes an input register from an output one — the compiler now enforces that this register is only ever read.

## Why this matters

Beyond correctness, these qualifiers document intent and let the compiler catch mistakes:
- Accidentally writing to a read-only input register -> caught at compile time.
- Accidentally reassigning a register pointer to another address -> caught at compile time.

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

Aynı input-kontrollü LED (PA0 -> PD12), bu sefer register pointer'ları **`const` ve `volatile` niteleyicilerinin doğru birleşimiyle** tanımlandı — her pointer tam olarak ne olduğunu ifade ediyor: sabit adresli, değeri donanımdan değişen bir register, ve (input için) yazılmaması gereken bir register. 06 ile işlevsel olarak aynı; buradaki amaç kesin ve kendi kendini belgeleyen register tanımları.

**İki niteleyici konumu:** `const`/`volatile` yıldızın (`*`) hangi tarafındaysa ona uygulanır — yıldızdan **önce** gösterilen veriye, **sonra** pointer'ın kendisine.

**Output/clock/mode register'ları:** `uint32_t volatile * const` — veri `volatile` (donanımca değişir), pointer `const` (adres sabit). Veri tarafında `const` yok, çünkü bu register'lara **yazılıyor** (LED sürülüyor).

**Input register'ı (asıl fark):** `uint32_t volatile const * const` — üç niteleyici: `volatile` (değer donanımdan değişir), `const` veri (salt okunur, koda yazmaya izin vermez → `*pPortAInReg = x` derleme hatası olur), `const` pointer (adres sabit). Bu, bir **input data register (IDR)**'ın doğru tanımı: salt okunur, dışarıdan değişen, sabit adresli. Veri tarafındaki `const`, input register'ını output'tan ayıran şey — derleyici artık bu register'ın yalnızca okunduğunu garanti ediyor.

**Neden önemli:** Bu niteleyiciler hem niyeti belgeliyor hem de hataları derleyiciye yakalatıyor — salt okunur input register'ına yanlışlıkla yazmak veya register pointer'ını başka adrese atamak, derleme anında hata verir.
