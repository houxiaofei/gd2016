/*
 * 02speed.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int targetspeed=0,Motor_PWM_MAX=400,Motor_PWM_MIN=-400;
//**********************差速参数***************************/
signed int Speed_kc=15000;
signed int wheel_distance=9;//半车距8
signed int RPID=0;	
double r=0;

//**********************电机PID参数**********************************************;	
signed int ErrorLeft=0,PreErrorLeft=0,Pre2ErrorLeft,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,Pre2ErrorRight=0,SumErrorRight=0;
int Pwm_Delta_Left=0,Pwm_Delta_Right=0; 
int tsl_PWM=0,tsr_PWM=0;
//double Speed_kp_Left=5,Speed_ki_Left=0.8,Speed_kd_Left=0.1;//16
//double Speed_kp_Right=5,Speed_ki_Right=1,Speed_kd_Right=0.1;	//电机位置式PID
double Speed_kp_Left=6,Speed_ki_Left=0.01,Speed_kd_Left=0;//16
double Speed_kp_Right=6,Speed_ki_Right=0,Speed_kd_Right=0;	//电机增量式PID


//********************辅助调试参数******************************************
unsigned int Counter_Error_Left=0,Counter_Error_Right=0;		//光编接触不牢靠错误计数量






/*************************电机接口函数*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R =leftSpeed;}
		else {EMIOS_0.CH[21].CBDR.R = -leftSpeed;EMIOS_0.CH[22].CBDR.R = 0;}//左轮  E5左退   E6左进
	if(rightSpeed>=0) {EMIOS_0.CH[19].CBDR.R = rightSpeed;EMIOS_0.CH[20].CBDR.R = 0;}
		else {EMIOS_0.CH[19].CBDR.R = 0;EMIOS_0.CH[20].CBDR.R = -rightSpeed;}//右轮  E3右进   E4右退
}

///*************************速度控制函数*********************/
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
//void SpeedControl()//闭环,加差速
//{
////	RPID=CENTER-Steer_PWM[3];
////	r=Speed_kc/RPID;
////	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
////	tsl=((r+wheel_distance+2)/r)*targetspeed;//左轮加速
////	SET_motor(tsl,tsr);
//	tsl=targetspeed;
//	tsr=targetspeed;
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
//	tsl=Speed_kp_Left*ErrorLeft+Speed_ki_Left*SumErrorLeft+Speed_kd_Left*(ErrorLeft-PreErrorLeft);
//	tsr=Speed_kp_Right*ErrorRight+Speed_ki_Left*SumErrorRight+Speed_kd_Left*(ErrorRight-PreErrorRight);
//	
//	if(tsl>Motor_PWM_MAX)  tsl=Motor_PWM_MAX;
//	else if(tsl<Motor_PWM_MIN)  tsl=Motor_PWM_MIN;	    
//	if(tsr>Motor_PWM_MAX)  tsr=Motor_PWM_MAX;
//	else if(tsr<Motor_PWM_MIN)  tsr=Motor_PWM_MIN;
//
//	SET_motor(tsl,tsr);
//	//SET_motor(100,100);
//	
//	PreErrorLeft=ErrorLeft;
//	PreErrorRight=ErrorRight;
//}

void SpeedControl()//闭环,加差速
{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//右轮减速
//	tsl=((r+wheel_distance+2)/r)*targetspeed;//左轮加速
//	SET_motor(tsl,tsr);
	tsl=targetspeed;
	tsr=targetspeed;
	
	ErrorLeft=(signed int)(tsl)-(signed int)(csl);
	ErrorRight=(signed int)(tsr)-(signed int)(csr);
	
	Pwm_Delta_Left=(Speed_kp_Left+Speed_ki_Left+Speed_kd_Left)*ErrorLeft;
	Pwm_Delta_Left-=(Speed_kp_Left+2*Speed_kd_Left)*PreErrorLeft;
	Pwm_Delta_Left+=(Speed_kd_Left)*Pre2ErrorLeft;
	tsl_PWM+=Pwm_Delta_Left;
	
	Pwm_Delta_Right=(Speed_kp_Right+Speed_ki_Right+Speed_kd_Right)*ErrorRight;
	Pwm_Delta_Right-=(Speed_kp_Right+2*Speed_kd_Right)*PreErrorRight;
	Pwm_Delta_Right+=(Speed_kd_Right)*Pre2ErrorRight;
	tsr_PWM+=Pwm_Delta_Right;

//	if(tsl>Motor_PWM_MAX)  tsl=Motor_PWM_MAX;
//	else if(tsl<Motor_PWM_MIN)  tsl=Motor_PWM_MIN;	    
//	if(tsr>Motor_PWM_MAX)  tsr=Motor_PWM_MAX;
//	else if(tsr<Motor_PWM_MIN)  tsr=Motor_PWM_MIN;

	SET_motor(tsl_PWM,tsr_PWM);
	//SET_motor(100,100);
	
	Pre2ErrorLeft=PreErrorLeft;
	Pre2ErrorRight=PreErrorRight;
	PreErrorLeft=ErrorLeft;
	PreErrorRight=ErrorRight;
}
