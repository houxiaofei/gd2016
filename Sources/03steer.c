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
unsigned int Steer_PWM[4]={0,0,0,0};//������ֵ��¼

/*************************����ӿں���***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[11].CBDR.R = steer;}
/*************************������ƣ�PD***********************/
void SteerControl(void)
{
	Steer_PWM[3]=CENTER+Steer_kp*target_offset+Steer_kd*(target_offset-last_offset);//λ��ʽPD

	SET_steer(Steer_PWM[3]);
	
	//����ֵ��offsetֵ
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
