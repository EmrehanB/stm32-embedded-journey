#include "RCC.h"

const uint8_t AHB_Prescaler [] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9} ;
const uint8_t APB_Prescaler [] = {0,0,0,0,1,2,3,4} ;

uint32_t RCC_GetSystemClock(){

	uint32_t SystemCoreClock = 0 ;
	uint8_t  ClockSource     = 0 ;  //RCC registerlarından RCC clock congig registerin SWS (System clock switch status) bitini buna okuruz.

	ClockSource = ((RCC->CFGR >> 2U) & 0x3U); //2.ve 3. bitler olduğundna önce 2 şer bit sağa kaydırdık ardından 0011 ile AND leyerek değerini öğrendik

	switch(ClockSource)
	{

	case 0  : SystemCoreClock = 16000000; break; //16MHz
	case 1  : SystemCoreClock = 8000000 ; break; //8MHz

	default : SystemCoreClock = 16000000;

	}

	return SystemCoreClock ;

}


uint32_t RCC_GetHClock(void){

	uint32_t AHB_PeriphClock = 0 ;
	uint32_t SystemCoreClock = 0 ;


	SystemCoreClock = RCC_GetSystemClock();

	uint8_t  tempValue = 0;
	uint8_t HPRE_Value = ((RCC->CFGR >> 4) & 0xFU) ;

	tempValue = AHB_Prescaler[HPRE_Value];

	AHB_PeriphClock = SystemCoreClock >> tempValue;


	return AHB_PeriphClock;
}


uint32_t RCC_GetPClock1(void){

	uint32_t APB1_PeriphClock = 0;
	uint32_t HClock = 0 ;

	uint8_t HPRE1_Value = 0;

	HClock = RCC_GetHClock();

	HPRE1_Value = ((RCC->CFGR >> 10U) & 0x7U );

	uint8_t  tempValue = 0;
	tempValue = APB_Prescaler[HPRE1_Value];

	APB1_PeriphClock = HClock >> tempValue;

	return APB1_PeriphClock;
}


uint32_t RCC_GetPClock2(void){

	uint32_t APB2_PeriphClock = 0;
	uint32_t HClock = 0 ;

	uint8_t HPRE2_Value = 0;

	HClock = RCC_GetHClock();

	HPRE2_Value = ((RCC->CFGR >> 13U) & 0x7U );

	uint8_t  tempValue = 0;
	tempValue = APB_Prescaler[HPRE2_Value];

	APB2_PeriphClock = HClock >> tempValue;

	return APB2_PeriphClock;
}










