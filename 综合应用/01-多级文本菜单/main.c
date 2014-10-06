//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������༶�ı��˵�Ӧ�ó���                               //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "main.h"

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
  Init_Func()                                                ; 
  Init()                                                     ;
  MAIN_POWER_ON                                              ; 
  for(;;)
  {
    LCD_Init()                                               ;   
    Clear_LCD(Color_BK)                                      ;
    _EINT()                                                  ;
    RunMenu()                                                ;
  }
}

//=====================================================================//
//                                                                     //
//������void RunMenu(void)                                             //
//������ִ�в˵�����                                                   //
//                                                                     //
//=====================================================================//

void RunMenu(void)
{
  unsigned char key = 0xFF , menu_index  , SubMenuIndex                   ;
  unsigned char First_Item , Active_Item , Active_Item_Index              ;
  unsigned char CallBackIndex                                             ;

  Clear_LCD(STATUS_BK_COLOR)                                              ;
  Color_BK = STATUS_BK_COLOR                                              ;
  PutStringCN24(10,294,"ȷ��")                                            ; // ����
  PutStringCN24(185,294,"ȡ��")                                           ; 
  LoadMenu(MENU_MAIN,PERMISSION)                                          ;
  Initial_Menu()                                                          ;
  DisplayTime()                                                           ; 
  for(;;)
  {
    key = GetKeyPress()                                                   ;
    switch(key)
    {
    case Up: case Down: case Left: case Right:
      MenuMessage(key)                                                    ;
      break                                                               ;
    case  8:
      MenuMessage(Up)                                                     ;
      break                                                               ;
    case  0:
      MenuMessage(Down)                                                   ;
      break                                                               ;
    case Esc:
      {
        menu_index = GetMenuIndex()                                       ;
        if(Menus[menu_index].MasterMenu!=EMPTY)
        {
          LoadMenu(Menus[menu_index].MasterMenu,PERMISSION)               ;
          Initial_Menu()                                                  ;
        }
        break                                                             ;
      }
    case OK: case Enter:
      {
        First_Item          = GetFirstItem()                              ;
        Active_Item         = GetActiveItem()                             ;
        Active_Item_Index   = GetActiveItemIndex()                        ;        
        if(MENU_ITEM[Active_Item_Index].CallBackFuncIndex!=EMPTY)           // �в�������
        {
          CallBackIndex= MENU_ITEM[Active_Item_Index].CallBackFuncIndex   ;
          SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex        ;
          if(SubMenuIndex!=EMPTY)                                           // �в����������ж�̬�Ӳ˵�
          {
            SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex      ; 
            if(Item_OP[CallBackIndex](0,Active_Item_Index  )==0x00)         // �����Ӳ˵��ɹ�
              Initial_Menu()                                              ; // ��ʼ����̬�Ӳ˵�
            else
              Redraw_Menu(First_Item,Active_Item)                         ; // δ�����Ӳ˵����ػ�ԭ�˵�
          }
          else
          {
            Item_OP[CallBackIndex](0,Active_Item_Index  )                 ; // �޶�̬�Ӳ˵�
            Redraw_Menu(First_Item,Active_Item)                           ; // �ػ��˵�
          }
        }
        else  
        if(MENU_ITEM[Active_Item_Index].SubMenuIndex!=EMPTY)
        {
          LoadMenu(MENU_ITEM[Active_Item_Index].SubMenuIndex,PERMISSION)  ;
          Initial_Menu()                                                  ;
        }
        break                                                             ;          
      }
    case Power:
      return                                                              ;
    default:
      break                                                               ;
    }
  }
}  

//=====================================================================//
//                                                                     //
//������Show_Help(NC1,NC2)                                             //
//��������ʾ������Ϣ                                                   //
//                                                                     //
//=====================================================================//
unsigned char Show_Help(unsigned int NC1,unsigned char NC2)
{
  unsigned char key                                          ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"������Ϣ")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"����")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  PutString24M(10,70 ,"������[8]")                           ; 
  PutString24M(10,100,"������[0]")                           ;   
  PutString24M(10,130,"�˵�ѡ��[Enter]")                   ; 
  PutString24M(10,160,"�˵����أ�[Cancel]")                  ; 
  PutString24M(10,190,"�� һ ҳ��[��]")                      ;
  PutString24M(10,220,"�� һ ҳ��[��]")                      ;  
  PutString24M(10,250,"���뷨�л���[*]")                     ;
  for(;;)
  {
    key  = ReadKey()                                         ;
    if(key==Esc)   break                                     ;
  }
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color_BK = STATUS_BK_COLOR                                 ;
  Color    = STATUS_COLOR                                    ;
  PutStringCN24(10,294,"ȷ��")                               ; // ����
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  return 0x00                                                ;  
}

//=====================================================================//
//                                                                     //
//������Set_Time(NC1,NC2)                                              //
//˵������ʾ��������ʾʱ�����óɹ�                                     //
//                                                                     //
//=====================================================================//
unsigned char Set_Time(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"����ʱ��")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"����")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("��Ϣ��ʾ","ʱ�������óɹ�")                    ;
  return 0x00                                                ;
}

//=====================================================================//
//                                                                     //
//������Set_Backlight(NC1,NC2)                                         //
//˵������ʾ��������ʾ����������                                       //
//                                                                     //
//=====================================================================//
unsigned char Set_Backlight(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"���ñ���")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"����")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("��Ϣ��ʾ","���������óɹ�")                    ;
  return 0x00                                                ;
}

//=====================================================================//
//                                                                     //
//������View_Board_Info(NC1,NC2)                                       //
//��������ʾ������Ϣ                                                   //
//                                                                     //
//=====================================================================//
unsigned char View_Board_Info(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"��������Ϣ")                          ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"����")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("��Ϣ��ʾ","BW_DK5438")                         ;
  return 0x00                                                ;
}
  
/*
//=====================================================================//
//������View_Handset_Info(NC1,NC2)                                     //
//�������ֳֻ���Ϣ                                                     //
//=====================================================================//
unsigned char View_Handset_Info(unsigned int NC1,unsigned char NC2)
{ 
  unsigned char key,storage                                  ;
  char string[20]                                            ;
  unsigned int temp                                          ;
  unsigned int year,hour                                     ;
  unsigned char month,day,minute,second                      ;
  for(;RTCCTL01&RTCRDY;)        _NOP()                       ; // �ȴ�RTC��Ч                                
  for(;!(RTCCTL01&RTCRDY);)     _NOP()                       ;                                     
  second   =  RTCSEC                                         ;
  minute   =  RTCMIN                                         ;
  hour     =  RTCHOUR                                        ;  
  day      =  RTCDAY                                         ;
  month    =  RTCMON                                         ;
  year     =  RTCYEARH*100                                   ;
  year    +=  RTCYEARL                                       ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(55,33,"�ֳֻ���Ϣ")                          ; 
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  PutStringCN24(10,75,"���ڣ�")                              ; 
  PutString24M(10,110,"ʱ�䣺")                              ;   
  PutString24M(10,145,"�̼��汾��")                          ; 
  PutString24M(10,180 ,"ˮ���¼��")                         ; 
  PutStringCN24(10,215,"��������¼��")                       ;
  PutString24M(10,250,"�洢��ռ�ã�")                        ;

  sprintf(string,"%d-%02d-%02d",year,month,day)              ; // ����  
  CreateEditDig(101,80,73,150,30)                            ;
  IniEditDig(101,1,0,1,10,string)                            ;

  sprintf(string,"%  02d:%02d:%02d",hour,minute,second)      ; // ʱ��
  CreateEditDig(106,80,108,150,30)                           ; 
  IniEditDig(106,1,0,1,10,string)                            ;    
  
  CreateEditDig(102,130,143,100,30)                          ; // �̼��汾
  IniEditDig(102,1,0,1,2,"1.2.8-W")                          ; 
  for(;;)
  {
    key  = ReadKey()                                         ;
    if(key==Esc)   break                                     ;
    if(second!=RTCSEC)
    {
      second   =  RTCSEC                                     ;
      minute   =  RTCMIN                                     ;
      hour     =  RTCHOUR                                    ;  
      day      =  RTCDAY                                     ;
      month    =  RTCMON                                     ;
      year     =  RTCYEARH*100                               ;
      year    +=  RTCYEARL                                   ;
      sprintf(string,"%d-%02d-%02d",year,month,day)          ; // ����  
      SetEditDig(101,string)                                 ;
      sprintf(string,"  %02d:%02d:%02d",hour,minute,second)  ; // ʱ��
      SetEditDig(106,string)                                 ;          
    }
  }
  DeleteEditDig(101)                                         ;
  DeleteEditDig(102)                                         ;
  DeleteEditDig(103)                                         ;
  DeleteEditDig(104)                                         ;
  DeleteEditDig(105)                                         ;
  DeleteEditDig(106)                                         ;
  return 0x00                                                ;  
}


//=====================================================================//
//������unsigned char Set_Handset_Clock(NC1,NC2)                       //
//���ܣ������ֳֻ�ʱ��                                                 //
//��������Ч                                                           //
//=====================================================================//
unsigned char Set_Handset_Clock(unsigned int NC1,unsigned char NC2)
{
  unsigned char temp,edit=101                                ;
  signed int year                                            ;
  signed int month,day,hour,minute,second,temp_i             ;
  char string[20]                                            ;
  _DINT()                                                    ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(35,33,"�����ֳֻ�ʱ��")                      ; 
  for(;RTCCTL01&RTCRDY;)        _NOP()                       ; // �ȴ�RTC��Ч                                
  for(;!(RTCCTL01&RTCRDY);)     _NOP()                       ;                                   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  PutString24M(10,100,"��:")                                 ;   
  PutString24M(10,150,"��:")                                 ;      
  PutString24M(10,200,"��:")                                 ;   
  PutString24M(125,100,"ʱ:")                                ;      
  PutString24M(125,150,"��:")                                ; 
  PutString24M(125,200,"��:")                                ; 
  month= RTCYEARL                                            ;
  day  = RTCYEARH                                            ;
  year = month+day*100                                       ;
  sprintf(string,"%d",year)                                  ;  
  CreateEditDig(101,50,97,65,30)                             ; // ��
  IniEditDig(101,1,0,1,4,string)                             ;  
  
  sprintf(string,"%d",RTCMON)                                ;
  CreateEditDig(102,50,147,65,30)                            ; // ��
  IniEditDig(102,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCDAY)                                ;
  CreateEditDig(103,50,197,65,30)                            ; // ��
  IniEditDig(103,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCHOUR)                               ;
  CreateEditDig(104,165,97,65,30)                            ; // ʱ
  IniEditDig(104,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCMIN)                                ;
  CreateEditDig(105,165,147,65,30)                           ; // ��
  IniEditDig(105,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCSEC)                                ;
  CreateEditDig(106,165,197,65,30)                           ; // ��
  IniEditDig(106,1,0,1,2,string)                             ;  
  
  ActiveEditDig(101,1)                                       ; 
  _EINT()                                                    ;
  for(;;)
  {
    temp = Read_Edit_Val(edit,string,101)                    ; 
    if(temp==0xFF)  
      break                                                  ;
    if(temp==NEXT_EDIT)                                        // �û�ѡ����һEDIT�ؼ�
    {
      if(edit<106)       edit++                              ;
      ActiveEditDig(edit,1)                                  ;       
    }
    else if(temp==PRE_EDIT)
    {
      if(edit>101)       edit--                              ; // �û�ѡ����һEDIT�ؼ�
      ActiveEditDig(edit,1)                                  ;       
    }   
    else if(temp==0x00)
    {
      ReadEditDig(101,(unsigned char *)string)               ;
      year    =  atoi(string)                                ;
      ReadEditDig(102,(unsigned char *)string)               ;
      month   =  atoi(string)                                ;
      ReadEditDig(103,(unsigned char *)string)               ;
      day     =  atoi(string)                                ;
      ReadEditDig(104,(unsigned char *)string)               ;
      hour    =  atoi(string)                                ;
      ReadEditDig(105,(unsigned char *)string)               ;
      minute  =  atoi(string)                                ;
      ReadEditDig(106,(unsigned char *)string)               ;
      second  =  atoi(string)                                ;
      if(  year<1
         ||month<1 ||month>12
         ||day<1   ||day>31 
         ||hour<0  ||hour>23
         ||minute<0||minute>59 
         ||second<0||second>59)
      {
        DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)           ; 
        ActiveEditDig(edit,0)                                ;
        if(year<1)
          MessageBox("��������","������ô���")              ;
        else if(month<1||month>12)
          MessageBox("��������","�·����ô���")              ;
        else if(day<1  ||day>31)
          MessageBox("��������","�������ô���")              ;
        else if(hour<0 ||hour>23)
          MessageBox("��������","ʱ�����ô���")              ;
        else if(minute<0||minute>59)
          MessageBox("��������","ʱ�����ô���")              ;
        else if(second<0||second>59)
          MessageBox("��������","ʱ�����ô���")              ;
        DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)           ; 
        Color    = WINDOW_COLOR                              ;
        Color_BK = WINDOW_BK_COLOR                           ;
        PutString24M(10,100,"��:")                           ;   
        PutString24M(10,150,"��:")                           ;      
        PutString24M(10,200,"��:")                           ;   
        PutString24M(125,100,"ʱ:")                          ;      
        PutString24M(125,150,"��:")                          ; 
        PutString24M(125,200,"��:")                          ; 
        ActiveEditDig(101,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(101,0)                                 ;
        ActiveEditDig(102,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(102,0)                                 ;
        ActiveEditDig(103,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(103,0)                                 ;
        ActiveEditDig(104,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(104,0)                                 ;
        ActiveEditDig(105,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(105,0)                                 ;
        ActiveEditDig(106,1)                                 ;
        EditDigMessage(128,string)                           ;
        ActiveEditDig(106,0)                                 ;
        ActiveEditDig(edit,1)                                ;
        continue                                             ;
      }
      temp_i       = year/100                                ;
      temp_i       = temp_i*256                              ;
      temp_i      +=(year%100)                               ;
      year         = temp_i                                  ;
      for(;RTCCTL01&RTCRDY;)        _NOP()                   ; // �ȴ�RTC��Ч                                
      for(;!(RTCCTL01&RTCRDY);)     _NOP()                   ;                                   
      for(;;)
      {  
        SetRTCYEAR(year)                                     ; // RTCTIM0 = 0x0F19
        SetRTCMON(month)                                     ; // RTCTIM1 = 0x010E
        SetRTCDOW(1)                                         ; // RTCDATE = 0x0611
        SetRTCDAY(day)                                       ; // RTCYEAR = 0x07D8
        SetRTCHOUR(hour)                                     ;
        SetRTCMIN(minute)                                    ;
        SetRTCSEC(second)                                    ;
        break                                                ;
      }
      __delay_cycles(280000)                                 ; 
      DeleteEditDig(101)                                     ;
      DeleteEditDig(102)                                     ;
      DeleteEditDig(103)                                     ;
      DeleteEditDig(104)                                     ;
      DeleteEditDig(105)                                     ;
      DeleteEditDig(106)                                     ;
      DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)             ; 
      DisplayTime()                                          ;
      MessageBox("�����ɹ�","ʱ���������")                  ;
      _EINT()                                                ;
      break                                                  ;
    }    
  }
  DeleteEditDig(101)                                         ;
  DeleteEditDig(102)                                         ;
  DeleteEditDig(103)                                         ;
  DeleteEditDig(104)                                         ;
  DeleteEditDig(105)                                         ;
  DeleteEditDig(106)                                         ;  
  _EINT()                                                    ;
  return  0x00                                               ;
}
*/

//=====================================================================//
//������void VmainMon(void) ������ص�ѹ���                           //
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
//������void VmainMon(void) ����ʾ����Դ����                           //
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
//������void ReadKey(void) ����ȡ����                                  //
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

//=====================================================================//
//������void Init_Func(void)                                           //
//��������ʼ�������б�                                                 //
//=====================================================================//

void Init_Func(void)
{
  Item_OP[FUNC_SHOW_HELP]      = Show_Help        ; // ������Ϣ
  Item_OP[FUNC_BOARD_INFO]     = View_Board_Info  ; // ��������Ϣ  
  Item_OP[FUNC_SET_TIME]       = Set_Time         ; // ����ʱ��
  Item_OP[FUNC_SET_BACKLIGHT]  = Set_Backlight    ; // ���ñ���
}


//=====================================================================//
//������void MessageBox(char *title,char *string)                      //
//���ܣ���ʾ��Ϣ��                                                     //
//����: title  ����Ϣ�����                                            //
//      string ����Ϣ������                                            //
//=====================================================================//
void MessageBox(char *title,char *string)
{
  unsigned char key                               ;
  _DINT()                                         ;
  Color    = TITLE_COLOR                          ;
  Color_BK = TITLE_BK_COLOR                       ;
  DrawRectFill(20,115,200,30,TITLE_BK_COLOR)      ; 
  PutStringCN24(120-strlen(title)*6,118,
                (unsigned char *)title  )         ;
  DrawRectFill(20,145,200,80,WINDOW_BK_COLOR)     ;
  DrawRectFill(20,145,200,80,0xFFFF)              ;
  DrawRect(20,145,200,80,WINDOW_COLOR)            ;
  Color    = WINDOW_COLOR                         ;
  Color_BK = 0xFFFF                               ;
  PutString24M(120-strlen(string)*6,173,
               (unsigned char *)string  )         ;
  Color_BK = WINDOW_BK_COLOR                      ;
  _EINT()                                         ;
  for(;;)
  {
    key  =  GetKeyPress()                         ;
    if(key==Enter||key==Esc||key==OK)
      break                                       ;
  }
}

