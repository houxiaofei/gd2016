/*
 * capture.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#define CCDL_SI  SIU.GPDO[10].R   //���������Դ������Ķ˿� SI  A10
#define CCDL_CLK SIU.GPDO[11].R   //���������Դ������Ķ˿� CLK A11

#define CCDM_SI  SIU.GPDO[5].R   //���������Դ������Ķ˿� SI  A5
#define CCDM_CLK SIU.GPDO[76].R   //���������Դ������Ķ˿� CLK E12

#define CCDR_SI  SIU.GPDO[66].R   //���������Դ������Ķ˿� SI  E2
#define CCDR_CLK SIU.GPDO[6].R    //���������Դ������Ķ˿� CLK A6

extern void ImageCapture(int L[],int M[],int R[]);
extern void ImageCopy(int L[],int PL[]);
extern void SpeedCount(void);

extern unsigned int speedcounter1,speedcounter2,speedcounter3,speedcounter4;

#endif /* CAPTURE_H_ */
