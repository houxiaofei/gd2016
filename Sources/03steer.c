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
unsigned int RIGHT=3250;
unsigned int LEFT=4182;
unsigned int Steer_PWM[4]={0,0,0,CENTER};//������ֵ��¼

/*************************����ӿں���***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[11].CBDR.R = steer;}
/*************************���PD��������***********************/
void Steer_PDSet(void)
{
	target_offset=error;
	if(targetspeed<30)//120
	{
		Steer_kp=8;Steer_kd=0;
		return;
	}
	else if(targetspeed<50)//120(150)-5 8 8 12 12 10//130-170
	{
		if(ABS(target_offset)<25)        {Steer_kp=4;Steer_kd=5;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.5+4;Steer_kd=5;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.6+9;Steer_kd=5;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+15;Steer_kd=5;}
		else                            {Steer_kp=16;Steer_kd=5;}
		return;
	}
	else if(targetspeed<70)//140-180//150-190//160 5 8 8 10 10 10
	{
		if(ABS(target_offset)<25)        {Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.5+5;Steer_kd=5;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.6+10;Steer_kd=5;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+16;Steer_kd=5;}
		else                            {Steer_kp=17;Steer_kd=5;}
		return;
	}
	else if(targetspeed<90)
	{
		if(ABS(target_offset)<25)        {Steer_kp=4;Steer_kd=5;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.5+4;Steer_kd=5;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.6+9;Steer_kd=5;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+15;Steer_kd=5;}
		else                            {Steer_kp=16;Steer_kd=5;}
		return;
	}
	else if(targetspeed<300)
	{
		if(ABS(target_offset)<25)        {Steer_kp=6;Steer_kd=3;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.2+6;Steer_kd=3;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.6+8;Steer_kd=3;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.2+14;Steer_kd=3;}
		else                            {Steer_kp=16;Steer_kd=3;}
		return;
	}
	else
	{
		if(ABS(target_offset)<25)        {Steer_kp=6;Steer_kd=5;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.2+6;Steer_kd=5;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.6+8;Steer_kd=5;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.2+14;Steer_kd=5;}
		else                            {Steer_kp=16;Steer_kd=5;}
		return;
	}
}
/*************************������ƣ�PD***********************/
void SteerControl(void)
{
	if(wrong_flag==1)
	{
		Bee=1;
		Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
		SET_steer(Steer_PWM[3]);
		//����ֵ
		Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
		return;
	}
	
	Bee=0;

	Steer_PWM[3] = CENTER-(Steer_kp-1)*target_offset-Steer_kd*(target_offset-last_offset); //λ��ʽPD

	if(Steer_PWM[3]>LEFT) Steer_PWM[3]=LEFT;
	else if(Steer_PWM[3]<RIGHT) Steer_PWM[3]=RIGHT;

	SET_steer(Steer_PWM[3]);
	
	//����ֵ��offsetֵ
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
