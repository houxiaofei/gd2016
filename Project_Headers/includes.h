/*
 * includes.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include "MPC5604B.h"
#include "01init.h"
#include "02speed.h"
#include "03steer.h"
#include "04capture.h"
#include "05debug.h"
#include "06pit.h"
#include "07test.h"
#include "08sci.h"
#include "09analysis.h"
#include "delay.h"
#include "IntcInterrupts.h"
#include "string.h"

#define ABS(x)              ((x) > 0 ? (x) : 0-(x))

extern int PixelLeft[128];
extern int PixelRight[128];

/***************foreward*****************/
#define forewardleft SIU.GPDI[59].R	//����⳵ʵ�ʷ���:1��ǰ�� 0����
#define backwardright SIU.GPDI[6].R	//����⳵ʵ�ʷ���:1�ҵ��� 0��ǰ��

/***************bee*****************/
#define Bee  SIU.GPDO[0].R

/***************switch*****************/
#define switch1 SIU.GPDI[50].R
#define switch2 SIU.GPDI[52].R
#define switch3 SIU.GPDI[54].R	//E0
#define switch4 SIU.GPDI[56].R
/***************keys*****************/
#define S6 SIU.GPDI[28].R
#define S5 SIU.GPDI[29].R
#define S4 SIU.GPDI[30].R	//E0
#define S3 SIU.GPDI[31].R

/***************steer*****************/
#define TargetSteer EMIOS_0.CH[11].CBDR.R // ���ֵ
#define CENTER 3692//3720

/***************stm*****************/
#define TIME STM.CNT.R


#endif /* INCLUDES_H_ */
