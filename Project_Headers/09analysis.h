/*
 * analysis.h
 *
 *  Created on: Feb 25, 2016
 *      Author: lenovo
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

extern int AA[128];
extern int BB[128];
extern int al_end,ar_end,bl_end,br_end;
extern int a_start,b_start;
extern int a_value,b_value;                          //�ж������صĲ�ֵ��׼
extern int a_T,b_T;                                  //�ڰ���ֵ
extern int al_count,ar_count,bl_count,br_count;  //�׵����
extern int al_flag,ar_flag,bl_flag,br_flag;//0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
extern int al_edge,ar_edge,bl_edge,br_edge;//������
extern int error;

extern void DataSet(void);
void PixelScan(void);
void ErrorCalculate(void);

#endif /* ANALYSIS_H_ */
