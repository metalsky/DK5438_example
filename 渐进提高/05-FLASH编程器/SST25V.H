//===========================================================================//
//                                                                           //
// 文件：  SST25V.H                                                          //
// 说明：  BW-DK5438开发板SST25V016B操作函数头文件                           //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.23                                                        //
// 备注：  SST25V016B内预置多个中/英文字库，编程时请谨慎处理                 //
//                                                                           //
//===========================================================================//

#ifndef _SST25V_H
#define _SST25V_H

#include "PIN_DEF.H"
#include "msp430x54x.h"
#define   USER_DATA         0x199000                      // 用户数据区

void Init_SSTSPI(void)                                   ;
void Read_JEDECID(char *ID)                              ;
void ReadFlash(long addr,int length,char *Buffer)        ;
void Sector_Erase(unsigned long Dst)                     ;
void Byte_Program(unsigned long Dst, unsigned char byte) ; 
unsigned char Read_Status_Register(void)                 ;
void WRSR(unsigned char byte)                            ;
void WREN(void)                                          ;
void WRDI(void)                                          ;
void Wait_Busy(void)                                     ;
void EWSR(void)                                          ;

#endif

