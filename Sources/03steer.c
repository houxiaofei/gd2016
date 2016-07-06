/*
 * 03steer.c
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#include "includes.h"

/*************************舵机参数***************************/
int target_offset=0,last_offset=0;	//舵机偏差值记录
double Steer_kp=0,Steer_kd=0;//舵机P、D值
unsigned int RIGHT=2830;//3250//3742-3302打死
unsigned int LEFT=3630;//4182//3742-4162打死
unsigned int Steer_PWM[4]={0,0,0,CENTER};//舵机输出值记录

/*************************舵机接口函数***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[11].CBDR.R = steer;}
/*************************舵机PD参数设置***********************/
void Steer_PDSet(void)
{
	target_offset=error;
	if(targetspeed<100)//120
	{
		Steer_kp=8;Steer_kd=0;
		return;
	}
	else if(targetspeed<149)//120(150)-5 8 8 12 12 10//130-170
	{
		if(ABS(target_offset)<30)  {Steer_kp=2;Steer_kd=8;}
		else if(ABS(target_offset)<40)  {Steer_kp=(ABS(target_offset)-30)*0.3+2;Steer_kd=8;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-40)*0.3+5;Steer_kd=8;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+6.5;Steer_kd=8;}
		else                            {Steer_kp=7.5;Steer_kd=8;}
		return;
	}
	else if(targetspeed<250)//140-180//150-190//160 5 8 8 10 10 10
	{
//		if(enter_flag==1)
//		{
//			if(error<0)
//			{
//				if(ABS(target_offset)<10)  {Steer_kp=2;Steer_kd=8;}
//				else if(ABS(target_offset)<20)  {Steer_kp=(ABS(target_offset)-10)*0.25+2;Steer_kd=8;}
//				else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-20)*0.26+4.5;Steer_kd=8;}
//				else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.2+8.4;Steer_kd=8;}
//				else if(ABS(target_offset)<50)  {Steer_kp=-(ABS(target_offset)-45)*0.25+10.4;Steer_kd=8;}
//				else                            {Steer_kp=9.15;Steer_kd=8;}
//				return;
//			}
//			else
//			{
//				if(ABS(target_offset)<10)  {Steer_kp=2;Steer_kd=8;}
//				else if(ABS(target_offset)<20)  {Steer_kp=(ABS(target_offset)-10)*0.2+2;Steer_kd=8;}
//				else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-20)*0.26+4;Steer_kd=8;}
//				else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.2+7.9;Steer_kd=8;}
//				else if(ABS(target_offset)<50)  {Steer_kp=-(ABS(target_offset)-45)*0.25+9.9;Steer_kd=8;}
//				else                            {Steer_kp=8.65;Steer_kd=8;}
//				return;
//			}
//		}
//		else
//		{
//			if(error<0)
//			{
//				if(ABS(target_offset)<30)  {Steer_kp=4;Steer_kd=8;}
//				else if(ABS(target_offset)<40)  {Steer_kp=(ABS(target_offset)-30)*0.3+4;Steer_kd=8;}
//				else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-40)*0.3+7;Steer_kd=8;}
//				else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+8.5;Steer_kd=8;}
//				else                            {Steer_kp=9.5;Steer_kd=8;}
//				return;
//			}
//			else
//			{
//				if(ABS(target_offset)<30)  {Steer_kp=4;Steer_kd=8;}
//				else if(ABS(target_offset)<40)  {Steer_kp=(ABS(target_offset)-30)*0.2+4;Steer_kd=8;}
//				else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-40)*0.2+6;Steer_kd=8;}
//				else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.1+7;Steer_kd=8;}
//				else                            {Steer_kp=8;Steer_kd=8;}
//				return;
//			}
		if(ABS(target_offset)<25)        {Steer_kp=4;Steer_kd=10;}
		else if(ABS(target_offset)<35)  {Steer_kp=(ABS(target_offset)-25)*0.1+4;Steer_kd=10;}
		else if(ABS(target_offset)<45)  {Steer_kp=(ABS(target_offset)-35)*0.2+5;Steer_kd=10;}
		else if(ABS(target_offset)<55)  {Steer_kp=(ABS(target_offset)-45)*0.3+7;Steer_kd=10;}
		else                            {Steer_kp=10;Steer_kd=10;}
		return;
//		}
	}
	else if(targetspeed<280)
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
/*************************舵机控制，PD***********************/
void SteerControl(void)
{
	if(wrong_flag==1)
	{
		Bee=1;
		Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
		SET_steer(Steer_PWM[3]);
		//存舵机值
		Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
		return;
	}
	
	Bee=0;

	Steer_PWM[3] = CENTER-(Steer_kp)*target_offset-Steer_kd*(target_offset-last_offset); //位置式PD

	if(Steer_PWM[3]>LEFT) Steer_PWM[3]=LEFT;
	else if(Steer_PWM[3]<RIGHT) Steer_PWM[3]=RIGHT;

	SET_steer(Steer_PWM[3]);
	
	//存舵机值和offset值
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
