/*
 * analysis_new.c
 *
 *  Created on: Jul 9, 2016
 *      Author: lenovo
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int C[128]={0};
int al_end=36,ar_end=96,bl_end=22,br_end=110;
int al_start=70,ar_start=62,a_start=66,a_offset=2,bl_start=72,br_start=60,b_start=66,b_offset=4;
int a_T=320,b_T=324;                                  //�ڰ���ֵ
int c_T=262;
int a_PixelNumber=30,b_PixelNumber=44;
int a_allwhite=20,a_allblack=8,b_allwhite=34,b_allblack=10;                  //ȫ��,ȫ���жϱ�׼
int a_scan=6,a_scan1=6,a_scan2=10,a_scan3=10,a_expand=5,a_expand1=0,a_expand2=5;
int b_scan=8,b_scan1=8,b_scan2=12,b_scan3=12,b_expand=5,b_expand1=0,b_expand2=5;
int al_scan_i=56,ar_scan_i=76,bl_scan_i=48,br_scan_i=84;
int a_value2=110,a_value3=110,a_value_T1=120,a_value_T2=110,a_value_T3=80;   //�ж������صĲ�ֵ��׼
int b_value2=150,b_value3=150,b_value_T1=160,b_value_T2=140,b_value_T3=120;      //�ж������صĲ�ֵ��׼

int al_count=0,ar_count=0,bl_count=0,br_count=0;  //�׵����
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
int a_flag=44,b_flag=44;
int aa_flag[4]={0,0,0,0},all_flag[10]={0,0,0,0,0,0,0,0,0,0};
int wrong_flag=0,a_wrong_flag=0;
int stop_flag=0,stop_cnt=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//������
int al_edge_left=0,al_edge_right=0,ar_edge_left=0,ar_edge_right=0;
int bl_edge_left=0,bl_edge_right=0,br_edge_left=0,br_edge_right=0;
int error=0,a_error=0,b_error=0,pre_error=0;
int aa_error[4]={0,0,0,0},bb_error[4]={0,0,0,0};

int error_his[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int his_num=8,trend=0,trend_value=3,trend_value2=3,b_error_value=5,b_error_value2=0;
int enter_flag=0;//����flag
int a_rem=16,ab_rem=24,b_rem=9;                  //����ֵ
int i=0,j=0;

int b_value_end=30,b_scan_end=10;//�յ�30,10
int a_edg_err=0,a_bar_value=22,a_bar_cnt=0,a_bar_flag=0,a_bar_value2=100,al_bar_flag=0,ar_bar_flag=0;//�ϰ���
int b_bar_value=28,b_bar_cnt=0,b_bar_cnttop=1,ab_difference=0,ab_difference_value=4;//�ϰ���


int AverageCalculate(int a, int b, int c[])     //������ƽ��ֵ����
{
	int max=0,min=0;
	int i=0;
	max=c[a];
	min=c[a];
	for(i=a;i<=b;i++)
	{
		if(c[i]>max)
			max=c[i];
		if(c[i]<min)
			min=c[i];
	}
	return (max+min)/2;
}

int EdgeCalculate(int a, int b, int c[],int avg)   //�������������
{
	int err_min=0;
	int i=0,j=0;
	err_min=ABS(c[a]-avg);
	for(i=a;i<=b;i++)
	{
		if(ABS(c[i]-avg)<err_min)
		{
			err_min=ABS(c[i]-avg);
			j=i;
		}
	}
	return j;	
}


void PixelScan_A(void)
{
	al_count=0,ar_count=0;
	al_flag=4,ar_flag=4;
	//al_edge=0,ar_edge=0;
	for(i=al_start;i>al_end;i--)
	{
		if(i>al_scan_i)
		{
			a_scan=a_scan1;
			a_value2=a_value_T1;
			a_value3=a_value_T2;
		}
		else
		{
			a_scan=a_scan2;
			a_value2=a_value_T2;
			a_value3=a_value_T3;
		}
		if(i<=a_start&&A[i]>a_T)
		{
			al_count++;
		}
		if(i>(al_end+a_scan))
		{
			if(A[i]-A[i-a_scan]>a_value2&&A[i-1]-A[i-a_scan-1]>a_value2)
			{
				al_edge_left=i-a_scan-a_expand2;
				al_edge_right=i+a_expand1;
				a_T=AverageCalculate((i-a_scan-a_expand2),(i+a_expand1),A);
				al_edge=EdgeCalculate((i-a_scan-a_expand2),(i+a_expand1),A,a_T);
				if(al_edge<a_start+a_offset)
				{
					al_flag=2;
					break;
				}
			}
			if(A[i-a_scan]-A[i]>a_value3&&A[i-a_scan-1]-A[i-1]>a_value3)
			{
				al_edge_left=i-a_scan-a_expand2;
				al_edge_right=i+a_expand1;
				a_T=AverageCalculate((i-a_scan-a_expand2),(i+a_expand1),A);
				al_edge=EdgeCalculate((i-a_scan-a_expand2),(i+a_expand1),A,a_T);
				if(al_edge<a_start+a_offset)
				{
					al_flag=3;
					break;
				}
			}
		}
		if(al_count>a_allwhite)
			al_flag=1;
		else if(al_count<a_allblack)
			al_flag=0;
		else
			al_flag=4;
	}
	for(i=ar_start;i<ar_end;i++)
	{
		if(i<ar_scan_i)
		{
			a_scan=a_scan1;
			a_value2=a_value_T1;
			a_value3=a_value_T2;
		}
		else
		{
			a_scan=a_scan2;
			a_value2=a_value_T2;
			a_value3=a_value_T3;
		}
		if(i>=a_start&&A[i]>a_T)
			ar_count++;
		if(i<(ar_end-a_scan))
		{
			if(A[i]-A[i+a_scan]>a_value2&&A[i+1]-A[i+a_scan+1]>a_value2)
			{
				ar_edge_left=i-a_expand1;
				ar_edge_right=i+a_scan+a_expand2;
				a_T=AverageCalculate((i-a_expand1),(i+a_scan+a_expand2),A);
				ar_edge=EdgeCalculate((i-a_expand1),(i+a_scan+a_expand2),A,a_T);
				if(ar_edge>a_start-a_offset)
				{
					ar_flag=2;
					break;
				}
			}
			if(A[i+a_scan]-A[i]>a_value3&&A[i+a_scan+1]-A[i+1]>a_value3)
			{
				ar_edge_left=i-a_expand1;
				ar_edge_right=i+a_scan+a_expand2;
				a_T=AverageCalculate((i-a_expand1),(i+a_scan+a_expand2),A);
				ar_edge=EdgeCalculate((i-a_expand1),(i+a_scan+a_expand2),A,a_T);
				if(ar_edge>a_start-a_offset)
				{
					ar_flag=3;
					break;
				}
			}
		}
		if(ar_count>a_allwhite)
			ar_flag=1;
		else if(ar_count<a_allblack)
			ar_flag=0;
		else
			ar_flag=4;
	}
	a_flag=al_flag*10+ar_flag;
}

void PixelScan_B(void)
{
	bl_count=0,br_count=0;
	bl_flag=4,br_flag=4;	//���ұ�־����ʼֵΪ�����־
	bl_edge=0,br_edge=0;	//����������
	wrong_flag=0;           //0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
	for(i=bl_start;i>bl_end;i--)//������b_start����࣬ɨ��bl_end+b_scan
	{
		if(i>bl_scan_i)
		{
			b_scan=b_scan1;
			b_value2=b_value_T1;
			b_value3=b_value_T2;
		}
		else
		{
			b_scan=b_scan2;
			b_value2=b_value_T2;
			b_value3=b_value_T3;
		}
		if(i<=b_start&&B[i]>b_T)
			bl_count++;
		if(i>(bl_end+b_scan))
		{
			if(B[i]-B[i-b_scan]>b_value2&&B[i-1]-B[i-b_scan-1]>b_value2)	//��-��
			{
				bl_edge_left=i-b_scan-b_expand2;
				bl_edge_right=i+b_expand1;
				b_T=AverageCalculate((i-b_scan-b_expand2),(i+b_expand1),B);
				bl_edge=EdgeCalculate((i-b_scan-b_expand2),(i+b_expand1),B,b_T);				//��ȡ����������
				if(bl_edge<b_start+b_offset)
				{
					bl_flag=2;
					break;
				}
			}
			if(B[i-b_scan]-B[i]>b_value3&&B[i-b_scan-1]-B[i-1]>b_value3)	//��-��
			{
				bl_edge_left=i-b_scan-b_expand2;
				bl_edge_right=i+b_expand1;
				b_T=AverageCalculate((i-b_scan-b_expand2),(i+b_expand1),B);
				bl_edge=EdgeCalculate((i-b_scan-b_expand2),(i+b_expand1),B,b_T);			//��ȡ����������
				if(bl_edge<b_start+b_offset)
				{
					bl_flag=3;
					break;
				}
			}
		}
		if(bl_count>b_allwhite)
			bl_flag=1;
		else if(bl_count<b_allblack)	//Ҫ�׵���С��10������ж�Ϊȫ��
			bl_flag=0;
		else
			bl_flag=4;
	}
	for(i=br_start;i<br_end;i++)	//������b_start���Ҳ࣬ɨ��br_end-b_scan
	{
		if(i<br_scan_i)
		{
			b_scan=b_scan1;
			b_value2=b_value_T1;
			b_value3=b_value_T2;
		}
		else
		{
			b_scan=b_scan2;
			b_value2=b_value_T2;
			b_value3=b_value_T3;
		}
		if(i>=b_start&&B[i]>b_T)
			br_count++;
		if(i<(br_end-b_scan))
		{
			if(B[i]-B[i+b_scan]>b_value2&&B[i+1]-B[i+b_scan+1]>b_value2)	//��-��
			{
				br_edge_left=i-b_expand1;
				br_edge_right=i+b_scan+b_expand2;
				b_T=AverageCalculate((i-b_expand1),(i+b_scan+b_expand2),B);
				br_edge=EdgeCalculate((i-b_expand1),(i+b_scan+b_expand2),B,b_T);
				if(br_edge>b_start-b_offset)
				{
					br_flag=2;
					break;
				}
			}
			if(B[i+b_scan]-B[i]>b_value3&&B[i+b_scan+1]-B[i+1]>b_value3)	//��-��
			{
				br_edge_left=i-b_expand1;
				br_edge_right=i+b_scan+b_expand2;
				b_T=AverageCalculate((i-b_expand1),(i+b_scan+b_expand2),B);
				br_edge=EdgeCalculate((i-b_expand1),(i+b_scan+b_expand2),B,b_T);
				if(br_edge>b_start-b_offset)
				{
					br_flag=3;
					break;
				}
			}
		}
		if(br_count>b_allwhite)
			br_flag=1;
		else if(br_count<b_allblack)
			br_flag=0;
		else
			br_flag=4;
	}
	b_flag=bl_flag*10+br_flag;
	//�����ν�ǰ��9�ε��ж������ǰһ������Ԫ�ش棬��all_flag[n]->all_flag[n-1]
	all_flag[0]=all_flag[1];all_flag[1]=all_flag[2];all_flag[2]=all_flag[3];all_flag[3]=all_flag[4];all_flag[4]=all_flag[5];
	all_flag[5]=all_flag[6];all_flag[6]=all_flag[7];all_flag[7]=all_flag[8];all_flag[8]=all_flag[9];
	all_flag[9]=a_flag*100+b_flag;//ÿһ��ɨ�轫���µ��ж��������all_flag[9]
}

void ErrorCalculate_A(void)
{
	a_wrong_flag=0;
	switch(a_flag){
	case 22: a_error=(al_edge-al_end+ar_edge-ar_end); break;//ֱ��
	case 11: a_error=0; break;                              //ʮ��·��
	case 21: a_error=(al_edge-al_end); break;        //��ת
	case 23: a_error=(al_edge-al_end); break;        //��ת
	case 03: a_error=(ar_edge-al_end); break;        //��ת��
	case 12: a_error=(ar_edge-ar_end); break;        //��ת
	case 32: a_error=(ar_edge-ar_end); break;        //��ת
	case 30: a_error=(al_edge-ar_end); break;        //��ת��
	case 33:
		if(aa_flag[3]==30)
		{
			a_error=(al_edge-ar_end);
			a_flag=30;
		}
		else if(aa_flag[3]==3)
		{
			a_error=(ar_edge-al_end);
			a_flag=3;
		}
		else
			a_wrong_flag=1;
	default: a_wrong_flag=1; break;
	}
	if(a_wrong_flag==1)
		a_error=(aa_error[3]+aa_error[2])*0.5;
	aa_error[0]=aa_error[1];aa_error[1]=aa_error[2];aa_error[2]=aa_error[3];aa_error[3]=a_error;
	aa_flag[0]=aa_flag[1];aa_flag[1]=aa_flag[2];aa_flag[2]=aa_flag[3];aa_flag[3]=a_flag;
}

void ErrorCalculate(void)
{
	if(b_flag==0&&a_flag==0)	//ȫ�����
	{
		stop_cnt++;
		if(stop_cnt>10)		//�жϵ�3��ȫ�������ͣ��
		{
			stop_cnt=0;
			stop_flag=1;
		}
	}
	else
		stop_cnt=0;
	if(a_bar_flag==1)                                        //�ϰ���
	{
		BarrierControl();
		return;
	}
	switch(b_flag){
	case 22:
		if(end_judge_flag)
			EndJudge();
		if(a_flag==22)
			BarrierJudge();
		b_error=(bl_edge-bl_end+br_edge-br_end);
		error=a_error*0.5+b_error;
		break;
	case 11: b_error=0; error=a_error*0.7+b_error; break;//11ʮ�֣�����û�������أ�ֱ��
	case 12://12��ת
		b_error=br_edge-br_end;
		if(a_flag==0||a_flag==3||a_flag==23||a_flag==21)               //00,03,23,21
			error=b_error-a_rem-b_rem;
		else
			error=a_error*0.3+b_error-b_rem;
		break;
	case 32: b_error=br_edge-br_end; error=b_error-ab_rem; break;
	case 30: b_error=bl_edge-br_end; error=b_error-ab_rem; break;
	case 21://21��ת
		b_error=bl_edge-bl_end;
		if(a_flag==0||a_flag==30||a_flag==32||a_flag==12)           //00,30,32,12
			error=b_error+a_rem+b_rem;
		else
			error=a_error*0.3+b_error+b_rem;
		break;
	case 23: b_error=bl_edge-bl_end; error=b_error+ab_rem; break;
	case 03: b_error=br_edge-bl_end; error=b_error+ab_rem; break;
	default: wrong_flag=1; break;
	}
}

void EndJudge(void)
{
	int k=0;
	int cnt=0;
	for(i=bl_edge;i<br_edge;i++)
	{
		switch(k){
		case 0:
			if((B[i+b_scan_end]-B[i]<-b_value_end)&&(B[i+1+b_scan_end]-B[i+1]<-b_value_end))//�½���
				k=1;
			break;
		case 1:
			if((B[i+b_scan_end]-B[i]>b_value_end)&&(B[i+1+b_scan_end]-B[i+1]>b_value_end))//������
				k=2;
			break;
		case 2:
			if((B[i+b_scan_end]-B[i]<-b_value_end)&&(B[i+1+b_scan_end]-B[i+1]<-b_value_end))//�½���
				k=3;
			break;
		case 3:
			if((B[i+b_scan_end]-B[i]>b_value_end)&&(B[i+1+b_scan_end]-B[i+1]>b_value_end))//������
				k=4;
			break;
		case 4:
			stop_flag=1;
			break;
		}	
	}
}

void BarrierJudge(void)		//�ϰ����ж�
{
	a_edg_err=ar_edge-al_edge;
//	ab_difference=ABS(ar_edge-a_start-(a_start-al_edge));
	if(a_bar_flag==1)
		return;
	if(a_edg_err<a_bar_value)
	{
		a_bar_cnt++;	
	}
//	if(a_edg_err<a_bar_value&&ab_difference>ab_difference_value)
//	{
//		a_bar_cnt++;	
//	}
	if(((ABS(al_edge-a_start)-ABS(ar_edge-a_start))<0)&&al_bar_flag==0&&a_bar_cnt>0)//�ϰ��������
	{
		i=0;j=0;
		for(i=al_edge;i>al_end;i--)
		{
			if(A[i-4]-A[i]>a_bar_value2)
			{
				for(j=al_end;j<i;j++)
				{
					if(A[j+4]-A[j]>a_bar_value2)
						al_bar_flag=1;
				}
			}
		}
	}
	if(((ABS(al_edge-a_start)-ABS(ar_edge-a_start))>0)&&ar_bar_flag==0&&a_bar_cnt>0)//�ϰ������ұ�
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
	if((a_bar_cnt>1)&&(al_bar_flag==1||ar_bar_flag==1))
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
			a_error=al_edge+ar_edge-2*a_start;
			b_error=bl_edge+br_edge-2*b_start;
			if(a_error>b_error)
				error=a_error;
			else
				error=b_error;
			if(error>0)
				error=error*4;
			else		//ʵ�ʲ������
				error=error*3/2;
		}
		else if(br_flag==1)
		{
			error=(br_end-b_scan-(b_start-8))*2;
		}
		else		//�Ҳ�ͼ��ȫ�ڣ������
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
			a_error=al_edge+ar_edge-2*a_start;
			b_error=bl_edge+br_edge-2*b_start;
			if(a_error<b_error)
				error=a_error;
			else
				error=b_error;
			if(error<0)
				error=error*4;
			else				//ʵ�ʲ�Ӧ����
				error=error*3/2;
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

void TrendCalculate(void)
{
	int i,a,b;
	int his1=0,his2=0;
	a=his_num;
	b=his_num/2;
	for(i=0;i<a-1;i++)
	{
		error_his[i]=error_his[i+1];
	}
	error_his[a-1]=error;
	for(i=0;i<b;i++)
	{
		his1+=error_his[i];
		his2+=error_his[i+b];
	}
	trend=(his2-his1)/b;
}
void EnterJudge(void)
{
	if(ABS(trend)>=trend_value&&ABS(b_error)<=b_error_value)
	{
		if(b_error>0&&trend>0)
			enter_flag=1;
		else if(b_error<0&&trend<0)
			enter_flag=1;
	}
	else if(ABS(trend)<=trend_value2&&ABS(b_error)>=b_error_value2)
	{
		enter_flag=0;
	}
}

void DataSet(void)
{
	
}

