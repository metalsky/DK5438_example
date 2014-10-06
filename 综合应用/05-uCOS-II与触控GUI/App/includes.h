//===========================================================================//
//                                                                           //
// �ļ���  INCLUDES.H                                                        //
// ˵����  BW-DK5438������uC/OS-II��ʾ����ȫ��ͷ�ļ�                         //
// ���룺  IAR Embedded Workbench IDE for msp430 v5.20                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdarg.h>
#include    <string.h>
#include    <lib_def.h>
#include    <lib_mem.h>
#include    <lib_str.h>
#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include    <app_cfg.h>
#include    <cpu.h>
#include    <os_cpu.h>
#include    <os_cfg.h>
#include    <bsp.h>
#include    <ucos_ii.h>
#include    <LED.H>
#include    <enc28j60.h>
#include    <TOUCH.H>
#include    <GUI.H>
#include    <KEY.H>
#include    <math.h>
#include    <CLOCK.H>
#include    <RTC.H>
#include    <TFTDRV.H>
#endif
