

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f407xx.h"




// @def_group SPI_BaudRates
#define SPI_BAUDRATE_DIV2  	   ( (uint32_t) (0x00) ) // 3. , 4. ve 5. bitlere bakıyoruz ilkinin 3. biti sıfır yani her şey sıfır
#define SPI_BAUDRATE_DIV4      ( (uint32_t) (0x08) ) // bu aslında 3. biti 1 olan bir sayıdır basit bir define
#define SPI_BAUDRATE_DIV8      ( (uint32_t) (0x10) )
#define SPI_BAUDRATE_DIV16     ( (uint32_t) (0x18) ) // 3. ve 4. bit 1 yani sayımız bit olarak 11000 --> 24 e  eşittir. 24 hexadecimal olarak 0x18 dir
#define SPI_BAUDRATE_DIV32     ( (uint32_t) (0x20) )
#define SPI_BAUDRATE_DIV64     ( (uint32_t) (0x28) )
#define SPI_BAUDRATE_DIV128    ( (uint32_t) (0x30) )
#define SPI_BAUDRATE_DIV256    ( (uint32_t) (0x38) )



// @def_group SPI_CPHA_Values
#define SPI_CPHA_FirstEdge 	  (uint32_t)(0x00)  //0. bit 0
#define SPI_CPHA_SecondEdge   (uint32_t)(0x01)  //0. bit 1



// @def_group SPI_CPOL_Values
#define SPI_CPOL_LOW          (uint32_t)(0x00)  //1. bit 0
#define SPI_CPOL_HIGH		  (uint32_t)(0x02)  //1. bit 1  ---> 0010 yani 0x02



// @def_group SPI_DataFrameFormat
#define SPI_DFF_8BITS  		  (uint32_t)(0x000)   // 11. bit 0
#define SPI_DFF_16BITS		  (uint32_t)(0x800)   // 11. bit 1  --> 1000 0000 0000 yani 0x800



//@def_group SPI_MSTR_ModeValues
#define SPI_MODE_SLAVE        (uint32_t)(0x00)	  // 2. bit 1   --> 0100 yani 0x04
#define SPI_MODE_MASTER       (uint32_t)(0x04)	  // 2. bit 0




//@def_group FrameFormatValues
#define SPI_FrameFormat_MSB   (uint32_t)(0x00)	  // 7. bit 0 --> 0000 0000
#define SPI_FrameFormat_LSB   (uint32_t)(0x80)    // 7. bit 1 --> 1000 0000



/*
 * BIDIMODE (Bidirectional Data Mode) : Kac veri teli kullanilacak?
 *                                      0 -> 2 tel (MOSI + MISO), full duplex
 *                                      1 -> 1 tel, half duplex -- yon degistirilerek kullanilir
 *
 * BIDIOE (Bidirectional Output Enable) : Sadece BIDIMODE = 1 iken anlamli.
 *                                        Tek telin o anki yonunu secer.
 *                                        0 -> alma (receive)
 *                                        1 -> gonderme (transmit)
 *
 * RXONLY (Receive Only) : Sadece BIDIMODE = 0 iken anlamli.
 *                         0 -> full duplex, hem gonder hem al
 *                         1 -> yalnizca al; MOSI hatti kullanilmaz
 *
 * Uc kombinasyon:
 *   BIDIMODE=0, RXONLY=0 -> Full duplex   (varsayilan ve en yaygin)
 *   BIDIMODE=0, RXONLY=1 -> 2 tel ama yalnizca alma
 *   BIDIMODE=1           -> Half duplex, yon BIDIOE ile secilir
 */



//@def_group BusConfigValues
#define SPI_BUS_FullDuplex    (uint32_t)(0x000)   // 15. ve 10. bit 0
#define SPI_BUS_ReceiveOnly   (uint32_t)(0x400)   // 15. bit 0 ama 10. bit 1 yani full duplex ama receive only
#define SPI_BUS_HalfDuplexR   (uint32_t)(0x8000)  // 15. bit 1 ve 14. bit 0 (half duplex receive only)
#define SPI_BUS_HalfDuplexT   (uint32_t)(0xC000)  // 15. bit 1 ve 14. bit 1 (half duplex transmit only)


/*
 * NSS (Negated Slave Select) : Slave secim sinyali. Aktif dusuk -- 0 iken slave secili.
 *                              Diger isimleri: CS, nCS, SS. Ayni tel, farkli isim.
 *
 * SSM (Software Slave Management) : NSS'in degeri nereden okunsun?
 *                                   0 -> fiziksel NSS pininden (donanim yonetimi)
 *                                   1 -> SSI bitinden          (yazilim yonetimi)
 *
 * SSI (Internal Slave Select) : SSM = 1 iken NSS yerine gecen register biti.
 *                               Master modda 1 tutulmali; aksi halde donanim
 *                               "baska bir master var" sanip kendini slave'e
 *                               dusurur (MODF hatasi).
 *
 * Master icin yaygin kalip: SSM = 1, SSI = 1 --> chip select ayri bir GPIO ile yonetilir.
 */


/*
 * MODF (Mode Fault) : SPI_SR'deki hata bayragi.
 *
 * Sebep: Master modundayken (MSTR = 1) NSS DUSUK gorulurse olusur. Donanim bunu
 *        "baska bir master hatti ele gecirdi" sanar ve cakismayi onlemek icin
 *        kendini kapatir. NSS'in nereden okundugu SSM'e baglidir:
 *        SSM = 0 -> fiziksel pin,  SSM = 1 -> SSI biti.
 *
 * Sonuc: MSTR ve SPE bitleri donanim tarafindan temizlenir -- SPI aniden durur,
 *        kodda hicbir hata gorunmez.
 *
 * Tek master sistemde: NSS pini kullanilmayip havada kalirsa gurultu onu asagi
 *        cekebilir ve SPI kendiliginden coker. Bu yuzden SSM = 1, SSI = 1 yapilir.
 *
 * Temizleme: once SPI_SR okunur, sonra SPI_CR1'e yazilir. Ardindan MSTR ve SPE
 *        elle tekrar set edilmelidir.
 */



// @def_group SSM_Values
#define SPI_SSM_DISABLE		  (uint32_t)(0x000) // 9. bit 0

#define SPI_SSM_ENABLE        (uint32_t)(0x300) // Hem 9. bit 1 hem 8. bit 1 olacak 0011 0000 0000  . Yukarıda anlatılan olaydan dolayı (MODF hatası).


typedef enum{  //Flaglerim SR registerda

	SPI_FLAG_RESET = 0x0U,
	SPI_FLAG_SET   = !SPI_FLAG_RESET

}SPI_FlagStatus_t;


typedef struct{

	uint32_t Mode		  ;    // @def_group SPI_ModeValues'e göre değerler atanacaktır. Master-Slave
	uint32_t CPHA   	  ;    // @def_group SPI_CPHA_Values'e göre değerler atanacaktır.
	uint32_t CPOL   	  ;    // @def_group SPI_CPOL_Values'e göre değerler atanacaktır.
	uint32_t BaudRate	  ;    // @def_group SPI_BaudRates'e göre değerler atanacaktır.
	uint32_t SSM_Cmd	  ;    // @def_group SSM_Values
	uint32_t DFF          ;    // @def_group SPI_DataFrameFormat'a göre değerler atanacaktır. Data frame format yani 8 bit data mı yollayacağım 16 bit mi? 0-->8 bit ve 1-->16 bit
	uint32_t BusConfig	  ;    // @def_group BusConfigValues . Half duplex (BIDIMODE 1) mi full duplex (BIDIMODE 0) mi ?  CR1 registerındaki BIDIMODE bitini etkileyerek ayarlıyoruz. BIDIMODE'u 0 a çekersem RXONLY bit'i ile de işimiz var ama 1 e çekersem BIDIOE bit'i devreye girer ve bu biti 0 yaparsan receive-only ve 1 yaparsak transmit-only mode a alırız.
	uint32_t FrameFormat  ;    // @def_group FrameFormatValues (CR1 registerının LSBFIRST bit'ine dokunacak) . Frame format önce yüksek öncelikli msb biti mi gitsin yoksa düşük öncelikli lsb biti mi?  @def_group FrameFormatValues (CR1 registerının LSBFIRST bit'ine dokunacak)

}SPI_InitTypeDef_t;


typedef struct{               //kodun daha güzel gözükmesi açısından oluşturdum.

	SPI_TypeDef_t *Instance ; // SPI1/2/3/4 bunlardan birinin adresini tutacak.
	SPI_InitTypeDef_t Init  ;

}SPI_HandleTypeDef_t;


void SPI_Init(SPI_HandleTypeDef_t *SPI_Handle);



void SPI_PeriphCmd(SPI_HandleTypeDef_t *SPI_Handle , FunctionalState_t stateOfSPI);


//polling metodu ile transmit (send) data yapacağız
void SPI_TransmitData(SPI_HandleTypeDef_t *SPI_Handle , uint8_t *pData , uint16_t sizeOfData);
// aşağıda veri gönderme presdürünün açıklaması var


SPI_FlagStatus_t SPI_GetFlagStatus(SPI_HandleTypeDef_t *SPI_Handle , uint16_t SPI_Flag);


/*
 * SPI VERI GONDERME PROSEDURU  (RM0090, "Transmitting/Receiving data")
 *
 * ---- Uc bayrak (SPI_SR registerinda) ----
 *
 * TXE  (Transmit buffer Empty, bit 1)
 *      1 -> gonderme tamponu bos, DR'ye yeni veri yazilabilir
 *      0 -> tampon dolu; yazilirsa uzerine yazilir ve veri kaybolur
 *      DR'ye yazmak bu bayragi temizler.
 *
 * RXNE (Receive buffer Not Empty, bit 0)
 *      1 -> alma tamponunda okunmamis veri var
 *      DR'yi okumak bu bayragi temizler.
 *
 * BSY  (Busy, bit 7)
 *      1 -> SPI hala hat uzerinde is yapiyor
 *      0 -> son bit de hattan cikti
 *
 *
 * ---- Neden ayri bayraklar: cift tamponlama ----
 *
 *   Yazilim -> [TX tampon] -> [Kaydirma yazmaci] -> MOSI
 *   Yazilim <- [RX tampon] <- [Kaydirma yazmaci] <- MISO
 *
 *   Kaydirma yazmaci hatti surerken tampona bir sonraki bayt konabilir. Boylece
 *   bir bayt biter bitmez digeri baslar ve hat bos kalmaz.
 *   TXE = 1 olmasi verinin HATTAN CIKTIGI anlamina GELMEZ -- sadece tamponun
 *   bosaldigini soyler.
 *
 *
 * ---- Adimlar ----
 *
 * 1) SPE = 1 yapilir (SPI_CR1).
 *    Tum konfigurasyon (CPOL, CPHA, BaudRate, MSTR...) SPE = 0 iken yapilmis
 *    olmalidir; SPE acikken bu bitleri degistirmek tanimsiz davranistir.
 *
 * 2) Ilk veri DR'ye yazilir. TXE zaten 1 oldugu icin beklemeye gerek yoktur.
 *
 * 3) Dongu: her veri icin
 *       TXE = 1 olana kadar bekle  -> sonraki veriyi DR'ye yaz
 *       RXNE = 1 olana kadar bekle -> gelen veriyi DR'den oku
 *
 *    SPI full duplex oldugu icin her gonderilen bayt karsiliginda bir bayt da
 *    gelir. Okunmazsa RX tamponu tasar (OVR - overrun hatasi). Sadece gonderiyor
 *    olsan bile gelen bayt okunup ATILMALIDIR.
 *
 *    Zamanlama: gonderme, almadan bir adim ondedir (boru hatti davranisi).
 *
 * 4) Dongu n-1 veriyi isler. Son gonderilen baytin karsiligi hala yoldadir:
 *    RXNE = 1 beklenip son veri okunur.
 *
 * 5) Kapatmadan once IKI kontrol:
 *       TXE = 1 bekle  -> tampon bosaldi
 *       BSY = 0 bekle  -> son bit gercekten hattan cikti
 *    Sonra SPE = 0 yapilabilir.
 *
 *    TXE tek basina YETMEZ: tampon bos olsa da kaydirma yazmacinda veri
 *    olabilir. Beklemeden SPE = 0 yapilirsa son bayt yarim kalir, slave bozuk
 *    veri alir ve senkronizasyon bozulur.
 *
 *
 * ---- Uc uygulama seviyesi ----
 *
 *   Polling : while dongusuyle bayrak beklenir. Islemci baska is yapamaz.
 *   Kesme   : SPI_CR2'deki TXEIE ve RXNEIE bitleri acilir; bayraklar kesme uretir.
 *   DMA     : Islemci hic karismadan tum blok aktarilir.
 *
 *   Bu surucude polling kullaniliyor.
 */



#endif /* INC_SPI_H_ */
