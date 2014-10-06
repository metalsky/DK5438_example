//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板电子时钟程序                                       //
//         在数码管上显示RTC时钟，亮度依次递增                               //
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
#include "DS18B20-BK.H"
#include "LED.H"
#include "RTC.H"

#define FLL_FACTOR 649

unsigned char SMG[10]=
{
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF)       ,
  ~(SEGB+SEGC)                           ,
  ~(SEGA+SEGB+SEGD+SEGE+SEGG)            ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGG)            ,
  ~(SEGB+SEGC+SEGF+SEGG)                 ,
  ~(SEGA+SEGC+SEGD+SEGF+SEGG)            ,
  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG)       ,
  ~(SEGA+SEGB+SEGC)                      ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF+SEGG)  ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG)       ,
}                                                    ;

LED_STRUCT LED[8]                                    ;

//***************************************************************************//
//                                                                           //
//看门狗中断服务程序，刷新显示数据                                           //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  LPM3_EXIT                                          ;
}

//***************************************************************************//
//                                                                           //
//    TIMERB1中断服务程序，数码管显示                                        //
//                                                                           //
//***************************************************************************//
#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
  static unsigned char on_off         = 1            ;
  static unsigned char digit          = 1            ; 
  static unsigned int  flicker_timer  = 0            ;
  P8OUT           = LED[digit-1].data                ;  
  switch(digit)
  {
  case 1: case 2: case 3: case 4:
    MASK_DIGITS                                      ;
    if(LED[digit-1].mode&DIGIT_FLICKER)
      if(flicker_timer>256) 
        break                                        ;
    if(LED[digit-1].mode&DIGIT_OFF)
      break                                          ;
    if(on_off)
    {
      LOW_DIGITS_OUT(~(0x01<<(digit-1)))             ;
      P8OUT           = LED[digit-1].data            ;        
    }      
    break                                            ;
  case 5: case 6: case 7: case 8:
    MASK_DIGITS                                      ;
    if(LED[digit-1].mode&DIGIT_FLICKER)
      if(flicker_timer>256) 
        break                                        ;
    if(LED[digit-1].mode&DIGIT_OFF)
      break                                          ;
    if(on_off)
    {
      HIGH_DIGITS_OUT(~(0x10<<(digit-5)))            ;
      P8OUT           = LED[digit-1].data            ;        
    }
    break                                            ;
  }
  
  if(on_off) 
    if(++flicker_timer>=512) 
      flicker_timer = 0                              ; 
  if(on_off)       
    TBCCR0          = LED[digit-1].lightness         ; // 亮度调节
  else
    TBCCR0          = 64 - LED[digit-1].lightness    ; 
  if(on_off)
    if(++digit>8)     
      digit         = 1                              ;
  on_off           ^= 0x01                           ;
  TBCTL            &= ~TBIFG                         ;
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

void DisplayTime(void)
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
  LED_Disp_Int(time,FIT_ZERO)                        ;
  LED[2].data = MINUS                                ;  
  LED[5].data = MINUS                                ;     
}


void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ; 
  Init_RTC()                                         ;
  Ini_LED(8)                                         ;  
  LED_Flicker_Digit(8)                               ; // 仅为演示
  OW_DIO_PULLUP                                      ;  
  SFRIE1  |= WDTIE                                   ; // 使能WDT中断
  WDTCTL   = WDT_ADLY_1000                           ; // 设置WDT为INTERVAL模式，中断间隔100ms
  MAIN_POWER_ON                                      ;
  TBCTL    = TBSSEL_1 + MC_1 + TBCLR + TBIE          ; // 时钟源   = ACLK
  TBCCR0   = 64                                      ; // 刷新频率 = 32768/64/8=64Hz  
  _EINT()                                            ;
  for(;;)
  {
    DisplayTime()                                    ;   
    LPM3                                             ;
    _NOP()                                           ;
  }
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  RTCCTL01  &=~(RTCRDYIFG + RTCTEVIFG)               ;
}

