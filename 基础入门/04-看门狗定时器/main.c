//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板看门狗定时器设置实验程序                           //
//         设置看门狗定时器每秒中断一次，翻转LED驱动端输出，                 //
//         控制LED以每2秒一次闪烁                                            //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  P1OUT   ^= 0xF0                                         ; 
  P9OUT   ^= 0x0F                                         ; 
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): 设置IO端口                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR  |= POWER                                         ; // 主电源
  MAIN_POWER_ON                                           ;
  P7DIR  |= LED_PWR                                       ; // 发光二极管电源
  P7OUT  &=~LED_PWR                                       ;
  P1OUT   = 0xF0                                          ; // 设置LED控制端IO属性
  P1DIR   = 0xF0                                          ;
  P9OUT   = 0x0F                                          ; 
  P9DIR   = 0x0F                                          ;
}


int main( void )
{
  WDTCTL   = WDTPW + WDTHOLD                              ;
  Init_Port()                                             ;
  P7SEL   |= 0x03                                         ; // 使能XT1
  UCSCTL6 &=~XT1OFF                                       ;  
  UCSCTL6 |= XCAP_3                                       ; 
  UCSCTL4 |= SELA_0                                       ; // ACLK时钟源选择XT1  
  SFRIE1  |= WDTIE                                        ; // 使能WDT中断
  WDTCTL   = WDT_ADLY_1000                                ; // 设置WDT为INTERVAL模式，中断间隔1000ms
  _EINT()                                                 ;
  LPM3                                                    ;
}
