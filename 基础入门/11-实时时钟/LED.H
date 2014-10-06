//===========================================================================//
//                                                                           //
// 文件：  LED.H                                                             //
// 说明：  BW-DK5438开发板数码管显示函数库                                   //
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
#include "PIN_DEF.H"
#include "string.h"
#include "math.h"

//数码管位显示模式常量定义
#define  DIGIT_OFF       BIT0
#define  DIGIT_FLICKER   BIT1

//整数显示模式常量定义
#define  FIT_ZERO        BIT0
#define  NO_ZERO         0x00

//负号常量
#define  MINUS          ~SEGG

//小数点常量
#define  DPOINT         ~SEGDP

//数码管显示内容、模式定义
typedef struct
{
  unsigned char data                                       ; // 显示数据
  unsigned char mode                                       ; // 显示模式
  unsigned char lightness                                  ; // 显示亮度
}LED_STRUCT                                                ;

//外部变量
extern unsigned char SMG[10]                               ;
extern LED_STRUCT LED[8]                                   ;

//函数定义
void LED_Disp_Int(long iValue,unsigned char mode)          ;
void LED_Disp_Float(float fValue,
                    unsigned char dpLength,
                    unsigned char mode         )           ;
void Clear_LED(void)                                       ;
void LED_Hide_Digit(unsigned char digit)                   ;
void LED_Flicker_Digit(unsigned char digit)                ;
void Ini_LED(unsigned char lightness)                      ;

//***************************************************************************//
//                                                                           //
// 函数：LED_Disp_Int(long,unsigned char)                                    //
// 功能：数码管显示整数                                                      //
// 参数：iValue ——显示数值                                                 //
//       mode   ——显示模式                                                 //
//                                                                           //
//***************************************************************************//
void LED_Disp_Int(long iValue,unsigned char mode)
{
  unsigned char i,l                                        ;
  char string[10]                                          ;
  Clear_LED()                                              ;
  sprintf(string,"%ld",iValue)                             ;
  if(mode&FIT_ZERO)
    sprintf(string,"%08ld",iValue)                         ;
  l                = strlen(string)                        ;
  if(iValue>=0)
  {
    for(i=0;i<l;i++)
      LED[7-i].data  = SMG[string[l-i-1]-0x30]             ;
  }
  else
  {
    for(i=0;i<l-1;i++)
      LED[7-i].data  = SMG[string[l-i-1]-0x30]             ;
  }
  if(iValue<0)
    LED[8-l].data  = MINUS                                 ; 
  if(l<8)
  {
    for(i=0;i<8-l;i++)
      LED[i].mode |= DIGIT_OFF                             ;
  }
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Disp_Float(float,unsigned char,unsigned char)                   //
// 功能：数码管显示整数                                                      //
// 参数：fValue   ——显示数值                                               //
//       dpLength ——小数点后位数                                           //
//       mode     ——显示模式                                               //
//                                                                           //
//***************************************************************************//
void LED_Disp_Float(float fValue,unsigned char dpLength,unsigned char mode)
{
  char string[10]                                          ;
  long lvTemp                                              ;
  lvTemp       = fValue*pow(10,dpLength)                   ;
  sprintf(string,"%ld",lvTemp)                             ;
  LED_Disp_Int(lvTemp,mode)                                ;
  LED[7-dpLength].data  &= DPOINT                          ;
}

//***************************************************************************//
//                                                                           //
// 函数：Clear_LED(void)                                                     //
// 功能：清除LED显示缓冲区                                                   //
// 参数：无                                                                  //
//                                                                           //
//***************************************************************************//
void Clear_LED(void)
{
  unsigned char i                                          ;
  for(i=0;i<8;i++)
  {
//    LED[i].data   = 0xFF                                   ;
    LED[i].mode  &=~DIGIT_OFF                              ;
  }
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Hide_Digit(unsigned char)                                       //
// 功能：消隐LED指定位                                                       //
// 参数：digit  —— 消隐位                                                  //
//                                                                           //
//***************************************************************************//
void LED_Hide_Digit(unsigned char digit)
{
  if(digit<=8)
    LED[digit-1].mode  |= DIGIT_OFF                        ;
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Flicker_Digit(unsigned char)                                    //
// 功能：闪烁LED指定位                                                       //
// 参数：digit  —— 闪烁位                                                  //
//                                                                           //
//***************************************************************************//
void LED_Flicker_Digit(unsigned char digit)
{
  if(digit<=8)
    LED[digit-1].mode  |= DIGIT_FLICKER                    ;
}

//***************************************************************************//
//                                                                           //
// 函数：Ini_LED(void)                                                       //
// 功能：初始化数码管显示                                                    //
// 参数：lightness  —— 显示亮度                                            //
//                                                                           //
//***************************************************************************//
void Ini_LED(unsigned char lightness)
{
  unsigned char i                                          ;
  Clear_LED()                                              ;
  if(lightness==0||lightness>63)
    lightness          = 10                                ;
  for(i=0;i<8;i++)
    LED[i].lightness   = lightness                         ;
  SET_DIGITS_OUT                                           ;
  SET_SEGS_OUT                                             ;
}
