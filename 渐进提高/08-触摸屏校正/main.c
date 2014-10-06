//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板触摸屏校正程序                                     //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//
#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PIN_DEF.H"
#include "TFTDRV.H"
#include "TOUCH.H"

#define  FLL_FACTOR     649                                          // FLL_FACTOR: DCO倍频系数   
//--------------------------- LCD 颜色 ----------------------//
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Grey2          0xF79E
#define Dark_Grey      0x6B4D
#define Dark_Grey2     0x52AA
#define Light_Grey     0xE71C
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned Color,Color_BK                                            ;

float cx[1]={0.0670567926},\
      cy[1]={0.0900232554},\
      ox[1]={-18.3293762 },\
      oy[1]={-18.2325592 }                                         ;

//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                                     ; // 关看门狗
  P7SEL     |= 0x03                                                ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                              ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                              ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                            ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                              ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                          ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                              ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                           ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                                ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                          ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                          ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG)         ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &= ~OFIFG                                              ; 
  }while(SFRIFG1&OFIFG)                                            ; // 检测振荡器错误标志 
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): 设置IO端口                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR  |= POWER                                                  ; // 主电源
  MAIN_POWER_ON                                                    ;
}

//***************************************************************************//
//                                                                           //
//  TP_Adj(void): 触摸屏校正程序                                             //
//                                                                           //
//***************************************************************************//
void TP_Adj(void)
{
  unsigned int temp[8]                                             ;
  Color     = Green                                                ;
  Color_BK  = Black                                                ;
  TP_Init()                                                        ;
  Clear_LCD(Color_BK)                                              ; 
  PutStringCN24(58,120,"触摸屏校正")                               ;
  Color     = Cyan                                                 ;
  PutStringCN16(30,165,"请用触控笔点测试点中心")                   ;
  for(;;)
  {
    Draw_Test_Point(20,20,Yellow)                                  ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[0],&temp[1]))  break                  ;
    }
    DrawRectFill(0,0,40,40,Color_BK)                               ;
    Wait_Pen_Up()                                                  ;
  
    Draw_Test_Point(220,20,Yellow)                                 ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[2],&temp[3]))  break                  ;
    }
    DrawRectFill(200,0,40,40,Color_BK)                             ;
    Wait_Pen_Up()                                                  ;
    
    Draw_Test_Point(20,300,Yellow)                                 ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[4],&temp[5]))  break                  ;
    }
    DrawRectFill(0,280,40,40,Color_BK)                             ;
    Wait_Pen_Up()                                                  ;
    
    Draw_Test_Point(220,300,Yellow)                                ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[6],&temp[7]))  break                  ;       
    }
    DrawRectFill(200,280,40,40,Color_BK)                           ;
    Wait_Pen_Up()                                                  ;
    if(TP_Adjust(temp))                     break                  ;
  }
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  LCD_Init()                                                       ;
  _EINT()                                                          ;
  TP_Adj()                                                         ;
  LPM3                                                             ;
}
