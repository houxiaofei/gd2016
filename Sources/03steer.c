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
unsigned int RIGHT=3250;
unsigned int LEFT=4182;
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
	else if(targetspeed<130)//120(150)-5 8 8 12 12 10//130-170
	{
		if(ABS(target_offset)<5)        {Steer_kp=5;Steer_kd=0;}
		else if(ABS(target_offset)<10)  {Steer_kp=5;Steer_kd=0;}
		else if(ABS(target_offset)<30)  {Steer_kp=5;Steer_kd=0;}
		else if(ABS(target_offset)<35)  {Steer_kp=8;Steer_kd=0;}
		else if(ABS(target_offset)<40)  {Steer_kp=8;Steer_kd=0;}
		else							{Steer_kp=5;Steer_kd=0;}
		return;
	}
	else if(targetspeed<200)//140-180//150-190//160 5 8 8 10 10 10
	{
		if(ABS(target_offset)<20)        {Steer_kp=6;Steer_kd=5;}
		else if(ABS(target_offset)<30)  {Steer_kp=(ABS(target_offset)-20)*0.2+6;Steer_kd=5;}
		else if(ABS(target_offset)<40)  {Steer_kp=(ABS(target_offset)-30)*0.3+8;Steer_kd=5;}
		else if(ABS(target_offset)<50)  {Steer_kp=(ABS(target_offset)-40)*0.1+11;Steer_kd=5;}
		else                            {Steer_kp=12;Steer_kd=5;}
//		
//		if(ABS(target_offset)<5)        {Steer_kp=5;Steer_kd=5;}
//		else if(ABS(target_offset)<10)  {Steer_kp=8;Steer_kd=5;}
//		else if(ABS(target_offset)<30)  {Steer_kp=8;Steer_kd=5;}
//		else if(ABS(target_offset)<35)  {Steer_kp=11;Steer_kd=5;}
//		else if(ABS(target_offset)<40)  {Steer_kp=10;Steer_kd=5;}
//		else                            {Steer_kp=10;Steer_kd=5;}
		return;
	}
	else if(targetspeed<250)
	{
		if(ABS(target_offset)<20)        {Steer_kp=8;Steer_kd=5;}
		else if(ABS(target_offset)<30)  {Steer_kp=(ABS(target_offset)-20)*0.1+8;Steer_kd=5;}
		else if(ABS(target_offset)<40)  {Steer_kp=(ABS(target_offset)-30)*0.3+9;Steer_kd=5;}
		else if(ABS(target_offset)<50)  {Steer_kp=(ABS(target_offset)-40)*0.1+12;Steer_kd=5;}
		else                            {Steer_kp=13;Steer_kd=5;}
		
//		if(ABS(target_offset)<20)        {Steer_kp=5;Steer_kd=0.8;}
//		else if(ABS(target_offset)<40)  {Steer_kp=ABS(target_offset)*0.25;Steer_kd=1;}
//		else if(ABS(target_offset)<40)  {Steer_kp=8;Steer_kd=2;}
//		else if(ABS(target_offset)<46)  {Steer_kp=10;Steer_kd=3;}
//		else if(ABS(target_offset)<50)  {Steer_kp=10;Steer_kd=3;}
//		else                            {Steer_kp=10;Steer_kd=2;}
		return;
	}
	else
	{
		if(ABS(target_offset)<5)        {Steer_kp=8;Steer_kd=0;}
		else if(ABS(target_offset)<10)  {Steer_kp=10;Steer_kd=0;}
		else if(ABS(target_offset)<20)  {Steer_kp=12;Steer_kd=0;}
		else if(ABS(target_offset)<30)  {Steer_kp=13;Steer_kd=0;}
		else if(ABS(target_offset)<40)  {Steer_kp=14;Steer_kd=0;}
		else                            {Steer_kp=15;Steer_kd=0;}	
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

	Steer_PWM[3] = CENTER-(Steer_kp+5)*target_offset-Steer_kd*(target_offset-last_offset); //位置式PD

	if(Steer_PWM[3]>LEFT) Steer_PWM[3]=LEFT;
	else if(Steer_PWM[3]<RIGHT) Steer_PWM[3]=RIGHT;

	SET_steer(Steer_PWM[3]);
	
	//存舵机值和offset值
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
