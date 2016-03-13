/*
 * capture.h
 *
 *  Created on: Dec 26, 2015
 *      Author: lenovo
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#define CCDL_SI  SIU.GPDO[61].R   //定义左线性传感器的端口 SI  D13
#define CCDL_CLK SIU.GPDO[27].R   //定义左线性传感器的端口 CLK B11

#define CCDM_SI  SIU.GPDO[2].R   //定义中线性传感器的端口 SI  A2
#define CCDM_CLK SIU.GPDO[46].R   //定义中线性传感器的端口 CLK C14

#define CCDR_SI  SIU.GPDO[62].R   //定义右线性传感器的端口 SI  D14
#define CCDR_CLK SIU.GPDO[63].R   //定义右线性传感器的端口 CLK D15

extern void ImageCapture(int L[],int R[]);
extern void ImageCopy(int L[],int PL[]);
extern void SpeedCount(void);

extern unsigned int speedcounter1,speedcounter2,speedcounter3,speedcounter4;

#endif /* CAPTURE_H_ */
