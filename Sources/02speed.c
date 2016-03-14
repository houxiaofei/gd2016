/*
 * 02speed.c
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#include "includes.h"

int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int targetspeed=0,Motor_PWM_MAX=300,Motor_PWM_MIN=80;
//**********************���ٲ���***************************/
signed int Speed_kc=15000;
signed int wheel_distance=8;//�복��
signed int RPID=0;	
double r=0;




////**********************�������**********************************************
//signed long target_offset=0,last_offset=0;	//���ƫ��ֵ��¼
//double Steer_kp=0,Steer_kd=0;//���P��Dֵ
//unsigned int Steer_PWM[4]={0,0,0,0};//������ֵ��¼

//**********************�������**********************************************
//word speedcounter1=0,speedcounter2=0,speedcounter3=0,speedcounter4=0;
signed int targetspeedleft=0,targetspeedright=0;		//��ǰ�ٶȡ�Ŀ���ٶ�,Ӧ����100-300��Χ����	
signed int ErrorLeft=0,PreErrorLeft=0,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,SumErrorRight=0;
 
double Speed_kp_Left=0,Speed_ki_Left=0,Speed_kd_Left=0,Speed_kp_Right=0,Speed_ki_Right=0,Speed_kd_Right=0;	//���PID


//********************�������Բ���******************************************
unsigned int Counter_Error_Left=0,Counter_Error_Right=0;		//���Ӵ����ο����������

//********************�������Բ���******************************************
//double Steer_k1=5,Steer_k2=5,Steer_k3=15,Steer_k4=15,Steer_k5=18,Steer_k6=18,Steer_k7=5,Steer_k8=0;
double Speed_Left_k1=60,Speed_Left_k2=8,Speed_Left_k3=0;
double Speed_Right_k1=60,Speed_Right_k2=8,Speed_Right_k3=0;
//signed int Speed_kc_k=150;





/*************************����ӿں���*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R =leftSpeed;}
		else {EMIOS_0.CH[21].CBDR.R = -leftSpeed;EMIOS_0.CH[22].CBDR.R = 0;}//����  E5����   E6���
	if(rightSpeed>=0) {EMIOS_0.CH[19].CBDR.R = rightSpeed;EMIOS_0.CH[20].CBDR.R = 0;}
		else {EMIOS_0.CH[19].CBDR.R = 0;EMIOS_0.CH[20].CBDR.R = -rightSpeed;}//����  E3�ҽ�   E4����
}

///*************************�ٶȿ��ƺ���*********************/
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
void SpeedControl()//�ջ�,�Ӳ���
{
         targetspeed=100;
		 RPID=CENTER-Steer_PWM[3];
		 if(RPID>0)//���ҹ�
		 {
					 r=Speed_kc/RPID;
			targetspeedright=((r-wheel_distance)/r)*targetspeed;//���ּ���
			targetspeedleft=((r+wheel_distance)/r)*targetspeed;//���ּ���
		 }
		else if(RPID<0)//�����
		 {
					 r=-Speed_kc/RPID;
			targetspeedright=((r+wheel_distance)/r)*targetspeed;//���ּ���
			targetspeedleft=((r-wheel_distance)/r)*targetspeed;//���ּ���
		 }
		else
		{
			targetspeedleft=targetspeed;
			targetspeedright=targetspeed;
		}
		
		Speed_kp_Left=Speed_Left_k1/10;
		Speed_ki_Left=Speed_Left_k2/10;
		Speed_kd_Left=Speed_Left_k3/10;
		
		Speed_kp_Right=Speed_Right_k1/10;
		Speed_ki_Right=Speed_Right_k2/10;
		Speed_kd_Right=Speed_Right_k3/10;
		
	    ErrorLeft=(signed int)(targetspeedleft)-(signed int)(csl);
	    ErrorRight=(signed int)(targetspeedright)-(signed int)(csr);
	    
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
