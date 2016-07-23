/*
 * steer.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef STEER_H_
#define STEER_H_

extern int target_offset,last_offset;
extern unsigned int Steer_PWM[4];
extern double Steer_kp,Steer_kd;
extern unsigned int LEFT,RIGHT;
extern unsigned char sp_x2,sp_x3;
extern double sp_x1;
extern int steer_amb_value;

void SET_steer(unsigned int steer);
extern void Steer_PDSet(void);
extern void SteerControl(void);

#endif /* 03STEER_H_ */
