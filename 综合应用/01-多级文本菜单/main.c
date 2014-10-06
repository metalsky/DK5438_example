//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板多级文本菜单应用程序                               //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
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
//函数：void RunMenu(void)                                             //
//描述：执行菜单调度                                                   //
//                                                                     //
//=====================================================================//

void RunMenu(void)
{
  unsigned char key = 0xFF , menu_index  , SubMenuIndex                   ;
  unsigned char First_Item , Active_Item , Active_Item_Index              ;
  unsigned char CallBackIndex                                             ;

  Clear_LCD(STATUS_BK_COLOR)                                              ;
  Color_BK = STATUS_BK_COLOR                                              ;
  PutStringCN24(10,294,"确认")                                            ; // 底栏
  PutStringCN24(185,294,"取消")                                           ; 
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
        if(MENU_ITEM[Active_Item_Index].CallBackFuncIndex!=EMPTY)           // 有操作函数
        {
          CallBackIndex= MENU_ITEM[Active_Item_Index].CallBackFuncIndex   ;
          SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex        ;
          if(SubMenuIndex!=EMPTY)                                           // 有操作函数且有动态子菜单
          {
            SubMenuIndex = MENU_ITEM[Active_Item_Index].SubMenuIndex      ; 
            if(Item_OP[CallBackIndex](0,Active_Item_Index  )==0x00)         // 生成子菜单成功
              Initial_Menu()                                              ; // 初始化动态子菜单
            else
              Redraw_Menu(First_Item,Active_Item)                         ; // 未生成子菜单，重画原菜单
          }
          else
          {
            Item_OP[CallBackIndex](0,Active_Item_Index  )                 ; // 无动态子菜单
            Redraw_Menu(First_Item,Active_Item)                           ; // 重画菜单
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
//函数：Show_Help(NC1,NC2)                                             //
//描述：显示帮助信息                                                   //
//                                                                     //
//=====================================================================//
unsigned char Show_Help(unsigned int NC1,unsigned char NC2)
{
  unsigned char key                                          ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"帮助信息")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"返回")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  PutString24M(10,70 ,"↑键：[8]")                           ; 
  PutString24M(10,100,"↓键：[0]")                           ;   
  PutString24M(10,130,"菜单选择：[Enter]")                   ; 
  PutString24M(10,160,"菜单返回：[Cancel]")                  ; 
  PutString24M(10,190,"上 一 页：[←]")                      ;
  PutString24M(10,220,"下 一 页：[→]")                      ;  
  PutString24M(10,250,"输入法切换：[*]")                     ;
  for(;;)
  {
    key  = ReadKey()                                         ;
    if(key==Esc)   break                                     ;
  }
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color_BK = STATUS_BK_COLOR                                 ;
  Color    = STATUS_COLOR                                    ;
  PutStringCN24(10,294,"确认")                               ; // 底栏
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  return 0x00                                                ;  
}

//=====================================================================//
//                                                                     //
//函数：Set_Time(NC1,NC2)                                              //
//说明：演示函数，显示时间设置成功                                     //
//                                                                     //
//=====================================================================//
unsigned char Set_Time(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"设置时间")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"返回")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("信息提示","时间已设置成功")                    ;
  return 0x00                                                ;
}

//=====================================================================//
//                                                                     //
//函数：Set_Backlight(NC1,NC2)                                         //
//说明：演示函数，显示背光已设置                                       //
//                                                                     //
//=====================================================================//
unsigned char Set_Backlight(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"设置背光")                            ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"返回")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("信息提示","背光已设置成功")                    ;
  return 0x00                                                ;
}

//=====================================================================//
//                                                                     //
//函数：View_Board_Info(NC1,NC2)                                       //
//描述：显示帮助信息                                                   //
//                                                                     //
//=====================================================================//
unsigned char View_Board_Info(unsigned int NC1,unsigned char NC2)
{
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ; 
  DrawRectFill(0,30 ,240,30 ,WINDOW_COLOR)                   ;  
  Color    = TITLE_COLOR                                     ;
  Color_BK = TITLE_BK_COLOR                                  ;
  PutStringCN24(70,33,"开发板信息")                          ;
  DrawRectFill(0,290,240,30,STATUS_BK_COLOR)                 ; 
  Color    = STATUS_COLOR                                    ;
  Color_BK = STATUS_BK_COLOR                                 ;
  PutStringCN24(185,294,"返回")                              ;   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  MessageBox("信息提示","BW_DK5438")                         ;
  return 0x00                                                ;
}
  
/*
//=====================================================================//
//函数：View_Handset_Info(NC1,NC2)                                     //
//描述：手持机信息                                                     //
//=====================================================================//
unsigned char View_Handset_Info(unsigned int NC1,unsigned char NC2)
{ 
  unsigned char key,storage                                  ;
  char string[20]                                            ;
  unsigned int temp                                          ;
  unsigned int year,hour                                     ;
  unsigned char month,day,minute,second                      ;
  for(;RTCCTL01&RTCRDY;)        _NOP()                       ; // 等待RTC有效                                
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
  PutStringCN24(55,33,"手持机信息")                          ; 
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                 ;   
  PutStringCN24(10,75,"日期：")                              ; 
  PutString24M(10,110,"时间：")                              ;   
  PutString24M(10,145,"固件版本：")                          ; 
  PutString24M(10,180 ,"水表记录：")                         ; 
  PutStringCN24(10,215,"集中器记录：")                       ;
  PutString24M(10,250,"存储器占用：")                        ;

  sprintf(string,"%d-%02d-%02d",year,month,day)              ; // 日期  
  CreateEditDig(101,80,73,150,30)                            ;
  IniEditDig(101,1,0,1,10,string)                            ;

  sprintf(string,"%  02d:%02d:%02d",hour,minute,second)      ; // 时间
  CreateEditDig(106,80,108,150,30)                           ; 
  IniEditDig(106,1,0,1,10,string)                            ;    
  
  CreateEditDig(102,130,143,100,30)                          ; // 固件版本
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
      sprintf(string,"%d-%02d-%02d",year,month,day)          ; // 日期  
      SetEditDig(101,string)                                 ;
      sprintf(string,"  %02d:%02d:%02d",hour,minute,second)  ; // 时间
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
//函数：unsigned char Set_Handset_Clock(NC1,NC2)                       //
//功能：设置手持机时间                                                 //
//参数：无效                                                           //
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
  PutStringCN24(35,33,"设置手持机时钟")                      ; 
  for(;RTCCTL01&RTCRDY;)        _NOP()                       ; // 等待RTC有效                                
  for(;!(RTCCTL01&RTCRDY);)     _NOP()                       ;                                   
  Color    = WINDOW_COLOR                                    ;
  Color_BK = WINDOW_BK_COLOR                                 ;
  PutString24M(10,100,"年:")                                 ;   
  PutString24M(10,150,"月:")                                 ;      
  PutString24M(10,200,"日:")                                 ;   
  PutString24M(125,100,"时:")                                ;      
  PutString24M(125,150,"分:")                                ; 
  PutString24M(125,200,"秒:")                                ; 
  month= RTCYEARL                                            ;
  day  = RTCYEARH                                            ;
  year = month+day*100                                       ;
  sprintf(string,"%d",year)                                  ;  
  CreateEditDig(101,50,97,65,30)                             ; // 年
  IniEditDig(101,1,0,1,4,string)                             ;  
  
  sprintf(string,"%d",RTCMON)                                ;
  CreateEditDig(102,50,147,65,30)                            ; // 月
  IniEditDig(102,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCDAY)                                ;
  CreateEditDig(103,50,197,65,30)                            ; // 日
  IniEditDig(103,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCHOUR)                               ;
  CreateEditDig(104,165,97,65,30)                            ; // 时
  IniEditDig(104,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCMIN)                                ;
  CreateEditDig(105,165,147,65,30)                           ; // 分
  IniEditDig(105,1,0,1,2,string)                             ;  
  
  sprintf(string,"%d",RTCSEC)                                ;
  CreateEditDig(106,165,197,65,30)                           ; // 秒
  IniEditDig(106,1,0,1,2,string)                             ;  
  
  ActiveEditDig(101,1)                                       ; 
  _EINT()                                                    ;
  for(;;)
  {
    temp = Read_Edit_Val(edit,string,101)                    ; 
    if(temp==0xFF)  
      break                                                  ;
    if(temp==NEXT_EDIT)                                        // 用户选择下一EDIT控件
    {
      if(edit<106)       edit++                              ;
      ActiveEditDig(edit,1)                                  ;       
    }
    else if(temp==PRE_EDIT)
    {
      if(edit>101)       edit--                              ; // 用户选择上一EDIT控件
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
          MessageBox("参数错误","年份设置错误")              ;
        else if(month<1||month>12)
          MessageBox("参数错误","月份设置错误")              ;
        else if(day<1  ||day>31)
          MessageBox("参数错误","日期设置错误")              ;
        else if(hour<0 ||hour>23)
          MessageBox("参数错误","时间设置错误")              ;
        else if(minute<0||minute>59)
          MessageBox("参数错误","时间设置错误")              ;
        else if(second<0||second>59)
          MessageBox("参数错误","时间设置错误")              ;
        DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)           ; 
        Color    = WINDOW_COLOR                              ;
        Color_BK = WINDOW_BK_COLOR                           ;
        PutString24M(10,100,"年:")                           ;   
        PutString24M(10,150,"月:")                           ;      
        PutString24M(10,200,"日:")                           ;   
        PutString24M(125,100,"时:")                          ;      
        PutString24M(125,150,"分:")                          ; 
        PutString24M(125,200,"秒:")                          ; 
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
      for(;RTCCTL01&RTCRDY;)        _NOP()                   ; // 等待RTC有效                                
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
      MessageBox("操作成功","时钟设置完成")                  ;
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
//函数：void VmainMon(void) —主电池电压检测                           //
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
//函数：void VmainMon(void) —显示主电源电量                           //
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
//函数：void ReadKey(void) —读取按键                                  //
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
//函数：void Init_Func(void)                                           //
//描述：初始化函数列表                                                 //
//=====================================================================//

void Init_Func(void)
{
  Item_OP[FUNC_SHOW_HELP]      = Show_Help        ; // 帮助信息
  Item_OP[FUNC_BOARD_INFO]     = View_Board_Info  ; // 开发板信息  
  Item_OP[FUNC_SET_TIME]       = Set_Time         ; // 设置时间
  Item_OP[FUNC_SET_BACKLIGHT]  = Set_Backlight    ; // 设置背光
}


//=====================================================================//
//函数：void MessageBox(char *title,char *string)                      //
//功能：显示消息框                                                     //
//参数: title  —消息框标题                                            //
//      string —消息框内容                                            //
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

