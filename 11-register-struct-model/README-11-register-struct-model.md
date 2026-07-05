# 11 — Modeling registers with structs & headers

Blinking the on-board **green LED (LD4, PD12)**, but this time the peripheral registers are **modeled as bit-field structs in a header file**, and accessed through typed pointers with the `->` operator — instead of raw address casts and manual shift/mask.

This is a step up from exercises 01–07: the same hardware result, expressed with a structured, self-documenting register model split across a header and a source file.

## What's new here

**1. Registers as bit-field structs (in `main.h`)**

Each register is described as a struct where every field is one named, correctly-sized bit-field:

```c
typedef struct {
    uint32_t gpio_a_en : 1;
    uint32_t gpio_b_en : 1;
    uint32_t gpio_c_en : 1;
    uint32_t gpio_d_en : 1;
    // ... reserved fields keep the layout aligned to the real register
} RCC_AHB1ENR_t;
```

The reserved fields (`reserved1:3`, `reserved2:5`, …) are included so each named bit lands on its real hardware position — the struct mirrors the actual RCC_AHB1ENR bit map from the reference manual.

**2. Header / source separation**

Register definitions live in `main.h`; the logic lives in the `.c` file which includes it. This is how real firmware is organized — hardware descriptions in headers, application logic in source files.

**3. Typed access with `->`**

```c
volatile RCC_AHB1ENR_t * const pRCC = (RCC_AHB1ENR_t*) 0x40023830;
pRCC->gpio_d_en = 1;        // enable GPIOD clock — reads like plain English
pPortD_ModerReg->pin_12 = 1; // set PD12 mode
pPortD_ODRreg->pin_12   = 1; // drive PD12 high
```

Instead of `*pReg |= (1 << 3)`, the intent is now written directly: `pRCC->gpio_d_en = 1`. The field name says what it does.

## Qualifiers

The pointers use `volatile ... * const`: `volatile` because the register value changes from hardware, `const` because the pointer's address is fixed (carried over from exercises 06–07).

## Correct addressing

`RCC_AHB1ENR` is at RCC base + 0x30 = `0x40023830` (not the RCC base itself) — the struct is mapped onto the exact register address so each named field aligns with the real hardware bit.

## A note on portability

This bit-field register model is expressive and reads cleanly. It relies on how the compiler orders bit-fields within a word; since that ordering can differ across platforms, portable firmware often also uses the `volatile uint32_t` + shift/mask style (as in exercises 01–07). Both approaches are worth knowing — this exercise explores the struct model; the earlier ones use explicit bit operations.

---

## Türkçe

Yine yeşil LED'i (LD4, PD12) yakıp söndürüyorum, ama bu sefer çevre birimi register'larını **bir header dosyasında bit-field struct'ları olarak modelleyip**, tip'li pointer'lar ve `->` operatörüyle erişiyorum — ham adres cast'leri ve elle shift/mask yerine.

Bu, 01-07 alıştırmalarından bir adım ileri: aynı donanım sonucu, header ve source dosyasına bölünmüş, yapılandırılmış ve kendi kendini belgeleyen bir register modeliyle ifade ediliyor.

**Yeni olan ne:**

**1. Register'lar bit-field struct olarak (`main.h`):** Her register, her alanı isimli ve doğru boyutlu bir bit-field olan bir struct olarak tanımlanıyor. Reserved alanlar (`reserved1:3` vb.) dahil ediliyor ki her isimli bit gerçek donanım konumuna denk gelsin — struct, reference manual'daki gerçek RCC_AHB1ENR bit haritasını yansıtıyor.

**2. Header / source ayrımı:** Register tanımları `main.h`'de, mantık ise onu include eden `.c` dosyasında. Gerçek firmware böyle organize edilir — donanım tanımları header'da, uygulama mantığı source'ta.

**3. Tip'li erişim (`->`):** `*pReg |= (1 << 3)` yerine artık niyet doğrudan yazılıyor: `pRCC->gpio_d_en = 1`. Alan ismi ne yaptığını söylüyor.

**Niteleyiciler:** Pointer'lar `volatile ... * const` — register değeri donanımdan değiştiği için `volatile`, pointer adresi sabit olduğu için `const` (06-07'den taşınan yaklaşım).

**Doğru adresleme:** `RCC_AHB1ENR`, RCC base + 0x30 = `0x40023830` (RCC base'in kendisi değil) — struct tam register adresine oturtuluyor ki her isimli alan gerçek donanım bitine denk gelsin.

**Taşınabilirlik notu:** Bu bit-field register modeli anlamlı ve okunur. Derleyicinin bir kelime içinde bit-field'ları nasıl sıraladığına dayanır; bu sıralama platformlar arasında değişebildiği için, taşınabilir firmware genelde `volatile uint32_t` + shift/mask stilini de kullanır (01-07'deki gibi). İki yaklaşımı da bilmek değerli — bu alıştırma struct modelini keşfediyor, öncekiler açık bit işlemlerini kullanıyor.
