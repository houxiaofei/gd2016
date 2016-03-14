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

#endif /* 02SPEED_H_ */
