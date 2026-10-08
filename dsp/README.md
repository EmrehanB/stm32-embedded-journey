# Digital Signal Processing on ARM Cortex-M4

Signal processing algorithms written in C and run on the STM32F407 (Cortex-M4F). The algorithms are implemented by hand first, then repeated with Arm's [CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) library so the two can be compared on the same hardware.

This section is based on the Udemy course *[DSP From Ground Up™ on ARM Processors](https://www.udemy.com/course/arm-cortex-dsp/)* by BHM Engineering Academy. It is kept outside the `course-N` numbering used for the [Fastbit sequence](../) because it is a separate learning resource. The course targets an STM32 Nucleo board; the exercises here run on the F407 Discovery, and the differences are recorded below.

## Scope

- Signal statistics and noise: mean, variance, standard deviation
- Sampling and quantization
- Convolution, running sum and moving average
- FIR and IIR filters, filter design
- Discrete Fourier transform and its inverse
- Signal acquisition through a register-level ADC driver and a FIFO buffer
- CMSIS-DSP versions of the same algorithms
- Processing a live signal sampled by the ADC

## Exercises

Exercises are added as the course progresses. Each one lives in its own numbered folder (`01-…`, `02-…`) with its source files and a README explaining the approach.

## Hardware Notes: Nucleo → F407 Discovery

- **ADC input pin.** Several Discovery pins are wired to on-board chips: PA4 to the audio DAC, PA5–PA7 to the accelerometer, PC3 to the microphone. Pins listed as free I/O in [UM1472](https://www.st.com/resource/en/user_manual/um1472-discovery-kit-with-stm32f407vg-mcu-stmicroelectronics.pdf) Table 7 and usable as ADC inputs: PA1 (ADC123_IN1), PB0 (ADC12_IN8), PB1 (ADC12_IN9), PC1 (ADC123_IN11), PC2 (ADC123_IN12).

- **Getting data to the PC.** On a Nucleo, USART2 is wired to the ST-LINK virtual COM port, so one USB cable carries both debug and serial output. On the Discovery it is not ([UM1472](https://www.st.com/resource/en/user_manual/um1472-discovery-kit-with-stm32f407vg-mcu-stmicroelectronics.pdf) §7.3.3). The alternatives are a USB-to-UART adapter on PA2/PA3, the SWO trace pin (PB3, connected to the ST-LINK by default) with ITM, or reading buffers directly through the debugger.

- **Clock source.** The Discovery's HSE is an 8 MHz crystal. Where an exercise configures the PLL from HSE, the oscillator runs in crystal mode (`HSEBYP = 0`) rather than the bypass mode used when a clock is fed in externally.

- **The FPU is off after reset.** The Cortex-M4F floating-point unit must be enabled by giving full access to coprocessors CP10 and CP11 in `SCB->CPACR` (bits 20–23) before the first floating-point instruction executes. Otherwise the core raises a UsageFault, which escalates to a HardFault. Any exercise that uses `float` depends on this, so it is the first thing to check when one faults.

- **CMSIS-DSP is used as a math library, not as a peripheral layer.** The repository rule — register-level access wherever hardware is involved — still holds: the ADC and every other peripheral are driven through registers, and CMSIS-DSP is only called for the numerical work.

## References

- [The Scientist and Engineer's Guide to Digital Signal Processing](https://www.dspguide.com/) — Steven W. Smith
- [CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP) — Arm
- [RM0090 Reference Manual](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — ADC chapter
- Further reading is collected in [`RESOURCES.md`](../RESOURCES.md)

---

## Türkçe Özet

STM32F407 (Cortex-M4F) üzerinde C ile yazılmış sinyal işleme algoritmaları. Algoritmalar önce elle yazılıyor, sonra Arm'ın CMSIS-DSP kütüphanesiyle tekrar edilerek aynı donanım üzerinde karşılaştırılıyor.

BHM Engineering Academy'nin *DSP From Ground Up™ on ARM Processors* Udemy kursunu temel alıyor. Kurs bir STM32 Nucleo kartı kullanıyor; buradaki alıştırmalar F407 Discovery'de çalışıyor ve aradaki farklar yukarıda kayıtlı: ADC için boş pinler, sanal seri portun Discovery'de MCU'ya bağlı olmaması, HSE'nin kristal modu ve FPU'nun reset sonrası kapalı gelmesi.

CMSIS-DSP yalnızca matematik kütüphanesi olarak kullanılıyor; ADC dahil tüm çevre birimleri register seviyesinde sürülüyor.
