#ifndef __INIT_H
#define __INIT_H

#include <msp430x54x.h>
#include "PIN_DEF.H"

#define ALL_ITEMS                   60
#define FLL_FACTOR                 649

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define Power  BIT0
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void Initport_LCD(void)        ;
extern void Init_LCD(void)            ;
extern void InitRF(void)              ;
extern void (*Menu_OP[1])(void)       ;
extern void GotoSleep(void)           ;// FOR DEBUG   


/*
void Init_MCLK(void)                  ;
void Init_timer(void)                 ;
void Init_ADC(void)                   ;
void Init_KeyPad(void)                ;
void Init_RTC(void)                   ;
void Init_Port(void)                  ;
*/
void Init_UART(void)                  ;
void Init_UART_115200(void)           ;
void BackLight()                      ;

#endif