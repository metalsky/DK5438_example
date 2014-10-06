//===========================================================================//
//                                                                           //
// 文件：  GUI.C                                                             //
// 说明：  BW-DK5438开发板图形用户界面函数文件, 支持uC/OS-II                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//
#include <ucos_ii.h>
#include "string.h"
#include "TFTDRV.H"
#include "GUI.H"
#include "MENU_RESOURCE.H"
#include "PIN_DEF.H"

unsigned char Active_Edit_Dig  = 0xFF           ; // 活动的数字编辑控件
unsigned char Active_Digit     = 0xFF           ;
unsigned char flicker_status                    ;
unsigned char Edit2Read        = 0xFF           ; // 读数据
unsigned char ReadDigit        = 0xFF           ; // 读数据位

extern unsigned      Color,Color_BK             ;
extern OS_EVENT     *MBKeyTP                    ;
extern OS_EVENT     *MBFunc                     ;
extern OS_EVENT     *SPIMutex                   ;
extern EDIT          Edit_Dig[10]               ; // 编辑控件
extern BUTTON        Button[10]                 ; // 按钮控件
extern unsigned char DISP_MODE                  ; // 编辑控件显示模式
extern char          MBFuncBuffer[6]            ; // 函数操作邮箱缓冲区


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ShowMenu_Item(unsigned char ,unsigned char,unsigned char)       //
//描述：显示菜单项                                                           //
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

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ActivateMenu_Item(unsigned char,unsigned char)                  //
//描述：激活菜单项                                                           //
//参数：no    ——显示位置序号                                               //
//      mode  ——显示模式                                                   //
//		                                                             //
//---------------------------------------------------------------------------//
void ActivateMenu_Item(unsigned char no,unsigned char mode)
{
  if(mode)
    DrawRect(12+no%3*80,33+no/3*84,56,54,Color)                          ; 
  else
    DrawRect(12+no%3*80,33+no/3*84,56,54,Color_BK)                       ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：InactivateItem(unsigned char)                                        //
//描述：设置图标菜单项无效                                                   //
//参数：Item_No ——菜单项索引                                               //
//		                                                             //
//---------------------------------------------------------------------------//
void InactivateItem(unsigned char Item_No)
{
  DrawRect(12+Item_No%3*80,33+Item_No/3*84,56,54,Color_BK)               ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：ActivateItem(unsigned char)                                          //
//描述：设置图标菜单项有效                                                   //
//参数：Item_No ——菜单项索引                                               //
//		                                                             //
//---------------------------------------------------------------------------//
void ActivateItem(unsigned char Item_No)
{
  DrawRect(12+Item_No%3*80,33+Item_No/3*84,56,54,Color)                  ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：RunMenu(unsigned char)                                               //
//描述：菜单运行函数                                                         //
//参数：index ——菜单项索引                                                 //
//		                                                             //
//---------------------------------------------------------------------------//
void RunMenu(unsigned char index)
{
  char key,i,Active_Item  = 0x00,rx_char                                 ;
  char Event                                                             ;
  union
  {
    unsigned int  pos[2]                                                 ;
    char byte[4]                                                         ;
  }TPoint                                                                ;
  INT8U err[1]                                                           ;
  char *ptmb                                                             ;
Ini_Background:  
  Color     = Black                                                      ;
  Color_BK  = 0xEF9F                                                     ;
  Clear_LCD(Color_BK)                                                    ;
  DrawRectFill(0,0,240,25,Blue)                                          ;
  DrawRectFill(0,295,240,25,Blue)                                        ;
  Color    = 0xFFFF                                                      ;
  Color_BK = Blue                                                        ;
  PutStringCN16(4,4,(unsigned char *)Menus[index].MenuTitle)             ;
  PutStringEN16(80,302,"BW-DK5438")                                      ;
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
    ptmb           = OSMboxPend(MBKeyTP,0,err)                           ;
    Event          = *(ptmb++)                                           ;
    key            = *(ptmb++)                                           ;
    rx_char        = *(ptmb++)                                           ;
    TPoint.byte[0] = *(ptmb++)                                           ;
    TPoint.byte[1] = *(ptmb++)                                           ;
    TPoint.byte[2] = *(ptmb++)                                           ;
    TPoint.byte[3] = *(ptmb++)                                           ;
    rx_char        = rx_char                                             ;    
    if(Event&EVENT_KEY_PRESSED)
    {
      switch(key)
      {
      case Right:
        InactivateItem(Active_Item)                                      ;
        if(++Active_Item>=Menus[index].MenuConfig[0])                      // 超过菜单项数
          Active_Item = 0x00                                             ; // 回到起始项
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case Left:
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==0) 
          Active_Item = Menus[index].MenuConfig[0]-1                     ; // 在起始项
        else                                                               // 到最后一项
          Active_Item--                                                  ;
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
/* 菜单项不足9项时，向上、向下键行为较为怪异，取消
      case 8:                                                              // 做向上键用
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==0)                                                 // 在起始项
          Active_Item = Menus[index].MenuConfig[0]-1                     ; // 到最后一项
        else if(Active_Item==1||Active_Item==2)
          Active_Item = Active_Item+5                                    ;
        else 
          Active_Item = Active_Item-3                                    ;          
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
      case 0:                                                              // 做向下键用
        InactivateItem(Active_Item)                                      ;
        if(Active_Item==8) 
          Active_Item = 0x00                                             ;
        else if(Active_Item==6||Active_Item==7)
          Active_Item = Active_Item-5                                    ;
        else 
          Active_Item = Active_Item+3                                    ;          
        ActivateItem(Active_Item)                                        ;
        break                                                            ;
*/        
      case Enter:
        Event       = EVENT_OP_STARTED                                   ;
        MBFuncBuffer[0] = Event                                          ;
        MBFuncBuffer[1] = Active_Item                                    ;
        OSMboxPost(MBFunc,(void *)MBFuncBuffer)                          ;
        OSTaskSuspend(GUI_TASK_PRIO)                                     ;
        goto Ini_Background                                              ;
      default:
        break                                                            ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      for(i=0;i<Menus[index].MenuConfig[0];i++)
      {        
        if(  (TPoint.pos[0]<(60+i%3*80)&&TPoint.pos[0]>(20+i%3*80))
           &&(TPoint.pos[1]<(95+i/3*84)&&TPoint.pos[1]>(55+i/3*82)))
        {
          if(i!=Active_Item)
          {
            ActivateMenu_Item(Active_Item,0)                             ;
            ActivateMenu_Item(i,1)                                       ;
            Active_Item  = i                                             ;
          }
        }
      }      
    }
    else if(Event&EVENT_TP_PRESSED)
    {
      for(i=0;i<Menus[index].MenuConfig[0];i++)
      {        
        if(  (TPoint.pos[0]<(60+i%3*80)&&TPoint.pos[0]>(20+i%3*80))
           &&(TPoint.pos[1]<(95+i/3*84)&&TPoint.pos[1]>(55+i/3*82)))
        {
          Event       = EVENT_OP_STARTED                                 ;
          Active_Item = i                                                ;
          MBFuncBuffer[0] = Event                                        ;
          MBFuncBuffer[1] = Active_Item                                  ;
          OSMboxPost(MBFunc,(void *)MBFuncBuffer)                        ;
          OSTaskSuspend(GUI_TASK_PRIO)                                   ;
          goto Ini_Background                                            ;
        }
      }
    }
  }
}
 
//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CreateEditDig(unsigned char，                                   //
//                         unsigned int ,unsigned int,                       //
//                         unsigned char,unsigned char)                      //
//描述：创建数字编辑控件                                                     //
//参数：ID     —— 编辑控件索引号                                           //
//      x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
void CreateEditDig(unsigned char ID   ,
                   unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)
{
  unsigned char i                           ;
  Color    = WINDOW_COLOR                   ; 
  Color_BK = 0xFFFF                         ;   
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].status&EDIT_VALID) 
	  continue                          ;
    else
    {
      Edit_Dig[i].status  |= EDIT_VALID     ; // 设置有效标志
      Edit_Dig[i].id       = ID             ; // 设置标识
      Edit_Dig[i].x        = x_s            ; // 设置坐标
      Edit_Dig[i].y        = y_s            ;
      Edit_Dig[i].width    = width          ; // 设置大小
      Edit_Dig[i].height   = height         ;
      break                                 ;
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void IniEditDig(unsigned char，unsigned char, unsigned char,         //
//                      unsigned char, unsigned char, char*         )        //
//描述：初始化数字编辑控件                                                   //
//参数：ID     —— 编辑控件索引号                                           //
//      type   —— 类型，插入式/删除式                                      //
//      font   —— 字体                                                     //
//      frame  —— 边框类型                                                 //
//      limit  —— 字符数限制                                               //
//      p      —— 初值字符串                                               //
//		                                                             //
//---------------------------------------------------------------------------//
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p            )
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                    ;
	else
	{ 
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                     ;	  
	  if(type!=0)
	    Edit_Dig[i].status    |= EDIT_MODE                        ; // 设置编辑模式
	  else
	    Edit_Dig[i].status    &=~EDIT_MODE                        ; 
	  if(font!=0)
	    Edit_Dig[i].status    |= EDIT_FONT                        ; // 设置字体
	  else
	    Edit_Dig[i].status    &=~EDIT_FONT                        ; 
	  Edit_Dig[i].property     = frame<<5                         ; // 设置边框属性
	  Edit_Dig[i].property    += limit                            ; // 设置最大字符数  
	  for(j=0;j<strlen(p);j++)
	    Edit_Dig[i].string[j]  = p[j]                             ;	    
//   	  Edit_Dig[i].string[limit]= 0x00                             ; 
   	  Edit_Dig[i].string[j]= 0x00                                 ; 
	  RedrawEditDig(i)                                            ;    
	  break                                                       ;
	}
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetEditDig(unsigned char，char*)                                //
//描述：初始化数字编辑控件                                                   //
//参数：ID     —— 编辑控件索引号                                           //
//      p      —— 设置字符串                                               //
//		                                                             //
//---------------------------------------------------------------------------//
void SetEditDig(unsigned char ID,char* p)
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                    ;
	else
	{ 
          if(Edit_Dig[i].status&EDIT_ACTIVE)
	    Active_Digit           = 0                                ;
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                     ;	  
	  for(j=0;j<strlen(p);j++)
	    Edit_Dig[i].string[j]  = p[j]                             ;	    
   	  Edit_Dig[i].string[j]    = 0x00                             ; 
	  RedrawEditDig(i)                                            ;    
	  break                                                       ;
	}
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ActiveEditDig(unsigned char，unsigned char)                     //
//描述：初始化数字编辑控件                                                   //
//参数：ID     —— 编辑控件索引号                                           //
//      Active —— 激活状态：1-激活 2-休眠                                  // 
//		                                                             //
//---------------------------------------------------------------------------//
void ActiveEditDig(unsigned char ID   ,unsigned char Active)
{
  unsigned char i                                                       ;
  Color    = WINDOW_COLOR                                               ; 
  Color_BK = 0xFFFF                                                     ;   
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                      ;
    else
    {
      if(!(Edit_Dig[i].status&EDIT_VALID))
	break                                                           ;
      else
      {
        if(Active)
        {
          if(Active_Edit_Dig!=0xFF)
	  {
            Edit_Dig[Active_Edit_Dig].status |= EDIT_ACTIVE             ; // 如有活动控件，取消其活动状态并消隐其光标显示
	    RedrawEditDig(Active_Edit_Dig)                              ;                                               		  
	  }
          Active_Edit_Dig     = i                                       ;
	  Edit_Dig[i].status |= EDIT_ACTIVE                             ; // 设置激活状态
	  Active_Digit = strlen(Edit_Dig[i].string)                     ; // 光标设置在最后一位
          RedrawEditDig(i)                                              ;
        }
        else                                                              // 取消控件活动状态
	{
          Edit_Dig[i].status &=~EDIT_ACTIVE                             ; 
          if(Active_Edit_Dig == i)
            Active_Edit_Dig  =  0xFF                                    ;
          RedrawEditDig(i)                                              ;
	}
      }
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ReadEditDig(unsigned char，char*)                               //
//描述：初始化数字编辑控件                                                   //
//参数：ID      —— 编辑控件索引号                                          //
//      Ret_Buf —— 读数缓冲区                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)
{
  unsigned char i                                                       ;
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                      ;
	else
	{	  
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                       ;
	  else
	  {	
            strcpy((char *)Ret_Buf,Edit_Dig[i].string)                  ;
	    Edit2Read   = i                                             ; // 读数据
            ReadDigit   = 0x00                                          ; // 读数据位
	  }
	}
  }
}  

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void EditDigMessage(unsigned char，char*)                            //
//描述：编辑控件消息处理                                                     //
//参数：Message —— 消息                                                    //
//      p       —— 中文输入字符缓冲区                                      //
//		                                                             //
//---------------------------------------------------------------------------//
void EditDigMessage(unsigned char Message,char *p)                    
{
  unsigned char i,temp,k                                                ;
  Color    = WINDOW_COLOR                                               ; 
  Color_BK = 0xFFFF                                                     ;   
  i      = Active_Edit_Dig                                              ;
  Sweep_Cursor(i)                                                       ; // 
  if(Message==129)                                                        // 输入了中文字符
  {
    if(strlen(Edit_Dig[i].string)+1<(Edit_Dig[i].property&EDIT_LIMIT))
    {
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+2] = 0x00           ; // 插入字符
      for(k=strlen(Edit_Dig[i].string)+1;k>Active_Digit;k--)                
        Edit_Dig[i].string[k] = Edit_Dig[i].string[k-2]                 ; 
      Edit_Dig[i].string[Active_Digit++] = *p                           ;
      Edit_Dig[i].string[Active_Digit++] = *(++p)                       ;
      RedrawEditDig(i)                                                  ;
    }
  }
  switch(Message)
  {
  case 128:                                                               // Message=128  -- 重画Edig控件
    RedrawEditDig(i)                                                    ;
  case 0xFF:                                                              // Message=0xFF -- 退出读取
    break                                                               ;
  case Left:
    if(Active_Digit==0) 
      Active_Digit = strlen(Edit_Dig[i].string)                         ; 
    else                
      Active_Digit--                                                    ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    flicker_status = 0x00                                               ;
    Flicker_Cursor()                                                    ;
    break                                                               ;
  case Right:
    if(Active_Digit==strlen(Edit_Dig[i].string))
      Active_Digit = 0                                                  ; 
    else              
      Active_Digit++                                                    ;
    if(Edit_Dig[i].string[Active_Digit-1]>128)
      Active_Digit++                                                    ;
    flicker_status = 0x00                                               ;
    Flicker_Cursor()                                                    ;
    break                                                               ;
  case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:case 0:
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': 
  case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': 
  case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
  case 'Y': case 'Z':
    if(strlen(Edit_Dig[i].string)<(Edit_Dig[i].property&EDIT_LIMIT))
    {
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+1] = 0x00           ; // 插入字符
      for(k=strlen(Edit_Dig[i].string);k>Active_Digit;k--)                
        Edit_Dig[i].string[k] = Edit_Dig[i].string[k-1]                 ; 
      if(Message<=9)
        Edit_Dig[i].string[Active_Digit] = Message+0x30                 ;
      else
        Edit_Dig[i].string[Active_Digit] = Message                      ;
      Active_Digit++                                                    ;
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  case Delete:
    if(strlen(Edit_Dig[i].string)>0)
    {
      temp = strlen(Edit_Dig[i].string)                                 ;
     if(Edit_Dig[i].string[Active_Digit]>128)
        for(k=Active_Digit;k<temp-1;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // 删除下一中文字符
      else  
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // 删除下一西文字符
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  case Backspace:
    if(Active_Digit==0)     break                                       ;
    Active_Digit--                                                      ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    if(strlen(Edit_Dig[i].string)>0)
    {
      temp = strlen(Edit_Dig[i].string)                                 ;
      if(Edit_Dig[i].string[Active_Digit]>128)
        for(k=Active_Digit;k<temp-1;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // 删除下一中文字符
      else
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // 删除下一字符
      RedrawEditDig(i)                                                  ;
    }
    break                                                               ;
  default:
    break                                                               ;
  }
  flicker_status = 0x00                                                 ;
}


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void RedrawEditDig(unsigned char)                                    //
//描述：重绘编辑控件                                                         //
//参数：Index —— 编辑控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void RedrawEditDig(unsigned char Index)                    
{
  unsigned char  i,j                                                    ;
  char* p                                                               ;
  unsigned char string[20]                                              ;
  i        = Index                                                      ;
  p        = Edit_Dig[i].string                                         ;
  Color    = Black                                                      ;
  Color_BK = White                                                      ;
  DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	       Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)        ; // 清除显示 
  if(Edit_Dig[i].property&EDIT_FRAME)                                     // 需绘制边框
  {
    DrawVerticalLine(Edit_Dig[i].x-1,Edit_Dig[i].y,
                     Edit_Dig[i].height+1,Dark_Grey)                    ;
    DrawHorizonLine(Edit_Dig[i].x-1,Edit_Dig[i].y-1,
                    Edit_Dig[i].width+2,Dark_Grey  )                    ;
    DrawVerticalLine(Edit_Dig[i].x+Edit_Dig[i].width,Edit_Dig[i].y,
                     Edit_Dig[i].height+1,Light_Grey               )    ;
    DrawHorizonLine(Edit_Dig[i].x-2,Edit_Dig[i].y+Edit_Dig[i].height,
                    Edit_Dig[i].width+3,Light_Grey                   )  ;
  }
  if(DISP_MODE==0x00)
  {
    if(Edit_Dig[i].status&EDIT_FONT)
      PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6,          // 居中显示
	          Edit_Dig[i].y+Edit_Dig[i].height/2-12,(unsigned char*)p);
    else
      PutStringEN16(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*9/2,      // 居中显示
	            Edit_Dig[i].y+Edit_Dig[i].height/2-7,(unsigned char*)p);      
  }
  else
  {
    for(j=0;j<strlen(p);j++)
      string[j]  = '*'                                                  ;
    string[j]    = 0x00                                                 ;
    if(Edit_Dig[i].status&EDIT_FONT)
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,string    )       ;
    else
    PutStringCN16(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,          
	        Edit_Dig[i].y+Edit_Dig[i].height/2-5,string    )        ;
      
  }
  flicker_status = 0x00                                                 ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void DeleteEditDig(unsigned char)                                    //
//描述：删除编辑控件                                                         //
//参数：Index —— 编辑控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void DeleteEditDig(unsigned char ID)
{
  unsigned char i                                                           ;
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                           ;
	  else
	  {	
	    Edit_Dig[i].status   &=~EDIT_VALID                              ; // 清除有效状态
	    if(Edit_Dig[i].status&EDIT_ACTIVE)		  
	      Active_Edit_Dig = 0xFF                                        ; // 消隐光标
	    Edit_Dig[i].status   &=~EDIT_ACTIVE                             ; // 清除活动状态		
	    DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	                 Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)  ; // 清除显示 		
	  }
	}
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Flicker_Cursor(void)                                            //
//描述：编辑控件光标闪烁                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
void Flicker_Cursor(void)
{
  unsigned int temp                                                   ;
  static unsigned char flicker_status = 0x00                          ;                   
  if(Active_Edit_Dig!=0xFF)
  {
    if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
    {
      temp   = Edit_Dig[Active_Edit_Dig].x
	      +Edit_Dig[Active_Edit_Dig].width/2
	      -strlen(Edit_Dig[Active_Edit_Dig].string)*6             ;
      temp  += Active_Digit*13-1                                      ; // 计算光标横坐标
    }
    else
    {
      temp   = Edit_Dig[Active_Edit_Dig].x
	      +Edit_Dig[Active_Edit_Dig].width/2
	      -strlen(Edit_Dig[Active_Edit_Dig].string)*9/2           ;
      temp  += Active_Digit*9-2                                       ; // 计算光标横坐标
    }
    if(flicker_status++==0x00)
      if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
        DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color)    ; // 绘制光标
      else
        DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-8,16,Color)    ; // 绘制光标
    else
    {
      if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
        DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color_BK) ; // 消隐光标
      else
        DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-8,16,Color_BK) ; 
      flicker_status = 0x00                                           ;
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Sweep_Cursor(void)                                              //
//描述：消除编辑控件光标                                                     //
//参数：Index —— 编辑控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void Sweep_Cursor(unsigned char Index)
{
  unsigned int temp                                                   ;
  INT8U    err                                                        ;
  if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
  {
    temp   = Edit_Dig[Index].x
	    +Edit_Dig[Index].width/2
	    -strlen(Edit_Dig[Index].string)*6                         ;
    temp  += Active_Digit*13-1                                        ; // 计算光标横坐标
  }
  else
  {
    temp   = Edit_Dig[Index].x
	    +Edit_Dig[Index].width/2
	    -strlen(Edit_Dig[Index].string)*9/2                       ;
    temp  += Active_Digit*9-2                                         ; // 计算光标横坐标
  }  
  OSMutexPend(SPIMutex,0,&err)                                        ;
  if(Edit_Dig[Index].status&EDIT_FONT)
    DrawVerticalLine( temp,Edit_Dig[Index].y+Edit_Dig[Index].height/2-10,22,Color_BK); // 消隐光标
  else
    DrawVerticalLine( temp,Edit_Dig[Index].y+Edit_Dig[Index].height/2-8,16,Color_BK) ; 
  flicker_status = 0x01                                               ;
  OSMutexPost(SPIMutex)                                     ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CreateButton (unsigned char,char *      ,                       //
//                         unsigned int ,unsigned int,                       //
//                         unsigned char,unsigned char)                      //
//描述：创建数字编辑控件                                                     //
//参数：ID     —— 按钮索引号                                               //
//      title  —— 按钮文字                                                 //
//      x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
void CreateButton( unsigned char ID   ,char *title     ,
                   unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)
{
  unsigned char i                           ;
  Color    = WINDOW_COLOR                   ; 
  Color_BK = 0xFFFF                         ;   
  for(i=0;i<10;i++)
  {
    if(Button[i].status&BUTTON_VALID) 
	  continue                          ;
    else
    {
      Button[i].status  |= BUTTON_VALID     ; // 设置有效标志
      Button[i].status  |= BUTTON_ACTIVE    ; // 设置为活动控件
      Button[i].id       = ID               ; // 设置标识
      Button[i].x        = x_s              ; // 设置坐标
      Button[i].y        = y_s              ;
      Button[i].width    = width            ; // 设置大小
      Button[i].height   = height           ;
      strcpy(Button[i].Title,title)         ; // 设置按钮文字
      RedrawButton(i)                       ;
      break                                 ;
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetButtonDown(unsigned char)                                    //
//描述：设置按键按下                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void SetButtonDown(unsigned char Index)
{
  unsigned char i                           ;
  for(i=0;i<10;i++)
  {
    if(Button[i].id!=Index) 
	  continue                          ;
    else
    {
      Button[i].status  |= BUTTON_DOWN      ; // 设置标志
      RedrawButton(i)                       ;
      break                                 ;
    }
  }  
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetButtonDown(unsigned char)                                    //
//描述：设置按键按下                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void SetButtonUp(unsigned char Index)
{
  unsigned char i                           ;
  for(i=0;i<10;i++)
  {
    if(Button[i].id!=Index) 
	  continue                          ;
    else
    {
      Button[i].status  &=~BUTTON_DOWN      ; // 设置标志
      RedrawButton(i)                       ;
      break                                 ;
    }
  }  
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void InScopeButton(unsigned，unsigned)                               //
//描述：查询点所在位置的按钮控件                                             //
//参数：x,y    ——点坐标                                                    //
//返回：0xFF   ——无按钮                                                    //
//      0~0xFE ——按钮控件索引                                              //    
//		                                                             //
//---------------------------------------------------------------------------//
#define X_MARGIN  5
#define Y_MARGIN  0
unsigned char InScopeButton(unsigned int x,unsigned int y)
{
  unsigned char i                                           ;
  for(i=0;i<10;i++)
  {
    if(!(Button[i].status&BUTTON_VALID))
      continue                                              ;
    if(  x<Button[i].x+X_MARGIN
       ||x>Button[i].x+Button[i].width-X_MARGIN
       ||y<Button[i].y+Y_MARGIN
       ||y>Button[i].y+Button[i].height-Y_MARGIN) 
	  continue                                          ;
    else
      return Button[i].id                                   ;
  }  
  return  0xFF                                              ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetButtonTitle(unsigned char)                                   //
//描述：设置按钮文字                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void SetButtonTitle(unsigned char ID,char *new_title)
{
  unsigned char i                                           ;
  for(i=0;i<10;i++)
  {
    if(Button[i].id != ID) 
	  continue                                          ;
    else
    {	  
      if(!(Button[i].status&BUTTON_VALID))
	 break                                              ;
      else
      {	
	strcpy(Button[i].Title,new_title)                   ; 
        RedrawButton(i)                                     ;  
        break                                               ; 
      }
    }
  }
}
//---------------------------------------------------------------------------//
//                                                                           //
//函数：void RedrawButton(unsigned char)                                     //
//描述：重绘编辑控件                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void RedrawButton(unsigned char Index)
{
  unsigned char i                                                       ;
  char* p                                                               ;  
  i        = Index                                                      ;
  p        = Button[i].Title                                            ;
  if(Button[i].status&BUTTON_DOWN)
  {
    DrawRect(Button[i].x-3,Button[i].y-3,
             Button[i].width+6,Button[i].height+6,Dark_Grey2)           ;      
    DrawRectFill(Button[i].x-2,Button[i].y-2,
	         Button[i].width+4, Button[i].height+4,Grey2)           ; // 清除显示   
    DrawRect(Button[i].x-2,Button[i].y-2,
	     Button[i].width+4, Button[i].height+4,Dark_Grey)           ; // 清除显示   
    Color_BK   = Grey2                                                  ;
    if(Button[i].status&BUTTON_ACTIVE)
      Color      = Black                                                ;
    else
      Color      = Dark_Grey                                            ;
    PutStringCN16(Button[i].x+Button[i].width/2-strlen(p)*4+2,            // 居中显示
	          Button[i].y+Button[i].height/2-7,(unsigned char*)p)   ;
    return                                                              ;
  }
  DrawRectFill(Button[i].x,Button[i].y,
	       Button[i].width, Button[i].height,Grey2)                 ; // 清除显示   
  DrawVerticalLine(Button[i].x-2,Button[i].y-2,
                   Button[i].height+4,White)                            ;
  DrawRect(Button[i].x-1,Button[i].y-1,
           Button[i].width+2,Button[i].height+2,Light_Grey)             ;
  DrawHorizonLine(Button[i].x-2,Button[i].y-2,
                   Button[i].width+4,White)                             ;
  DrawVerticalLine(Button[i].x+Button[i].width+1,Button[i].y-2,
                   Button[i].height+4,Dark_Grey)                        ;
  DrawHorizonLine(Button[i].x-1,Button[i].y+Button[i].height+2,
                   Button[i].width+3,Dark_Grey)                         ; 
  DrawRect(Button[i].x-3,Button[i].y-3,
           Button[i].width+6,Button[i].height+6,Dark_Grey2)             ;  
  Color_BK   = Grey2                                                    ;
  if(Button[i].status&BUTTON_ACTIVE)
    Color      = Black                                                  ;
  else
    Color      = Dark_Grey                                              ;
  PutStringCN16(Button[i].x+Button[i].width/2-strlen(p)*4,                  // 居中显示
	        Button[i].y+Button[i].height/2-8,(unsigned char*)p)     ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void DeleteButton(unsigned char)                                     //
//描述：删除按钮控件                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void DeleteButton(unsigned char ID)
{
  unsigned char i                                                           ;
  for(i=0;i<10;i++)
  {
    if(Button[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Button[i].status&BUTTON_VALID))
	    break                                                           ;
	  else
	  {	
	    Button[i].status    = 0x00                                      ; // 清除有效状态
	    DrawRectFill(Button[i].x-3,Button[i].y-3,
	                 Button[i].width+6, Button[i].height+6,Color_BK)    ; // 清除显示 		
	  }
	}
  }
}


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void DrawPanel ( unsigned int ,unsigned int,                         //
//                       unsigned char,unsigned char)                        //
//描述：画操作面板                                                           //
//参数：x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
#define DRAW_LEFT 26
void DrawPanel(unsigned int x_s   ,unsigned int y_s,
               unsigned char width,unsigned char height,char *Title)
{
  DrawRect(x_s,y_s,width,height,Dark_Grey)           ;
  DrawVerticalLine(x_s+1,y_s+1,height-2,White)       ;
  DrawVerticalLine(x_s+width+1,y_s+1,height+1,White) ;
  DrawHorizonLine(x_s+1,y_s+1,width-2,White)         ;
  DrawHorizonLine(x_s,y_s+height+1,width+2,White)    ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  PutStringCN16(x_s+width/2-strlen(Title)*4-DRAW_LEFT, // 可设置缩进
	        y_s-8,(unsigned char*)Title)         ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void DrawFrame ( unsigned int ,unsigned int,                         //
//                       unsigned char,unsigned char)                        //
//描述：画凹型边框                                                           //
//参数：x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
#define DARK3    0x9CD3
#define DARK2    0xA514
#define DARK1    0xB596
#define LIGHT3   0xCE59
#define LIGHT2   0xCE79
#define LIGHT1   0xD69A

void DrawFrame(unsigned int x_s   ,unsigned int y_s,
               unsigned char width,unsigned char height)
{
  DrawHorizonLine(x_s,y_s,width,DARK3)                   ;
  DrawHorizonLine(x_s-1,y_s-1,width+2,DARK2)             ;
  DrawHorizonLine(x_s-2,y_s-2,width+4,DARK1)             ;
  DrawVerticalLine(x_s,y_s,height,DARK3)                 ;
  DrawVerticalLine(x_s-1,y_s-1,height+2,DARK2)           ;
  DrawVerticalLine(x_s-2,y_s-2,height+4,DARK1)           ;
  
  DrawHorizonLine(x_s,y_s+height-1,width,LIGHT3)         ;
  DrawHorizonLine(x_s-1,y_s+height,width+2,LIGHT2)       ;
  DrawHorizonLine(x_s-2,y_s+height+1,width+4,LIGHT1)     ;    
  DrawVerticalLine(x_s+width,y_s,height,LIGHT3)          ;
  DrawVerticalLine(x_s+width+1,y_s-1,height+2,LIGHT2)    ;
  DrawVerticalLine(x_s+width+2,y_s-2,height+4,LIGHT1)    ;
}


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CreateView(VIEW *      ,                                        //
//                     unsigned int ,unsigned int,                           //
//                     unsigned char,unsigned char)                          //
//描述：创建数字编辑控件                                                     //
//参数：view   —— 视图控件指针                                             //
//      x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
void CreateView(VIEW *view         ,unsigned int limit,
                unsigned int x_s   ,unsigned int y_s,
                unsigned char width,unsigned char height)
{
  Color            = WINDOW_COLOR                      ; 
  Color_BK         = 0xFFFF                            ;   
  (*view).x        = x_s                               ; // 设置坐标
  (*view).y        = y_s                               ;
  (*view).width    = width                             ; // 设置大小
  (*view).height   = height                            ;
  (*view).length   = 0x00                              ; // 当前字符串长度
  (*view).positionx= x_s+4                             ; // 下一字符显示位置X坐标 
  (*view).positiony= y_s+6                             ; // 下一字符显示位置Y坐标   
  (*view).limit    = limit                             ; // 下一字符显示位置Y坐标     
  RedrawView(view)                                     ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void RedrawButton(unsigned char)                                     //
//描述：重绘编辑控件                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void RedrawView(VIEW *view)
{
  DrawRectFill((*view).x,(*view).y,
               (*view).width,(*view).height,White)        ;
  DrawVerticalLine((*view).x-1,(*view).y,
                   (*view).height+1,Dark_Grey)            ;
  DrawHorizonLine((*view).x-1,(*view).y-1,
                   (*view).width+2,Dark_Grey)             ;
  DrawVerticalLine((*view).x+(*view).width,(*view).y,
                   (*view).height+1,Light_Grey)           ;
  DrawHorizonLine((*view).x-2,(*view).y+(*view).height,
                   (*view).width+3,Light_Grey)            ;  
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ViewMessage(unsigned char，char*)                               //
//描述：编辑控件消息处理                                                     //
//参数：Message —— 消息                                                    //
//      p       —— 中文输入字符缓冲区                                      //
//		                                                             //
//---------------------------------------------------------------------------//
void ViewMessage(VIEW *view,char Message,unsigned char *p)                    
{
  Color    = WINDOW_COLOR                                               ; 
  Color_BK = 0xFFFF                                                     ;   
  if(Message==CN_INPUT)                                                   // 输入了中文字符
  {
    if((*view).length+2<(*view).limit)
    {
      (*view).string[(*view).length]   = *p++                           ; // 追加字符
      (*view).string[(*view).length+1] = *p                             ; 
      (*view).string[(*view).length+2] = 0x00                           ; 
      (*view).length                  += 2                              ;
      if((*view).positionx+16>(*view).x+(*view).width-2)
      {
        (*view).positionx       = (*view).x+4                           ;
        (*view).positiony      += 18                                    ;        
      }
      PutStringCN16((*view).positionx,(*view).positiony,(--p))          ;   
      if((*view).positionx+25>(*view).x+(*view).width-2)
      {
        (*view).positionx       = (*view).x+2                           ;
        (*view).positiony      += 16                                    ;        
      }      
      else
        (*view).positionx      += 16                                    ;        
    }
  }
  else if(Message==EN_INPUT)
  {
    if((*view).length+1<(*view).limit)
    {
      (*view).string[(*view).length]     = *p                           ; // 追加字符
      (*view).string[(*view).length+1]   = 0x00                         ;
      (*view).length++                                                  ;
      PutStringEN16((*view).positionx,(*view).positiony,p)              ;   
      if((*view).positionx+18>=(*view).x+(*view).width-2)
      {
        (*view).positionx       = (*view).x+4                           ;
        (*view).positiony      += 18                                    ;        
      }      
      else
        (*view).positionx      += 9                                     ; 
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ViewMessage(unsigned char，char*)                               //
//描述：编辑控件消息处理                                                     //
//参数：Message —— 消息                                                    //
//      p       —— 中文输入字符缓冲区                                      //
//		                                                             //
//---------------------------------------------------------------------------//
void ClearViewContent(VIEW *view)  
{
  DrawRectFill((*view).x,(*view).y,(*view).width,(*view).height,White)  ;
  (*view).length   = 0x00                                               ; // 当前字符串长度
  (*view).positionx= (*view).x+4                                        ; // 下一字符显示位置X坐标 
  (*view).positiony= (*view).y+6                                        ; // 下一字符显示位置Y坐标     
}


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void DeleteButton(unsigned char)                                     //
//描述：删除按钮控件                                                         //
//参数：Index —— 按钮控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void DeleteView(unsigned char ID)
{
  unsigned char i                                                           ;
  for(i=0;i<10;i++)
  {
    if(Button[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Button[i].status&BUTTON_VALID))
	    break                                                           ;
	  else
	  {	
	    Button[i].status   &=~BUTTON_VALID                              ; // 清除有效状态
	    if(Button[i].status&BUTTON_ACTIVE)		  
	      Active_Edit_Dig = 0xFF                                        ; // 消隐光标
	    Button[i].status   &=~EDIT_ACTIVE                               ; // 清除活动状态		
	    DrawRectFill(Button[i].x-3,Button[i].y-3,
	                 Button[i].width+6, Button[i].height+6,Color_BK)    ; // 清除显示 		
	  }
	}
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CreateCheck(CHEXK_BOX *,                                        //
//                       unsigned int ,unsigned int,                         //
//                       unsigned char,unsigned char)                        //
//描述：创建选择框控件                                                       //
//参数：check  —— 选择框控件指针                                           //
//      x_s    —— x坐标                                                    //
//      y_s    —— 坐标                                                     //
//      width  —— 宽度                                                     //
//      height —— 高度                                                     //
//		                                                             //
//---------------------------------------------------------------------------//
void CreateCheck(CHECK_BOX *check  ,
                unsigned int x_s   ,unsigned int y_s,
                unsigned char width,unsigned char height,
                char *title                              )
{
  Color          = WINDOW_COLOR                         ; 
  Color_BK       = 0xFFFF                               ;   
  (*check).x     = x_s                                  ; // 设置坐标
  (*check).y     = y_s                                  ;
  (*check).width = width                                ; // 设置大小
  (*check).height= height                               ;
  strcpy((*check).Title,title)                          ;
  Redrawcheck(check )                                   ; 
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Redrawcheck(CHECK_BOX* )                                        //
//描述：重绘选择框控件                                                       //
//参数：check —— 选择框控件指针                                            //
//		                                                             //
//---------------------------------------------------------------------------//
void Redrawcheck(CHECK_BOX *check)
{
  DrawRectFill((*check).x,(*check).y,
               (*check).width,(*check).height,White)           ;
  DrawVerticalLine((*check).x-1,(*check).y,
                   (*check).height+1,Dark_Grey)                ;
  DrawHorizonLine((*check).x-1,(*check).y-1,
                   (*check).width+2,Dark_Grey)                 ;
  DrawVerticalLine((*check).x+(*check).width,(*check).y,
                   (*check).height+1,Light_Grey)               ;
  DrawHorizonLine((*check).x-2,(*check).y+(*check).height,
                   (*check).width+3,Light_Grey)                ;
  Color          = WINDOW_COLOR                                ; 
  Color_BK       = WINDOW_BK_COLOR                             ;     
  PutString16M((*check).x+(*check).width+3,                        
               (*check).y-2,(unsigned char*)(*check).Title)    ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CheckCheck(CHECK_BOX*)                                          //
//描述：选择CHECK_BOX控件                                                    //
//参数：check —— 选择框控件控件指针                                        //
//		                                                             //
//---------------------------------------------------------------------------//
void CheckCheck(CHECK_BOX *check)
{
  (*check).status  |= CHECK_CHECKED                            ;
  DrawRectFill((*check).x+3,(*check).y+3,
               (*check).width-6,(*check).height-6,Black)       ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void CheckCheck(CHECK_BOX*)                                          //
//描述：选择CHECK_BOX控件                                                    //
//参数：check —— 选择框控件控件指针                                        //
//		                                                             //
//---------------------------------------------------------------------------//
void unCheckCheck(CHECK_BOX *check)
{
  (*check).status  &=~CHECK_CHECKED                            ;
  DrawRectFill((*check).x+1,(*check).y+1,
               (*check).width-2,(*check).height-2,White)       ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void InScopeCheck(unsigned，unsigned)                                //
//描述：查询点是否在选择框控件范围                                           //
//参数：x,y    ——点坐标                                                    //
//      check  ——选择框控件指针                                            //
//返回：0xFF   ——不在选择框范围内                                          //
//      0      ——在选择框范围内                                            //    
//		                                                             //
//---------------------------------------------------------------------------//
unsigned char InCheckScope(unsigned int x,unsigned int y,CHECK_BOX *check)
{
  if(  x<(*check).x
     ||x>(*check).x+20
     ||y<(*check).y
     ||y>(*check).y+(*check).height) 
    return  0xFF                                  ;
  return  0x00                                    ;
}
//=====================================================================//
//函数：void MessageBox(char *title,char *string)                      //
//功能：显示消息框                                                     //
//参数: title  —消息框标题                                            //
//      string —消息框内容                                            //
//=====================================================================//
void MessageBox(char *title,char *string)
{
  char Event                                     ;
  char key                                       ;
  union
  {
    unsigned int  pos[2]                         ;
    char byte[4]                                 ;
  }TPoint                                        ;
  char  rx_char                                  ;
  char *ptmb                                     ;
  INT8U err[1]                                                           ;
  OSMutexPend(SPIMutex,0,err)                    ;
  Color    = TITLE_COLOR                         ;
  Color_BK = TITLE_BK_COLOR                      ;
  DrawRectFill(20,115,200,25,TITLE_BK_COLOR)     ; 
  ShowIcon24(195,116,24,23,ICON_CLOSE)           ;
  PutStringCN16(120-strlen(title)*4,119,
                (unsigned char *)title  )        ;
  DrawRectFill(20,140,200,55,WINDOW_BK_COLOR)    ;
  DrawRectFill(20,140,200,55,0xFFFF)             ;
  DrawRect(20,140,200,55,WINDOW_COLOR)           ;
  Color    = WINDOW_COLOR                        ;
  Color_BK = 0xFFFF                              ;
  PutStringCN16(120-strlen(string)*4,158,
               (unsigned char *)string  )        ;
  Color_BK = WINDOW_BK_COLOR                     ;
  OSMutexPost(SPIMutex)                          ;
  for(;;)
  {
    ptmb           = OSMboxPend(MBKeyTP,0,err)      ;
    Event          = *(ptmb++)                      ;
    key            = *(ptmb++)                      ;
    rx_char        = *(ptmb++)                      ;
    TPoint.byte[0] = *(ptmb++)                      ;
    TPoint.byte[1] = *(ptmb++)                      ;
    TPoint.byte[2] = *(ptmb++)                      ;
    TPoint.byte[3] = *(ptmb++)                      ;
    rx_char        = rx_char                        ;
    if(Event&EVENT_KEY_PRESSED)
      if(key==Cancel)   break                       ;
    else if(Event&EVENT_TP_TOUCHED)
    {
      if(  TPoint.pos[0]>195&&TPoint.pos[0]<215
         &&TPoint.pos[1]>116&&TPoint.pos[1]<140)
        break                                       ;
    }  
  }
}

//=====================================================================//
//函数：void ConfirmBox(char *title,char *string)                      //
//功能：显示消息框                                                     //
//参数: title  —消息框标题                                            //
//      string —消息框内容                                            //
//=====================================================================//
unsigned char ConfirmBox(char *title,char *string)
{
  char Event                                     ;
  char key,index,down_id=0xFF,rx_char            ;
  union
  {
    unsigned int  pos[2]                         ;
    char byte[4]                                 ;
  }TPoint                                        ;
  unsigned char result                           ;  
  char *ptmb                                     ;
  INT8U err[1]                                   ;
  Color    = TITLE_COLOR                         ;
  Color_BK = TITLE_BK_COLOR                      ;
  DrawRectFill(20,115,200,25,TITLE_BK_COLOR)     ; 
  ShowIcon24(196,116,24,23,ICON_CLOSE)           ;
  PutStringCN16(120-strlen(title)*4,119,
                (unsigned char *)title  )        ;
  DrawRectFill(20,140,200,70,WINDOW_BK_COLOR)    ;  
  DrawRectFill(20,140,200,70,0xFFFF)             ;
  DrawRect(20,140,200,70,WINDOW_COLOR)           ;
  Color    = WINDOW_COLOR                        ;
  Color_BK = 0xFFFF                              ;
  PutStringCN16(120-strlen(string)*4,153,
               (unsigned char *)string  )        ;
  Color    = Black                               ;
  Color_BK = WINDOW_BK_COLOR                     ;
  CreateButton(1,"确认",60 ,184,50,18)           ;
  CreateButton(2,"取消",130,184,50,18)           ; 
  for(;;)
  {
    ptmb           = OSMboxPend(MBKeyTP,0,err)                           ;
    Event          = *(ptmb++)                                           ;
    key            = *(ptmb++)                                           ;
    rx_char        = *(ptmb++)                                           ;
    TPoint.byte[0] = *(ptmb++)                                           ;
    TPoint.byte[1] = *(ptmb++)                                           ;
    TPoint.byte[2] = *(ptmb++)                                           ;
    TPoint.byte[3] = *(ptmb++)                                           ;
    rx_char        = rx_char                     ;
    if(Event&EVENT_KEY_PRESSED)
    {
      switch(key)
      {
      case Enter:
        result   = 0x00                          ;
        break                                    ;
      case Cancel:
        result   = 0xFF                          ;
        break                                    ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==0xFF)
      {
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
      }
      else if(index!=down_id)
      {
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        SetButtonDown(down_id)                       ;
      }
    }
    else if(Event&EVENT_TP_PRESSED)  
    {  
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==1)
      {
        result = 0x00                                ;
        break                                        ;
      }
      else if(index==2)
      {
        result = 0xFF                                ;
        break                                        ;
      }
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      if(  TPoint.pos[0]>195&&TPoint.pos[0]<215
         &&TPoint.pos[1]>116&&TPoint.pos[1]<140)
        break                                        ;
    }
  }
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  return result                                      ;
}

