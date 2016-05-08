/*
 * pit.h
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#ifndef PIT_H_
#define PIT_H_

void initPIT(void);
void PitISR(void);
void PitISR2(void);

extern unsigned int timecount;
extern unsigned int pitcount0,pitcount1,pitcount2,pitcount3,pitcount4,pitcount5;

#endif /* PIT_H_ */
