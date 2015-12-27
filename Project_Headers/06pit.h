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

extern unsigned int IOtest;

#endif /* PIT_H_ */
