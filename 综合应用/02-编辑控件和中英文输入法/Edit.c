//===========================================================================//
//                                                                           //
// �ļ���  Main.c                                                            //
// ˵����  BW-DK5438������༭�ؼ�����                                       //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "string.h"
#include "Edit.h"
#include "TFTDRV.H"
#include "INPUT_CN.H"

unsigned char Active_Edit_Dig  = 0xFF      ; // ������ֱ༭�ؼ�
unsigned char Active_Digit     = 0xFF      ;
unsigned char flicker_status               ;
unsigned char Edit2Read        = 0xFF      ; // ������
unsigned char ReadDigit        = 0xFF      ; // ������λ

extern EDIT_DIG Edit_Dig[6]                ; // �༭�ؼ�
extern unsigned char DISP_MODE             ;
extern unsigned int  Color                 ; // ǰ����ɫ
extern unsigned int  Color_BK              ; // ������ɫ
extern unsigned char INP_MODE              ; // ����ģʽ
extern unsigned char INP_MODE_LMT          ; // ����ģʽ���� 
extern unsigned char DISP_MODE             ; // �༭�ؼ���ʾģʽ
extern unsigned long int Interval          ; // ��������
extern unsigned Event                      ;

extern unsigned char GetKeyPress(void)     ;


//=====================================================================//
//������long int Read_Edit_Val(unsigned char Edit_Index)               //
//���ܣ�Edit�ؼ����벢��ȡ����                                         //
//����: Edit_Index       �����ȡ��EDIT�ؼ�ID                          //
//      string           �����ݻ�����ָ��                              //
//      Renew_Edit_Index ����ʾ��ˢ�µ�EDIT�ؼ�ID                      //
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
    case F1:                                 // �л����뷨
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
      return PRE_EDIT                      ; // �û��л�����һEdit�ؼ�
    case Down:       
      return NEXT_EDIT                     ; // �û��л�����һEdit�ؼ�
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
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0xFF                          ; // �û��˳�����
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0x00                          ; // �û���������
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
    case F1:                                 // �л����뷨
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      if(Interval<MAX_INTERVAL)                                // ͬ��������ʱ�������ֵ
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
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return PRE_EDIT                                        ; // �û��л�����һEdit�ؼ�
    case Down:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return NEXT_EDIT                                       ; // �û��л�����һEdit�ؼ�
    case Esc:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0xFF                                            ; // �û��˳�����
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0x00                                            ; // �û���������
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
    case F1:                                 // �л����뷨
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
      if(Input_CN(key,char_cn)==1)           // ����һ������
      {
        ActiveEditDig(Edit_Index,1)        ;
        EditDigMessage(129,char_cn)        ; 
      }
      ActiveEditDig(Renew_Edit_Index,1)    ;
      EditDigMessage(128,char_cn)          ;
      ActiveEditDig(Edit_Index,1)          ;
      break                                ;
    case Up:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return PRE_EDIT                      ; // �û��л�����һEdit�ؼ�
    case Down:       
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return NEXT_EDIT                     ; // �û��л�����һEdit�ؼ�
    case Power2:
//    SetBackLightAuto(0)                  ; 
      break                                ;
    case Left: case Right: case Backspace: case Delete:
      EditDigMessage(key,char_cn)          ; 
      break                                ;    
    case Esc:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0xFF                          ; // �û��˳�����
    case OK:case Enter:
      DrawRectFill(80,290,80,30,STATUS_BK_COLOR);// ����
      return 0x00                          ; // �û���������
    default:
      break                                ;
    }    
  }
  goto Input_Digital                       ;  
}


//***************************************************************************************
//																	                    *
// 		void CreateEditDig(): �������ֱ༭�ؼ�                           		        *
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
      Edit_Dig[i].status  |= EDIT_VALID     ; // ������Ч��־
      Edit_Dig[i].id       = ID             ; // ���ñ�ʶ
      Edit_Dig[i].x        = x_s            ; // ��������
      Edit_Dig[i].y        = y_s            ;
      Edit_Dig[i].width    = width          ; // ���ô�С
      Edit_Dig[i].height   = height         ;
      break                                 ;
    }
  }
}
//***************************************************************************************
//																	                    *
// 		void IniEditDig(): ��ʼ�����ֱ༭�ؼ�                           		        *
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
	    Edit_Dig[i].status    |= EDIT_MODE                        ; // ���ñ༭ģʽ
	  else
	    Edit_Dig[i].status    &=~EDIT_MODE                        ; 
	  if(font!=0)
	    Edit_Dig[i].status    |= EDIT_FONT                        ; // ��������
	  else
	    Edit_Dig[i].status    &=~EDIT_FONT                        ; 
	  Edit_Dig[i].property     = frame<<5                         ; // ���ñ߿�����
	  Edit_Dig[i].property    += limit                            ; // ��������ַ���  
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
// 		void IniEditDig(): ��ʼ�����ֱ༭�ؼ�                           		        *
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
// 		void ActiveEditDig(): �������ֱ༭�ؼ��״̬                     		        *
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
            Edit_Dig[Active_Edit_Dig].status |= EDIT_ACTIVE             ; // ���л�ؼ���ȡ����״̬������������ʾ
	    RedrawEditDig(Active_Edit_Dig)                              ;                                               		  
	  }
          Active_Edit_Dig     = i                                       ;
	  Edit_Dig[i].status |= EDIT_ACTIVE                             ; // ���ü���״̬
	  Active_Digit = strlen(Edit_Dig[i].string)                     ; // ������������һλ
        }
        else                                                              // ȡ���ؼ��״̬
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
// 		void ReadEditDig(): �������ֱ༭�ؼ��״̬                     		        *
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
	    Edit2Read   = i                                                 ; // ������
            ReadDigit   = 0x00                                              ; // ������λ
	  }
	}
  }
}

//***************************************************************************************
//									                *
// 		void ActiveEditDig(): �������ֱ༭�ؼ��״̬                          *
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
  temp  += Active_Digit*13-1-4                                          ; // ������λ��   
  i      = Active_Edit_Dig                                              ;
  DrawVerticalLine( temp,
	            Edit_Dig[Active_Edit_Dig].y
		   +Edit_Dig[Active_Edit_Dig].height/2-10,
		    22,Color_BK                           )             ; // �������
  if(Message==129)                                                        // �����������ַ�
  {
    if(strlen(Edit_Dig[i].string)+1<(Edit_Dig[i].property&EDIT_LIMIT))
    {
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+2] = 0x00           ; // �����ַ�
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
      Edit_Dig[i].string[strlen(Edit_Dig[i].string)+1] = 0x00           ; // �����ַ�
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
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // ɾ����һ�����ַ�
      else  
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // ɾ����һ�����ַ�
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
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+2]               ; // ɾ����һ�����ַ�
      else
        for(k=Active_Digit;k<temp;k++)                
          Edit_Dig[i].string[k] = Edit_Dig[i].string[k+1]               ; // ɾ����һ�ַ�
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
// 		void ActiveEditDig(): �������ֱ༭�ؼ��״̬                          *
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
	       Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)        ; // �����ʾ 
  if(Edit_Dig[i].property&EDIT_FRAME)                                     // ����Ʊ߿�
    DrawRect(Edit_Dig[i].x,Edit_Dig[i].y,
	     Edit_Dig[i].width,
	     Edit_Dig[i].height,Color    )                              ; 
  if(DISP_MODE==0x00)
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // ������ʾ
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,(unsigned char*)p);
  else
  {
    for(j=0;j<strlen(p);j++)
      string[j]  = '*'                                                  ;
    string[j]    = 0x00                                                 ;
    PutString24(Edit_Dig[i].x+Edit_Dig[i].width/2-strlen(p)*6-4,            // ������ʾ
	        Edit_Dig[i].y+Edit_Dig[i].height/2-12,string    )       ;
  }
  Event         |= SHOULD_FLICKER                                       ;
  flicker_status = 0x00                                                 ;
}
//***************************************************************************************
//											*
// 		void DeleteEditDig(): �������ֱ༭�ؼ��״̬                     	*
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
	    Edit_Dig[i].status   &=~EDIT_VALID                              ; // �����Ч״̬
	    if(Edit_Dig[i].status&EDIT_ACTIVE)		  
	      Active_Edit_Dig = 0xFF                                        ; // �������
	    Edit_Dig[i].status   &=~EDIT_ACTIVE                             ; // ����״̬		
	    DrawRectFill(Edit_Dig[i].x,Edit_Dig[i].y,
	                 Edit_Dig[i].width, Edit_Dig[i].height+1,Color_BK)  ; // �����ʾ 		
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
    temp  += Active_Digit*13-1-4                                      ; // �����������
    if(flicker_status++==0x00)
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color)    ; // ���ƹ��
    else
    {
      DrawVerticalLine( temp,Edit_Dig[Active_Edit_Dig].y+Edit_Dig[Active_Edit_Dig].height/2-10,22,Color_BK) ; // �������
      flicker_status = 0x00                                           ;
    }
  }
}
