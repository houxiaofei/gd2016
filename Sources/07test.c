/*
 * 07test.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned int pit1count=0;
unsigned char test_output=0;

void test(void){
	
	SET_motor_test();
	SET_steer_test();
	IO_test();
	
}

/*************************µç»ú²âÊÔº¯Êý*********************/
void SET_motor_test(void)                                      //CH16 Count500
{
	EMIOS_0.CH[19].CBDR.R = 80;EMIOS_0.CH[20].CBDR.R = 0;//ÓÒÂÖ  E3ÓÒ½ø   E4ÓÒÍË
	EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R = 100;//×óÂÖ  E5×óÍË   E6×ó½ø
}

/*************************¶æ»ú²âÊÔº¯Êý***********************/
void SET_steer_test(void)                                   //CH23 Count50000
{
	EMIOS_0.CH[11].CBDR.R = CENTER;
}

/*************************¶æ»ú²âÊÔº¯Êý***********************/
void IO_test(void)                                   //CH23 Count50000
{
	if(IOtest==1)
	{
		IOtest=0;
		pit1count=~pit1count;
		if(pit1count==0)
			test_output=0;
		else
			test_output=1;
//		SIU.GPDO[24].R = test_output;//CCDL AO  B8
//		SIU.GPDO[27].R = ~SIU.GPDO[27].R;//CCDL CLK B11
//		SIU.GPDO[61].R = ~SIU.GPDO[61].R;//CCDL SI  D13
//		SIU.GPDO[26].R = ~SIU.GPDO[26].R;//CCDR AO  B10
//		SIU.GPDO[63].R = ~SIU.GPDO[63].R;//CCDR CLK D15
//		SIU.GPDO[62].R = ~SIU.GPDO[62].R;//CCDR SI  D14
//		SIU.GPDO[25].R = ~SIU.GPDO[25].R;//CCDM AO  B9
//		SIU.GPDO[46].R = ~SIU.GPDO[46].R;//CCDM CLK C14
//		SIU.GPDO[2].R = ~SIU.GPDO[2].R ; //CCDM SI  A2
//		SIU.GPDO[59].R = ~SIU.GPDO[59].R;//COUNTER1 D11
//		SIU.GPDO[60].R = ~SIU.GPDO[60].R;//COUNTER1 D12
//		SIU.GPDO[6].R = ~SIU.GPDO[6].R ; //COUNTER2 A6
//		SIU.GPDO[8].R = ~SIU.GPDO[8].R ; //COUNTER1 A8
//		SIU.GPDO[72].R = ~SIU.GPDO[72].R;//OLED     E8
//		SIU.GPDO[74].R = ~SIU.GPDO[74].R;//OLED     E10
//		SIU.GPDO[75].R = ~SIU.GPDO[75].R;//OLED     E11
//		SIU.GPDO[42].R = ~SIU.GPDO[42].R;//OLED     C10
//		SIU.GPDO[17].R = ~SIU.GPDO[17].R;//OLED     B1
//		SIU.GPDO[0].R = ~SIU.GPDO[0].R ; //BEE      A0
//		SIU.GPDO[9].R = ~SIU.GPDO[9].R ; //SUPER1   A9
//		SIU.GPDO[5].R = ~SIU.GPDO[5].R ; //SUPER1   A5
//		SIU.GPDO[66].R = ~SIU.GPDO[66].R;//SUPER1   E2
//		SIU.GPDO[18].R = ~SIU.GPDO[18].R;//UART     B2
//		SIU.GPDO[19].R = ~SIU.GPDO[19].R;//UART     B3
		
	}
}
