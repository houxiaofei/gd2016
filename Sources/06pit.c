/*
 * 06pit.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned int IOtest=0;
unsigned int pitcount0=0,pitcount1=0,pitcount2=0,pitcount3=0,pitcount4=0,pitcount5=0;

void initPIT(void) 
{                                   //1ms一个控制周期// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[1].LDVAL.R = 80000;      // PIT1 timeout = 80000 sysclks x 1sec/80M sysclks = 1 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,2); 
}

void PitISR(void)//1ms一个控制周期
{
	pitcount0++;                                  //5ms一次清零
	if(pitcount0==2)
	{
		pitcount2++;
		if(pitcount2>=2)                          //10ms一次
		{
			pitcount2=0;
			ImageCapture(PixelLeft,PixelRight);
		}
	}
	if(pitcount0==3)
	{
		pitcount3++;
		if(pitcount3>=20)                          //100ms一次
		{
			pitcount3=0;
			//SpeedCount();
		}
	}
	if(pitcount0==5)
	{
		pitcount0=0;
		pitcount5++;
		if(pitcount5>=200)                         //1s一次
		{
			pitcount5=0;
			//  IOtest=1;
			//	Bee=1;
			//	udelay(100000);
			//	Bee=0;
			BlueTx();
		}
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 清除标志位
}
