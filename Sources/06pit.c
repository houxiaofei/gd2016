/*
 * 06pit.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned int timecount=0;
unsigned int pitcount0=0,pitcount1=0,pitcount2=0,pitcount3=0,pitcount4=0,pitcount5=0;

void initPIT(void) 
{                                   //1ms一个控制周期// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[2].LDVAL.R = 1000000;      // PIT2 timeout = 800000 sysclks x 1sec/80M sysclks = 10 msec 
  PIT.CH[2].TCTRL.R = 0x000000003; // Enable PIT2 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR2,61,5); 
  udelay(1);
  PIT.CH[1].LDVAL.R = 800000;      // PIT1 timeout = 800000 sysclks x 1sec/80M sysclks = 10 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,4); 
}

void PitISR(void)//10ms一个控制周期
{
	pitcount1++;                                  
	//time1=TIME;
	ImageCapture(A,B);
	PixelScan();
	PixelScan_A();
	ErrorCalculate_A();
	ErrorCalculate();
	Steer_PDSet();
	SteerControl();
	//time2=TIME;
	//time3=TimeMesure();
	if(pitcount1>=100)                         //1s一次
	{
		pitcount1=0;
		OLED_Test();
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 清除标志位
}

void PitISR2(void)
{
	if(stop_flag==1)
	{
		targetspeed=0;
		SET_motor(0,0);
		timecount=0;
	}
	SpeedCount();
	SpeedControl(); 
	PIT.CH[2].TFLG.B.TIF = 1;//write 1 to clear PIT2 清除标志位
}
