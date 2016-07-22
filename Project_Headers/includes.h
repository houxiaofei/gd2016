/*
 * includes.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#ifndef _TYPEDEF_SBYTE_
#define _TYPEDEF_SBYTE_
typedef signed char SBYTE;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_SWORD_
#define _TYPEDEF_SWORD_
typedef signed short SWORD;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
//typedef unsigned int DWORD;
typedef unsigned long int DWORD;
#endif

#include "MPC5604B.h"
#include "01init.h"
#include "02speed.h"
#include "03steer.h"
#include "04capture.h"
#include "05debug.h"
#include "06pit.h"
#include "08sci.h"
//#include "09analysis.h"
#include "analysis_new.h"
#include "delay.h"
#include "IntcInterrupts.h"
#include "string.h"
#include "oled.h"

#define ABS(x)              ((x) > 0 ? (x) : 0-(x))

extern int PixelLeft[128];
extern int PixelRight[128];

/***************foreward*****************/
#define forwardleft SIU.GPDI[41].R	//光编检测车实际方向:1左前进 0左倒退
#define forwardright SIU.GPDI[47].R	//光编检测车实际方向:1右倒退 0右前进


/***************switch*****************/
#define switch1 SIU.GPDI[16].R
#define switch2 SIU.GPDI[38].R
#define switch3 SIU.GPDI[43].R
#define switch4 SIU.GPDI[65].R
#define switch5 SIU.GPDI[1].R
/***************keys*****************/
#define S6 SIU.GPDI[13].R
#define S5 SIU.GPDI[14].R
#define S4 SIU.GPDI[39].R
#define S3 SIU.GPDI[15].R

/***************steer*****************/
#define TargetSteer EMIOS_0.CH[4].CBDR.R // 舵机值
#define CENTER 3651//3692 3710 3664 3704 3756新车3278 老车3712   3520   校内赛主车3500

/***************stm*****************/
#define TIME STM.CNT.R


#endif /* INCLUDES_H_ */
