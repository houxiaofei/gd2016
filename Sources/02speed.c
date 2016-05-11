/*
 * 02speed.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int targetspeed=0,Motor_PWM_MAX=300,Motor_PWM_MIN=-300;
double csxs=0.6;//����ϵ��
//**********************���ٲ���***************************/
signed int Speed_kc1=20000,Speed_kc2=1800;
signed int wheel_distance=9;//�복��8
signed int RPID=0;
double r=0;
//**********************���PID����**********************************************;	
signed int ErrorLeft=0,PreErrorLeft=0,Pre2ErrorLeft,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,Pre2ErrorRight=0,SumErrorRight=0;
int Pwm_Delta_Left=0,Pwm_Delta_Right=0; 
int tsl_PWM=0,tsr_PWM=0;

double Speed_kp_Left=0.5,Speed_ki_Left=0,Speed_kd_Left=0;//16
double Speed_kp_Right=0.5,Speed_ki_Right=0,Speed_kd_Right=0;	//�������ʽPID

//double Speed_kp_Left=1,Speed_ki_Left=0.8,Speed_kd_Left=0.5;//16 I=0.01-1(0.8),d=1-10(0.5)
//double Speed_kp_Right=1,Speed_ki_Right=0.8,Speed_kd_Right=0.5;	//���λ��ʽPID
//double Speed_kp_Left=1.2,Speed_ki_Left=0.1,Speed_kd_Left=0.01;//16
//double Speed_kp_Right=1.2,Speed_ki_Right=0.1,Speed_kd_Right=0.01;	//�������ʽPID


//********************�������Բ���******************************************
unsigned int Counter_Error_Left=0,Counter_Error_Right=0;		//���Ӵ����ο����������






/*************************����ӿں���*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R =leftSpeed;}
		else {EMIOS_0.CH[21].CBDR.R = -leftSpeed;EMIOS_0.CH[22].CBDR.R = 0;}//����  E5����   E6���
	if(rightSpeed>=0) {EMIOS_0.CH[19].CBDR.R = rightSpeed;EMIOS_0.CH[20].CBDR.R = 0;}
		else {EMIOS_0.CH[19].CBDR.R = 0;EMIOS_0.CH[20].CBDR.R = -rightSpeed;}//����  E3�ҽ�   E4����
}

/*************************�ٶȿ��ƺ���*********************/
//void Speed_control(void)
//{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
//	tsl=((r+wheel_distance)/r)*targetspeed;//���ּ���
//	SET_motor(tsl,tsr);
//}
//*****************************************************************************************************************
//************************************************���ֲ���PID�ٶȿ���************************************************    	  *
//*****************************************************************************************************************
//void SpeedControl()//�ջ�,�Ӳ���λ��ʽ1111
//{
////	RPID=CENTER-Steer_PWM[3];
////	r=Speed_kc1/RPID;
////	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
////	tsl=((r+wheel_distance)/r)*targetspeed;//���ּ���
//	
//	r=Speed_kc2/error;
//	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
//	tsl=((r+wheel_distance)/r)*targetspeed;//���ּ���
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



void SpeedControl()//�ջ�,�Ӳ�������ʽ2222
{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
//	tsl=((r+wheel_distance+2)/r)*targetspeed;//���ּ���
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

	if(tsl_PWM>Motor_PWM_MAX)  tsl_PWM=Motor_PWM_MAX;
	else if(tsl_PWM<Motor_PWM_MIN)  tsl_PWM=Motor_PWM_MIN;	    
	if(tsr_PWM>Motor_PWM_MAX)  tsr_PWM=Motor_PWM_MAX;
	else if(tsr_PWM<Motor_PWM_MIN)  tsr_PWM=Motor_PWM_MIN;

	SET_motor(tsl_PWM,tsr_PWM);
	//SET_motor(100,100);
	
	Pre2ErrorLeft=PreErrorLeft;
	Pre2ErrorRight=PreErrorRight;
	PreErrorLeft=ErrorLeft;
	PreErrorRight=ErrorRight;
}


//void SpeedControl()//�ٶȿ���,�Ӳ�������ʽjucic
//{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
//	tsl=((r+wheel_distance+2)/r)*targetspeed;//���ּ���
//
////	tsl=targetspeed;
////	tsr=targetspeed;
//	
//	Current_Speed_Error=(signed int)(csl)-(signed int)(csr);
//	Target_Speed_Error=(signed int)(tsl)-(signed int)(tsr);
//	
//	Differ_Speed=Target_Speed_Error-Current_Speed_Error;
//	
//	Pwm_Delta_Left=(Speed_kp_Left+Speed_ki_Left+Speed_kd_Left)*Differ_Speed;
//	Pwm_Delta_Left-=(Speed_kp_Left+2*Speed_kd_Left)*Differ_Speed1;
//	Pwm_Delta_Left+=(Speed_kd_Left)*Differ_Speed2;
//	tsl_PWM+=Pwm_Delta_Left;
//	
//	Pwm_Delta_Right=(Speed_kp_Right+Speed_ki_Right+Speed_kd_Right)*ErrorRight;
//	Pwm_Delta_Right-=(Speed_kp_Right+2*Speed_kd_Right)*PreErrorRight;
//	Pwm_Delta_Right+=(Speed_kd_Right)*Pre2ErrorRight;
//	tsr_PWM+=Pwm_Delta_Right;
//
//	SET_motor(tsl,tsr);
//	//SET_motor(100,100);
//	
//	PreErrorLeft=ErrorLeft;
//	PreErrorRight=ErrorRight;
//}

