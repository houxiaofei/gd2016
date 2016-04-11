/*
 * analysis.c
 *
 *  Created on: Feb 25, 2016
 *      Author: lenovo
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int al_end=40,ar_end=100,bl_end=23,br_end=111;
int a_start=70,b_start=67;
int a_value=200,b_value=200;                          //判断跳变沿的差值标准
int a_T=400,b_T=400;                                  //黑白阈值
int al_count=0,ar_count=0,bl_count=0,br_count=0;  //白点计数
int a_PixelNumber=30,b_PixelNumber=44;
int a_allwhite=28,a_allblack=2,b_allwhite=42,b_allblack=2;                  //全白,全黑判断标准
int a_scan=4,a_halfscan=2,b_scan=8,b_halfscan=4;
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;黑，白，白-黑，黑-白，错误
int b_value2=4,b_scan2=3,b_cnt=4;
int wrong_flag=0;
int stop_flag=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//跳变沿
int error=0,a_error=0;
int rem=-5;                  //补线值
int i=0,j=0;

void DataSet(void)
{
	int k;
	int bs[10],bscnt=0;
	int bv[10],bvcnt=0;
	int bt[10],btcnt=0;
	int as[10],ascnt=0;
	int av[10],avcnt=0;
	int at[10],atcnt=0;
	stop_flag=0;
	ImageCapture(PixelLeft,PixelRight);
	mdelay(1);
	for(k=0;k<10;k++)
	{
		mdelay(3);
		ImageCapture(PixelLeft,PixelRight);
		bv[k]=(PixelRight[70]+PixelRight[71]-PixelRight[20]-PixelRight[21])/6;
		av[k]=(PixelLeft[70]+PixelLeft[71]-PixelLeft[20]-PixelLeft[21])/6;
		bvcnt+=bv[k];
		avcnt+=av[k];
		bt[k]=(PixelRight[70]+PixelRight[71]+PixelRight[20]+PixelRight[21])/4.5;
		at[k]=(PixelLeft[70]+PixelLeft[71]+PixelLeft[20]+PixelLeft[21])/4.5;
		btcnt+=bt[k];
		atcnt+=at[k];
		for(i=70;i>0;i--)
		{
			if(PixelRight[i]-PixelRight[i-b_scan]>bv[k]&&PixelRight[i-1]-PixelRight[i-b_scan-1]>bv[k])
			{
				bl_edge=i-b_halfscan;
				break;
			}
		}
		for(i=70;i>0;i--)
		{
			if(PixelLeft[i]-PixelLeft[i-a_scan]>av[k]&&PixelLeft[i-1]-PixelLeft[i-a_scan-1]>av[k])
			{
				al_edge=i-a_halfscan;
				break;
			}
		}
		for(i=70;i<127;i++)
		{
			if(PixelRight[i]-PixelRight[i+b_scan]>bv[k]&&PixelRight[i+1]-PixelRight[i+b_scan+1]>bv[k])
			{
				br_edge=i+b_halfscan;
				break;
			}
		}
		for(i=70;i<127;i++)
		{
			if(PixelLeft[i]-PixelLeft[i+a_scan]>av[k]&&PixelLeft[i+1]-PixelLeft[i+a_scan+1]>av[k])
			{
				ar_edge=i+a_halfscan;
				break;
			}
		}
		bs[k]=(bl_edge+br_edge)/2;
		bscnt+=bs[k];
		as[k]=(al_edge+ar_edge)/2;
		ascnt+=as[k];
	}
	b_value=bvcnt/10;
	b_start=bscnt/10;
	bl_end=b_start-b_PixelNumber;
	br_end=b_start+b_PixelNumber;
	b_T=btcnt/10;
	a_value=avcnt/10;
	a_start=ascnt/10;
	al_end=a_start-a_PixelNumber;
	ar_end=a_start+a_PixelNumber;
	a_T=atcnt/10;
}

void PixelScan(void)
{
	bl_count=0,br_count=0;
	bl_flag=4,br_flag=4;
	bl_edge=0,br_edge=0;
	error=0;
	wrong_flag=0;
	stop_flag=0;
	for(i=b_start;i>bl_end;i--)
	{
		if(B[i]>b_T)
			bl_count++;
		if(B[i]-B[i-b_scan]>b_value&&B[i-1]-B[i-b_scan-1]>b_value)
		{
			bl_flag=2;
			bl_edge=i-b_halfscan;
			break;
		}
		if(B[i-b_scan]-B[i]>b_value&&B[i-b_scan-1]-B[i-1]>b_value)
		{
			bl_flag=3;
			bl_edge=i-b_halfscan;
			break;
		}
		if(bl_count>b_allwhite)
			bl_flag=1;
		else if(bl_count<b_allblack)
			bl_flag=0;
		else
			bl_flag=4;
	}
	for(i=b_start;i<br_end;i++)
	{
		if(B[i]>b_T)
			br_count++;
		if(B[i]-B[i+b_scan]>b_value&&B[i+1]-B[i+b_scan+1]>b_value)
		{
			br_flag=2;
			br_edge=i+b_halfscan;
			break;
		}
		if(B[i+b_scan]-B[i]>b_value&&B[i+b_scan+1]-B[i+1]>b_value)
		{
			br_flag=3;
			br_edge=i+b_halfscan;
			break;
		}
		if(br_count>b_allwhite)
			br_flag=1;
		else if(br_count<b_allblack)
			br_flag=0;
		else
			br_flag=4;
	}
}

void PixelScan_A(void)
{
	al_count=0,ar_count=0;
	al_flag=4,ar_flag=4;
	al_edge=0,ar_edge=0;
	for(i=a_start;i>al_end;i--)
	{
		if(A[i]>a_T)
			al_count++;
		if(A[i]-A[i-a_scan]>a_value&&A[i-1]-A[i-a_scan-1]>a_value)
		{
			al_flag=2;
			al_edge=i-a_halfscan;
			break;
		}
		if(A[i-a_scan]-A[i]>a_value&&A[i-a_scan-1]-A[i-1]>a_value)
		{
			al_flag=3;
			al_edge=i-a_halfscan;
			break;
		}
		if(al_count>a_allwhite)
			al_flag=1;
		else if(al_count<a_allblack)
			al_flag=0;
		else
			al_flag=4;
	}
	for(i=a_start;i<ar_end;i++)
	{
		if(A[i]>a_T)
			ar_count++;
		if(A[i]-A[i+a_scan]>a_value&&A[i+1]-A[i+a_scan+1]>a_value)
		{
			ar_flag=2;
			ar_edge=i+a_halfscan;
			break;
		}
		if(A[i+a_scan]-A[i]>a_value&&A[i+a_scan+1]-A[i+1]>a_value)
		{
			ar_flag=3;
			ar_edge=i+a_halfscan;
			break;
		}
		if(ar_count>a_allwhite)
			ar_flag=1;
		else if(ar_count<a_allblack)
			ar_flag=0;
		else
			ar_flag=4;
	}
}

void ErrorCalculate(void)
{
	if(bl_flag==2&&br_flag==2)                              //22直道
	{
		EndJudge();
		error=(bl_edge-b_start+br_edge-b_start);
		return;
	}
	if(bl_flag==1&&br_flag==1)                              //11十字
	{
		error=0;
		return;
	}
	if(bl_flag==1&&br_flag==2)                              //12左转小
	{
		error=br_edge-br_end-rem;
		return;
	}
	if(bl_flag==1&&br_flag==0)                              //10左转中
	{
		error=b_start-br_end-rem;
		return;
	}
	if(bl_flag==3&&br_flag==0)                              //30左转大
	{
		error=bl_edge-br_end-rem;
		return;
	}
	if(bl_flag==2&&br_flag==1)                              //21右转小
	{
		error=bl_edge-bl_end+rem;
		return;
	}
	if(bl_flag==0&&br_flag==1)                              //01右转小
	{
		error=b_start-bl_end+rem;
		return;
	}
	if(bl_flag==0&&br_flag==3)                              //03右转小
	{
		error=br_edge-bl_end+rem;
		return;
	}
//	if(bl_flag==4||br_flag==4)
//	{
//		wrong_flag=1;
//	}
	wrong_flag=1;
}

void ErrorCalculate_A(void)
{
	if(al_flag==2&&ar_flag==2)
		a_error=(al_edge-a_start+ar_edge-a_start)*0.3;  //全直
	if(al_flag==2&&ar_flag==1)//即将进入右转
		a_error=(al_edge-al_end)*0.3;
	if(al_flag==0&&ar_flag==1)
		a_error=(a_start-al_end)*0.3;
	if(al_flag==0&&ar_flag==3)
		a_error=(ar_edge-al_end)*0.3;
	if(al_flag==1&&ar_flag==2)//即将进入左转
		a_error=(ar_edge-ar_end)*0.3;
	if(al_flag==1&&ar_flag==0)
		a_error=(a_start-ar_end)*0.3;
	if(al_flag==3&&ar_flag==0)
		a_error=(al_edge-ar_end)*0.3;
}

void EndJudge(void)
{
	int k=0;
	int cnt=0;
	for(i=bl_edge;i<br_edge;i++)
	{
		switch(k){
		case 0:
			if(B[i+b_scan2]-B[i]<-b_value2)
				cnt++;
			if(cnt>b_cnt)
			{
				cnt=0;
				k=1;
			}
			break;
		case 1:
			if(B[i+b_scan2]-B[i]>b_value2)
				cnt++;
			if(cnt>b_cnt)
			{
				cnt=0;
				k=2;
			}
			break;
		case 2:
			if(B[i+b_scan2]-B[i]<-b_value2)
				cnt++;
			if(cnt>b_cnt)
			{
				cnt=0;
				k=3;
			}
			break;
		case 3:
			if(B[i+b_scan2]-B[i]>b_value2)
				cnt++;
			if(cnt>b_cnt)
			{
				cnt=0;
				k=4;
			}
			break;
		case 4:
			stop_flag=1;
			break;
		}	
	}
}


//ImageCopy(AA,PixelLeft);
//ImageCopy(BB,PixelRight);
