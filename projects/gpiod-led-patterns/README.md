# GPIOD LED Patterns

A bare-metal LED blink sequence on the STM32F407G-DISC1's onboard LEDs (PD12–PD15), written during a power outage — no HAL, register-level, using the `GPIO_TypeDef_t*` cast pattern from the driver-development header.

## What it does

Runs one continuous loop: each of the four onboard LEDs (PD13, PD12, PD14, PD15) blinks individually in sequence, then all four blink together for 4 rounds, before the whole thing repeats.

## Hardware

| Signal | Pin | Role |
|---|---|---|
| Onboard LEDs | PD12, PD13, PD14, PD15 | Output, orange/green/red/blue |

No external wiring — all four onboard LEDs of the STM32F407G-DISC1.

## Notes

Uses a local, trimmed-down version of the `GPIO_TypeDef_t` struct (only through `ODR`, since that's all this program needs) rather than the full shared header in `driver-development/`. Timing is busy-wait based (`volatile` loop counters), same approach as the Reaction Timer project.

---

## Türkçe

Elektrik/internet kesintisi sırasında can sıkıntısından yazılmış, STM32F407G-DISC1'in kart üstü dört LED'ini (PD12–PD15) register seviyesinde kontrol eden bir blink dizisi. Önce her LED sırayla tek tek yanıp sönüyor, sonra hepsi birlikte 4 tur yanıp sönüyor, döngü baştan başlıyor.
