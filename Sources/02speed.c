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
int targetspeed=0,Motor_PWM_MAX=300,Motor_PWM_MIN=-300;
float csxs=0.6;//差速系数
//**********************差速参数***************************/
signed int Speed_kc1=16000,Speed_kc2=1300;
signed int wheel_distance=9;//半车距8
signed int RPID=0;
float r=0;
//**********************电机PID参数**********************************************;	
int ErrorLeft=0,PreErrorLeft=0,Pre2ErrorLeft=0,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,Pre2ErrorRight=0,SumErrorRight=0;
int intErrorLeft=0,intErrorRight=0;
float Pwm_Delta_Left=0,Pwm_Delta_Right=0; 
int tsl_PWM=0,tsr_PWM=0,tsr_Delta=0,error_Delta=0;

//float Speed_kp_Left=6,Speed_ki_Left=0.1,Speed_kd_Left=0;//16
//float Speed_kp_Right=0.1,Speed_ki_Right=0.01,Speed_kd_Right=0;	//电机增量式PID

float Speed_kp_Left=1,Speed_ki_Left=0.8,Speed_kd_Left=0.5;//16 I=0.01-1(0.8),d=1-10(0.5)
float Speed_kp_Right=1,Speed_ki_Right=0.8,Speed_kd_Right=0.5;	//电机位置式PID
//double Speed_kp_Left=1.2,Speed_ki_Left=0.1,Speed_kd_Left=0.01;//16
//double Speed_kp_Right=1.2,Speed_ki_Right=0.1,Speed_kd_Right=0.01;	//电机增量式PID


//********************辅助调试参数******************************************
unsigned int Counter_Error_Left=0,Counter_Error_Right=0;		//光编接触不牢靠错误计数量

//**********************双PID差速控制参数**********************************************;	
float KP_speed=3,KI_speed=0.02,KD_speed=0.4;
int Speed_Err=0,Speed_Err_his=0,Speed_Err_his2=0;
int Pwm_Delta=0;
int MotorPWM=0;
float KP_DifSpd=0.8,KI_DifSpd=0.01,KD_DifSpd=0.2;
int Cur_DifSpd=0,Tar_DifSpd=0;
int DifSpd_Err=0,DifSpd_Err_his=0,DifSpd_Err_his2=0;
int DifferPWM_Delta=0;
int DifferPWM=0;
int LMotorPWM=0,RMotorPWM=0;



/*************************电机接口函数*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R =leftSpeed;}
		else {EMIOS_0.CH[21].CBDR.R = -leftSpeed;EMIOS_0.CH[22].CBDR.R = 0;}//左轮  E5左退   E6左进
	if(rightSpeed>=0) {EMIOS_0.CH[19].CBDR.R = rightSpeed;EMIOS_0.CH[20].CBDR.R = 0;}
		else {EMIOS_0.CH[19].CBDR.R = 0;EMIOS_0.CH[20].CBDR.R = -rightSpeed;}//右轮  E3右进   E4右退
}

//**********************双PID差速控制*******************************************
void Speed_PID(void)
{
	Speed_Err=targetspeed-((csl+csr)/2);
	Pwm_Delta=KP_speed*(Speed_Err-Speed_Err_his)+KI_speed*Speed_Err+KD_speed*(Speed_Err+Speed_Err_his2-2*Speed_Err_his);
	MotorPWM+=Pwm_Delta;
	if(MotorPWM>Motor_PWM_MAX)  MotorPWM=Motor_PWM_MAX;
	else if(MotorPWM<Motor_PWM_MIN)  MotorPWM=Motor_PWM_MIN;
	Speed_Err_his2=Speed_Err_his;
	Speed_Err_his=Speed_Err;
	//SET_motor(MotorPWM,MotorPWM);
	//SET_motor(100,100);
}
void DifferSpeed_PID(void)
{
	RPID=CENTER-Steer_PWM[3];
	r=Speed_kc1/RPID;
	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
	Cur_DifSpd=csl-csr;
	Tar_DifSpd=tsl-tsr;
	DifSpd_Err=Tar_DifSpd-Cur_DifSpd;
	DifferPWM_Delta=KP_DifSpd*(DifSpd_Err-DifSpd_Err_his)+KI_DifSpd*DifSpd_Err+KD_DifSpd*(DifSpd_Err+DifSpd_Err_his2-2*DifSpd_Err_his);
	DifSpd_Err_his2=DifSpd_Err_his;
	DifSpd_Err_his=DifSpd_Err;
	DifferPWM+=DifferPWM_Delta;
	LMotorPWM=MotorPWM+DifferPWM;
	RMotorPWM=MotorPWM-DifferPWM;
	SET_motor(LMotorPWM,RMotorPWM);
}

/*************************速度控制函数*********************/
//void Speed_control(void)
//{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
//	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//	SET_motor(tsl,tsr);
//}
//*****************************************************************************************************************
//************************************************后轮差速PID速度控制************************************************    	  *
//*****************************************************************************************************************
//void SpeedControl()//闭环,加差速位置式1111
//{
////	RPID=CENTER-Steer_PWM[3];
////	r=Speed_kc1/RPID;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//	
////	r=Speed_kc2/error;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance)/r)*targetspeed;//左轮加速
//
////	tsl=targetspeed;
////	tsr=targetspeed;
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
//	ErrorLeft=(signed int)(tsl)-(signed int)(csl);
//	ErrorRight=(signed int)(tsr)-(signed int)(csr);
//	
//	SumErrorLeft+=ErrorLeft;
//	if(SumErrorLeft>350) SumErrorLeft=350;
//	if(SumErrorLeft<-350) SumErrorLeft=-350;	    
//	SumErrorRight+=ErrorRight;
//	if(SumErrorRight>350) SumErrorRight=350;
//	if(SumErrorRight<-350) SumErrorRight=-350;
//	
//	tsl_PWM=Speed_kp_Left*ErrorLeft+Speed_ki_Left*SumErrorLeft+Speed_kd_Left*(ErrorLeft-PreErrorLeft);
//	tsr_PWM=Speed_kp_Right*ErrorRight+Speed_ki_Left*SumErrorRight+Speed_kd_Left*(ErrorRight-PreErrorRight);
//	
//	if(tsl_PWM>Motor_PWM_MAX)  tsl_PWM=Motor_PWM_MAX;
//	else if(tsl_PWM<Motor_PWM_MIN)  tsl_PWM=Motor_PWM_MIN;	    
//	if(tsr_PWM>Motor_PWM_MAX)  tsr_PWM=Motor_PWM_MAX;
//	else if(tsr_PWM<Motor_PWM_MIN)  tsr_PWM=Motor_PWM_MIN;
//
//	SET_motor(tsl_PWM,tsr_PWM);
//	//SET_motor(100,100);
//	
//	PreErrorLeft=ErrorLeft;
//	PreErrorRight=ErrorRight;
//}



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
