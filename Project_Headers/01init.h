/*
 * init.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef INIT_H_
#define INIT_H_

void initALL(void);
void disableWatchdog(void);				//关看门狗
void initModesAndClock(void);			//时钟初始化80M
void initEMIOS_0MotorAndSteer(void);    //PWM初始化:电机正反转E3、E4、E5、E6；舵机输出A11
void enableIrq(void);
void initLINFlex_0_UART(void);
void initAD(void);
void initTestIO(void);

#endif /* 01INIT_H_ */
