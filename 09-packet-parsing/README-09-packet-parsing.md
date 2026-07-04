# 09 — Packet bit-field parsing

Taking a raw **32-bit packet value** and splitting it into meaningful fields using **shift-and-mask** bit manipulation, then storing each field in a struct.

This is a core embedded task: raw data arrives from a sensor, register, or communication link as a single value, and the individual fields have to be extracted bit by bit.

## The packet layout

The 32 bits are divided into 8 fields. Each field occupies a fixed range of bits:

| Field | Bits | Width | Mask | Shift |
|-------|------|-------|------|-------|
| crc | 0–1 | 2 | `0x3` | 0 |
| status | 2 | 1 | `0x1` | 2 |
| payload | 3–14 | 12 | `0xFFF` | 3 |
| bat | 15–17 | 3 | `0x7` | 15 |
| sensor | 18–20 | 3 | `0x7` | 18 |
| longAddr | 21–28 | 8 | `0xFF` | 21 |
| shortAddr | 29–30 | 2 | `0x3` | 29 |
| addrMode | 31 | 1 | `0x1` | 31 |

Total: 2 + 1 + 12 + 3 + 3 + 8 + 2 + 1 = **32 bits** — the whole packet, no gaps.

## The extraction pattern

Each field is pulled out the same way: shift it down to bit 0, then mask off everything above its width.

```c
packet.payload = (uint16_t)((packetValue >> 3) & 0xFFF);
```

- `>> 3` moves the payload field down so it starts at bit 0.
- `& 0xFFF` keeps only the lowest 12 bits (the payload's width), discarding everything else.

This shift-and-mask idiom is the same one used throughout the register exercises (01–07) — the difference here is applying it systematically to decode a whole structured packet.

## Struct storage

Each extracted field is stored in a `struct Packet` member sized to fit (`uint8_t` for small fields, `uint16_t` for the 12-bit payload). The struct groups the decoded fields into one named record.

## Why this matters

Decoding a packed value into fields is exactly what happens when reading a status register, parsing a protocol frame (CAN, sensor data), or unpacking a communication packet. Getting the shift and mask right for every field — with no off-by-one bit errors — is the actual skill; a single wrong shift corrupts every field after it.

---

## Türkçe

Ham bir **32-bit paket değerini** alıp, **kaydır-maskele (shift-and-mask)** bit işlemleriyle anlamlı alanlara bölüyorum ve her alanı bir struct'ta saklıyorum.

Bu, temel bir embedded işi: ham veri bir sensörden, register'dan veya haberleşme hattından tek bir değer olarak gelir ve tek tek alanların bit bit çıkarılması gerekir.

**Paket düzeni:** 32 bit, 8 alana bölünmüş (crc 2 bit, status 1 bit, payload 12 bit, bat 3 bit, sensor 3 bit, longAddr 8 bit, shortAddr 2 bit, addrMode 1 bit). Toplam tam 32 bit, boşluk yok.

**Çıkarma kalıbı:** Her alan aynı yöntemle ayıklanıyor — önce alanı bit 0'a indir (`>>`), sonra sadece kendi genişliği kadar maskele (`&`). Örneğin `(packetValue >> 3) & 0xFFF`: payload'ı 3 bit aşağı kaydırıp en alttaki 12 biti tutuyor. Bu kaydır-maskele kalıbı, register alıştırmalarında (01-07) kullandığımın aynısı — buradaki fark, onu bütün bir yapılandırılmış paketi çözmek için sistematik uygulamak.

**Neden önemli:** Paketlenmiş bir değeri alanlara çözmek, tam olarak bir status register'ı okurken, bir protokol çerçevesini (CAN, sensör verisi) ayrıştırırken veya bir haberleşme paketini açarken yapılan şey. Her alan için doğru kaydırma ve maskeyi bulmak — tek bir bitlik kayma olmadan — asıl beceri; bir yanlış kaydırma, sonraki tüm alanları bozar.
