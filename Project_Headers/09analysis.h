/*
 * analysis.h
 *
 *  Created on: Feb 25, 2016
 *      Author: lenovo
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

extern int A[128];
extern int B[128];
extern int al_end,ar_end,bl_end,br_end;
extern int a_start,b_start;
extern int a_value,b_value;                          //�ж������صĲ�ֵ��׼
extern int a_T,b_T;                                  //�ڰ���ֵ
extern int al_count,ar_count,bl_count,br_count;  //�׵����
extern int al_flag,ar_flag,bl_flag,br_flag,allflag;//0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
extern int b_value2,b_scan2,b_cnt;
extern int wrong_flag;
extern int stop_flag;
extern int al_edge,ar_edge,bl_edge,br_edge;//������
extern int error,a_error,b_error;

extern void DataSet(void);
extern void PixelScan(void);
extern void PixelScan_A(void);
extern void ErrorCalculate(void);
extern void ErrorCalculate_A(void);
extern void ErrorCalculate_B(void);
void EndJudge(void);

#endif /* ANALYSIS_H_ */
