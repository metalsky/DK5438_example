//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机SPI端口实验程序                              //
//         设置SPI端口工作模式，通过SPI端口发送数据                          //
//         可在P3.1/P3.3端口观察数据、时钟波形                               //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PIN_DEF.H"

#define  FLL_FACTOR     649                                 // FLL_FACTOR: DCO倍频系数    
char  event, RXBuffer[2]                                  ;


//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
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
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志 
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
  INTERNAL_PULL_UP                                        ; // 使能键盘端口内部上拉电阻 
  ROW_IN_COL_OUT                                          ; // 设置行输入，列输出0
}

//***************************************************************************//
//                                                                           //
//  Init_SPI(void): 设置SPI端口                                              //
//                                                                           //
//***************************************************************************//
void Init_SPI(void)
{  
  P8OUT    |= LCD_NCS                                     ; // SPI端口复用，DisableTFT液晶端口
  P8DIR    |= LCD_NCS                                     ;
  P3OUT    |= NCS25                                       ; 
  P3DIR    |= NCS25                                       ;
  P3SEL    &= 0xF0                                        ;
  P3SEL    |= 0x0E                                        ; // // P3.1/2/3功能选择为SPI
  UCB0CTL1 |= UCSWRST                                     ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB                   ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                                    ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x02                                        ; // SCK = SMCK/2
  UCB0BR1   = 0                                           ; 
  UCB0CTL1 &=~UCSWRST                                     ; // SPI状态机使能
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                ;
  Init_CLK()                                              ;
  Init_Port()                                             ;
  Init_SPI()                                              ;
  for(;;)
  {
     UCB0TXBUF = 0x55                                     ; 
     __delay_cycles(280)                                  ; 
  }
}
