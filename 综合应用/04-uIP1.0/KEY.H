//===========================================================================//
//                                                                           //
// 文件：  KEY.H                                                             //
// 说明：  BW-DK5438开发板键盘处理函数头文件                                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef __KEY_H
#define __KEY_H

#include "msp430x54x.h"
#include "PIN_DEF.H"

void Init_KeyPad(void)                          ;
unsigned char ReadKey(void)                     ;
unsigned char GetKeyPress(void)                 ;

#endif