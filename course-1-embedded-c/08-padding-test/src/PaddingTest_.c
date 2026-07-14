

#include <stdio.h>
#include <stdint.h>

struct Ornek{
  int a;
  char b;
  short c;
};



int main()
{
   struct Ornek x={1,2,3};

   printf("Total boyut: ");

   printf("%d",sizeof(x));

   //Padding (dolgu) yüzünden sizeof, üyelerin toplamından büyük çıkar.
   //int(4) + char(1) + 1 byte padding + short(2) = 8
   //short 2'nin katı adreste durmalı, bu yüzden char'dan sonra 1 byte boş bırakılır.


    printf("\n\n Addresss                    valueOfAddress (1 byte )\n");
    printf("-------------------------------------------------------\n");

    uint8_t *ptr=(uint8_t*) &x;

    uint8_t recordSize=sizeof(x);

    for(int i =0;i<recordSize;i++){

    printf("%p                     %d\n",ptr,*ptr);
    ptr++;


    }
    getchar();
    return 0;

}
