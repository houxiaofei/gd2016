/*
 * delay.c
 *
 *  Created on: Jan 23, 2016
 *      Author: lenovo
 */

#include "includes.h"

void udelay(int us)
{
   int udelay_i;
   for(udelay_i=0;udelay_i<us; udelay_i++)
   {
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop); //_asm nop 1个时钟周期     1us
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   }
}
