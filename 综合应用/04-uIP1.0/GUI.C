//===========================================================================//
//                                                                           //
// 文件：  GUI.C                                                             //
// 说明：  BW-DK5438开发板图形用户界面函数文件                               //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//
#include "string.h"
#include "TFTDRV.H"
#include "GUI.H"
#include "PIN_DEF.H"

unsigned char Active_Edit_Dig  = 0xFF           ; // 活动的数字编辑控件
unsigned char Active_Digit     = 0xFF           ;
unsigned char flicker_status                    ;
unsigned char Edit2Read        = 0xFF           ; // 读数据
unsigned char ReadDigit        = 0xFF           ; // 读数据位

extern unsigned      Color,Color_BK             ;
extern EDIT          Edit_Dig[10]               ; // 编辑控件
extern BUTTON        Button[10]                 ; // 按钮控件
extern unsigned char DISP_MODE                  ; // 编辑控件显示模式
extern char          Event                      ;

extern unsigned char GetKeyPress(void)          ;

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
	  Edit_Dig[i].limit        = limit                            ; // 设置最大字符数  
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

//=====================================================================//
//函数：long int Read_Edit_Val(unsigned char Edit_Index)               //
//功能：Edit控件输入并读取内容                                         //
//参数: Edit_Index       —需读取的EDIT控件ID                          //
//      string           —数据缓冲区指针                              //
//      Renew_Edit_Index —显示需刷新的EDIT控件ID                      //
//=====================================================================//

unsigned char Read_Edit_Val(unsigned char Edit_Index,char* string,
                            unsigned char Renew_Edit_Index        )
{
  unsigned char key                        ;
  char char_cn[3]                          ;
  unsigned char Repeat, LastKey = 0xEE     ; 

  if(INP_MODE&INP_DIG)
  {
    Color    = STATUS_COLOR                ;
    Color_BK = STATUS_BK_COLOR             ;
    PutStringEN24(115,292,"1 ")            ;
    Color    = WINDOW_COLOR                ;
    Color_BK = WINDOW_BK_COLOR             ;
  }
  else if(INP_MODE&INP_EN)
  {
    Color    = STATUS_COLOR                ;
    Color_BK = STATUS_BK_COLOR             ;
    PutStringEN24(115,292,"EN")            ;
    Color    = WINDOW_COLOR                ;
    Color_BK = WINDOW_BK_COLOR             ;
  }
  else if(INP_MODE&INP_CN)
  {
    Color    = STATUS_COLOR                ;
    Color_BK = STATUS_BK_COLOR             ;
    PutStringEN24(115,292,"CN")            ;
    Color    = WINDOW_COLOR                ;
    Color_BK = WINDOW_BK_COLOR             ;
  }
Input_Digital:
  for(;INP_MODE==INP_DIG;)
  {
    key = GetKeyPress()                    ;
    if(key==Power)
      return Power                         ;
    switch(key)
    {
    case F1:                                 // 切换输入法
      if(INP_MODE_LMT&INP_EN)
      {
        Color    = STATUS_COLOR            ;
        Color_BK = STATUS_BK_COLOR         ;
        PutStringEN24(115,292,"EN")        ;        
        Color    = WINDOW_COLOR            ;
        Color_BK = WINDOW_BK_COLOR         ;
        INP_MODE = INP_EN                  ;
        LastKey  = 0xEE                    ;
      }
      else
      if(INP_MODE_LMT&INP_CN)
      {
        Color    = STATUS_COLOR            ;
        Color_BK = STATUS_BK_COLOR         ;
        PutStringEN24(115,292,"CN")        ;        
        Color    = WINDOW_COLOR            ;
        Color_BK = WINDOW_BK_COLOR         ;
        INP_MODE  = INP_CN                 ;
      }
      break                                ;
    case Up:       
      return PRE_EDIT                      ; // 用户切换到上一Edit控件
    case Down:       
      return NEXT_EDIT                     ; // 用户切换到下一Edit控件
    case Power2:
      break                                ;
    case 1: case 2: case 3: case 4: case 5: 
    case 6: case 7: case 8: case 9: case 0:
      EditDigMessage(key,char_cn)          ; 
      break                                ;
    case Left: case Right: case Backspace: case Delete:
      EditDigMessage(key,char_cn)          ; 
      break                                ;
    case Esc:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0xFF                          ; // 用户退出输入
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0x00                          ; // 用户结束输入
    }
  }

  for(;INP_MODE==INP_EN;)
  {
    key = GetKeyPress()                    ;
    if(key==Power)
      return Power                         ;
    if((key!=LastKey)&&(key<10)&&(key>1))
    {
      Interval = MAX_INTERVAL              ;
      Repeat   = 0x00                      ;
    }
    switch(key)
    {
    case F1:                                 // 切换输入法
      if(INP_MODE_LMT&INP_CN)
      {
        Color    = STATUS_COLOR            ;
        Color_BK = STATUS_BK_COLOR         ;
        INP_MODE  = INP_CN                 ;
        PutStringEN24(115,292,"CN")        ;
        Color    = WINDOW_COLOR            ;
        Color_BK = WINDOW_BK_COLOR         ;
      }
      else
        INP_MODE  = INP_DIG                ;
      break                                ;  
    case 2:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%3==0)  
        EditDigMessage('A',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('B',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('C',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 3:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ; 
      }
      if(Repeat%3==0)  
        EditDigMessage('D',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('E',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('F',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;      
    case Left: case Right: case Backspace: case Delete:
      EditDigMessage(key,char_cn)                            ; 
      break                                                  ;
    case 4:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%3==0)  
        EditDigMessage('G',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('H',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('I',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 5:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%3==0)  
        EditDigMessage('J',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('K',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('L',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 6:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%3==0)  
        EditDigMessage('M',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('N',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('O',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 7:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%4==0)  
        EditDigMessage('P',char_cn)                          ;
      if(Repeat%4==1)  
        EditDigMessage('Q',char_cn)                          ;
      if(Repeat%4==2)  
        EditDigMessage('R',char_cn)                          ;
      if(Repeat%4==3)  
        EditDigMessage('S',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 8:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%3==0)  
        EditDigMessage('Y',char_cn)                          ;
      if(Repeat%3==1)  
        EditDigMessage('U',char_cn)                          ;
      if(Repeat%3==2)  
        EditDigMessage('V',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case 9:
      if(Interval>=MAX_INTERVAL)
        Repeat     = 0x00                                    ;
      if(Interval<MAX_INTERVAL)                                // 同按键连击时间间隔最大值
      {
        EditDigMessage(Backspace,char_cn)                    ;
        Repeat++                                             ;          
      }
      if(Repeat%4==0)  
        EditDigMessage('W',char_cn)                          ;
      if(Repeat%4==1)  
        EditDigMessage('X',char_cn)                          ;
      if(Repeat%4==2)  
        EditDigMessage('Y',char_cn)                          ;
      if(Repeat%4==3)  
        EditDigMessage('Z',char_cn)                          ;
      LastKey      = key                                     ;
      break                                                  ;
    case Up:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return PRE_EDIT                                        ; // 用户切换到上一Edit控件
    case Down:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return NEXT_EDIT                                       ; // 用户切换到下一Edit控件
    case Esc:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0xFF                                            ; // 用户退出输入
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0x00                                            ; // 用户结束输入
    default: 
      break                                                  ;
    }
  }
  
  for(;INP_MODE==INP_CN;)
  {
    key = GetKeyPress()                    ;
    if(key==Power)
      return Power                         ;
    switch(key)
    {
    case F1:                                 // 切换输入法
      if(INP_MODE_LMT&INP_DIG)
      {
        Color    = STATUS_COLOR            ;
        Color_BK = STATUS_BK_COLOR         ;
        PutStringEN24(115,292,"1 ")        ;
        INP_MODE  = INP_DIG                ;
        Color    = WINDOW_COLOR            ;
        Color_BK = WINDOW_BK_COLOR         ;
      }
      break                                ;  
    case 2: case 3: case 4: 
    case 5: case 6: case 7: 
    case 8: case 9: case 0:
      ActiveEditDig(Renew_Edit_Index,0)    ;
//      if(Input_CN(key,char_cn)==1)           // 输入一个汉字
      {
        ActiveEditDig(Edit_Index,1)        ;
        EditDigMessage(129,char_cn)        ; 
      }
      ActiveEditDig(Renew_Edit_Index,1)    ;
      EditDigMessage(128,char_cn)          ;
      ActiveEditDig(Edit_Index,1)          ;
      break                                ;
    case Up:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return PRE_EDIT                      ; // 用户切换到上一Edit控件
    case Down:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return NEXT_EDIT                     ; // 用户切换到下一Edit控件
    case Power2:
//    SetBackLightAuto(0)                  ; 
      break                                ;
    case Left: case Right: case Backspace: case Delete:
      EditDigMessage(key,char_cn)          ; 
      break                                ;    
    case Esc:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0xFF                          ; // 用户退出输入
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// 擦除
      return 0x00                          ; // 用户结束输入
    default:
      break                                ;
    }    
  }
  goto Input_Digital                       ;  
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
//  Sweep_Cursor(i)                                                       ; // 
  if(Message==129)                                                        // 输入了中文字符
  {
    if(strlen(Edit_Dig[i].string)+1<Edit_Dig[i].limit)
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
    flicker_status = 0x01                                               ;
    Flicker_Cursor()                                                    ;
    if(Active_Digit==0) 
      Active_Digit = strlen(Edit_Dig[i].string)                         ; 
    else                
      Active_Digit--                                                    ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    break                                                               ;
  case Right:
    flicker_status = 0x01                                               ;
    Flicker_Cursor()                                                    ;
    if(Active_Digit==strlen(Edit_Dig[i].string))
      Active_Digit = 0                                                  ; 
    else              
      Active_Digit++                                                    ;
    if(Edit_Dig[i].string[Active_Digit-1]>128)
      Active_Digit++                                                    ;
    break                                                               ;
  case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:case 0:
  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': 
  case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': 
  case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
  case 'Y': case 'Z':
    if(strlen(Edit_Dig[i].string)<Edit_Dig[i].limit)
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
  unsigned char  i,j,k = 1,line = 0                                        ;
  char* p                                                                  ;
  unsigned char string[50]                                                 ;
  i        = Index                                                         ;
  p        = Edit_Dig[i].string                                            ;
  Color    = Black                                                         ;
  Color_BK = White                                                         ;
  DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	       Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)           ; // 清除显示 
  if(Edit_Dig[i].property&EDIT_FRAME)                                        // 需绘制边框
  {
    DrawVerticalLine(Edit_Dig[i].x-1,Edit_Dig[i].y,
                     Edit_Dig[i].height+1,Dark_Grey)                       ;
    DrawHorizonLine(Edit_Dig[i].x-1,Edit_Dig[i].y-1,
                    Edit_Dig[i].width+2,Dark_Grey  )                       ;
    DrawVerticalLine(Edit_Dig[i].x+Edit_Dig[i].width,Edit_Dig[i].y,
                     Edit_Dig[i].height+1,Light_Grey               )       ;
    DrawHorizonLine(Edit_Dig[i].x-2,Edit_Dig[i].y+Edit_Dig[i].height,
                    Edit_Dig[i].width+3,Light_Grey                   )     ;
  }
  if(Edit_Dig[i].disp_mode==0x00)                                            // 非密码，单行居中显示
  {
    if(Edit_Dig[i].status&EDIT_FONT)
      PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6,             // 居中显示
	          Edit_Dig[i].y+Edit_Dig[i].height/2-12,(unsigned char*)p) ;
    else
      PutStringEN16(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*9/2,         // 居中显示
	            Edit_Dig[i].y+Edit_Dig[i].height/2-7,(unsigned char*)p);      
  }
  else if(Edit_Dig[i].disp_mode&MULTILINE)                                   // 多行显示
  {
    while(k)    
    {
      k          = str_cpy_num((char *)string,p,Edit_Dig[i].char_per_line) ;
      string[k]  = 0                                                       ;
      if(Edit_Dig[i].status&EDIT_FONT)
        PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6,             
	            Edit_Dig[i].y+PRE_INTERVAL+line*LINE_INTERVAL,string)  ; // 水平居中  
      else
        PutStringEN16(Edit_Dig[i].x+Edit_Dig[i].width/2-k*9/2,      
	              Edit_Dig[i].y+PRE_INTERVAL+line*LINE_INTERVAL,string);
      line++                                                               ;
      p         += k                                                       ;
    }                                                                      ; 
  }
  else if(Edit_Dig[i].disp_mode&MASKED)                                      // 单行居中加密显示
  {
    for(j=0;j<strlen(p);j++)
      string[j]  = '*'                                                     ;
    string[j]    = 0x00                                                    ;
    if(Edit_Dig[i].status&EDIT_FONT)
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,             // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,string    )          ;
    else
    PutStringCN16(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,          
	        Edit_Dig[i].y+Edit_Dig[i].height/2-5,string    )           ;
      
  }
  flicker_status = 0x00                                                    ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetCharPerLine(unsigned char,unsigned char)                     //
//描述：设置编辑控件显示模式                                                 //
//参数：Index —— 编辑控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void SetCharPerLine(unsigned char ID,unsigned char num)
{
  unsigned char i                                                           ;
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
    else
    {	  
      if(!(Edit_Dig[i].status&EDIT_VALID))
	break                                                               ;
      else
	Edit_Dig[i].char_per_line  = num                                    ; // 更改显示模式
    }        
  }  
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void SetEditDigDispMode(unsigned char,unsigned char)                 //
//描述：设置编辑控件显示模式                                                 //
//参数：Index —— 编辑控件索引                                              //
//		                                                             //
//---------------------------------------------------------------------------//
void SetEditDigDispMode(unsigned char ID,unsigned char mode)
{
  unsigned char i                                                           ;
  for(i=0;i<10;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
    else
    {	  
      if(!(Edit_Dig[i].status&EDIT_VALID))
	break                                                               ;
      else
	Edit_Dig[i].disp_mode  = mode                                       ; // 更改显示模式
    }        
  }  
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
  unsigned char line = 0,Active_in_line,HowMany_in_line               ;
  if(Active_Edit_Dig!=0xFF&&!(Edit_Dig[Active_Edit_Dig].disp_mode&MULTILINE))
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
      flicker_status = 0x00                                                        ;
    }
  }
  else if(Active_Edit_Dig!=0xFF)                                                     // 多行显示
  {    
    Active_in_line = Active_Digit                                                  ;
    while(Active_in_line>Edit_Dig[Active_Edit_Dig].char_per_line)
    {
      Active_in_line    -=Edit_Dig[Active_Edit_Dig].char_per_line                  ;
      line++                                                                       ; // 搜索其所在行的位置
    }
    HowMany_in_line= strlen(Edit_Dig[Active_Edit_Dig].string)
                    -line*Edit_Dig[Active_Edit_Dig].char_per_line                  ;
    if(HowMany_in_line>Edit_Dig[Active_Edit_Dig].char_per_line)
       HowMany_in_line=Edit_Dig[Active_Edit_Dig].char_per_line                     ;
    if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
    {
      temp   = Edit_Dig[Active_Edit_Dig].x
	      +Edit_Dig[Active_Edit_Dig].width/2 -HowMany_in_line*6                ;
      temp  += Active_in_line*13-1                                                 ; // 计算光标横坐标
    }
    else
    {
      temp   = Edit_Dig[Active_Edit_Dig].x
	      +Edit_Dig[Active_Edit_Dig].width/2 -HowMany_in_line*9/2              ;
      temp  += Active_in_line*9-2                                                  ; // 计算光标横坐标
    }
    if(++flicker_status==0x01)
    {
      if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
        DrawVerticalLine( temp, Edit_Dig[Active_Edit_Dig].y 
                               +PRE_INTERVAL+line*LINE_INTERVAL,22,Color)          ; // 绘制光标
      else
        DrawVerticalLine( temp, Edit_Dig[Active_Edit_Dig].y
                               +PRE_INTERVAL+line*LINE_INTERVAL,16,Color)          ; // 绘制光标
    }
    else
    {
      if(Edit_Dig[Active_Edit_Dig].status&EDIT_FONT)
        DrawVerticalLine( temp, Edit_Dig[Active_Edit_Dig].y 
                               +PRE_INTERVAL+line*LINE_INTERVAL,22,Color_BK)      ; // 消隐光标
      else
        DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y
                              +PRE_INTERVAL+line*LINE_INTERVAL,16,Color_BK)       ; // 消隐光标
      flicker_status = 0x00                                                       ;
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void InScopeEdit(unsigned，unsigned)                                 //
//描述：查询点所在位置的按钮控件                                             //
//参数：x,y    ——点坐标                                                    //
//返回：0xFF   ——无编辑控件                                                //
//      0~0xFE ——编辑控件索引                                              //    
//		                                                             //
//---------------------------------------------------------------------------//
unsigned char InScopeEdit(unsigned int x,unsigned int y)
{
  unsigned char i                                           ;
  for(i=0;i<10;i++)
  {
    if(!(Edit_Dig[i].status&EDIT_VALID))
      continue                                              ;
    if(  x<Edit_Dig[i].x+3
       ||x>Edit_Dig[i].x+Edit_Dig[i].width-3
       ||y<Edit_Dig[i].y+3
       ||y>Edit_Dig[i].y+Edit_Dig[i].height-3) 
	  continue                                          ;
    else
      return Edit_Dig[i].id                                 ;
  }  
  return  0xFF                                              ;
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
  if(Edit_Dig[Index].status&EDIT_FONT)
    DrawVerticalLine( temp,Edit_Dig[Index].y+Edit_Dig[Index].height/2-10,22,Color_BK); // 消隐光标
  else
    DrawVerticalLine( temp,Edit_Dig[Index].y+Edit_Dig[Index].height/2-8,16,Color_BK) ; 
  flicker_status = 0x01                                               ;
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
//描述：重绘VIEW控件                                                         //
//参数：Index —— VIEW控件索引                                              //
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
//描述：VIEW控件消息处理，显示内容追加字符                                   //
//参数：*view   —— VIEW指针                                                //
//      Message —— 消息                                                    //
//      *p      —— 字符指针                                                //
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
//      (*view).length  += strlen((char *)p)                              ;
      PutStringEN16((*view).positionx,(*view).positiony,p)              ;   
      if((*view).positionx+18>=(*view).x+(*view).width-2)
      {
        (*view).positionx       = (*view).x+4                           ;
        (*view).positiony      += 18                                    ;        
      }      
      else
//        (*view).positionx      += 9*(strlen((char *)p))                 ; 
        (*view).positionx      += 9                                     ; 
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ViewInsertLine(VIEW*，char*)                                    //
//描述：VIEW控件追加字符串                                                   //
//参数：view    —— VIEW控件指针                                            //
//      p       —— 字符串缓冲区                                            //
//		                                                             //
//---------------------------------------------------------------------------//
void ViewInsertLine(VIEW *view,unsigned char *p)                    
{
  unsigned char l,i,p_char[3]                                            ;
  l        =  strlen((char *)p)                                          ;
  for(i=0;i<l;i++)
  {
    p_char[1]  = 0                                                       ;
    p_char[2]  = 0                                                       ;
    if(p[i]<128) 
    {
      p_char[0] = p[i]                                                   ; 
      ViewMessage(view,EN_INPUT,p_char)                                  ;
    }
    else 
    {
      p_char[0] = p[i]                                                   ; 
      p_char[1] = p[++i]                                                 ; 
      ViewMessage(view,CN_INPUT,p_char)                                  ;
    }
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void ViewGotoNextLine(VIEW *)                                        //
//描述：VIEW显示位置移至下一行                                               //
//参数：view    —— VIEW控件指针                                            //
//		                                                             //
//---------------------------------------------------------------------------//
void ViewGotoNextLine(VIEW *view)                    
{
    (*view).positionx       = (*view).x+4                               ;
    (*view).positiony      += 20                                        ;        
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：ClearViewContent(VIEW *)                                             //
//描述：清空VIEW控件内容                                                     //
//参数：view —— VIEW控件指针                                               //
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
//函数：void DeleteView(unsigned char)                                       //
//描述：删除VIEW控件                                                         //
//参数：Index —— VIEW控件索引                                              //
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
  for(;;)
  {
    if(Event&EVENT_KEY_PRESSED)
      if(key==Cancel)   break                    ;
    else if(Event&EVENT_TP_TOUCHED)
    {
      if(  TPoint.pos[0]>195&&TPoint.pos[0]<215
         &&TPoint.pos[1]>116&&TPoint.pos[1]<140)
        break                                    ;
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
  char index,down_id=0xFF                        ;
  union
  {
    unsigned int  pos[2]                         ;
    char byte[4]                                 ;
  }TPoint                                        ;
  unsigned char result                           ;  
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
    if(Event&EVENT_TP_TOUCHED)
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
    Event   =  0                                     ;
  }
  Event     =  0                                     ;
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  return result                                      ;
}
              
//=====================================================================//
//函数：nsigned char str_cpy_num(char* , char* ,unsigned char )        //
//功能：从源字符串拷贝指定数目字符到目的字符串                         //
//      如果源字符串字符数不足，则拷贝至字符串尾                       //
//参数: Destination  —目的字符串                                      //
//      Source       —源字符串                                        //
//      Num          - 拷贝字符数                                      //
//返回: 实际拷贝的字符数                                               // 
//=====================================================================//
unsigned char str_cpy_num(char* Destination, char* Source,unsigned char Num)
{
  unsigned char i                                            ;
  for(i=0;i<Num;i++)
  {
    if(Source[i]==0)
      break                                                  ;
    Destination[i] = Source[i]                               ;
  }
  return   i                                                 ;
}

