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
int CurrentSteer=0;
unsigned long time1=0;
unsigned long time2=0;
unsigned long time3=0;

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
	unsigned char edge='Y';
	unsigned char data='Z';
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
				Ts=14;
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
			Ts=14;
		}
		break;
//	case 9: 
//		CurrentSteer=TargetSteer;
//		LINFlex_TX(steer);
//		Ts=10;
//		break;
//	case 10: 
//		LINFlex_TX(SendInt1(CurrentSteer));        //发送舵机值
//		Ts=11;
//		break;
//	case 11: 
//		LINFlex_TX(SendInt2(CurrentSteer));
//		Ts=12;
//		break;
//	case 12: 
//		LINFlex_TX(SendInt3(CurrentSteer));
//		Ts=13;
//		break;
//	case 13:
//		LINFlex_TX(SendInt4(CurrentSteer));
//		Ts=14;
//		break;
	case 14: 
		LINFlex_TX(edge);
		Ts=15;
		break;
	case 15: 
		LINFlex_TX(SendInt1(csl));
		//LINFlex_TX(SendInt2(a_value));        //发送B跳变沿标准
		//LINFlex_TX(SendInt1(tsl));
		Ts=16;
		break;
	case 16: 
		LINFlex_TX(SendInt2(csl));
		//LINFlex_TX(SendInt3(a_value));
		//LINFlex_TX(SendInt2(tsl));
		Ts=17;
		break;
	case 17: 
		LINFlex_TX(SendInt3(csl));
		//LINFlex_TX(SendInt4(a_value));
		//LINFlex_TX(SendInt3(tsl));
		Ts=18;
		break;
	case 18:
		LINFlex_TX(SendInt4(csl));
		//LINFlex_TX(SendInt3(a_start));        //发送B起始位
		//LINFlex_TX(SendInt4(tsl));
		Ts=19;
		break;
	case 19: 
		LINFlex_TX(SendInt1(csr));
		//LINFlex_TX(SendInt4(a_start));
		//LINFlex_TX(SendInt1(tsr));
		Ts=20;
		break;
	case 20: 
		LINFlex_TX(SendInt2(csr));
		//LINFlex_TX(SendInt2(a_T));           //发送B阈值
		//LINFlex_TX(SendInt2(tsr));
		Ts=21;
		break;
	case 21: 
		LINFlex_TX(SendInt3(csr));
		//LINFlex_TX(SendInt3(a_T));
		//LINFlex_TX(SendInt3(tsr));
		Ts=22;
		break;
	case 22:
		LINFlex_TX(SendInt4(csr));
		//LINFlex_TX(SendInt4(a_T));
		//LINFlex_TX(SendInt4(tsr));
		Ts=23;
		break;
	case 23: 
		LINFlex_TX(data);
		Ts=24;
		break;
	case 24: 
		LINFlex_TX(SendInt4(enter_flag));        //发送A_flag
		Ts=25;
		break;
	case 25: 
		LINFlex_TX(SendInt4(ar_flag));
		Ts=26;
		break;
	case 26: 
		LINFlex_TX(SendInt4(bl_flag));        //发送B_flag
		Ts=27;
		break;
	case 27: 
		LINFlex_TX(SendInt4(br_flag));
		Ts=28;
		break;
	case 28: 
		LINFlex_TX(SendInt1(error));        //发送error
		Ts=29;
		break;
	case 29:
		LINFlex_TX(SendInt2(error));
		Ts=30;
		break;
	case 30: 
		LINFlex_TX(SendInt3(error));        
		Ts=31;
		break;
	case 31:
		LINFlex_TX(SendInt4(error));
		Ts=32;
		break;
	case 32: 
		LINFlex_TX(SendInt1(tsl));        //发送a_error
		Ts=33;
		break;
	case 33:
		LINFlex_TX(SendInt2(tsl));
		Ts=34;
		break;
	case 34: 
		LINFlex_TX(SendInt3(tsl));      
		Ts=35;
		break;
	case 35:
		LINFlex_TX(SendInt4(tsl));
		Ts=36;
		break;
	case 36: 
		LINFlex_TX(SendInt1(tsr));        //发送b_error
		Ts=37;
		break;
	case 37:
		LINFlex_TX(SendInt2(tsr));
		Ts=38;
		break;
	case 38: 
		LINFlex_TX(SendInt3(tsr));      
		Ts=39;
		break;
	case 39:
		LINFlex_TX(SendInt4(tsr));
		Ts=40;
		break;
	case 40:
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
			//turnspeed=X[7]*100+X[8]*10+X[9];
			//deadspeed=X[10]*100+X[11]*10+X[12];
			sp_x1=((double)(X[4]*100+X[5]*10+X[6]))/10000;
			Speed_kc1=X[7]*10000+X[8]*1000+X[9]*100;
			targetspeed=X[10]*100+X[11]*10+X[12];
			barspeed=X[13]*100+X[14]*10+X[15];
			RX_flag=0;
			break;
		case 2:
//			Speed_kp_Left=(float)(Y[1]*10+Y[2])+((float)(Y[3]*10+Y[4]))/100;
//			Speed_kp_Right=(float)(Y[5]*10+Y[6])+((float)(Y[7]*10+Y[8]))/100;
			KP_speed=(float)(Y[1]*10+Y[2])+((float)(Y[3]*10+Y[4]))/100;
			KI_speed=(float)(Y[5]*10+Y[6])+((float)(Y[7]*10+Y[8]))/100;
			KD_speed=(float)(Y[9]*10+Y[10])+((float)(Y[11]*10+Y[12]))/100;
			KP_DifSpd=(float)(Y[13]*10+Y[14])+((float)(Y[15]*10+Y[16]))/100;
			KI_DifSpd=(float)(Y[17]*10+Y[18])+((float)(Y[19]*10+Y[20]))/100;
			KD_DifSpd=(float)(Y[21]*10+Y[22])+((float)(Y[23]*10+Y[24]))/100;
			RX_flag=0;
			break;
		case 3:
			sp_x2=Z[1]*10+Z[2];
			sp_x3=Z[3]*10+Z[4];
//			trend_value=Z[1]*10+Z[2];
//			b_error_value=Z[3]*10+Z[4];
//			trend_value2=Z[5]*10+Z[6];
//			his_num=Z[7]*10+Z[8];
//			b_error_value2=Z[9]*10+Z[10];
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
		keymode=1;
//		csxs+=0.1;
//		Speed_ki_Right+=0.01;
//		Speed_ki_Left+=0.1;
//	    Speed_kp_Right+=1;
//	    Speed_kd_Right+=0.1;
//		targetspeed+=10;
//		Speed_kp_Left+=1;
//		Speed_kp_Right+=1;
		TargetSteer+=100;
//		KI_DifSpd-=0.01;
//		Speed_kc1+=1000;
//		tsr-=1;
		}
	if(S4==0&&S4_last==1){   //按键S4按下
	    keymode=2;
//		csxs-=0.1;
//		Speed_ki_Right-=0.01;
//		Speed_ki_Left-=0.1;
//		Speed_kp_Right-=1;
//		Speed_kd_Right-=0.1;
//	    targetspeed-=10;
//	    Speed_kp_Left-=1;
//	    Speed_kp_Right-=1;
	    TargetSteer-=100;
//	    KI_DifSpd+=0.01;
//	    Speed_kc1-=1000;
//	    tsr+=1;
	    }
	if(S5==0&&S5_last==1){   //按键S5按下
		keymode=3;
//		csxs+=0.01;
//		Speed_ki_Right+=0.1;
//		Speed_ki_Left+=0.1;
//    	Speed_kp_Right+=0.1;
//		Speed_kd_Left+=0.001;
//		Speed_kd_Right+=0.001;
//		targetspeed+=10;
//		SET_motor(targetspeed,targetspeed);
//		Speed_ki_Left+=0.1;
//		Speed_ki_Right+=0.1;
		TargetSteer+=10;
//		Steer_kd+=1;
//		KI_DifSpd+=0.1;
//		tsl-=1;
		}
	if(S6==0&&S6_last==1){   //按键S6按下
		keymode=4;
//		csxs-=0.01;
//		Speed_ki_Right-=0.1;
//		Speed_ki_Left-=0.1;
//		Speed_kp_Right-=0.1;
//		Speed_kd_Left-=0.001;
//		Speed_kd_Right-=0.001;
//		targetspeed-=10;
//		SET_motor(targetspeed,targetspeed);
//		Speed_ki_Left-=0.1;
//		Speed_ki_Right-=0.1;
		TargetSteer-=10;
//		KI_DifSpd-=0.1;
//		Steer_kd-=1;
//		tsl+=1;
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

