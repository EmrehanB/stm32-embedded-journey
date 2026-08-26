# Kaynaklar ve Makaleler / Resources & Articles

The courses, official documentation, and articles/resources I benefited from while working on this repository.

## Kurslar (Courses)

- [Microcontroller Embedded C Programming: Absolute Beginners](https://www.udemy.com/course/microcontroller-embedded-c-programming/) — Fastbit Embedded Brain Academy (Kiran Nayak), Udemy — Course 1
- [Embedded Systems Programming on ARM Cortex-M3/M4 Processor](https://www.udemy.com/course/embedded-system-programming-on-arm-cortex-m3m4/) — Fastbit Embedded Brain Academy (Kiran Nayak), Udemy — Course 2
- [Mikrodenetleyici Driver Geliştirme (GPIO, SPI, USART, I2C)](https://www.udemy.com/course/mikrodenetleyici-driver-gelistirme-gpio-spi-usart-i2c/learn/) — Erhan Konak, Udemy

## Resmi Dokümanlar (Official Documents)

### STMicroelectronics

- [STM32F407VG Datasheet](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — Pin haritası, elektriksel özellikler, çevre birimi listesi, blok diyagramı ve bus yapısı
- [RM0090 Reference Manual](https://www.st.com/en/microcontrollers-microprocessors/stm32f407vg.html#documentation) — Register haritaları, bit tanımları, çevre birimi davranışları. Bu depodaki her register erişiminin doğrulandığı kaynak
- [UM1472 — Discovery kit with STM32F407VG MCU](https://www.st.com/resource/en/user_manual/um1472-discovery-kit-with-stm32f407vg-mcu-stmicroelectronics.pdf) — Kart yerleşimi, LED ve buton pin atamaları, ST-LINK bağlantıları, lehim köprüleri
- [STM32F4DISCOVERY Şema ve Kart Dosyaları](https://www.st.com/en/evaluation-tools/stm32f4discovery.html#cad-resources) — Buton pull-down direnci, LED sürme devresi ve besleme bölümünün okunduğu şema

### ARM

- [Cortex-M4 Devices Generic User Guide (DUI0553)](https://developer.arm.com/documentation/dui0553/latest) — Yazılım geliştirici perspektifi: programcı modeli, işlemci modları, register seti, istisna modeli, komut seti ve çekirdek çevre birimleri (NVIC, SysTick, MPU)
- [Cortex-M4 Technical Reference Manual (DDI0439)](https://developer.arm.com/documentation/ddi0439/latest) — Donanım perspektifi: çekirdek iç yapısı, bus arayüzleri, pipeline, zamanlama karakteristikleri
- [ARMv7-M Architecture Reference Manual (DDI0403)](https://developer.arm.com/documentation/ddi0403/latest) — Mimarinin tam tanımı: komut seti detayları, istisna davranışı, bellek modeli kuralları

*Not: Generic User Guide yazılım geliştiriciler, Technical Reference Manual donanım tasarımcıları içindir. Günlük sürücü geliştirmede birincisi daha kullanışlıdır.*

## Makaleler ve Kaynaklar (Articles & Resources)

### C ve Derleyici

- [Introduction To The Volatile Keyword In C/C++](https://www.embedded.com/introduction-to-the-volatile-keyword/) — Nigel Jones, Embedded.com. Donanım registerlarına erişimde `volatile` neden zorunlu
- [What are Header Files in C Programming?](https://usemynotes.com/what-are-header-files-in-c/) — Alimam Miya, UseMyNotes
- [How do header and source files in C work?](https://stackoverflow.com/questions/5904530/how-do-header-and-source-files-in-c-work) — Stack Overflow

### Mimari

- [The Advanced Microcontroller Bus Architecture: An Introduction](https://www.allaboutcircuits.com/technical-articles/introduction-to-the-advanced-microcontroller-bus-architecture/) — Stephen St. Michael, All About Circuits. AHB ve APB ayrımı, köprü yapısı
- [Memory Space — an overview](https://www.sciencedirect.com/topics/engineering/memory-space) — ScienceDirect Topics

### Haberleşme Protokolleri

- [Serial Communication](https://learn.sparkfun.com/tutorials/serial-communication) — SparkFun. Seri ve paralel aktarım farkı, senkron ve asenkron ayrımı, baud rate, çerçeveleme. 
- [Serial Peripheral Interface (SPI)](https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/all) — SparkFun. MOSI/MISO/SCK/SS hatları, kaydırma yazmacı modeli, CPOL ve CPHA modları, çoklu slave bağlantısı
- [Shift Registers](https://learn.sparkfun.com/tutorials/shift-registers) — SparkFun. Seri-paralel dönüşüm ve kaydırma yazmacı mantığı — SPI'ın çalışma prensibinin donanımsal temeli

---

## Türkçe

Bu depodaki çalışmalar boyunca faydalandığım kurslar, resmi dokümanlar ve okuduğum makalelerin listesi. Kaynak göstermek amacıyla tutuluyor. Çalıştıkça burayı genişletiyorum.

Liste, çevre birimi sürücüleri eklendikçe (SPI, USART, I2C) yeni başlıklarla büyüyecek.
