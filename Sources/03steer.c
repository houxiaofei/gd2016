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
unsigned int Steer_PWM[4]={0,0,0,0};//舵机输出值记录

/*************************舵机接口函数***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[11].CBDR.R = steer;}
/*************************舵机控制，PD***********************/
void SteerControl(void)
{
	Steer_PWM[3]=CENTER+Steer_kp*target_offset+Steer_kd*(target_offset-last_offset);//位置式PD

	SET_steer(Steer_PWM[3]);
	
	//存舵机值和offset值
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}
