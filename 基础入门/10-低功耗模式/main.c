//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板低功耗模式设置实验程序                             //
//         设置MSP430F5438为LPM3模式，由WDT每4秒钟闪动发光二极管一次         //
//         将电流表串接于开发板J0，则可读出开发板的电流消耗                  //
//         此程序工作平均电流为5uA左右                                       //
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
   LPM3_EXIT                                     ; // 退出低功耗模式
}


int main( void )
{
  WDTCTL   = WDTPW + WDTHOLD                     ;
  P7SEL   |= 0x03                                ; // 使能XT1
  UCSCTL6 &=~XT1OFF                              ;  
  UCSCTL6 |= XCAP_3                              ; 
  UCSCTL4 |= SELA_0                              ; // ACLK时钟源选择XT1  
  UCSCTL5 |= DIVA1                               ; // ACLK = XT1/4
  P1DIR    = 0xFF                                ; // All P1.x outputs
  P1OUT    = 0x00                                ; // All P1.x reset
  P2DIR    = 0xFF                                ; // All P2.x outputs
  P2OUT    = 0x00                                ; // All P2.x reset
  P3DIR    = 0xFF                                ; // All P3.x outputs
  P3OUT    = 0x00                                ; // All P3.x reset
  P4DIR    = 0xFF                                ; // All P4.x outputs
  P4OUT    = 0x00                                ; // All P4.x reset
  P5DIR    = 0xFF                                ; // All P5.x outputs
  P5OUT    = 0x00                                ; // All P5.x reset
  P6DIR    = 0xFF                                ; // All P6.x outputs
  P6OUT    = 0x00                                ; // All P6.x reset
  P7DIR    = 0xFF                                ; // All P6.x outputs
  P7OUT    = 0x00                                ; // All P6.x reset
  P8DIR    = 0xFF                                ; // All P6.x outputs
  P8OUT    = 0xFF                                ; // All P6.x reset
  P9DIR    = 0xFF                                ; // All P6.x outputs
  P9OUT    = 0x00                                ; // All P6.x reset
  P10DIR   = 0xFF                                ; // All P6.x outputs
  P10OUT   = 0x00                                ; // All P6.x reset
  P11DIR   = 0xFF                                ; // All P6.x outputs
  P11OUT   = 0x00                                ; // All P6.x reset
  SFRIE1  |= WDTIE                               ; // 使能WDT中断
  WDTCTL   = WDT_ADLY_1000                       ; // 设置WDT为INTERVAL模式，中断间隔1000ms
  _EINT()                                        ;
  for(;;)
  {
    LPM3                                         ;
    _NOP()                                       ;    
    MAIN_POWER_ON                                ;
    __delay_cycles(100)                          ;
    MAIN_POWER_OFF                               ;
  }
}
