//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板RTC实验程序                                        //
//         设置RTC时钟，通过看门狗中断定时查看RTC运行情况                    //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "RTC.H"

#define FLL_FACTOR 649

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  RTCCTL01  &=~(RTCRDYIFG + RTCTEVIFG)               ;
}

//***************************************************************************//
//                                                                           //
//看门狗中断服务程序，令单片机退出低功耗模式                                 //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  LPM3_EXIT                                          ;
}



//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                  ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                              ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                            ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                             ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                  ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                            ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                            ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)  ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &=~OFIFG                                 ; 
  }while(SFRIFG1&OFIFG)                              ; // 检测振荡器错误标志 
}

//***************************************************************************//
//                                                                           //
//                 初始化RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // 时钟模式,每分钟中断, BCD格式
  SetRTCYEAR(2010)                                   ; 
  SetRTCMON(9)                                       ;
  SetRTCDAY(19)                                      ;
  SetRTCDOW(7)                                       ;
  SetRTCHOUR(17)                                     ;
  SetRTCMIN(2)                                       ;
  SetRTCSEC(35)                                      ;
}

//***************************************************************************//
//                                                                           //
//                 读取RTC时钟                                               //
//                                                                           //
//***************************************************************************//
unsigned long GetTime(void)
{
  int hour,minute,second                             ;
  unsigned long time                                 ;
  for(;RTCCTL01&RTCRDY;)        _NOP()               ; // 等待RTC有效                                
  for(;!(RTCCTL01&RTCRDY);)     _NOP()               ;                                     
  hour    = GetRTCHOUR()                             ;
  minute  = GetRTCMIN()                              ;
  second  = GetRTCSEC()                              ;
  time    = (unsigned long)hour*1000000
           +(unsigned long)minute*1000
           +(unsigned long)second                    ;
  return time                                        ;
}


void main(void)
{
  unsigned long time                                 ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ; 
  Init_RTC()                                         ;
  SFRIE1  |= WDTIE                                   ; // 使能WDT中断
  WDTCTL   = WDT_ADLY_1000                           ; // 设置WDT为INTERVAL模式，中断间隔100ms
  MAIN_POWER_ON                                      ;
  _EINT()                                            ;
  for(;;)
  {
    time = GetTime()                                 ;   
    LPM3                                             ;
    _NOP()                                           ;
  }
}

