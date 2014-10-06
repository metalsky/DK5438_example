//===========================================================================//
//                                                                           //
// 文件：  LED.H                                                             //
// 说明：  BW-DK5438开发板数码管显示函数库头文件, 支持uC/OS-II               //
// 编译：  IAR Embedded Workbench IDE for msp430 v5.20                       //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef __LED_H
#define __LED_H

#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "string.h"
#include "math.h"
#include <ucos_ii.h>

//数码管位显示模式常量定义
#define  DIGIT_OFF       BIT0
#define  DIGIT_FLICKER   BIT1

//整数显示模式常量定义
#define  FIT_ZERO        BIT0
#define  NO_ZERO         0x00

//负号常量
#define  MINUS          ~SEGG

//小数点常量
#define  DPOINT         ~SEGDP

//数码管显示内容、模式定义
typedef struct
{
  unsigned char data                                       ; // 显示数据
  unsigned char mode                                       ; // 显示模式
  unsigned char lightness                                  ; // 显示亮度
}LED_STRUCT                                                ;

//外部变量
extern unsigned char SMG[10]                               ;
extern LED_STRUCT LED[8]                                   ;
extern INT32U exe_time                                     ;

//函数定义
void LED_Disp_Int(long iValue,unsigned char mode)          ;
void LED_Disp_Float(double fValue,
                    unsigned char dpLength,
                    unsigned char mode         )           ;
void LED_Disp_CPUUsage(long iValue)                        ;
void Clear_LED(void)                                       ;
void LED_Hide_Digit(unsigned char digit)                   ;
void LED_Flicker_Digit(unsigned char digit,unsigned char mode);
void Ini_LED(unsigned char lightness)                      ;
void Seg7LedRefresh(void)                                  ;

#endif