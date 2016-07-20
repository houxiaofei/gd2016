/*
 * 05debug.c
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#include "includes.h"

unsigned char *send;
unsigned char putstring[]="Image";
int Left[128];
int Middle[128];
int Right[128];
unsigned int Ts=0;
unsigned int Tc=0;
unsigned char Image_or_not=1,Speed_or_not=35;
int CurrentSteer=0;
unsigned long time1=0;
unsigned long time2=0;
unsigned long time3=0;
unsigned char mode=0,basic_mode=0;

unsigned char RX_data;
unsigned char RX_flag=0;
unsigned char RX_i=0,RX_j=0;
unsigned char Y[25]={0},Z[11]={0};
int X[16]={0};

unsigned char S3_last=1;
unsigned char S4_last=1;
unsigned char S5_last=1;
unsigned char S6_last=1;
unsigned char keymode=0;


//********************************************************************************************************
//****************************************拨码开关函数*****************************************************
//********************************************************************************************************
//void ChooseMode(void)
//{
//	mode=15-(switch1*8+switch2*4+switch3*2+switch4);
//}
//********************************************************************************************************
//****************************************蓝牙发送函数*****************************************************
//********************************************************************************************************

/*************************蓝牙发送函数*********************/
void BlueTx(void)                             //蓝牙发数据
{
//	LINFLEX_0.LINCR1.B.INIT=1;
//	LINFLEX_0.LINIER.B.DRIE=1;
//	LINFLEX_0.LINCR1.B.INIT=0; //退出初始化模式
	send = putstring;
	LINFlex_TX(*send++);
}
/*************************蓝牙接口函数*********************/
void LINFlex_TX(unsigned char data)
{
	LINFLEX_0.UARTSR.B.DTF=1;
	LINFLEX_0.BDRL.B.DATA0=data;       //发送语句
}
/*************************蓝牙中断函数*********************/
void LINFlex_TX_Interrupt(void)
{
	unsigned char aa='L';
	unsigned char bb='M';
	unsigned char cc='R';
	unsigned char steer='X';
	unsigned char speed='Y';
	unsigned char data='Z';
	unsigned char flag='W';
//		if(*send!=0x00&&Ts==0)
//			LINFlex_TX(*send++);
//		else
//		{
//			send = putstring;
//		}
	switch(Ts){
	case 0: if(*send!=0x00){
				LINFlex_TX(*send++);
				break;}
			else{
				Ts=Image_or_not;
				break;}
	case 1:
		LINFlex_TX(aa);
		ImageCopy(Left,A);
		Ts=2;
		break;
	case 2: 
		LINFlex_TX(SendInt2(Left[Tc]));        //发送A-CCD图像
		Ts=3;
		break;
	case 3: 
		LINFlex_TX(SendInt3(Left[Tc]));
		Ts=4;
		break;
	case 4: 
		LINFlex_TX(SendInt4(Left[Tc]));
		if(Tc<127){
			Tc++;
			Ts=2;}
		else{
			Tc=0;
			Ts=5;}
		break;
	case 5: 
		LINFlex_TX(bb);
		ImageCopy(Middle,B);
		Ts=6;
		break;
	case 6: 
		LINFlex_TX(SendInt2(Middle[Tc]));        //发送B-CCD图像
		Ts=7;
		break;
	case 7: 
		LINFlex_TX(SendInt3(Middle[Tc]));
		Ts=8;
		break;
	case 8:
		LINFlex_TX(SendInt4(Middle[Tc]));
		if(Tc<127){
			Tc++;
			Ts=6;}
		else{
			Tc=0;
			Ts=9;
		}
		break;
	case 9: 
		LINFlex_TX(cc);
		ImageCopy(Right,C);
		Ts=10;
		break;
	case 10: 
		LINFlex_TX(SendInt2(Right[Tc]));        //发送C-CCD图像
		Ts=11;
		break;
	case 11: 
		LINFlex_TX(SendInt3(Right[Tc]));
		Ts=12;
		break;
	case 12:
		LINFlex_TX(SendInt4(Right[Tc]));
		if(Tc<127){
			Tc++;
			Ts=10;}
		else{
			Tc=0;
			Ts=13;
		}
		break;
	case 13: 
		LINFlex_TX(data);
		if(basic_mode==8)
			Ts=18;
		else
			Ts=14;
		break;
	case 14: 
		LINFlex_TX(SendInt4(al_flag));        //发送A_flag
		Ts=15;
		break;
	case 15: 
		LINFlex_TX(SendInt4(ar_flag));
		Ts=16;
		break;
	case 16: 
		LINFlex_TX(SendInt4(bl_flag));        //发送B_flag
		Ts=17;
		break;
	case 17: 
		LINFlex_TX(SendInt4(br_flag));
		Ts=18;
		break;
	case 18: 
		LINFlex_TX(SendInt1(error));        //发送error
		Ts=19;
		break;
	case 19:
		LINFlex_TX(SendInt2(error));
		Ts=20;
		break;
	case 20: 
		LINFlex_TX(SendInt3(error));        
		Ts=21;
		break;
	case 21:
		LINFlex_TX(SendInt4(error));
		if(basic_mode==8)
			Ts=30;
		else
			Ts=22;
		break;
	case 22: 
		LINFlex_TX(SendInt1(a_error));        //发送a_error
		Ts=23;
		break;
	case 23:
		LINFlex_TX(SendInt2(a_error));
		Ts=24;
		break;
	case 24: 
		LINFlex_TX(SendInt3(a_error));      
		Ts=25;
		break;
	case 25:
		LINFlex_TX(SendInt4(a_error));
		Ts=26;
		break;
	case 26:                          //发送b_error
		LINFlex_TX(SendInt1(b_error));
		Ts=27;
		break;
	case 27:
		LINFlex_TX(SendInt2(b_error));
		Ts=28;
		break;
	case 28: 
		LINFlex_TX(SendInt3(b_error));      
		Ts=29;
		break;
	case 29:
		LINFlex_TX(SendInt4(b_error));
		Ts=30;
		break;
	case 30: 
		CurrentSteer=TargetSteer;
		LINFlex_TX(steer);
		Ts=31;
		break;
	case 31: 
		LINFlex_TX(SendInt1(CurrentSteer));        //发送舵机值
		Ts=32;
		break;
	case 32: 
		LINFlex_TX(SendInt2(CurrentSteer));
		Ts=33;
		break;
	case 33: 
		LINFlex_TX(SendInt3(CurrentSteer));
		Ts=34;
		break;
	case 34:
		LINFlex_TX(SendInt4(CurrentSteer));
		Ts=Speed_or_not;
		break;
	case 35: 
		LINFlex_TX(speed);
		Ts=36;
		break;
	case 36: 
		LINFlex_TX(SendInt1(csl));   //发送左轮当前速度
		Ts=37;
		break;
	case 37: 
		LINFlex_TX(SendInt2(csl));
		Ts=38;
		break;
	case 38: 
		LINFlex_TX(SendInt3(csl));
		Ts=39;
		break;
	case 39:
		LINFlex_TX(SendInt4(csl));
		Ts=40;
		break;
	case 40: 
		LINFlex_TX(SendInt1(csr));   //发送右轮当前速度
		Ts=41;
		break;
	case 41: 
		LINFlex_TX(SendInt2(csr));
		Ts=42;
		break;
	case 42: 
		LINFlex_TX(SendInt3(csr));
		Ts=43;
		break;
	case 43:
		LINFlex_TX(SendInt4(csr));
		Ts=44;
		break;
	case 44: 
		LINFlex_TX(SendInt1(tsl));        //发送a_error
		Ts=45;
		break;
	case 45:
		LINFlex_TX(SendInt2(tsl));
		Ts=46;
		break;
	case 46: 
		LINFlex_TX(SendInt3(tsl));      
		Ts=47;
		break;
	case 47:
		LINFlex_TX(SendInt4(tsl));
		Ts=48;
		break;
	case 48: 
		LINFlex_TX(SendInt1(tsr));        //发送b_error
		Ts=49;
		break;
	case 49:
		LINFlex_TX(SendInt2(tsr));
		Ts=50;
		break;
	case 50: 
		LINFlex_TX(SendInt3(tsr));      
		Ts=51;
		break;
	case 51:
		LINFlex_TX(SendInt4(tsr));
		Ts=52;
		break;
	case 52: 
		LINFlex_TX(flag);        //发送speedflag
		Ts=53;
		break;
	case 53:
		LINFlex_TX(SendInt4(a_bar_flag2));
		Ts=54;
		break;
	case 54: 
		LINFlex_TX(SendInt4(al_bar_flag2+1));      
		Ts=55;
		break;
	case 55:
		LINFlex_TX(SendInt4(ar_bar_flag2+1));
		Ts=56;
		break;
	case 56:
		LINFlex_TX(SendInt4(turn_flag));
		Ts=57;
		break;
	case 57:
		send = putstring;
		Ts=0;
//		LINFLEX_0.LINCR1.B.INIT=1;
//		LINFLEX_0.LINIER.B.DRIE=0;
//		LINFLEX_0.LINCR1.B.INIT=0; //退出初始化模式
		break;	
	}
}

void LINFlex_RX_Interrupt(void)
{
	RX_data=LINFLEX_0.BDRM.B.DATA4;
	if(RX_data==69)
	{
		switch(RX_flag){
		case 1:
			straightspeed=X[1]*100+X[2]*10+X[3];
			//transspeed=X[4]*100+X[5]*10+X[6];
			Speed_kc1=X[4]*10000+X[5]*1000+X[6]*100;
			//sp_x1=((double)(X[4]*100+X[5]*10+X[6]))/10000;
			turnspeed=X[7]*100+X[8]*10+X[9];transspeed=X[7]*100+X[8]*10+X[9];
			deadspeed=X[10]*100+X[11]*10+X[12];
			barspeed=X[13]*100+X[14]*10+X[15];
			RX_flag=0;
			break;
		case 2:
			if(mode==8||basic_mode==4)//单环位置式
			{
				Speed_kp_Left=(float)(Y[1]*10+Y[2])+((float)(Y[3]*10+Y[4]))/100;
				Speed_ki_Left=(float)(Y[5]*10+Y[6])+((float)(Y[7]*10+Y[8]))/100;
				Speed_kd_Left=(float)(Y[9]*10+Y[10])+((float)(Y[11]*10+Y[12]))/100;
				Speed_kp_Right=(float)(Y[13]*10+Y[14])+((float)(Y[15]*10+Y[16]))/100;
				Speed_ki_Right=(float)(Y[17]*10+Y[18])+((float)(Y[19]*10+Y[20]))/100;
				Speed_kd_Right=(float)(Y[21]*10+Y[22])+((float)(Y[23]*10+Y[24]))/100;
			}
			else if(mode==9)//双环位置式
			{
				KP_speed=(float)(Y[1]*10+Y[2])+((float)(Y[3]*10+Y[4]))/100;
				KI_speed=(float)(Y[5]*10+Y[6])+((float)(Y[7]*10+Y[8]))/100;
				KD_speed=(float)(Y[9]*10+Y[10])+((float)(Y[11]*10+Y[12]))/100;
				KP_DifSpd=(float)(Y[13]*10+Y[14])+((float)(Y[15]*10+Y[16]))/100;
				KI_DifSpd=(float)(Y[17]*10+Y[18])+((float)(Y[19]*10+Y[20]))/100;
				KD_DifSpd=(float)(Y[21]*10+Y[22])+((float)(Y[23]*10+Y[24]))/100;
			}
			RX_flag=0;
			break;
		case 3:
			sp_x2=Z[1]*10+Z[2];
			sp_x3=Z[3]*10+Z[4];
			ab_difference_value=Z[5]*10+Z[6];
			a_bar_value=Z[7]*10+Z[8];
			b_value_end=Z[9]*10+Z[10];
			RX_flag=0;
			break;
		}
	}
	else if(RX_data==88)
	{
		RX_flag=1;
		RX_i=0;
	}
	else if(RX_data==89)
	{
		RX_flag=2;
		RX_i=0;
	}
	else if(RX_data==90)
	{
		RX_flag=3;
		RX_i=0;
	}
	switch(RX_flag){
	case 0:
		break;
	case 1:
		X[RX_i]=RX_data-48;
		RX_i++;
		break;
	case 2:
		Y[RX_i]=RX_data-48;
		RX_i++;
		break;
	case 3:
		Z[RX_i]=RX_data-48;
		RX_i++;
		break;
	}
	LINFLEX_0.UARTSR.B.DRF=1;
}

//********************************************************************************************************
//****************************************按键调试函数*****************************************************
//********************************************************************************************************

void KeyJudge(void)
{
	if(S3==0&&S3_last==1){   //按键S3按下
//		keymode+=1;
//		targetspeed+=5;
//		SET_motor(targetspeed,targetspeed);
		TargetSteer+=10;
		}
	if(S4==0&&S4_last==1){   //按键S4按下
//	    keymode-=1;
//	    targetspeed-=5;
//		SET_motor(targetspeed,targetspeed);
	    TargetSteer-=10;
	    }
	if(S5==0&&S5_last==1){   //按键S5按下
//		targetspeed+=10;
//		SET_motor(targetspeed,targetspeed);
		TargetSteer+=10;
		}
	if(S6==0&&S6_last==1){   //按键S6按下
//		targetspeed-=10;
//		SET_motor(targetspeed,targetspeed);
		TargetSteer-=10;
		}
	S3_last=S3;             //保存按键的状态
	S4_last=S4;
	S5_last=S5;
	S6_last=S6;
}

//********************************************************************************************************
//****************************************程序计时函数*****************************************************
//********************************************************************************************************
unsigned long TimeMesure(void)
{
	if(time2<time1)
		return 0xFFFFFFFF+time2-time1;
	else
		return time2-time1;
}
//*****************************************************************************************************************
//****************************************拨码开关函数*****************************************************    	  *
//*****************************************************************************************************************
void ChooseMode(void)
{//拨码开关拨到下是1，拨到上是0
	mode=31-(switch1*16+switch2*8+switch3*4+switch4*2+switch5);
}

