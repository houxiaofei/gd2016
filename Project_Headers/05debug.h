/*
 * debug.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef DEBUG_H_
#define DEBUG_H_

extern int Left[128];
extern int Right[128];
extern unsigned char *send;
extern unsigned char putstring[];
extern unsigned int Ts;
extern unsigned int Tc;
extern int CurrentSteer;
extern unsigned char RX_data;
extern unsigned char Y[25],Z[11];

extern int X[16];

extern unsigned char RX_data;
extern unsigned char Y[25],Z[11];

extern unsigned char S3_last;
extern unsigned char S4_last;
extern unsigned char S5_last;
extern unsigned char S6_last;
extern unsigned char keymode;

extern unsigned long time1;
extern unsigned long time2;
extern unsigned long time3;

void BlueTx(void); 
void LINFlex_TX(unsigned char data);
void LINFlex_TX_Interrupt(void);
extern void KeyJudge(void);
extern unsigned long TimeMesure(void);
void LINFlex_RX_Interrupt(void);

#endif /* 05DEBUG_H_ */
