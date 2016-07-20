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
extern int al_count,ar_count,bl_count,br_count;  //°×µã¼ÆÊý
extern int al_flag,ar_flag,bl_flag,br_flag,allflag,a_flag,b_flag;//0,1,2,3,4;ºÚ£¬°×£¬°×-ºÚ£¬ºÚ-°×£¬´íÎó
extern int aa_flag[4],all_flag[10];
extern int wrong_flag;
extern int stop_flag;
extern int al_edge,ar_edge,bl_edge,br_edge;//Ìø±äÑØ
extern int al_edge_left,al_edge_right,ar_edge_left,ar_edge_right;
extern int bl_edge_left,bl_edge_right,br_edge_left,br_edge_right;
extern int error,a_error,b_error,pre_error;
extern int b_value_end,b_scan_end;//ÖÕµã
extern int a_edg_err,a_bar_value,a_bar_cnt,a_bar_flag,a_bar_value2,al_bar_flag,ar_bar_flag;//ÕÏ°­Îï
extern int b_bar_value,b_bar_cnttop,ab_difference,ab_difference_value;//ÕÏ°­Îï
extern int his_num,trend,trend_value,trend_value2,b_error_value,b_error_value2,enter_flag;//ÈëÍäÅÐ¶Ï
extern int barleft_kp,barright_kp;

extern int cl_flag,cr_flag,cl_edge,cr_edge;

extern int straight_flag,trans_enter_flag,trans_out_flag,turn_flag;

extern int bar_flag,bar_edge[10],bar_left_flag,bar_right_flag;
extern int a_bar_flag2,al_bar_flag2,ar_bar_flag2;

int AverageCalculate(int a, int b, int c[]);
int EdgeCalculate(int a, int b, int c[],int avg);
extern void DataSet(void);
extern void PixelScan_B(void);
extern void PixelScan_A(void);
extern void PixelScan_C(void);
extern void ErrorCalculate(void);
extern void ErrorCalculate_A(void);
void SpeedFlagJudge_C(void);
void TrendCalculate(void);
void EnterJudge(void);
void EndJudge(void);
void BarrierJudge(void);
void BarrierControl(void);

void BarrierJudgeNew(void);
void BarrierControlNew(void);

#endif /* ANALYSIS_NEW_H_ */
