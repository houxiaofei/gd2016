/*
 * speed.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef SPEED_H_
#define SPEED_H_

void SET_motor(int leftSpeed,int rightSpeed);
void SpeedControl(void);
void Speed_PID(void);
void DifferSpeed_PID(void);
void Speed_Set(void);
void Speed_PID2(void);

extern int csl,csr;
extern int tsl,tsr;
extern int csl_cnt[3],csr_cnt[3];
extern int targetspeed;
extern int straightspeed,transspeed,turnspeed,deadspeed,barspeed;
extern int tsl_PWM,tsr_PWM,tsr_Delta,error_Delta;
extern int ErrorRight,PreErrorRight;
extern signed int Speed_kc1;

extern float Speed_kp_Left,Speed_ki_Left,Speed_kd_Left,csxs;
extern float Speed_kp_Right,Speed_ki_Right,Speed_kd_Right;	//µç»úPID

extern float KP_speed,KI_speed,KD_speed;
extern float KP_DifSpd,KI_DifSpd,KD_DifSpd;

#endif /* 02SPEED_H_ */
