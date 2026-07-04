# 08 — Struct padding & data alignment

A desktop C experiment (not on the board) to see **struct padding** in action: the compiler inserts empty bytes so each member sits on its natural alignment boundary, which makes a struct larger than the sum of its members.

## The struct

```c
struct Ornek {
    int   a;   // 4 bytes
    char  b;   // 1 byte
    short c;   // 2 bytes
};
```

Naive size: 4 + 1 + 2 = **7 bytes**. Actual `sizeof`: **8 bytes**.

## Why 8, not 7

Each type must sit at an address that is a multiple of its size (its natural alignment):

| Member | Size | Address | Note |
|--------|------|---------|------|
| `a` (int) | 4 | 0–3 | int needs a multiple of 4 -> address 0 |
| `b` (char) | 1 | 4 | char can go anywhere -> address 4 |
| *(padding)* | 1 | 5 | inserted so `c` can align |
| `c` (short) | 2 | 6–7 | short needs a multiple of 2 -> address 6, not 5 |

The compiler leaves **1 byte of padding at address 5**, because `short` cannot start at an odd address — it must begin on a 2-byte boundary. Total: 8 bytes.

## Seeing it byte by byte

The program walks the struct one byte at a time with a `uint8_t*` pointer and prints each address and its value:

```c
uint8_t *ptr = (uint8_t*) &x;
for (int i = 0; i < sizeof(x); i++) {
    printf("%p  %d\n", ptr, *ptr);
    ptr++;
}
```

With `x = {1, 2, 3}`, the meaningful values appear at the member positions, while the **padding byte at index 5 holds garbage** — an uninitialized value, because nothing was ever written there. This makes the padding visible: it is a real, unused byte the compiler inserted purely for alignment.

## Why alignment exists

A 32-bit CPU reads memory in blocks. A value that sits aligned (on a boundary matching its size) is fetched in a single read; a misaligned value would span two blocks and need two reads — or crash on some architectures. The compiler pads structs to keep every member aligned and fast to access.

## Relevance to embedded

This matters for register structs (coming later): peripheral registers are modeled as a struct, and each member must land on the exact hardware offset. On STM32 every register is a `uint32_t` on a 4-byte boundary, so they're naturally aligned and padding usually doesn't interfere — but understanding padding is what lets you reason about offsets correctly when mixed sizes or reserved fields appear.

---

## Türkçe

Board üzerinde değil, masaüstünde yapılan bir C deneyi: **struct padding'i** canlı görmek. Derleyici, her üyeyi kendi doğal hizalama sınırına oturtmak için araya boş byte'lar ekler; bu yüzden struct, üyelerinin toplamından büyük olur.

**Struct:** `int a` (4) + `char b` (1) + `short c` (2). Naif toplam 7, gerçek `sizeof` **8**.

**Neden 8:** Her tip, adresi kendi boyutunun katı olan yerde durmalı. `a` (int) adres 0'da, `b` (char) adres 4'te, sonra `c` (short) 2'nin katı adreste durmalı — adres 5'te duramaz, bu yüzden adres 5'e **1 byte padding** eklenir ve `c` adres 6-7'ye gider. Toplam 8.

**Byte byte görme:** Program struct'ı `uint8_t*` pointer ile tek tek gezip her adresi ve değerini yazdırıyor. `x = {1,2,3}` ile anlamlı değerler üye konumlarında görünürken, **adres 5'teki padding byte'ı çöp değer** içeriyor — çünkü oraya hiç yazılmadı. Bu, padding'i görünür kılıyor: derleyicinin sırf hizalama için eklediği gerçek, kullanılmayan bir byte.

**Neden hizalama var:** 32-bit işlemci belleği bloklar halinde okur. Hizalı bir değer tek okumada alınır; hizasız bir değer iki bloğa yayılır, iki okuma gerektirir (bazı mimarilerde çöker). Derleyici, her üyeyi hizalı ve hızlı erişilir tutmak için struct'a padding ekler.

**Embedded ile ilgisi:** Bu, ileride register struct'larında önemli olacak — çevre birimi register'ları struct ile modellenir ve her üye tam donanım offset'ine denk gelmeli. STM32'de her register `uint32_t` ve 4'ün katı hizada olduğu için doğal hizalı, padding genelde sorun çıkarmaz; ama padding'i anlamak, farklı boyutlar veya reserved alanlar girdiğinde offset'leri doğru hesaplamanı sağlar.
