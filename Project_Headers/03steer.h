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

void SET_steer(unsigned int steer);
extern void Steer_PDSet(void);
extern void SteerControl(void);

#endif /* 03STEER_H_ */
