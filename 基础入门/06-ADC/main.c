//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板ADC实验程序                                        //
//         设置ADC12，由看门狗中断程序定时检测主电源电压并做平均处理         //
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
#define FLL_FACTOR                    649

float     Vmain = 0                                       ; // 主电源电压


//***************************************************************************//
//                                                                           //
//函数：void Init_CLK(void)                                                  //
//说明：初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                             //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                       ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                     ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                     ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                     ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                 ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)       ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &=~OFIFG                                      ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志 
}

//***************************************************************************//
//                                                                           //
//函数：void Init_ADC(void)                                                  //
//说明：初始化ADC12                                                          //
//                                                                           //
//***************************************************************************//
void Init_ADC(void)
{     //Vbat
  //VbatCON P6.3
  P6DIR   |=  BIT3                               ; //  P6.3 outputs
  P6OUT   |=  BIT3                               ; //  P6.3 set
  //P6OUT   &= ~BIT3                               ; //  P6.3 reset
  //Vbat P6.4
  P6DIR   &= ~BIT4                               ; // P6.4 Input   
  P6SEL   |=  BIT4                               ; // Enable A/D channel A0

  //P7SEL      |= TVBTM                                     ; // Enable A/D channel A0
  ADC12CTL0   = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V; // 打开ADC12, 采样时钟源为内部发生器，参考源为内部2.5V
  ADC12CTL1   = ADC12SHP                                  ; // 打开内部采样时钟
  ADC12MCTL0  = ADC12SREF_1+ ADC12INCH_4                 ; // Vr+=Vref+ and Vr-=AVss
  __delay_cycles(20000000)                                ; // Delay for reference start-up
  ADC12CTL0  |= ADC12ENC                                  ; // Enable conversions
}

//***************************************************************************//
//                                                                           //
//函数：void VmainMon(void)                                                  //
//说明：检测主电源电压，需做偏差补偿和平均处理                               //
//                                                                           //
//***************************************************************************//
void VmainMon(void)
{
  ADC12CTL0   |= ADC12SC                                  ; 
  while (!(ADC12IFG & BIT0))                              ;
  Vmain     +=(ADC12MEM0*5.0/4095-0.03)                   ; 
  //Vmain        = 0                                      ;
}

void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                              ;
  Init_CLK()                                              ;  
  Init_ADC()                                              ;
  SFRIE1  |= WDTIE                                        ; // 使能WDT中断
  WDTCTL   = WDT_ADLY_16                                  ; // 设置WDT为INTERVAL模式，中断间隔100ms
  MAIN_POWER_ON                                           ;
  _EINT()                                                 ;
  LPM3                                                    ;
}

//***************************************************************************//
//                                                                           //
//看门狗中断服务程序，检测主电池电压并做平均处理                             //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  static unsigned char i = 0                              ;
  VmainMon()                                              ;
  if(++i==1)
  {
    Vmain     /= 1                                       ;
    i          = 0                                        ;
    Vmain        = 0                                      ;
  }
}

