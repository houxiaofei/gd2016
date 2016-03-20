/*
 * analysis.c
 *
 *  Created on: Feb 25, 2016
 *      Author: lenovo
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int al_end=0,ar_end=0,bl_end=23,br_end=111;
int a_start=0,b_start=67;
int a_value=0,b_value=260;                          //判断跳变沿的差值标准
int a_T=0,b_T=600;                                  //黑白阈值
int al_count=0,ar_count=0,bl_count=0,br_count=0;  //白点计数
int a_PixelNumber=0,b_PixelNumber=44;
int a_allwhite=0,a_allblack=0,b_allwhite=42,b_allblack=2;                  //全白,全黑判断标准
int a_scan=5,a_halfscan=3,b_scan=5,b_halfscan=3;
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;黑，白，白-黑，黑-白，错误
int wrong_flag=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//跳变沿
int error=0;
int rem=6;                  //补线值
int i=0,j=0;

void DataSet(void)
{
	int k;
	int bs[10],bscnt=0;
	int bv[10],bvcnt=0;
	int bt[10],btcnt=0;
	for(k=0;k<10;k++)
	{
		mdelay(4);
		ImageCapture(PixelLeft,PixelRight);
		bv[k]=(PixelRight[70]+PixelRight[71]-PixelRight[20]-PixelRight[21])/5.2;
		bvcnt+=bv[k];
		bt[k]=(PixelRight[70]+PixelRight[71]+PixelRight[20]+PixelRight[21])/4.5;
		btcnt+=bt[k];
		for(i=70;i>0;i--)
		{
			if(PixelRight[i]-PixelRight[i-b_scan]>bv[k]&&PixelRight[i-1]-PixelRight[i-b_scan-1]>bv[k])
			{
				bl_edge=i-b_halfscan;
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
		bs[k]=(bl_edge+br_edge)/2;
		bscnt+=bs[k];
	}
	b_value=bvcnt/10;
	//b_value=bv[0];
	b_start=bscnt/10;
	bl_end=b_start-44;
	br_end=b_start+44;
	b_T=btcnt/10;
	//b_T=bt[0];
}

void PixelScan(void)
{
	al_count=0,ar_count=0,bl_count=0,br_count=0;
	al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;
	al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;
	error=0;
	wrong_flag=0;
	ImageCopy(A,PixelLeft);
	ImageCopy(B,PixelRight);
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

void ErrorCalculate(void)
{
	if(bl_flag==2&&br_flag==2)                              //22直道
	{
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


//ImageCopy(AA,PixelLeft);
//ImageCopy(BB,PixelRight);
