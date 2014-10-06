//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板MINI电子琴程序                                     //
//         由键盘端口KPC0~3输出低电平，KPR0~3设置为输入并内部上拉            //
//         当按键被按下时，单片机检测到KPR0~3的一路或多路被拉低，            //
//         则按被按下按键的行位置点亮相应的发光二极管                        //
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
#include "KEY.H"
#include "TONE.H"

#define FLL_FACTOR               649

unsigned int tone                                         ;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  P7OUT   ^= BUZZER                                       ;
  TA0CCR0  = tone                                         ;
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
  P7DIR  |= BUZZER                                        ;
  P7OUT  |= BUZZER                                        ;
  P1OUT   = 0xF0                                          ; // 设置LED控制端IO属性
  P1DIR   = 0xF0                                          ;
  P9OUT   = 0x0F                                          ; 
  P9DIR   = 0x0F                                          ;
}

//***************************************************************************//
//                                                                           //
//函数：Init_CLK(void)                                                       //
//功能：初始化主时钟: MCLK = XT2                                             //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P5SEL     |= 0x0C                                       ; // 端口功能选择振荡器
  UCSCTL6   &= ~XT2OFF                                    ; // 振荡器使能
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 驱动模式 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}

//***************************************************************************//
//                                                                           //
//  Init_TimerA0(void): 设置TimerA0                                              //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{
  TA0CTL   = 0                                               // 复位Timer0_A5, 分频系数设置为1
           | (1 << 2)                                        // 计数器清0
           | (2 << 8)                                     ; // 计数时钟设为SMCLK    
  TA0CCR0  =   600  - 1                                   ; // SMCK=EX2=16MHz，设置计数器溢出时间为1ms
  TA0CCTL0 = 0                                              // 初始化捕获控制
           | (1 << 4)                                     ; // 使能比较中断
  TA0CTL  |= (1 << 4)                                     ; // 设置计数器为加计数，启动
}


//***************************************************************************//
//                                                                           //
//                 主程序: 设置时钟并输出至P11.0~2                           //
//                                                                           //
//***************************************************************************//
void main( void )
{
  unsigned char key                                       ;
  WDTCTL = WDTPW + WDTHOLD                                ; // 关闭看门狗
  Init_CLK()                                              ;
  Init_Port()                                             ;  
  Init_KeyPad()                                           ;
  Init_Timer0_A5()                                        ;
  while(1)
  {
    key   = ReadKey()                                     ;
    switch(key)
    {
     case 1: case 2: case 3:case 4:
     case 5: case 6: case 7:case 8:
      {
        tone       = TONE[key]                            ;
        _EINT()                                           ;
        P1OUT     &=~0xF0                                 ; 
        P9OUT     &=~0x0F                                 ; 
      }
      break                                               ;
    default:
      _DINT()                                             ;
      P1OUT       |= 0xF0                                 ; 
      P9OUT       |= 0x0F                                 ; 
      break                                               ;      
    }
  }
}
