/*
 * 02speed.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int csl_cnt[3]={0,0,0},csr_cnt[3]={0,0,0};
int targetspeed=0,Motor_PWM_MAX=450,Motor_PWM_MIN=-400;
float csxs=0.6;//差速系数
//**********************变速参数***************************/
int straightspeed=230,transspeed=180,turnspeed=180,deadspeed=180,barspeed=190;//250,190,180
//**********************差速参数***************************/
signed int Speed_kc1=12000,Speed_kc2=1300;//170-17000  180 15000,
signed int wheel_distance=9;//半车距8
signed int RPID=0;
float r=0;
//**********************电机PID参数**********************************************;	
int ErrorLeft=0,PreErrorLeft=0,Pre2ErrorLeft=0,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,Pre2ErrorRight=0,SumErrorRight=0;
int intErrorLeft=0,intErrorRight=0;
float Pwm_Delta_Left=0,Pwm_Delta_Right=0; 
int tsl_PWM=0,tsr_PWM=0,tsr_Delta=0,error_Delta=0;
int ramp_flag=0;
//float Speed_kp_Left=6,Speed_ki_Left=0.1,Speed_kd_Left=0;//16
//float Speed_kp_Right=0.1,Speed_ki_Right=0.01,Speed_kd_Right=0;	//电机增量式PID

float Speed_kp_Left=10,Speed_ki_Left=0.2,Speed_kd_Left=1;//16 I=0.01-1(0.8),d=1-10(0.5)
					//8
float Speed_kp_Right=10,Speed_ki_Right=0.2,Speed_kd_Right=1;	//电机位置式PID 10,0.2,1
					//8
//**********************双PID差速控制参数(内外环增量式)**********************************************;	
//float KP_speed=3,KI_speed=0.01,KD_speed=0.4;//1,0.02，0.4
//int Speed_Err=0,Speed_Err_his=0,Speed_Err_his2=0;
//int Pwm_Delta=0;
//int MotorPWM=0;

//float KP_DifSpd=0.8,KI_DifSpd=0.01,KD_DifSpd=0.2;//0.8，0.01，0.2//2,0.02,0.1//0.8 0.01 0.2
//int Cur_DifSpd=0,Tar_DifSpd=0;
//int DifSpd_Err=0,DifSpd_Err_his=0,DifSpd_Err_his2=0;
//int DifferPWM_Delta=0;
//int DifferPWM=0;
//int LMotorPWM=0,RMotorPWM=0;

//**********************双PID差速控制参数(内外环位置式)**********************************************;	
float KP_speed=5,KI_speed=0.01,KD_speed=0.1;//4,0.8,0.1//15,0.01,0.1
int Sum_Speed_Err=0,Speed_Err=0,Pre_Speed_Err=0;
int Pwm_Delta=0;
int MotorPWM=0;

float KP_DifSpd=5,KI_DifSpd=0.01,KD_DifSpd=0.1;//0.8，0.01，0.2//15,0.6,0.1
int Cur_DifSpd=0,Tar_DifSpd=0;
int DifSpd_Err=0,Sum_DifSpd_Err=0,Pre_DifSpd_Err=0;
int DifferPWM_Delta=0;
int DifferPWM=0;
int LMotorPWM=0,RMotorPWM=0;


/*************************电机接口函数*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed>=0) {EMIOS_0.CH[19].CBDR.R = leftSpeed;EMIOS_0.CH[20].CBDR.R = 0;}
		else {EMIOS_0.CH[19].CBDR.R = 0;EMIOS_0.CH[20].CBDR.R = -leftSpeed;}//左轮  E3左进   E4左退
	if(rightSpeed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R =rightSpeed;}
		else {EMIOS_0.CH[21].CBDR.R = -rightSpeed;EMIOS_0.CH[22].CBDR.R = 0;}//右轮  E5右退   E6右进
}

/*************************变速控制函数*********************/
void Speed_Set(void)
{
//	int i=0,j=0,all_flag_count=0;
//	for(i=0;i<10;i++)
//	{
//		if(all_flag[i]!=2222)
//		{
//			all_flag_count++;
//		}
//		if(all_flag_count>=3)
//		{
//			j=1;
//			break;
//		}
//	}
//	if(stop_flag==1)
//		targetspeed=0;
//	else if(a_bar_flag==1)
//		targetspeed=barspeed;
//    else if(j==0||a_flag==11||b_flag==11)
//		targetspeed=straightspeed;
//	else if((a_flag==21||a_flag==12)&&b_flag==22)
//		targetspeed=transspeed;
//	//else if(error>=45)
//	else if(Steer_PWM[3]==LEFT||Steer_PWM[3]==RIGHT)
//		targetspeed=deadspeed;
//	else
//		targetspeed=turnspeed;	
	
	//C-CCD版本
	if(turn_flag)
		targetspeed=turnspeed;
	else if(straight_flag)
		targetspeed=straightspeed;
	else if(trans_enter_flag||trans_out_flag)
		targetspeed=transspeed;
	else if(Steer_PWM[3]==LEFT||Steer_PWM[3]==RIGHT)
		targetspeed=deadspeed;
	else if(a_bar_flag==1)
		targetspeed=barspeed;
	else
		targetspeed=turnspeed;
}

//**********************双PID差速控制*******************************************
void Speed_PID(void)//外环增量式
{
//	Speed_Err=targetspeed-((csl+csr)/2);
//	Pwm_Delta=KP_speed*(Speed_Err-Speed_Err_his)+KI_speed*Speed_Err+KD_speed*(Speed_Err+Speed_Err_his2-2*Speed_Err_his);
//	//Pwm_Delta=(KP_speed+KI_speed+KD_speed)*Speed_Err-(KP_speed+2KD_speed)*Speed_Err_his+KD_speed*Speed_Err_his2;
//	//Pwm_Delta=KP_speed*Speed_Err;
//	MotorPWM+=Pwm_Delta;
//	if(MotorPWM>Motor_PWM_MAX)  MotorPWM=Motor_PWM_MAX;
//	else if(MotorPWM<Motor_PWM_MIN)  MotorPWM=Motor_PWM_MIN;
//	Speed_Err_his2=Speed_Err_his;
//	Speed_Err_his=Speed_Err;
//	//SET_motor(MotorPWM,MotorPWM);
//	//SET_motor(100,100);
}
void DifferSpeed_PID(void)//内环增量式
{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc1/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
//	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//	Cur_DifSpd=csl-csr;
//	Tar_DifSpd=tsl-tsr;
//	DifSpd_Err=Tar_DifSpd-Cur_DifSpd;
//	DifferPWM_Delta=(int)(KP_DifSpd*(DifSpd_Err-DifSpd_Err_his)+KI_DifSpd*DifSpd_Err+KD_DifSpd*(DifSpd_Err+DifSpd_Err_his2-2*DifSpd_Err_his));
//	DifSpd_Err_his2=DifSpd_Err_his;
//	DifSpd_Err_his=DifSpd_Err;
//	DifferPWM+=DifferPWM_Delta;
//	if(DifferPWM>=0)
//	{
//		LMotorPWM=MotorPWM+DifferPWM;
//		RMotorPWM=MotorPWM-DifferPWM;
//	}
//	else
//	{
//		LMotorPWM=MotorPWM+DifferPWM;
//		RMotorPWM=MotorPWM-DifferPWM;
//	}
//	
//	if(LMotorPWM>Motor_PWM_MAX)  LMotorPWM=Motor_PWM_MAX;
//	else if(LMotorPWM<Motor_PWM_MIN)  LMotorPWM=Motor_PWM_MIN;
//	if(RMotorPWM>Motor_PWM_MAX)  RMotorPWM=Motor_PWM_MAX;
//	else if(RMotorPWM<Motor_PWM_MIN)  RMotorPWM=Motor_PWM_MIN;
//	
//	SET_motor(LMotorPWM,RMotorPWM);
}

void Speed_PID2(void) //外环位置式
{
	Speed_Err=targetspeed-((csl+csr)/2);
	Sum_Speed_Err+=Speed_Err;
//	if(Speed_Err/targetspeed>0.15)
//	{
//		MotorPWM=Motor_PWM_MAX;
//	}
//	else if(Speed_Err/targetspeed<-0.10)
//	{
//		MotorPWM=Motor_PWM_MIN;
//	}
//	else
//	{
	if(Sum_Speed_Err>220) Sum_Speed_Err=220;
	if(Sum_Speed_Err<-220) Sum_Speed_Err=-220;
	
	MotorPWM=(KP_speed*Speed_Err+KI_speed*Sum_Speed_Err+KD_speed*(Speed_Err-Pre_Speed_Err))+0.5;
	
	if(MotorPWM>Motor_PWM_MAX)  MotorPWM=Motor_PWM_MAX;
	else if(MotorPWM<Motor_PWM_MIN)  MotorPWM=Motor_PWM_MIN;

//	}

	Pre_Speed_Err=Speed_Err;
	//SET_motor(MotorPWM,MotorPWM);
	//SET_motor(100,100);
}

void DifferSpeed_PID2(void)//内环位置式
{
	RPID=CENTER-Steer_PWM[3];
	r=Speed_kc1/RPID;
//	if(RPID<80&&RPID>-80)
//	{
//		tsr=targetspeed;//右轮减速
//		tsl=targetspeed;//左轮加速
//	}
//	else
//	{
	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//	}
	
	Cur_DifSpd=csl-csr;
	Tar_DifSpd=tsl-tsr;
	DifSpd_Err=Tar_DifSpd-Cur_DifSpd;
	
	Sum_DifSpd_Err+=DifSpd_Err;
	if(Sum_DifSpd_Err>170) Sum_DifSpd_Err=170;
	if(Sum_DifSpd_Err<-170) Sum_DifSpd_Err=-170;
	DifferPWM=(KP_speed*DifSpd_Err+KI_speed*Sum_DifSpd_Err+KD_speed*(DifSpd_Err-Pre_DifSpd_Err))+0.5;
	
	if(error>=0)
	{
		LMotorPWM=MotorPWM+(int)(DifferPWM*0);
		RMotorPWM=MotorPWM-(int)(DifferPWM*2);
	}
	else
	{
		LMotorPWM=MotorPWM+(int)(DifferPWM*2);
		RMotorPWM=MotorPWM-(int)(DifferPWM*0);
	}
//	LMotorPWM=MotorPWM+DifferPWM;
//	RMotorPWM=MotorPWM-DifferPWM;
	
	if(LMotorPWM>Motor_PWM_MAX)  LMotorPWM=Motor_PWM_MAX;
	else if(LMotorPWM<Motor_PWM_MIN)  LMotorPWM=Motor_PWM_MIN;
	if(RMotorPWM>Motor_PWM_MAX)  RMotorPWM=Motor_PWM_MAX;
	else if(RMotorPWM<Motor_PWM_MIN)  RMotorPWM=Motor_PWM_MIN;
	
	SET_motor(LMotorPWM,RMotorPWM);
	
	Pre_DifSpd_Err=DifSpd_Err;
}

//*****************************************************************************************************************
//************************************************后轮差速PID速度控制************************************************    	  *
//*****************************************************************************************************************
void SpeedControl()//闭环,加差速位置式1111
{
	RPID=CENTER-Steer_PWM[3];
	r=Speed_kc1/RPID;
	
//	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
//	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
	
	if(error<0)
	{
	
	tsr=targetspeed;//右轮加速((r-1.5*wheel_distance)/r)*
	
	tsl=((r+wheel_distance)/r)*targetspeed;//左轮减速
	}
	else
	{
	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
	
	tsl=targetspeed;//左轮加速((r+1.5*wheel_distance)/r)*
	
	}
	
//	
////	r=Speed_kc2/error;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//
	
//	tsl=targetspeed;
//	tsr=targetspeed;
	
//	
//	
////	if(error<-20&&a_flag!=22)//&&b_flag!=22)
////	{
////		tsl=targetspeed*csxs;
////		tsr=targetspeed*1;
////	}
////	else if(error>20&&a_flag!=22)
////	{
////		tsr=targetspeed*csxs;
////		tsl=targetspeed*1;
////	}
////	else
////	{
////		tsr=targetspeed;
////		tsl=targetspeed;
////	}
//	
	ErrorLeft=(signed int)(tsl)-(signed int)(csl);
	ErrorRight=(signed int)(tsr)-(signed int)(csr);
	
//	if(ErrorLeft/tsl>0.15&&ErrorRight/tsr>0.15)
//	{
//		tsl_PWM=Motor_PWM_MAX;
//		tsr_PWM=Motor_PWM_MAX;
//	}
//	else
//	{
	
	SumErrorLeft+=ErrorLeft;
	if(SumErrorLeft>200) SumErrorLeft=200;
	if(SumErrorLeft<-200) SumErrorLeft=-200;	    
	SumErrorRight+=ErrorRight;
	if(SumErrorRight>200) SumErrorRight=200;
	if(SumErrorRight<-200) SumErrorRight=-200;
	
	tsl_PWM=Speed_kp_Left*ErrorLeft+Speed_ki_Left*SumErrorLeft+Speed_kd_Left*(ErrorLeft-PreErrorLeft);
	tsr_PWM=Speed_kp_Right*ErrorRight+Speed_ki_Left*SumErrorRight+Speed_kd_Left*(ErrorRight-PreErrorRight);
	
	if(tsl_PWM>Motor_PWM_MAX)  tsl_PWM=Motor_PWM_MAX;
	else if(tsl_PWM<Motor_PWM_MIN)  tsl_PWM=Motor_PWM_MIN;	    
	if(tsr_PWM>Motor_PWM_MAX)  tsr_PWM=Motor_PWM_MAX;
	else if(tsr_PWM<Motor_PWM_MIN)  tsr_PWM=Motor_PWM_MIN;
//	}

	SET_motor(tsl_PWM,tsr_PWM);
	//SET_motor(100,100);
	
	PreErrorLeft=ErrorLeft;
	PreErrorRight=ErrorRight;
}



//void SpeedControl()//闭环,加差速增量式2222
//{
////	RPID=CENTER-Steer_PWM[3];
////	r=Speed_kc/RPID;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance+2)/r)*targetspeed;//左轮加速
////	SET_motor(tsl,tsr);
////	r=Speed_kc2/error;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//	tsl=targetspeed;
//	tsr=targetspeed;
//	
//	ErrorLeft=tsl-csl;
//	ErrorRight=tsr-csr;
//	
//	tsl_PWM+=Speed_kp_Left*(ErrorLeft-PreErrorLeft)+Speed_ki_Left*ErrorLeft+Speed_kd_Left*(ErrorLeft+Pre2ErrorLeft-2*PreErrorLeft);
//	
////	Pwm_Delta_Right=(Speed_kp_Right+Speed_ki_Right+Speed_kd_Right)*ErrorRight;
////	Pwm_Delta_Right-=(Speed_kp_Right+2*Speed_kd_Right)*PreErrorRight;
////	Pwm_Delta_Right+=(Speed_kd_Right)*Pre2ErrorRight;
////	tsr_PWM+=(int)Pwm_Delta_Right;
//	tsr_Delta=Speed_kp_Right*(ErrorRight-PreErrorRight)+Speed_ki_Right*ErrorRight+Speed_kd_Right*(ErrorRight+Pre2ErrorRight-2*PreErrorRight);
//	tsr_PWM+=tsr_Delta;
//
//	if(tsl_PWM>Motor_PWM_MAX)  tsl_PWM=Motor_PWM_MAX;
//	else if(tsl_PWM<Motor_PWM_MIN)  tsl_PWM=Motor_PWM_MIN;	    
//	if(tsr_PWM>Motor_PWM_MAX)  tsr_PWM=Motor_PWM_MAX;
//	else if(tsr_PWM<Motor_PWM_MIN)  tsr_PWM=Motor_PWM_MIN;
//
//	SET_motor(tsl_PWM,tsr_PWM);
//	//SET_motor(100,100);
//	
//	Pre2ErrorLeft=PreErrorLeft;
//	Pre2ErrorRight=PreErrorRight;
//	PreErrorLeft=ErrorLeft;
//	PreErrorRight=ErrorRight;
//}
