//===========================================================================//
//                                                                           //
// 文件：  KEY.C                                                             //
// 说明：  BW-DK5438开发板键盘处理函数文件, 支持uC/OS-II                     //
// 编译：  IAR Embedded Workbench IDE for msp430 v5.20                       //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include <ucos_ii.h>
#include "KEY.H"
#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include "PIN_DEF.H"

extern OS_EVENT *MBKeyTP                         ;
extern char MBKeyTPBuffer[6]                     ;


//=====================================================================//
//函数：void Init_KeyPad(void) —初始化键盘IO属性                      //
//=====================================================================//
void Init_KeyPad(void)
{
  KEY_PORT_REN      =  0xFF                      ;
  KEY_PORT_OUT      =  0x00                      ;
}

//=====================================================================//
//函数：void ReadKey(void) —读取按键                                  //
//=====================================================================//
unsigned char ReadKey(void)
{
  unsigned char column,row,key                   ;
  unsigned char  Key                             ; 
  ROW_IN_COL_OUT                                 ;
  __delay_cycles(20)                              ;
  row = KEY_PORT_IN&(KPR0+KPR1+KPR2+KPR3)        ;
  if(row!=(KPR0+KPR1+KPR2+KPR3))
  {
    CLO_IN_ROW_OUT                               ;
    __delay_cycles(20)                            ;
    column  = KEY_PORT_IN&(KPC0+KPC1+KPC2+KPC3)  ;
    ROW_IN_COL_OUT                               ;
    Key     = row + column                       ;
    _NOP();
    switch(Key)
    {
    case 0x7E:
      key   = Enter                              ;
      break                                      ;
    case 0x7D:
      key   = Cancel                             ;
      break                                      ;
    case 0x02FE:
      key   = Up                                 ;
      break                                      ;
    case 0x02FD:
      key   = Down                               ;
      break                                      ;
    case 0xEE:
      key   = Left                               ;
      break                                      ;
    case 0xBE:
      key   = Right                              ;
      break                                      ;
    case 0x7B:
      key   = F1                                 ;
      key   = Delete                             ;
      break                                      ;        
    case 0x77:
      key   = F3                                 ;
      key   = Backspace                          ;
      break                                      ;        
    case 0xE7:
      key   =  1                                 ;
      break                                      ;        
    case 0xD7:
      key   =  2                                 ;
      break                                      ;  
    case 0xB7:
      key   =  3                                 ;
      break                                      ;        
    case 0xEB:
      key   =  4                                 ;
      break                                      ;        
    case 0xDB:
      key   =  5                                 ;
      break                                      ;        
    case 0xBB:
      key   =  6                                 ;
      break                                      ;        
    case 0xED:
      key   =  7                                 ;
      break                                      ;        
    case 0xDD:
      key   =  8                                 ;
      break                                      ;        
    case 0xBD:
      key   =  9                                 ;
      break                                      ;        
    case 0xDE:
      key   =  0                                 ;
      break                                      ;        
    default:
      key   =  0xFF                              ;
    }
  }
  else
    key            =  0xFF                       ;  
  KEY_PORT_DIR     =  0xFF                       ;
  KEY_PORT_OUT     =  0x00                       ;
  return key                                     ;
}

/*
unsigned char GetKeyPress(void)
{
  unsigned char Key                       ;
  static unsigned long power_time = 0     ;
det_key:  
  for(;;)
  {
    Key   = ReadKey()                     ;
    if(Key==Power)
    {
      if(++power_time>300000)
        return Key                        ;
      else
        continue                          ;
    }
    else
      power_time  = 0x00                  ;
    if(Key==0xFF)
    {
      __delay_cycles(560000)              ;  
      Key   = ReadKey()                   ;
      if(Key==0xFF)     break             ;  
    }        
  }
  Interval  = 0                           ;
  for(;;)
  {
    if(Event&SHOULD_FLICKER)
    {
      Event  &=~SHOULD_FLICKER            ;
      Flicker_Cursor()                    ;
      DrawBattery(VmainAvr)               ;
    }
    if(Event&SHOULD_DISPLAY_TIME)
    {
      Event  &=~SHOULD_DISPLAY_TIME       ;
      DisplayTime()                       ;  
    }
    Key = ReadKey()                       ;
    if(Interval++>MAX_INTERVAL)
      Interval   =MAX_INTERVAL            ;
    if(Key!=0xFF)
    {
       __delay_cycles(560000)             ;  
      if(Key==ReadKey()) 
      {
        return Key                        ;
      }
    }
  }
}

*/