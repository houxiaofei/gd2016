/*
 * init.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef INIT_H_
#define INIT_H_

void initALL(void);
void disableWatchdog(void);				//�ؿ��Ź�
void initModesAndClock(void);			//ʱ�ӳ�ʼ��80M
void initEMIOS_0MotorAndSteer(void);    //PWM��ʼ��:�������תE3��E4��E5��E6��������A11
void enableIrq(void);
void initLINFlex_0_UART(void);
void initAD(void);
void initTestIO(void);

#endif /* 01INIT_H_ */
