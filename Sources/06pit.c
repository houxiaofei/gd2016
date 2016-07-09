/*
 * 06pit.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned int timecount=0;
unsigned int pitcount0=0,pitcount1=0,pitcount2=0,pitcount3=0,pitcount4=0,pitcount5=0;
unsigned int steer_flag=0,oled_flag=0;
unsigned int end_judge_flag=0;

void initPIT(void) 
{                                   //1ms一个控制周期// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
    PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
    PIT.CH[1].LDVAL.R = 1000000;      // PIT1 timeout = 800000 sysclks x 1sec/80M sysclks = 10 msec //电信50万 无偏正片60
    PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
    INTC_InstallINTCInterruptHandler(PitISR,60,4); 
}
void initPIT2(void)
{
	PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode
    PIT.CH[2].LDVAL.R = 1000000;      // PIT2 timeout = 800000 sysclks x 1sec/80M sysclks = 10 msec 
    PIT.CH[2].TCTRL.R = 0x000000003; // Enable PIT2 interrupt and make PIT active to count 
    INTC_InstallINTCInterruptHandler(PitISR2,61,5); 
    udelay(10);
}

void PitISR(void)//10ms一个控制周期
{
	pitcount1++;
	pitcount2++;
	steer_flag=1;
	if(pitcount2>=1600)
		end_judge_flag=1;
	//time1=TIME;
	//time2=TIME;
	//time3=TimeMesure();
	if(pitcount1>=100)                         //1s一次
	{
		pitcount1=0;
		oled_flag=1;
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 清除标志位
}

void PitISR2(void)
{
	
	SpeedCount();  //光编计数，采速度值
	Speed_Set();   //速度设置，变速
//	if(stop_flag==1)  //停车
//	{
//		targetspeed=0;
//		if(((csl+csr)/2)>15)
//		{
//		    SpeedControl(); 
////			Speed_PID2();
////		    DifferSpeed_PID2();
//		}
//		else
//		{
//		SET_motor(0,0);
//		}
//		timecount=0;
//	}
//	else
//	{
////		SpeedControl(); //位置式控制
//		Speed_PID2();   //外环位置式
//		DifferSpeed_PID2(); //内环位置式
//	}
	
	
//	pitcount2++;
//	pitcount4++;
//	if(pitcount2>=400)                         //4s一次
//	{
//		pitcount2=0;
//		pitcount3++;
//		if(pitcount3==1)
//		{
//		    targetspeed+=40;
//		}
//		else if(pitcount3==2)
//		{
//			pitcount3=0;
//		    targetspeed-=40;
//		}
//	}
//	SpeedControl(); 
//	Speed_PID2();
//	DifferSpeed_PID();
//	if(pitcount4>=3)
//	{
//		pitcount4=0;
//		csl=(csl_cnt[0]+csl_cnt[1]+csl_cnt[2])/3;
//		csr=(csr_cnt[0]+csr_cnt[1]+csr_cnt[2])/3;
//		Speed_PID();
//		SpeedControl(); 
//	} 
	PIT.CH[2].TFLG.B.TIF = 1;//write 1 to clear PIT2 清除标志位
}
