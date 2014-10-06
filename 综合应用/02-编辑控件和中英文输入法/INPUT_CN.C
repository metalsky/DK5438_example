//===========================================================================//
//                                                                           //
// 文件：  INPUT_CN.C                                                        //
// 说明：  中/英文输入法函数                                                 //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "INPUT_CN.H"
#include "TFTDRV.H"
#include "string.h"
#include "edit.h"
#include "PY_MB.H"

unsigned char cpt9PY_Mblen                     ;
t9PY_index *cpt9PY_Mb[16]                      ;  
extern unsigned char INP_MODE                  ; // 输入模式
extern unsigned char GetKeyPress(void)         ; // 
extern unsigned int  Color                     ; // 前景颜色
extern unsigned int  Color_BK                  ; // 背景颜色

//============================================================================//
//                                                                            //
// 函数: unsigned char T9PY_Get_Match_PY_MB(char ,t9PY_index code  **)        //
// 功能: 获取与输入相匹配的T9拼音列表                                         //
// 参数: p_PadInput       - 输入的按键序列，由'0'~'9'组成的字符串             //
//	 List_match_PY_Mb - 存储"获取到的拼音索引地址"的数组                  //
// 返回: 获取到完全匹配拼音数量                                               //
//                                                                            //
//============================================================================//
unsigned char T9PY_Get_Match_PY_MB(char *p_PadInput,t9PY_index ** List_match_PY_Mb)
{
    t9PY_index *p_PY_CurrenIndex,*p_PY_LastIndex,*p_PY_TempIndex              ;
    unsigned char i,j,cInputStrLength                                         ;
    unsigned char T9PY_Match_NUM = 0                                          ; // 完全匹配拼音数量
    j=0                                                                       ; // j为匹配最大值
    if(*p_PadInput=='\0')       return(0)                                     ; // 如果输入空字符返回0
    cInputStrLength  = strlen(p_PadInput)                                     ; // 获取输入拼音串长度
    p_PY_CurrenIndex = (t9PY_index *)(&(t9PY_index2[0]))                      ; // 首索引地址赋值,p_PY_CurrenIndex为当前拼音索引地址
    p_PY_LastIndex   = (t9PY_index *)t9PY_index2                              ;
    p_PY_LastIndex  += sizeof(t9PY_index2)/sizeof(t9PY_index2[0] )            ; // 最后索引地址之后的地址
    while(p_PY_CurrenIndex < p_PY_LastIndex)                                    // 遍历字母索引表
   {
     for(i=0;i<cInputStrLength;i++)                                             // 查询匹配的字符数
     {
       if(*(p_PadInput+i)!=*((*p_PY_CurrenIndex).t9PY_T9+i))                    // 检查是否匹配
       {
         if (i+1 > j)            
         {
           j              = i+1                                               ; // j为不匹配字符串中前面字符匹配的最大值
           p_PY_TempIndex = p_PY_CurrenIndex                                  ;
         }
         break                                                                ; // 不匹配,退出
       }
     }
     if((i==cInputStrLength) && (T9PY_Match_NUM<16))                            // 匹配字符数等于输入字符串长度, 即为匹配，记录，最多16组
     {
       List_match_PY_Mb[T9PY_Match_NUM] = p_PY_CurrenIndex                    ; // 取索引值
       T9PY_Match_NUM++                                                       ;
     }
     p_PY_CurrenIndex++                                                       ;
   }
   if(j!=cInputStrLength)       List_match_PY_Mb[0] = p_PY_TempIndex          ; // 不匹配但最多匹配字符的1组
   return (T9PY_Match_NUM)                                                    ; // 输出完全匹配组数,0为无果而终
}

//============================================================================//
//                                                                            //
// 函数: unsigned char Input_CN(unsigned char* buffer)                        //
// 功能: 输入一个汉字                                                         //
// 参数: buffer — 输入汉字内码                                               //
// 返回: 0      —未输入汉字                                                  //
//       1      —输入一个汉字                                                //
//       0xFF   —用户选择退出汉字输入法                                      //
//                                                                            //
//============================================================================//
unsigned char Input_CN(char first_key,char* buffer)
{
  unsigned char No_py = 0,No_hz = 0                                        ; // 匹配到的拼音数和汉字数
  unsigned char Offset_py,Page_hz                                          ; // 已显示的拼音个数/已显示的汉字页数
  unsigned char Current_py                                                 ; // 当前窗口显示的拼音个数
  unsigned char Active_py,Active_hz                                        ; // 当前被选的拼音和汉字序号
  unsigned char update                                                     ;
  unsigned char mode = INPUT_PY                                            ; // 模式
  unsigned char i = 0,j = 0,temp = 0,key                                   ;
  char py[10]                                                              ;
  DrawRectFill(10,230,220,28,0xFFFF)                                       ; // 清除拼音选择窗口
  DrawRectFill(10,258,220,28,0xFFFF)                                       ; // 清除汉字选择窗口
  DrawRect(10,230,220,28,0x00A4)                                           ; // 拼音选择窗口边框
  DrawRect(10,258,220,28,0x00A4)                                           ; // 汉字选择窗口边框
  update    = 0x00                                                         ;
  Offset_py = 0x00                                                         ;
Input_py:  
  for(;mode==INPUT_PY;)
  {
    if(update != 0)
    {      
      SetColor(0xFFFFFF)                                                   ;        
      DrawRectFill(11,259,218,26,0xFFFF)                                   ; // 清除汉字选择窗口
      SetColor(0x0000A4)                                                   ;
      if(update>1)                                                           // 显示页数改变，清除窗口
      {
        SetColor(0xFFFFFF)                                                 ;        
        DrawRectFill(11,231,218,26,0xFFFF)                                 ; // 清除拼音选择窗口
        SetColor(0x0000A4)                                                 ;              
      }
      update   = 0                                                         ;
      temp     = 40                                                        ;
      for(i=0;(Offset_py+i<No_py);i++)                                       // 按长度调节每页显示拼音的数量
      {
        if(i==(Active_py-Offset_py))
        {
          SetColor(0xFFFFFF)                                               ;  
          SetBKColor(0x0000A4)                                             ;  
          PutStringEN24(temp,232,(unsigned char *)((*cpt9PY_Mb[Offset_py+i]).PY))   ;
          SetColor(0x0000A4)                                               ;  
          SetBKColor(0xFFFFFF)                                             ;  
        }    
        else
          PutStringEN24(temp,232 ,(unsigned char *)((*cpt9PY_Mb[Offset_py+i]).PY))  ;
        temp     += 12+13*strlen((char *)((*cpt9PY_Mb[Offset_py+i]).PY))   ; // 计算显示后的字符长度
        if(temp+13*strlen((char *)((*cpt9PY_Mb[Offset_py+i]).PY))>207) 
           break                                                           ; // 如果显示后的字符长度超过限制,停止显示
      }
      Current_py = i + 1                                                   ;
      if(Offset_py+Current_py<No_py)
        PutStringCN24(205,233,"→")                                        ;  
      if((Offset_py>0)&&(No_py>0))    
        PutStringCN24(12 ,232,"←")                                        ;  
      No_hz = strlen((char *)((*cpt9PY_Mb[Active_py]).PY_mb))/2            ; // 更新汉字选择窗口
      SetColor(0x0000A4)                                                   ;  
      SetBKColor(0xFFFFFF)                                                 ;  
      for(i=0;(i<6)&&(i<No_hz)&&(No_py>0);i++)                   
        PutCharCN24(40+i*28,260,(unsigned char *)(&(*cpt9PY_Mb[Active_py]).PY_mb[2*i+1]));
    }    
    if(first_key!= 0xFF)
    {
      key        = first_key                                               ;
      first_key  = 0xFF                                                    ;
    }
    else
      key = GetKeyPress()                                                  ;
    switch(key)
    {
    case F1:                                 // 切换输入法
      INP_MODE  = INP_DIG                                                  ;
      return  0                                                            ;
    case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
      py[j++] = key+0x30                                                   ;
      py[j]   = 0x00                                                       ;
      No_py   = T9PY_Get_Match_PY_MB(py,cpt9PY_Mb)                         ;      
      if(No_py==0)
      {
        py[--j] = 0x00                                                     ;
        No_py   = T9PY_Get_Match_PY_MB(py,cpt9PY_Mb)                       ;
      }
      else
      {
        Active_py = 0x00                                                   ;
        Offset_py = 0x00                                                   ;
        update    = 0xFF                                                   ;
      }
      break                                                                ;
    case Backspace:
      if(j==0)    break                                                    ;
      py[--j]   = 0x00                                                     ;
      No_py   = T9PY_Get_Match_PY_MB(py,cpt9PY_Mb)                         ;
      update  = 0xFF                                                       ;
      break                                                                ;
    case Right:
      if(No_py==0)    break                                                ; 
      Active_py++                                                          ;
      if(Active_py>=No_py)  
        Active_py = No_py-1                                                ;
      if(Offset_py+Current_py <= Active_py)
      {
        Offset_py = Offset_py+Current_py                                   ;
        update    = 0xFF                                                   ;
      }
      else
        update    = 1                                                      ;
      break                                                                ;
    case Left:      
      if((Active_py==0)||(No_py==0))    break                              ;          
      Active_py--                                                          ;
      if(Offset_py > Active_py)
      {
        temp      = 207                                                    ;
        for(i=0;(Offset_py-i-1!=0);i++)                                      // 计算上一页显示的拼音数
        {
          temp   -= 13*strlen((char *)(*cpt9PY_Mb[Offset_py-i-1]).PY)      ;
          if(temp-12-13*strlen((char *)(*cpt9PY_Mb[Offset_py-i-2]).PY)<40)
            break                                                          ;
          temp   -= 12                                                     ;
        }
        Offset_py = Offset_py - i - 1                                      ; 
        Active_py = Offset_py + i                                          ;
        update    = 0xFF                                                   ;
      }
      else
        update    = 1                                                      ;
      break                                                                ; 
    case Enter:case OK:
      if(No_py==0)    break                                                ; 
      mode        = INPUT_HZ                                               ;
      break                                                                ;     
    case Esc:
      DrawRectFill(10,230,220,28,WINDOW_BK_COLOR)                          ; // 清除拼音选择窗口
      DrawRectFill(10,258,220,29,WINDOW_BK_COLOR)                          ; // 清除汉字选择窗口      
      return     0xFF                                                      ; // 用户退出输入
    }    
  }  
  Page_hz   = 0x00                                                         ;  
  Active_hz = 0x00                                                         ;
  update    = 0x01                                                         ;
  for(;;)
  {
    if(update != 0)
    {
      if(update>1)                                                           // 显示页数改变，清除窗口
      {
        SetColor(0xFFFFFF)                                                 ;        
        DrawRectFill(11,259,218,26,0xFFFF)                                 ; // 清除汉字选择窗口
        SetColor(0x0000A4)                                                 ;              
      }
      update   = 0                                                         ;
      for(i=0;(i<6)&&(Page_hz*6+i<No_hz)&&(No_py>0);i++)                     // 每页显示6个汉字
      {
        if(i==(Active_hz%6))
        {
          SetColor(0xFFFFFF)                                               ;  
          SetBKColor(0x0000A4)                                             ;  
          PutCharCN24(40+i*28,260,(unsigned char *)(&(*cpt9PY_Mb[Active_py]).PY_mb[Page_hz*12+2*i+1]));
          SetColor(0x0000A4)                                               ;  
          SetBKColor(0xFFFFFF)                                             ;  
        }    
        else
          PutCharCN24(40+i*28,260,(unsigned char *)(&(*cpt9PY_Mb[Active_py]).PY_mb[Page_hz*12+2*i+1]));
        if(Page_hz*6<No_hz)
          PutCharCN24(205,261,"→")                                        ;  
        if(Page_hz>0)    
          PutCharCN24(12 ,260,"←")                                        ;  
      }
    }
    key = GetKeyPress()                                                    ;
    switch(key)
    {
    case F1:                                                                // 切换输入法
      INP_MODE  = INP_DIG                                                  ;
      return  0                                                            ;
    case Right:
      Active_hz++                                                          ;
      if(Active_hz>=No_hz)  
        Active_hz = No_hz-1                                                ;
      if(Page_hz != Active_hz/6)
        update  = 0xFF                                                     ;
      else
        update  = 1                                                        ;
      Page_hz     = Active_hz/6                                            ;
      break                                                                ;
    case Left:
      if(Active_hz==0)    break                                            ;          
      Active_hz--                                                          ;
      if(Page_hz != Active_hz/6)
        update  = 0xFF                                                     ;
      else
        update  = 1                                                        ;
      Page_hz     = Active_hz/6                                            ;
      break                                                                ; 
    case Enter:case OK:
      if(No_py == 0)     break                                             ;
      buffer[0]   = (*cpt9PY_Mb[Active_py]).PY_mb[2*Active_hz+1]           ;
      buffer[1]   = (*cpt9PY_Mb[Active_py]).PY_mb[2*Active_hz+2]           ;
      buffer[2]   = 0x00                                                   ;
      DrawRectFill(10,230,220,28,WINDOW_BK_COLOR)                          ; // 清除拼音选择窗口
      DrawRectFill(10,258,220,29,WINDOW_BK_COLOR)                          ; // 清除汉字选择窗口      
      return      1                                                        ;
    case Esc:
      mode        = INPUT_PY                                               ;
      update      = 0xFF                                                   ;
      Active_hz   = 0x00                                                   ;
      goto        Input_py                                                 ;      
    }
  }
}
