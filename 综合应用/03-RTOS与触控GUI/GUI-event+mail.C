//===========================================================================//
//                                                                           //
// 文件：  GUIMenu.H                                                         //
// 说明：  BW-DK5438开发板图形用户界面函数文件                               //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//
#include "RTOS.h"
#include "string.h"
#include "ILI9325V430.H"
#include "GUI.H"
#include "MENU_RESOURCE.H"

extern unsigned     Color,Color_BK                                       ;
extern OS_MAILBOX   MBKey                                                ;
extern OS_MAILBOX   MBTouch                                              ;
extern OS_MAILBOX   MBFunc                                               ;
extern OS_TASK      LCD_TASK_TCB                                         ; 
extern OS_TASK      MENU_OP_TASK_TCB                                     ; // 菜单操作函数
extern OS_TASK      KEY_TP_TASK_TCB                                      ; // 键盘\触摸屏检测


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ShowMenu_Item(unsigned char ,unsigned char,unsigned char)       //
//描述：显示菜单项子函数                                                     //
//参数：index ——菜单项索引                                                 //
//      no    ——显示位置序号                                               //
//      mode  ——显示模式                                                   //
//		                                                             //
//---------------------------------------------------------------------------//
void ShowMenu_Item(unsigned char index,unsigned char no,unsigned char mode)
{
  unsigned int title_pos                                                 ;
  title_pos = strlen((char *)MENU_ITEM[index].ItemTitle)                 ;
  title_pos = 40+no%3*80-title_pos*4                                     ;
  ShowIcon48(16+no%3*80,36+no/3*84,48,48,MENU_ITEM[index].IconAddr)      ; 
  PutStringCN16(title_pos,91+no/3*84,
                (unsigned char *)MENU_ITEM[index].ItemTitle)             ;
  if(mode)
    DrawRect(12+no%3*80,33+no/3*84,56,54,Color)                          ;
}

void InactivateItem(unsigned char Item_No)
{
  DrawRect(12+Item_No%3*80,33+Item_No/3*84,56,54,Color_BK)               ; 
}

void ActivateItem(unsigned char Item_No)
{
  DrawRect(12+Item_No%3*80,33+Item_No/3*84,56,54,Color)                  ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ShowMenu_Item(unsigned char ,unsigned char,unsigned char)       //
//描述：显示菜单项子函数                                                     //
//参数：index ——菜单项索引                                                 //
//      no    ——显示位置序号                                               //
//      mode  ——显示模式                                                   //
//		                                                             //
//---------------------------------------------------------------------------//
void RunMenu(unsigned char index)
{
  char key,i,Active_Item  = 0x00                                         ;
  OS_U8 Event                                                            ;
  union
  {
    unsigned int  pos[2]                                                 ;
    char byte[4]                                                         ;
  }TPoint                                                                ;
Ini_Background:  
  Color     = Black                                                      ;
  Color_BK  = 0xEF9F                                                     ;
  Clear_LCD(Color_BK)                                                    ;
  DrawRectFill(0,0,240,25,Blue)                                          ;
  DrawRectFill(0,295,240,25,Blue)                                        ;
  Color    = 0xFFFF                                                      ;
  Color_BK = Blue                                                        ;
  PutStringCN16(4,4,(unsigned char *)Menus[index].MenuTitle)             ;
  Color    = Black                                                       ;
  Color_BK = WINDOW_BK_COLOR                                             ;
  for(i=0;i<Menus[index].MenuConfig[0];i++)
  {
    if(i==Active_Item)
      ShowMenu_Item(Menus[index].ItemIndex[i],i,1)                       ;
    else
      ShowMenu_Item(Menus[index].ItemIndex[i],i,0)                       ;      
  }
  for(;;)
  {
//    Event = OS_WaitEvent(EVENT_KEY_PRESSED+EVENT_TP_PRESSED)             ; 
    OS_WaitMail(&MBKey)                                                  ;
//    if(Event&EVENT_KEY_PRESSED)
    {
      OS_GetMail1(&MBKey, &key)                                          ;  
      switch(key)
      {
      case Right:
        InactivateItem(Active_Item)                                      ;
        if(++Active_Item>=Menus[index].MenuConfig[0])
          Active_Item = 0x00                                             ;
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case Left:
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==0) 
          Active_Item = 0x08                                             ;
        else
          Active_Item--                                                  ;
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case 8:
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==0) 
          Active_Item = 0x08                                             ;
        else if(Active_Item==1||Active_Item==2)
          Active_Item = Active_Item+5                                    ;
        else 
          Active_Item = Active_Item-3                                    ;          
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case 0:
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==8) 
          Active_Item = 0x00                                             ;
        else if(Active_Item==6||Active_Item==7)
          Active_Item = Active_Item-5                                    ;
        else 
          Active_Item = Active_Item+3                                    ;          
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case Enter:
        OS_PutMail1(&MBFunc, &Active_Item)                               ;
        OS_SignalEvent(EVENT_OP_STARTED,&MENU_OP_TASK_TCB)               ;
        OS_Suspend(&LCD_TASK_TCB)                                        ;
        goto Ini_Background                                              ;
      default:
        break                                                            ;
      }
    }
/*    else
    {
      OS_GetMail(&MBTouch, TPoint.byte)                                  ; 
      for(i=0;i<Menus[index].MenuConfig[0];i++)
      {        
        if(  (TPoint.pos[0]<(60+i%3*80)&&TPoint.pos[0]>(20+i%3*80))
           &&(TPoint.pos[1]<(95+i/3*84)&&TPoint.pos[1]>(55+i/3*82)))
        {
          Active_Item = i                                                ;
          OS_PutMail1(&MBFunc, &Active_Item)                             ;
          OS_SignalEvent(EVENT_OP_STARTED,&MENU_OP_TASK_TCB)             ;
          OS_Suspend(&LCD_TASK_TCB)                                      ;
          goto Ini_Background                                            ;
        }
      }
    }
*/    
  }
}
      
  