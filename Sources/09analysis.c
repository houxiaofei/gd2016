/*
 * analysis.c
 *
 *  Created on: Feb 25, 2016
 *      Author: lenovo
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int al_end=43,ar_end=97,bl_end=23,br_end=111;
int a_start=70,b_start=67;
int a_value=120,b_value=100;                          //判断跳变沿的差值标准
int a_T=360,b_T=300;                                  //黑白阈值
int al_count=0,ar_count=0,bl_count=0,br_count=0;  //白点计数
int a_PixelNumber=30,b_PixelNumber=44;
int a_allwhite=20,a_allblack=8,b_allwhite=34,b_allblack=10;                  //全白,全黑判断标准
int a_scan=7,a_halfscan=0,b_scan=12,b_halfscan=0;
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;黑，白，白-黑，黑-白，错误
int a_flag=44,b_flag=44;
int aa_flag[4]={0,0,0,0};
int wrong_flag=0,a_wrong_flag=0;
int stop_flag=0,stop_cnt=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//跳变沿
int error=0,a_error=0,b_error=0;
int aa_error[4]={0,0,0,0};
int al_rem=0,ar_rem=0,b_rem=-13,ab_rem=16,ab_rem1=5;                  //补线值
int i=0,j=0;
int b_value2=15,b_scan2=10;//终点
int a_edg_err=0,a_bar_value=9,a_bar_cnt=0,a_bar_flag=0,a_bar_value2=30,al_bar_flag=0,ar_bar_flag=0;//障碍物
int b_bar_value=35,b_bar_cnt=0,b_bar_cnttop=1;//障碍物

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
		bv[k]=(PixelRight[70]+PixelRight[71]-PixelRight[20]-PixelRight[21])/5;
		av[k]=(PixelLeft[70]+PixelLeft[71]-PixelLeft[20]-PixelLeft[21])/5;
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
	for(i=b_start;i>(bl_end+b_scan);i--)
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
		if(bl_count>(b_allwhite-b_scan))
			bl_flag=1;
		else if(bl_count<b_allblack)
			bl_flag=0;
		else
			bl_flag=4;
	}
	for(i=b_start;i<(br_end-b_scan);i++)
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
		if(br_count>(b_allwhite-b_scan))
			br_flag=1;
		else if(br_count<b_allblack)
			br_flag=0;
		else
			br_flag=4;
	}
	b_flag=bl_flag*10+br_flag;
}

void PixelScan_A(void)
{
	al_count=0,ar_count=0;
	al_flag=4,ar_flag=4;
	al_edge=0,ar_edge=0;
	for(i=a_start;i>(al_end+a_scan);i--)
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
		if(al_count>(a_allwhite-a_scan))
			al_flag=1;
		else if(al_count<a_allblack)
			al_flag=0;
		else
			al_flag=4;
	}
	for(i=a_start;i<(ar_end-a_scan);i++)
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
		if(ar_count>(a_allwhite-a_scan))
			ar_flag=1;
		else if(ar_count<a_allblack)
			ar_flag=0;
		else
			ar_flag=4;
	}
	a_flag=al_flag*10+ar_flag;
	aa_flag[3]=a_flag;
}

void ErrorCalculate(void)
{
	if(bl_flag==0&&br_flag==0&al_flag==0&&ar_flag==0)
	{
		stop_cnt++;
		if(stop_cnt>3)
		{
			stop_cnt=0;
			stop_flag=1;
		}
	}
	else
	{
		stop_cnt=0;
	}
	if(a_bar_flag==1)                                        //障碍物
	{
		BarrierControl();
		return;
	}
	if(bl_flag==2&&br_flag==2)                              //22直道
	{
		EndJudge();
		if(a_flag==22)
		{
			BarrierJudge();
		}
		b_error=(bl_edge-b_start+br_edge-b_start);
		error=a_error*0.5+b_error;
		return;
	}
	if(bl_flag==1&&br_flag==1)                              //11十字
	{
		b_error=0;
		error=a_error*0.7+b_error;
		return;
	}
	if(bl_flag==1&&br_flag==2)                              //12左转小
	{
		b_error=br_edge-br_end-b_rem;
		if(a_flag==0||a_flag==3||a_flag==1)               //00,03,01
			error=b_error-12-ab_rem1;
		else
			error=a_error*0.3+b_error-ab_rem1;
		return;
	}
	if(bl_flag==1&&br_flag==0)                              //10左转中
	{
		b_error=b_start-br_end-b_rem;
		error=b_error-ab_rem;
		return;
	}
	if(bl_flag==3&&br_flag==0)                              //30左转大
	{
		b_error=bl_edge-br_end-b_rem;
		error=b_error-ab_rem;
		return;
	}
	if(bl_flag==2&&br_flag==1)                              //21右转小
	{
		b_error=bl_edge-bl_end+b_rem;
		if(a_flag==0||a_flag==30||a_flag==10)           //00,30,10
			error=b_error+12+ab_rem1;
		else
			error=a_error*0.3+b_error+ab_rem1;
		return;
	}
	if(bl_flag==0&&br_flag==1)                              //01右转中
	{
		b_error=b_start-bl_end+b_rem;
		error=b_error+ab_rem;
		return;
	}
	if(bl_flag==0&&br_flag==3)                              //03右转大
	{
		b_error=br_edge-bl_end+b_rem;
		error=b_error+ab_rem;
		return;
	}
	wrong_flag=1;
}

void ErrorCalculate_A(void)
{
	a_error=0;
	a_wrong_flag=1;
	if(a_flag==11)
	{
		a_error=0;
		a_wrong_flag=0;
	}
	if(al_flag==2&&ar_flag==2)//全直
	{
		a_error=(al_edge-a_start+ar_edge-a_start)*1.82;
		a_wrong_flag=0;
	}
	if(al_flag==2&&ar_flag==1)//即将进入右转
	{
		a_error=(al_edge-al_end+al_rem);
		a_wrong_flag=0;
	}
	if(al_flag==0&&ar_flag==1)
	{
		a_error=(a_start-al_end+al_rem);
		a_wrong_flag=0;
	}
	if(al_flag==0&&ar_flag==3)
	{
		a_error=(ar_edge-al_end+al_rem);
		a_wrong_flag=0;
	}
	if(al_flag==1&&ar_flag==2)//即将进入左转
	{
		a_error=(ar_edge-ar_end-ar_rem);
		a_wrong_flag=0;
	}
	if(al_flag==1&&ar_flag==0)
	{
		a_error=(a_start-ar_end-ar_rem);
		a_wrong_flag=0;
	}
	if(al_flag==3&&ar_flag==0)
	{
		a_error=(al_edge-ar_end-ar_rem);
		a_wrong_flag=0;
	}
	if(a_flag==33)
	{
		a_wrong_flag=0;
		if(aa_flag[2]==30)
		{
			a_error=(al_edge-ar_end-ar_rem);
			aa_flag[3]=30;
		}
		else if(aa_flag[2]==3)
		{
			a_error=(ar_edge-al_end+al_rem);
			aa_flag[3]=3;
		}
		else
			a_wrong_flag=1;
	}
	if(a_wrong_flag==1)
	{
		a_error=(aa_error[2]+aa_error[1])*0.5;
	}
	aa_error[3]=a_error;
	aa_error[0]=aa_error[1];aa_error[1]=aa_error[2];aa_error[2]=aa_error[3];
	aa_flag[0]=aa_flag[1];aa_flag[1]=aa_flag[2];aa_flag[2]=aa_flag[3];
}

void EndJudge(void)
{
	int k=0;
	int cnt=0;
	for(i=bl_edge-5;i<br_edge;i++)
	{
		switch(k){
		case 0:
			if((B[i+b_scan2]-B[i]<-b_value2)&&(B[i+1+b_scan2]-B[i+1]<-b_value2))
				k=1;
			break;
		case 1:
			if((B[i+b_scan2]-B[i]>b_value2)&&(B[i+1+b_scan2]-B[i+1]>b_value2))
				k=2;
			break;
		case 2:
			if((B[i+b_scan2]-B[i]<-b_value2)&&(B[i+1+b_scan2]-B[i+1]<-b_value2))
				k=3;
			break;
		case 3:
			if((B[i+b_scan2]-B[i]>b_value2)&&(B[i+1+b_scan2]-B[i+1]>b_value2))
				k=4;
			break;
		case 4:
			stop_flag=1;
			break;
		}	
	}
}

void BarrierJudge(void)
{
	if(a_bar_flag==1)
		return;
	a_edg_err=ar_edge-al_edge;
	if(a_edg_err<a_bar_value)
	{
		a_bar_cnt++;	
	}
	if(((ABS(al_edge-a_start)-ABS(ar_edge-a_start))<0)&&al_bar_flag==0&&a_bar_cnt>0)//障碍物在左边
	{
		i=0;j=0;
		for(i=al_edge;i>al_end;i--)
		{
			if(A[i]-A[i-4]>a_bar_value2)
			{
				for(j=al_end;j<i;j++)
				{
					if(A[j+4]-A[j]>a_bar_value2)
						al_bar_flag=1;
				}
			}
		}
	}
	if(((ABS(al_edge-a_start)-ABS(ar_edge-a_start))>0)&&ar_bar_flag==0&&a_bar_cnt>0)//障碍物在右边
	{
		i=0;j=0;
		for(i=ar_edge;i<ar_end;i++)
		{
			if(A[i+4]-A[i]>a_bar_value2)
			{
				for(j=ar_end;j>i;j--)
				{
					if(A[j-4]-A[j]>a_bar_value2)
						ar_bar_flag=1;
				}
			}
		}
	}
	if((a_bar_cnt>2)&&(al_bar_flag==1||ar_bar_flag==1))
	{
		a_bar_cnt=0;
		a_bar_flag=1;
	}
}

void BarrierControl(void)
{
	if(al_bar_flag==1)
	{
		if(br_flag==2)
		{
			error=br_edge-(b_start+3);
			if(error>=0)
				error=error*2.5;
			else
				error=error*4;
		}
		else if(br_flag==1)
		{
			error=(br_end-b_scan-(b_start+3))*3;
		}
		else
		{
			error=-25;
		}
		if(a_flag==22&&b_flag==22)
		{
			b_error=(bl_edge-b_start+br_edge-b_start);
			if((a_error+b_error)<-b_bar_value)
			{
				b_bar_cnt++;
				if(b_bar_cnt>b_bar_cnttop)
				{
					a_bar_flag=0;
					al_bar_flag=0;
					ar_bar_flag=0;
				}
			}
		}
	}
	else if(ar_bar_flag==1)
	{
		if(bl_flag==2)
		{
			error=bl_edge-(b_start-3);
			if(error<=0)
				error=error*2.5;
			else
				error=error*4;
		}
		else if(bl_flag==1)
		{
			error=(bl_end+b_scan-(b_start-3))*3;
		}
		else
		{
			error=25;
		}
		if(a_flag==22&&b_flag==22)
		{
			b_error=(bl_edge-b_start+br_edge-b_start);
			if((a_error+b_error)>b_bar_value)
			{
				b_bar_cnt++;
				if(b_bar_cnt>b_bar_cnttop)
				{
					a_bar_flag=0;
					al_bar_flag=0;
					ar_bar_flag=0;
				}
			}
		}
	}
}


//ImageCopy(AA,PixelLeft);
//ImageCopy(BB,PixelRight);
