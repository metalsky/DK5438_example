//===========================================================================//
//                                                                           //
// 文件：  Main.c                                                            //
// 说明：  BW-DK5438开发板编辑控件程序                                       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "string.h"
#include "Edit.h"
#include "TFTDRV.H"
#include "INPUT_CN.H"

unsigned char Active_Edit_Dig  = 0xFF      ; // 活动的数字编辑控件
unsigned char Active_Digit     = 0xFF      ;
unsigned char flicker_status               ;
unsigned char Edit2Read        = 0xFF      ; // 读数据
unsigned char ReadDigit        = 0xFF      ; // 读数据位

extern EDIT_DIG Edit_Dig[6]                ; // 编辑控件
extern unsigned char DISP_MODE             ;
extern unsigned int  Color                 ; // 前景颜色
extern unsigned int  Color_BK              ; // 背景颜色
extern unsigned char INP_MODE              ; // 输入模式
extern unsigned char INP_MODE_LMT          ; // 输入模式限制 
extern unsigned char DISP_MODE             ; // 编辑控件显示模式
extern unsigned long int Interval          ; // 输入键间隔
extern unsigned Event                      ;

extern unsigned char GetKeyPress(void)     ;


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
      if(Input_CN(key,char_cn)==1)           // 输入一个汉字
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


//***************************************************************************************
//																	                    *
// 		void CreateEditDig(): 创建数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void CreateEditDig(unsigned char ID   ,
                   unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)
{
  unsigned char i                           ;
  Color    = WINDOW_COLOR                   ; 
  Color_BK = 0xFFFF                         ;   
  for(i=0;i<6;i++)
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
//***************************************************************************************
//																	                    *
// 		void IniEditDig(): 初始化数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p            )
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<6;i++)
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
//***************************************************************************************
//																	                    *
// 		void IniEditDig(): 初始化数字编辑控件                           		        *
//																	                    *
//***************************************************************************************
void SetEditDig(unsigned char ID,char* p)
{
  unsigned char i,j                                                   ;
  Color    = WINDOW_COLOR                                             ; 
  Color_BK = 0xFFFF                                                   ;   
  for(i=0;i<6;i++)
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
//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void ActiveEditDig(unsigned char ID   ,unsigned char Active)
{
  unsigned char i                                                           ;
  Color    = WINDOW_COLOR                                                   ; 
  Color_BK = 0xFFFF                                                         ;   
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
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
        }
        else                                                              // 取消控件活动状态
	{
          Edit_Dig[i].status &=~EDIT_ACTIVE                             ; 
          if(Active_Edit_Dig == i)
            Active_Edit_Dig  =  0xFF                                    ;
	}
      }
    }
  }
}
//***************************************************************************************
//																	                    *
// 		void ReadEditDig(): 设置数字编辑控件活动状态                     		        *
//																	                    *
//***************************************************************************************
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)
{
  unsigned char i                                                           ;
  for(i=0;i<6;i++)
  {
    if(Edit_Dig[i].id != ID) 
	  continue                                                          ;
	else
	{	  
	  if(!(Edit_Dig[i].status&EDIT_VALID))
	    break                                                           ;
	  else
	  {	
            strcpy((char *)Ret_Buf,Edit_Dig[i].string)                              ;
	    Edit2Read   = i                                                 ; // 读数据
            ReadDigit   = 0x00                                              ; // 读数据位
	  }
	}
  }
}

//***************************************************************************************
//									                *
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                          *
//			         				                        *
//***************************************************************************************
void EditDigMessage(unsigned char Message,char * p)                    
{
  unsigned char i,temp,k                                                ;
  _DINT()                                                               ;  
  Color    = WINDOW_COLOR                                               ; 
  Color_BK = 0xFFFF                                                     ;   
  temp   = Edit_Dig[Active_Edit_Dig].x
	  +Edit_Dig[Active_Edit_Dig].width/2
	  -strlen(Edit_Dig[Active_Edit_Dig].string)*6                   ;
  temp  += Active_Digit*13-1-4                                          ; // 计算光标位置   
  i      = Active_Edit_Dig                                              ;
  DrawVerticalLine( temp,
	            Edit_Dig[Active_Edit_Dig].y
		   +Edit_Dig[Active_Edit_Dig].height/2-10,
		    22,Color_BK                           )             ; // 消隐光标
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
  case 128:
    RedrawEditDig(i)                                                    ;
  case 0xFF:
    break                                                               ;
  case Left:
    if(Active_Digit==0) 
      Active_Digit = strlen(Edit_Dig[i].string)                         ; 
    else                
      Active_Digit--                                                    ;
    if(Edit_Dig[i].string[Active_Digit]>128)
      Active_Digit--                                                    ;
    break                                                               ;
  case Right:
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
  Event         |= SHOULD_FLICKER                                       ;
  flicker_status = 0x00                                                 ;
  _EINT()                                                               ;  
}

//***************************************************************************************
//											*
// 		void ActiveEditDig(): 设置数字编辑控件活动状态                          *
//					                                                *
//***************************************************************************************
void RedrawEditDig(unsigned char Index)                    
{
  unsigned char  i,j                                                    ;
  char* p                                                               ;
  unsigned char string[20]                                              ;
  i      = Index                                                        ;
  p      = Edit_Dig[i].string                                           ;
  DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	       Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)        ; // 清除显示 
  if(Edit_Dig[i].property&EDIT_FRAME)                                     // 需绘制边框
    DrawRect(Edit_Dig[i].x,Edit_Dig[i].y,
	     Edit_Dig[i].width,
	     Edit_Dig[i].height,Color    )                              ; 
  if(DISP_MODE==0x00)
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,(unsigned char*)p);
  else
  {
    for(j=0;j<strlen(p);j++)
      string[j]  = '*'                                                  ;
    string[j]    = 0x00                                                 ;
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // 居中显示
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,string    )       ;
  }
  Event         |= SHOULD_FLICKER                                       ;
  flicker_status = 0x00                                                 ;
}
//***************************************************************************************
//											*
// 		void DeleteEditDig(): 设置数字编辑控件活动状态                     	*
//											*
//***************************************************************************************
void DeleteEditDig(unsigned char ID)
{
  unsigned char i                                                           ;
  for(i=0;i<6;i++)
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

void Flicker_Cursor(void)
{
  unsigned int temp                                                   ;
  static unsigned char flicker_status = 0x00                          ;                   
  if((Active_Edit_Dig!=0xFF))
  {
    temp   = Edit_Dig[Active_Edit_Dig].x
	    +Edit_Dig[Active_Edit_Dig].width/2
	    -strlen(Edit_Dig[Active_Edit_Dig].string)*6               ;
    temp  += Active_Digit*13-1-4                                      ; // 计算光标横坐标
    if(flicker_status++==0x00)
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color)    ; // 绘制光标
    else
    {
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color_BK) ; // 消隐光标
      flicker_status = 0x00                                           ;
    }
  }
}
