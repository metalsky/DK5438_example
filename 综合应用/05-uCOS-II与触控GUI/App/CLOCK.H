//===========================================================================//
//                                                                           //
// 文件：  CLOCK.H                                                           //
// 说明：  BW-DK5438开发板精美时钟程序头文件                                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//
#ifndef __CLOCK_H
#define __CLOCK_H

void Ini_Clock_Contex(void)                                  ;
void Gen_Hour_Hand(unsigned int angle)                       ; // 生成时针图形数据
void Save_Hour_Hand_BK(void)                                 ; // 备份时针遮挡的背景
void Draw_Hour_Hand(unsigned int color)                      ; // 画时针图形数据
void Restore_Hour_Hand_BK(void)                              ; // 恢复时针遮挡背景
void Gen_Minu_Hand(unsigned int angle)                       ; // 生成分针图形数据
void Save_Minu_Hand_BK(void)                                 ; // 备份分针遮挡的背景
void Draw_Minu_Hand(unsigned int color)                      ; // 画时分图形数据
void Restore_Minu_Hand_BK(void)                              ; // 恢复分针遮挡背景
void Gen_Second_Hand(unsigned int angle)                     ; // 生成秒针图形数据
void Save_Second_Hand_BK(void)                               ; // 备份秒针遮挡的背景
void Draw_Second_Hand(unsigned int color)                    ; // 画时秒针图形数据
void Restore_Second_Hand_BK(void)                            ; // 恢复秒针遮挡背景

#endif