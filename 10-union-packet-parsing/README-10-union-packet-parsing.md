# 10 — Union-based packet parsing

Decoding the same **32-bit packet** as exercise 09, but using a **union + struct** instead of manual shift-and-mask. Writing the raw value into one union member and reading the fields back through another — with no bit manipulation at all.

The new concept here is the **union**: unlike a struct (members side by side), a union's members all share the *same* memory. That shared-memory property is what makes this technique work.

## How a union differs from a struct

- **struct** — members sit next to each other; size = sum of members (plus padding).
- **union** — all members overlap at the same address; size = the *largest* member.

So a union lets you view the same bytes through different types.

## The layout

```c
union Packet {
    uint32_t packetValue;        // the raw 32-bit value

    struct {
        uint32_t crc:2;
        uint32_t status:1;
        uint32_t payload:12;
        uint32_t bat:3;
        uint32_t sensor:3;
        uint32_t longAddr:8;
        uint32_t shortAddr:2;
        uint32_t addrMode:1;
    } packetFields;              // the same 4 bytes, seen as bit-fields
};
```

Both members are exactly 4 bytes:
- `packetValue` — one raw `uint32_t`.
- `packetFields` — 8 bit-fields totalling 2+1+12+3+3+8+2+1 = 32 bits = 4 bytes.

Because both are 4 bytes and a union overlaps its members, they occupy the **same** 4 bytes. `sizeof(union) == 4`.

## What this buys you

```c
scanf("%X", &packet.packetValue);           // write raw value
printf("%#x", packet.packetFields.crc);     // read a field — already split
```

Write the raw value once, then read each field directly. The union makes the bit-fields reinterpret the same memory automatically — **all the shift-and-mask from exercise 09 disappears.** Much shorter and more readable.

## The trade-off (important)

This is the elegant version, but it is **less portable** than exercise 09's explicit shift-and-mask — for two reasons stacked on top of each other:

1. **Bit-field ordering is compiler-dependent.** The C standard doesn't fix whether `crc:2` lands in the low or high bits of the word (same caveat noted in exercise 09).
2. **Endianness (byte order) matters.** Reinterpreting the same bytes as a raw integer vs. as bit-fields depends on the machine's byte order, so the result can differ across platforms.

So the same code can produce correct output on this machine and wrong field values on another compiler or CPU. For a real hardware register or a wire protocol — where bit positions are fixed by the device — this is risky.

## When to use which

- **Union + struct (this exercise):** concise, convenient — fine for quick work that stays on one compiler/platform.
- **Explicit shift-and-mask (exercise 09):** longer, but every field sits where *you* put it, identically everywhere — the right choice for real registers and portable protocols.

Same result, different cost: this one trades portability for brevity. Knowing *which* to reach for is the actual engineering judgment.

---

## Türkçe

09. alıştırmadaki **aynı 32-bit paketi**, elle shift-and-mask yerine **union + struct** kullanarak çözüyorum. Ham değeri union'ın bir üyesine yazıp alanları başka bir üyeden okuyorum — hiç bit manipülasyonu yapmadan.

**Yeni kavram — union:** Struct'ta üyeler yan yana durur (boyut = üyelerin toplamı). Union'da ise tüm üyeler *aynı* belleği paylaşır (boyut = en büyük üye). Bu sayede aynı byte'lara farklı tiplerle bakabilirsin.

**Yapı:** Union'ın iki üyesi var, ikisi de 4 byte — `packetValue` (ham uint32) ve `packetFields` (8 bit-field, toplam tam 32 bit = 4 byte). İkisi de 4 byte olduğu ve union üyeleri üst üste bindiği için, **aynı** 4 byte'ı paylaşıyorlar. `sizeof(union) == 4`.

**Kazanç:** Ham değeri `packetValue`'ya bir kez yaz, sonra `packetFields.crc` gibi alanları doğrudan oku. Union, bit-field'ların aynı belleği otomatik yeniden yorumlamasını sağlıyor — 09'daki tüm shift-and-mask kayboluyor. Çok daha kısa ve okunur.

**Bedel (önemli):** Bu zarif versiyon, 09'un elle shift-and-mask'inden **daha az taşınabilir** — üst üste binen iki sebepten: (1) **bit-field bit sırası derleyiciye bağlı** (09'daki uyarının aynısı), (2) **endianness (byte sırası) önemli** — aynı byte'ları ham int vs bit-field olarak yorumlamak makinenin byte sırasına bağlı, sonuç platforma göre değişebilir. Yani aynı kod bu makinede doğru, başka bir derleyici/işlemcide yanlış alan değerleri verebilir. Bit konumlarının cihaz tarafından sabitlendiği gerçek register/protokol için riskli.

**Ne zaman hangisi:** Union+struct (bu alıştırma) — kısa ve pratik, tek derleyici/platformda kalacak hızlı işler için uygun. Elle shift-and-mask (09) — daha uzun ama her alan **senin** koyduğun yerde, her yerde aynı; gerçek register ve taşınabilir protokoller için doğru seçim. Aynı sonuç, farklı bedel: bu, taşınabilirliği kısalığa değişiyor. Hangisine uzanacağını bilmek asıl mühendislik muhakemesidir.
