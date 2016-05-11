/*
 * speed.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef SPEED_H_
#define SPEED_H_

void SET_motor(int leftSpeed,int rightSpeed);
void SpeedControl(void);

extern int csl,csr;
extern int tsl,tsr;
extern int targetspeed;

extern double Speed_kp_Left,Speed_ki_Left,Speed_kd_Left,csxs;
extern double Speed_kp_Right,Speed_ki_Right,Speed_kd_Right;	//µç»úPID

#endif /* 02SPEED_H_ */
