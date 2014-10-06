/* vim: set sw=8 ts=8 si et: */
/***********************************************
* Author: Guido Socher
* Copyright: GPL V2
**********************************************/
#include "timeout.h"
void delay_ms(unsigned char ms)
{
  unsigned char i                    ;
  for(i=0;i<ms;i++)
    __delay_cycles(20000)            ;
}
