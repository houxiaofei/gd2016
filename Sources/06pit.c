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
{                                   //1msһ����������// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[1].LDVAL.R = 80000;      // PIT1 timeout = 80000 sysclks x 1sec/80M sysclks = 1 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,4); 
}

void PitISR(void)//1msһ����������
{
	timecount++;
	pitcount0++;                                  //5+2+0.035msһ������
	if(pitcount0==2)
	{
		pitcount2++;
		if(pitcount2>=1)                          //10msһ��,��2ms,��12msһ��
		{
			//time1=TIME;
			pitcount2=0;
			ImageCapture(PixelLeft,PixelRight);
			PixelScan();
			ErrorCalculate();
			Steer_PDSet();
			SteerControl();
			//time2=TIME;
			//time3=TimeMesure();
		}
	}
	if(pitcount0==3)
	{
		pitcount3++;
		if(pitcount3>=3)                          //15msһ��,35us
		{
			//time1=TIME;
			pitcount3=0;
			if(timecount>30000)
			{
				SET_motor(0,0);
				timecount=0;
			}
			//SpeedCount();
			//SpeedControl(); 
			//time2=TIME;
			//time3=TimeMesure();
		}
	}
	if(pitcount0==5)
	{
		pitcount0=0;
		pitcount5++;
		if(pitcount5>=100)                         //1sһ��
		{
			pitcount5=0;
			OLED_Test();
			//  IOtest=1;
			//	Bee=1;
			//	udelay(100000);
			//	Bee=0;
			//BlueTx();
		}
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 �����־λ
}
