/*
 * sci.h
 *
 *  Created on: Jan 23, 2016
 *      Author: lenovo
 */

#ifndef SCI_H_
#define SCI_H_

unsigned char SendHexHigh(unsigned char hex);
unsigned char SendHexLow(unsigned char hex);
unsigned char SendInt1(int currentdata);
unsigned char SendInt2(int currentdata);
unsigned char SendInt3(int currentdata);
unsigned char SendInt4(int currentdata);

#endif /* SCI_H_ */
