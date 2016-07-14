/*
 * analysis_new.h
 *
 *  Created on: Jul 9, 2016
 *      Author: lenovo
 */

#ifndef ANALYSIS_NEW_H_
#define ANALYSIS_NEW_H_

extern int A[128];
extern int B[128];
extern int C[128];
extern int a_T,b_T,c_T; 
extern int al_count,ar_count,bl_count,br_count;  //�׵����
extern int al_flag,ar_flag,bl_flag,br_flag,allflag,a_flag,b_flag;//0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
extern int aa_flag[4],all_flag[10];
extern int wrong_flag;
extern int stop_flag;
extern int al_edge,ar_edge,bl_edge,br_edge;//������
extern int al_edge_left,al_edge_right,ar_edge_left,ar_edge_right;
extern int bl_edge_left,bl_edge_right,br_edge_left,br_edge_right;
extern int error,a_error,b_error,pre_error;
extern int b_value2,b_scan2,b_cnt;//�յ�
extern int a_edg_err,a_bar_value,a_bar_cnt,a_bar_flag,a_bar_value2,al_bar_flag,ar_bar_flag;//�ϰ���
extern int b_bar_value,b_bar_cnttop;
extern int his_num,trend,trend_value,trend_value2,b_error_value,b_error_value2,enter_flag;

int AverageCalculate(int a, int b, int c[]);
int EdgeCalculate(int a, int b, int c[],int avg);
extern void DataSet(void);
extern void PixelScan_B(void);
extern void PixelScan_A(void);
extern void PixelScan_C(void);
extern void ErrorCalculate(void);
extern void ErrorCalculate_A(void);
void TrendCalculate(void);
void EnterJudge(void);
void EndJudge(void);
void BarrierJudge(void);
void BarrierControl(void);

#endif /* ANALYSIS_NEW_H_ */
