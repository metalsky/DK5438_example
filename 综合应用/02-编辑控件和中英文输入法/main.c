//===========================================================================//
//                                                                           //
// �ļ���  Main.c                                                            //
// ˵����  BW-DK5438��������/Ӣ�����뷨��ʾ����                              //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TFTDRV.H"
#include "Edit.h"
#include "main.h"
#include "PIN_DEF.H"

#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
  VmainMon()                                                 ;
  Event    |= SHOULD_FLICKER                                 ;
  TBCTL    &= ~TBIFG                                         ;  
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  RTCCTL01  &=~(RTCRDYIFG + RTCTEVIFG)                       ;
  Event  |= SHOULD_DISPLAY_TIME                              ;
}

void main(void)
{
  WDTCTL = WDTPW+WDTHOLD                                     ;  
  Init()                                                     ;
  MAIN_POWER_ON                                              ; 
  for(;;)
  {
    LCD_Init()                                               ;   
    Clear_LCD(Color_BK)                                      ;
    _EINT()                                                  ;
    Input_OP_Password()                                      ;
  }
}

//=====================================================================//
//                                                                     //
//������long int Read_Edit_Val(unsigned char Edit_Index)               //
//�������������Ա����Ͳ�������                                       //
//                                                                     //
//=====================================================================//

void Input_OP_Password(void)
{
  unsigned char temp                                         ;  
  char string[20]                                            ;
  Clear_LCD(Color_BK)                                        ;
  TA0CCR1  = 220                                             ; 
  DrawRectFill(0,0  ,240,30 ,STATUS_BK_COLOR)                ; // ״̬��
  DrawRectFill(0,30 ,240,30 ,TITLE_BK_COLOR)                 ; // ������
  DrawRectFill(0,60 ,240,230,WINDOW_BK_COLOR)                ; // ����
  DrawRectFill(0,290,240,30 ,STATUS_BK_COLOR)                ; // ����
  DrawRectFill(26,105,185,30,WINDOW_COLOR)                   ; // ��ʾ���ڱ���
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutString24M(70,33,"�û���֤")                             ; // ����  
  PutString24M(50,108,"�����¼��Ϣ")                        ;      
  Color    = WINDOW_COLOR                                    ; 
  Color_BK = WINDOW_BK_COLOR                                 ; 
  DrawRect(26,135,185,90,WINDOW_COLOR)                       ;
  PutString24M(30,150,"�ʺţ�")                              ; 
  PutString24M(30,190,"���룺")                              ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutString24M(10,294,"ȷ��")                                ; // ����
  PutString24M(185,294,"ȡ��")                               ; 
  CreateEditDig(101,96,147,108,30)                           ; // �˺�
  IniEditDig(101,1,0,1,8,"")                                 ; 
  CreateEditDig(102,96,187,108,30)                           ; // ����
  IniEditDig(102,1,0,1,6,"")                                 ;   
  ActiveEditDig(101,1)                                       ; 
  DisplayTime()                                              ;
  _EINT()                                                    ;
  for(;;)
  {
    temp = Read_Edit_Val(101,string,101)                     ; 
    if(temp==0xFF)  
      SetEditDig(101,"")                                     ;
    if(temp==0x00)     break                                 ;
  }
  ActiveEditDig(102,1)                                       ; 
  DISP_MODE        = 0x01                                    ; 
  INP_MODE         = INP_DIG                                 ;
  for(;;)
  {
    temp = Read_Edit_Val(102,string,101)                     ; 
    if(temp==0xFF)  
      SetEditDig(102,"")                                     ;
    if(temp==0x00)      break                                ;
  }
  DISP_MODE  = 0x00                                          ; // �༭�ؼ���ʾģʽ
  DeleteEditDig(101)                                         ;
  DeleteEditDig(102)                                         ;
}  

  

//=====================================================================//
//                                                                     //
//������void VmainMon(void) ������ص�ѹ���                           //
//                                                                     //
//=====================================================================//
void VmainMon(void)
{
  static unsigned char i = 0                              ;
  static float Vmain = 0                                  ;
  ADC12CTL0 |= ADC12SC                                    ; 
  while (!(ADC12IFG & BIT0))                              ;
  Vmain     += ADC12MEM0*7.5/4095-0.4                     ; 
  if(++i==10)
  {
    Vmain   /= 10                                         ;
    VmainAvr = Vmain                                      ;
    Vmain    = 0                                          ;
    i        = 0                                          ;
  }  
}

//=====================================================================//
//                                                                     //
//������void VmainMon(void) ����ʾ����Դ����                           //
//                                                                     //
//=====================================================================//
void DrawBattery(float voltage)
{
  DrawRectFill(213,8,24,12,STATUS_BK_COLOR)              ;
  DrawRect(213,11,3,6,Green)                             ;
  DrawRect(215,8,20,12,Green)                            ;
  if(voltage>3.2)
    DrawRectFill(217,10,4,9,Green)                       ;
  if(voltage>3.0)
    DrawRectFill(223,10,4,9,Green)                       ;
  if(voltage>2.8)
    DrawRectFill(229,10,4,9,Green)                       ; 
}

//=====================================================================//
//                                                                     //
//������void ReadKey(void) ����ȡ����                                  //
//                                                                     //
//=====================================================================//
unsigned char ReadKey(void)
{
  unsigned char column,row,key                   ;
  unsigned char  Key                             ; 
  ROW_IN_COL_OUT                                 ;
  row = KEY_PORT_IN&(KPR0+KPR1+KPR2+KPR3)        ;
  if(row!=(KPR0+KPR1+KPR2+KPR3))
  {
    CLO_IN_ROW_OUT                               ;
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

//=====================================================================//
//                                                                     //
//������void GetKeyPress(void) ����ʾʱ��                              //
//                                                                     //
//=====================================================================//
unsigned char GetKeyPress(void)
{
  unsigned char Key                       ;
  static unsigned long power_time = 0     ;
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

//=====================================================================//
//                                                                     //
//������void DisplayTime(void) ����ʾʱ��                              //
//                                                                     //
//=====================================================================//
void DisplayTime(void)
{
  unsigned char Hour,Minute                       ;
  unsigned int color_temp,bkcolor_temp            ;
  char string[6]                                  ;
  Minute = RTCMIN                                 ;
  Hour   = RTCHOUR                                ;
  DrawRectFill(160,0,50,29,STATUS_BK_COLOR)       ;
  sprintf(string,"%02d:%02d",Hour,Minute)         ;
  color_temp   = Color                            ;
  bkcolor_temp = Color_BK                         ;
  Color        = STATUS_COLOR                     ;
  Color_BK     = STATUS_BK_COLOR                  ;
  PutStringEN16(160,8,(unsigned char *)string)    ;
  Color        = color_temp                       ;
  Color_BK     = bkcolor_temp                     ;
}
