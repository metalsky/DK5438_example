//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板指针式电子时钟程序                                 //
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
#include "TFTDRV.H"
#include "RTC.H"
#include "CLOCK.H"

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
//                 初始化RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // 时钟模式,十六进制格式
  SetRTCYEAR(2010)                                   ; 
  SetRTCMON(10)                                      ;
  SetRTCDAY(24)                                      ;
  SetRTCDOW(0)                                       ;
  SetRTCHOUR(20)                                     ;
  SetRTCMIN(24)                                      ;
  SetRTCSEC(35)                                      ;
}


//---------------------------------------------------------------------------//
//                                                                           //
//函数：char Proc_Show_Clock(char *NC)                                       //
//描述：电子时钟程序                                                         //
//参数：NC ——无意义                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
void Show_Clock(void)
{
  unsigned int Hour ,Minute,Second                                 ;
  Hour    = GetRTCHOUR()                                           ;
  while(Hour>=12)
  {
    Hour -= 12                                                     ;
  }
  Minute  = GetRTCMIN()                                            ;
  Second  = GetRTCSEC()                                            ;
  Ini_Clock_Contex()                                               ;
  ShowBMP240320(BMP_CLOCK)                                         ;
  Gen_Hour_Hand(Hour*30+Minute/2)                                  ;
  Save_Hour_Hand_BK()                                              ; 
  Draw_Hour_Hand(White)                                            ;
  Gen_Minu_Hand(Minute*6+Second/10)                                ;
  Save_Minu_Hand_BK()                                              ; 
  Draw_Minu_Hand(White)                                            ;
  Gen_Second_Hand(Second*6)                                        ;
  Save_Second_Hand_BK()                                            ; 
  Draw_Second_Hand(White)                                          ;
  for(;;)
  {
    if(Second==GetRTCSEC())     continue                           ;
    Second   = GetRTCSEC()                                         ;
    Minute  = GetRTCMIN()                                          ;
    Hour    = GetRTCHOUR()                                         ;
    while(Hour>=12)
    {
      Hour -= 12                                                   ;
    }
    if(Minute%6==0&&Second==0)                                      // 时针转动
    {
      Restore_Second_Hand_BK()                                     ;
      Restore_Minu_Hand_BK()                                       ;
      Restore_Hour_Hand_BK()                                       ;
      Gen_Hour_Hand(Hour*30+Minute/2)                              ;
      if(Second%10==0)
        Gen_Minu_Hand(Minute*6+Second/10)                          ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Hour_Hand_BK()                                          ; 
      Draw_Hour_Hand(White)                                        ;    
      Save_Minu_Hand_BK()                                          ; 
      Draw_Minu_Hand(White)                                        ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;
    }
    else if(Second%10==0)                                           // 分针转动
    {
      Restore_Second_Hand_BK()                                     ;
      Restore_Minu_Hand_BK()                                       ;      
      Gen_Minu_Hand(Minute*6+Second/10)                            ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Minu_Hand_BK()                                          ; 
      Draw_Minu_Hand(White)                                        ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;
    }
    else
    {
      Restore_Second_Hand_BK()                                     ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;      
    }
    Delay(100)                                                     ;
  }
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_RTC()                                                       ;
  Init_Port()                                                      ;
  LCD_Init()                                                       ;
  Show_Clock()                                                     ;
  for(;;) _NOP();
}
