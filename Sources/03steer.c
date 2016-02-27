/*
 * 03steer.c
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#include "includes.h"

/*************************�������***************************/
int target_offset=0,last_offset=0;	//���ƫ��ֵ��¼
double Steer_kp=0,Steer_kd=0;//���P��Dֵ
unsigned int RIGHT=3200;
unsigned int LEFT=4150;
unsigned int Steer_PWM[4]={0,0,0,0};//������ֵ��¼

/*************************����ӿں���***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[11].CBDR.R = steer;}
/*************************������ƣ�PD***********************/
void SteerControl(void)
{
	if(wrong_flag==1)
	{
		Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
		SET_steer(Steer_PWM[3]);
		//����ֵ
		Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
		return;
	}
	Steer_kp=8;
	Steer_kd=0;
	Steer_PWM[3] = CENTER-Steer_kp*target_offset-Steer_kd*(target_offset-last_offset); //λ��ʽPD

	if(Steer_PWM[3]>LEFT) Steer_PWM[3]=LEFT;
	else if(Steer_PWM[3]<RIGHT) Steer_PWM[3]=RIGHT;

	SET_steer(Steer_PWM[3]);
	
	//����ֵ��offsetֵ
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
