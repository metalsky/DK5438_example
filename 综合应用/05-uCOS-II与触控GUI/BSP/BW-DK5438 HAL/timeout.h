#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include <stdlib.h>
#include <stdio.h>


/* vim: set sw=8 ts=8 si et: */
#ifndef TOUT_H
#define TOUT_H

void delay_ms(unsigned char ms);
void wd_init(void);
void wd_kick(void);

#endif /* TOUT_H */
