
#include <stdint.h>


int main(void)
{

	int val=50;
	__asm volatile("MOV R0,%0": : "r" (val) ); // Buradaki r general registerların kullanılmasını saglıyor. Compiler a veri işlemek için register kullanmasını söyler.
	__asm volatile("MOV R0,%0": : "i" (0x50) ); // Birde i var bu ise immediate değerler içindi.

	//Aslında arka planda ldr ve mov komutları çalıştı bu bize bir soyutlama sağlıyor.

	uint32_t control_reg;

	__asm volatile("MRS %0,CONTROL":"=r"(control_reg) :: );

	//Control source ve %0 destination (C değişkeni olan control_reg)

	//Bu durumda control_reg i output operandı olarak düşünebiliriz çünkü destinasyon. MRS--> special register to register bu yüzden i değil r kullanmak zorundayım.

	//Başındaki "=" bir constraint modifier (kısıtlama değiştirici) ve şu demek: "bu operand sadece yazılır" (write-only, output).

	//"r"(x) → input, x okunur ------- "=r"(x) → output, x'e yazılır


	int var1=10,var2=20;

	__asm volatile ("MOV %0,%1" : "=r"(var1):"r"(var2));


	/*


	Output bölümü (ilk :'ten sonra): "=r"(var1)


	(var1) → sonuç buraya yazılacak

	"=r" → = bu bir output (write-only), r bir register kullan

	Yani: "MOV'un sonucunu bir register'a yaz, sonra o register'ı var1'e aktar"

	Input bölümü (ikinci :'ten sonra): "r"(var2)

	(var2) → kaynak değer buradan gelecek

	"r" → var2'yi bir register'a yükle (output değil, = yok)

	Yani: "var2'yi bir register'a al, MOV'da kaynak olarak kullan"


	*/


	//Disassembly:


	/*
	  LDR R3, [var2'nin adresi]    ; var2'yi (20) RAM'den register'a yükle

      MOV R2, R3                    ; senin MOV'un: R3'ü R2'ye kopyala

      STR R2, [var1'in adresi]      ; sonucu var1'in RAM adresine yaz


	 */








	for(;;);
}
