/*
 * 02speed.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int targetspeed=0,Motor_PWM_MAX=200,Motor_PWM_MIN=80;
//**********************差速参数***************************/
signed int Speed_kc=15000;
signed int wheel_distance=9;//半车距8
signed int RPID=0;	
double r=0;

//**********************电机PID参数**********************************************;	
signed int ErrorLeft=0,PreErrorLeft=0,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,SumErrorRight=0;
 
double Speed_kp_Left=15,Speed_ki_Left=0,Speed_kd_Left=0;//16
double Speed_kp_Right=15,Speed_ki_Right=0,Speed_kd_Right=0;	//电机PID


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
	
	SumErrorLeft+=ErrorLeft;
	if(SumErrorLeft>350) SumErrorLeft=350;
	if(SumErrorLeft<-350) SumErrorLeft=-350;	    
	SumErrorRight+=ErrorRight;
	if(SumErrorRight>350) SumErrorRight=350;
	if(SumErrorRight<-350) SumErrorRight=-350;
	
	tsl=Speed_kp_Left*ErrorLeft+Speed_ki_Left*SumErrorLeft+Speed_kd_Left*(ErrorLeft-PreErrorLeft);
	tsr=Speed_kp_Right*ErrorRight+Speed_ki_Left*SumErrorRight+Speed_kd_Left*(ErrorRight-PreErrorRight);
	
	if(tsl>Motor_PWM_MAX)  tsl=Motor_PWM_MAX;
	else if(tsl<Motor_PWM_MIN)  tsl=Motor_PWM_MIN;	    
	if(tsr>Motor_PWM_MAX)  tsr=Motor_PWM_MAX;
	else if(tsr<Motor_PWM_MIN)  tsr=Motor_PWM_MIN;

	SET_motor(tsl,tsr);
	
	PreErrorLeft=ErrorLeft;
	PreErrorRight=ErrorRight;
}
