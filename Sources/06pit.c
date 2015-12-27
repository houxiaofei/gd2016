/*
 * 06pit.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned int IOtest=0;

void initPIT(void) 
{                                   //100ms一个控制周期
                        	       // NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[1].LDVAL.R = 8000000;      // PIT1 timeout = 80000 sysclks x 1sec/80M sysclks = 1 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,2); 
}

void PitISR(void)//1ms一个控制周期
{
	IOtest=1;
	PIT.CH[1].TFLG.B.TIF = 1;	//write 1 to clear PIT1 清除标志位
}
