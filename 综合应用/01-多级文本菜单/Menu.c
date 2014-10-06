//===========================================================================//
//                                                                           //
// 文件：  Menu.c                                                            //
// 说明：  BW-DK5438开发板多级文本菜单操作函数                               //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//
#include "Menu.h"

extern void str_cpy(char* Destination, char* Source,unsigned char Num)  ;
extern const ITEM MENU_ITEM[]                                           ;
extern const MENU_RES Menus[]                                           ;
extern unsigned int  Color                                              ; // 前景颜色
extern unsigned int  Color_BK                                           ; // 背景颜色
extern MENU  Menu                                                       ;

unsigned char Font_CN          = 24                                     ; // 中文字体
unsigned char Font_EN          = 24                                     ; // 西文字体
unsigned char MenuNum          = 0xFF                                   ; // 需上传的菜单数组数量
unsigned char MenuUpdated      = 0xFF                                   ; // 菜单更新，需上传当前菜单选项
unsigned char First_Index_old  = 0xFF                                   ;
unsigned char y_Index_old      = 0xFF                                   ;
unsigned char First_Index      = 0x00                                   ;
unsigned char Active_Index     = 0x00                                   ;
unsigned char Dis_Menu_Num                                              ;


//=====================================================================//
//函数：void UpDate_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//功能：刷新整屏菜单显示函数
//参数：First_Index -  显示的第一条菜单号
//	    Menu_Index  -  处于选择状态的菜单项
//	    Menu_List   -  菜单资源链表指针
//返回：无
//日期:        2010.01.20        
//=====================================================================//
void UpDate_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  unsigned int  y_Index                                                                ;
  unsigned char i                                                                      ;
  _DINT()                                                                              ;
  if(Dis_Menu_Num+First_Index> Menu.MenuConfig[0])
    Dis_Menu_Num =  Menu.MenuConfig[0] - First_Index                                   ; 
  y_Index  = 0                                                                         ; // 
  if(First_Index_old!=First_Index)                                                       // window内首条菜单项改变 
  {
    if(y_Index_old==0xFF)                                                                // 初始绘制菜单
    {
      DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                                       ; // 清空窗口 
      Color    = TITLE_COLOR                                                           ; // 显示标题条
      Color_BK = TITLE_BK_COLOR                                                        ;
      DrawRectFill(0,30,240,30,Color_BK)                                               ;
      PutString24M(120-25*strlen((char*)Menu.MenuTitle)/4,33,Menu.MenuTitle)           ;
      y_Index = Y_OFFSET-1                                                             ; // 由上而下显示各菜单项
      for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
      {
        if(i==Menu_Index)
		{
          DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)             ; // 显示选择条		
          ShowMenu_Item(y_Index,i+1,1)                                                 ;
		}
	    else
          ShowMenu_Item(y_Index,i+1,0)                                                 ; // 显示非被选菜单项
	    y_Index += Y_WIDTH_MENU                                                    ;
      }        

      y_Index_old     = 0                                                              ;
      First_Index_old = 0                                                              ;
    }
    else                                                                                 // 首菜单项改变 
    {
      Color_BK = WINDOW_BK_COLOR                                                       ;            
      if(First_Index_old<First_Index)                                                    // 菜单向上滚动
      {
        y_Index = Y_OFFSET-1+Y_WIDTH_MENU*(Dis_Menu_Num-1)                             ;
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // 清选中菜单项内容
        DrawRectFill(0,Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                      )    ; // 清空被选菜单项上部内容
      	y_Index = Y_OFFSET-1 + Y_WIDTH_MENU*(Dis_Menu_Num-1)                           ; // 自下而上显示被选菜单项
        for(i=0;i<Dis_Menu_Num;i++)
        {
		  if(i==0)
            ShowMenu_Item(y_Index,Menu_Index-i+1,1)                                    ;
	      else
            ShowMenu_Item(y_Index,Menu_Index-i+1,0)                                    ; // 显示非被选菜单项
	      y_Index -= Y_WIDTH_MENU                                                  ;
        }        
      }
      else
      {
	y_Index = Y_OFFSET-1                                                           ;
        DrawRectFill(0,Y_WIDTH_MENU+Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                          ); // 清空被选菜单项下部内容
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // 清选中菜单项内容
        for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
        {
          if(i==Menu_Index)
            ShowMenu_Item(y_Index,i+1,1)                                               ;
	      else
            ShowMenu_Item(y_Index,i+1,0)                                               ; // 显示非被选菜单项
	      y_Index += Y_WIDTH_MENU                                                  ;
        }        
      }
    }
    First_Index_old = First_Index                                                      ;
  }
  else                                                                                   // 首菜单项无变化
  {
    y_Index = y_Index_old-First_Index_old                                              ;
    y_Index = y_Index*Y_WIDTH_MENU+Y_OFFSET-1                                          ;
    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,WINDOW_BK_COLOR)                   ;            
    ShowMenu_Item(y_Index,y_Index_old+1,0)                                             ;
    y_Index = Menu_Index-First_Index                                                   ;
    y_Index = y_Index*Y_WIDTH_MENU+Y_OFFSET-1                                          ;
//    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,WINDOW_COLOR)                      ;
    DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)                   ;
    ShowMenu_Item(y_Index,Menu_Index+1,1)                                              ;
  }
  y_Index_old = Menu_Index                                                             ;
  Update_ScrollBar(Dis_X_MAX+2,Y_OFFSET-5,289-Y_OFFSET,Menu_Index,Menu.MenuConfig[0])  ;
  _EINT()                                                                              ;
}

//=====================================================================//
//函数：UCHAR ShowMenu_Item(UCHAR y,UINT* Menu_String,UCHAR Font_Color)
//描述：显示菜单项子函数
//参数：space_front  显示缩进值
//		y            Y轴坐标
//		Menu_String  菜单项链表的首地址指针
//=====================================================================//
void ShowMenu_Item(unsigned int y,unsigned char index,unsigned char Selected)
{
  unsigned char id[3]                               ;
  Color_BK = WINDOW_BK_COLOR                        ;
  Color    = WINDOW_COLOR                           ;
  DrawRectFill(15,y+3,22,21,Color_BK)               ; // 填充背景色   
  id[2]    = 0x00                                   ;
  id[1]    = 0x00                                   ;
  id[0]    = index-id[1]*10 + 0x30                  ;
  if(index<10)
  {
    PutStringEN24(20,y,id)                          ;
    DrawRect(13,y+1,26,24,WINDOW_BK_COLOR)          ;
  }
  else
  {
    switch(index)
    {
    case 10:
      PutStringCN24(15,y+2,"⒑")                    ;
      break                                         ;
    case 11:
      PutStringCN24(15,y+2,"⒒")                    ;
      break                                         ;
    case 12:
      PutStringCN24(15,y+2,"⒓")                    ;
      break                                         ;
    case 13:
      PutStringCN24(15,y+2,"⒔")                    ;
      break                                         ;
    case 14:
      PutStringCN24(15,y+2,"⒕")                    ;
      break                                         ;
    case 15:
      PutStringCN24(15,y+2,"⒖")                    ;
      break                                         ;
    case 16:
      PutStringCN24(15,y+2,"⒗")                    ;
      break                                         ;
    case 17:
      PutStringCN24(15,y+2,"⒘")                    ;
      break                                         ;
    case 18:
      PutStringCN24(15,y+2,"⒙")                    ;
      break                                         ;
    case 19:
      PutStringCN24(15,y+2,"⒚")                    ;
      break                                         ;
    case 20:
      PutStringCN24(15,y+2,"⒛")                    ;
      break                                         ;
    }
    DrawRectFill(35,y+18,3,4,Color_BK)              ; // 擦除多余的点
  } 
  DrawRect(14,y+2,24,22,Color)                      ;
  if(Selected)
    DrawRectFill(14,y,24,1,MENU_SELE_COLOR)         ; // 擦除超出编号框范围的点
  if(Selected==0)
  {
    Color_BK = WINDOW_BK_COLOR                      ;
    Color    = WINDOW_COLOR                         ;
  }
  else
  {
    Color_BK = MENU_SELE_COLOR                      ;
    Color    = WINDOW_BK_COLOR                      ;
  }
  PutString24M(50,y+2,Menu.ItemTitle[index-1])      ;
}

//=====================================================================//
//函数：void Redraw_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//描述：刷新整屏菜单显示函数
//参数：First_Index  当面显示页的第一条菜单号
//		Menu_Index   当前处于选用的菜单项
//		Menu_List    菜单资源链表指针
//=====================================================================//
void Redraw_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  First_Index_old   = 0xFF                           ;
  y_Index_old       = 0xFF                           ;
  Dis_Menu_Num = (Dis_Y_MAX+1-Y_OFFSET)/Y_WIDTH_MENU ;  
  UpDate_Menu(First_Index,Menu_Index)                ;  
}
//=====================================================================//
//函数：void Initial_Menu(UINT** Menu_List)
//功能：刷新整屏菜单显示函数
//参数：Menu_List - 菜单资源链表指针
//=====================================================================//
void Initial_Menu(void)
{
  First_Index_old   = 0xFF                                ;
  y_Index_old       = 0xFF                                ;
  Dis_Menu_Num      = (Dis_Y_MAX+1-Y_OFFSET)/Y_WIDTH_MENU ;       
  Active_Index = 0x00                                     ;
  First_Index  = Active_Index                             ;
  if(First_Index<Dis_Menu_Num)
    First_Index    = 0x00                                 ;
  while(First_Index>Dis_Menu_Num)
    First_Index   -= (Dis_Menu_Num-1)                     ;
  UpDate_Menu(First_Index,Active_Index)                   ;
  Color_BK = STATUS_BK_COLOR                              ;
  Color    = STATUS_COLOR                                 ;
  PutStringCN24(10,294,"确认")                            ; // 底栏
  if(Menu.MenuIndex>0)
    PutStringCN24(185,294,"返回")                         ; 
  else
    DrawRectFill(185,294,54,25,STATUS_BK_COLOR)           ;
}
//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void MenuMessage(unsigned char Message)                    
{
  _DINT()                                                               ;
  switch(Message)
  {
  case Update:
    Redraw_Menu(First_Index,Active_Index)                               ;
    break                                                               ;
  case Up:
    if(Active_Index==0) 
      break                                                             ;
    else 
      Active_Index--                                                    ;
    if(Active_Index<First_Index)
      if(First_Index>0)       First_Index --                            ;
    UpDate_Menu(First_Index,Active_Index)                               ;
    break                                                               ;
  case Down:
    if(Active_Index==Menu.MenuConfig[0]-1)
      break                                                             ;
    else 
    {
      Active_Index++                                                    ;
      if(Active_Index>First_Index+6)
        First_Index++                                                   ;
      UpDate_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  case Left:
    if(First_Index-7>0)
    {
      First_Index   -= 7                                                ;
      Active_Index   = First_Index + 6                                  ;  
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    else if(First_Index!=0)
    {
      First_Index    = 0                                                ;
      Active_Index   = First_Index                                      ;  
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  case Right:
    if(First_Index+Dis_Menu_Num<Menu.MenuConfig[0])
    {
      First_Index   += Dis_Menu_Num                                     ;
      Active_Index   = First_Index                                      ;
      if(First_Index+7>Menu.MenuConfig[0])
      {
        Active_Index = Menu.MenuConfig[0]-1                             ;
        First_Index  = Active_Index-6                                   ;
      }
      Redraw_Menu(First_Index,Active_Index)                             ;
    }
    break                                                               ;
  }
  MenuUpdated = 0x00                                                    ;
  _EINT()                                                               ;
}

void Update_ScrollBar(unsigned int x,unsigned int y,unsigned char height,
                      unsigned char position,unsigned char total         )
{
  unsigned int temp                                       ;
  DrawRectFill(x,y,8,height,WINDOW_BK_COLOR)              ; // 外框
  DrawRect(x,y,8,height,WINDOW_COLOR)                     ; 
  temp = y+(unsigned int)(height-44)*position/(total-1)   ;
  DrawRectFill(x,temp,8,44,WINDOW_COLOR)                  ; // 滑块
}


void LoadMenu(unsigned char Menu_Index,unsigned char Permission)
{
  unsigned char i,j,k                                                                ; // 导入菜单配置项
  Menu.MenuIndex =  Menu_Index                                                       ;
  str_cpy((char *)Menu.MenuConfig,(char *)Menus[Menu_Index].MenuConfig,10)           ; // 拷贝菜单配置项
  strcpy((char *)Menu.MenuTitle,(char *)Menus[Menu_Index].MenuTitle)                 ; // 拷贝菜单标题
  Menu.MenuConfig[0] = 0                                                             ; // 菜单项数清0
  for(i=1;i< Menus[Menu_Index].MenuConfig[0]+1;i++)                                    // 导入菜单标题字符串
  {
    if(Permission>=(MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].AccessLevel))
    {
      j = Menu.MenuConfig[0]++                                                       ; // 计算菜单项数目
      Menu.ItemIndex[j] = Menus[Menu_Index].ItemIndex[i-1]                           ; // 记录菜单项索引
      for(k=0;k<20;k++)
        Menu.ItemTitle[j][k] = 0x00                                                  ;
      strcpy((char *)Menu.ItemTitle[j],
             (char *)MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].ItemTitle)          ; // 拷贝菜单项标题
      Menu.ItemConfig[j] =  MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].Config       ;     // 拷贝菜单项配置字      
    }    
  }
}

unsigned char GetFirstItem(void)
{
  return First_Index                      ;
}
unsigned char GetActiveItem(void)
{
  return Active_Index                     ;
}
unsigned char GetActiveItemIndex(void)
{
  return Menu.ItemIndex[Active_Index]     ;
}

unsigned char GetMenuIndex(void)
{
  return Menu.MenuIndex                   ;
}