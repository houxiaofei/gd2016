/*
 * pit.h
 *
 *  Created on: Dec 27, 2015
 *      Author: lenovo
 */

#ifndef PIT_H_
#define PIT_H_

void initPIT(void);
void initPIT2(void);
void PitISR(void);
void PitISR2(void);

extern unsigned int timecount;
extern unsigned int pitcount0,pitcount1,pitcount2,pitcount3,pitcount4,pitcount5;
extern unsigned int steer_flag,oled_flag,start_flag;
extern unsigned int end_judge_flag;

#endif /* PIT_H_ */
