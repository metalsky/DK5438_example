//===========================================================================//
//                                                                           //
// �ļ���  Menu.c                                                            //
// ˵����  BW-DK5438������༶�ı��˵���������                               //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//
#include "Menu.h"

extern void str_cpy(char* Destination, char* Source,unsigned char Num)  ;
extern const ITEM MENU_ITEM[]                                           ;
extern const MENU_RES Menus[]                                           ;
extern unsigned int  Color                                              ; // ǰ����ɫ
extern unsigned int  Color_BK                                           ; // ������ɫ
extern MENU  Menu                                                       ;

unsigned char Font_CN          = 24                                     ; // ��������
unsigned char Font_EN          = 24                                     ; // ��������
unsigned char MenuNum          = 0xFF                                   ; // ���ϴ��Ĳ˵���������
unsigned char MenuUpdated      = 0xFF                                   ; // �˵����£����ϴ���ǰ�˵�ѡ��
unsigned char First_Index_old  = 0xFF                                   ;
unsigned char y_Index_old      = 0xFF                                   ;
unsigned char First_Index      = 0x00                                   ;
unsigned char Active_Index     = 0x00                                   ;
unsigned char Dis_Menu_Num                                              ;


//=====================================================================//
//������void UpDate_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//���ܣ�ˢ�������˵���ʾ����
//������First_Index -  ��ʾ�ĵ�һ���˵���
//	    Menu_Index  -  ����ѡ��״̬�Ĳ˵���
//	    Menu_List   -  �˵���Դ����ָ��
//���أ���
//����:        2010.01.20        
//=====================================================================//
void UpDate_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  unsigned int  y_Index                                                                ;
  unsigned char i                                                                      ;
  _DINT()                                                                              ;
  if(Dis_Menu_Num+First_Index> Menu.MenuConfig[0])
    Dis_Menu_Num =  Menu.MenuConfig[0] - First_Index                                   ; 
  y_Index  = 0                                                                         ; // 
  if(First_Index_old!=First_Index)                                                       // window�������˵���ı� 
  {
    if(y_Index_old==0xFF)                                                                // ��ʼ���Ʋ˵�
    {
      DrawRectFill(0,60,240,230,WINDOW_BK_COLOR)                                       ; // ��մ��� 
      Color    = TITLE_COLOR                                                           ; // ��ʾ������
      Color_BK = TITLE_BK_COLOR                                                        ;
      DrawRectFill(0,30,240,30,Color_BK)                                               ;
      PutString24M(120-25*strlen((char*)Menu.MenuTitle)/4,33,Menu.MenuTitle)           ;
      y_Index = Y_OFFSET-1                                                             ; // ���϶�����ʾ���˵���
      for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
      {
        if(i==Menu_Index)
		{
          DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)             ; // ��ʾѡ����		
          ShowMenu_Item(y_Index,i+1,1)                                                 ;
		}
	    else
          ShowMenu_Item(y_Index,i+1,0)                                                 ; // ��ʾ�Ǳ�ѡ�˵���
	    y_Index += Y_WIDTH_MENU                                                    ;
      }        

      y_Index_old     = 0                                                              ;
      First_Index_old = 0                                                              ;
    }
    else                                                                                 // �ײ˵���ı� 
    {
      Color_BK = WINDOW_BK_COLOR                                                       ;            
      if(First_Index_old<First_Index)                                                    // �˵����Ϲ���
      {
        y_Index = Y_OFFSET-1+Y_WIDTH_MENU*(Dis_Menu_Num-1)                             ;
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // ��ѡ�в˵�������
        DrawRectFill(0,Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                      )    ; // ��ձ�ѡ�˵����ϲ�����
      	y_Index = Y_OFFSET-1 + Y_WIDTH_MENU*(Dis_Menu_Num-1)                           ; // ���¶�����ʾ��ѡ�˵���
        for(i=0;i<Dis_Menu_Num;i++)
        {
		  if(i==0)
            ShowMenu_Item(y_Index,Menu_Index-i+1,1)                                    ;
	      else
            ShowMenu_Item(y_Index,Menu_Index-i+1,0)                                    ; // ��ʾ�Ǳ�ѡ�˵���
	      y_Index -= Y_WIDTH_MENU                                                  ;
        }        
      }
      else
      {
	y_Index = Y_OFFSET-1                                                           ;
        DrawRectFill(0,Y_WIDTH_MENU+Y_OFFSET-1,Dis_X_MAX,Y_WIDTH_MENU*(Dis_Menu_Num-1),
		             WINDOW_BK_COLOR                                          ); // ��ձ�ѡ�˵����²�����
        DrawRectFill(0,y_Index-1,Dis_X_MAX,Y_WIDTH_MENU,MENU_SELE_COLOR)               ; // ��ѡ�в˵�������
        for(i=First_Index;i<Dis_Menu_Num+First_Index;i++)
        {
          if(i==Menu_Index)
            ShowMenu_Item(y_Index,i+1,1)                                               ;
	      else
            ShowMenu_Item(y_Index,i+1,0)                                               ; // ��ʾ�Ǳ�ѡ�˵���
	      y_Index += Y_WIDTH_MENU                                                  ;
        }        
      }
    }
    First_Index_old = First_Index                                                      ;
  }
  else                                                                                   // �ײ˵����ޱ仯
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
//������UCHAR ShowMenu_Item(UCHAR y,UINT* Menu_String,UCHAR Font_Color)
//��������ʾ�˵����Ӻ���
//������space_front  ��ʾ����ֵ
//		y            Y������
//		Menu_String  �˵���������׵�ַָ��
//=====================================================================//
void ShowMenu_Item(unsigned int y,unsigned char index,unsigned char Selected)
{
  unsigned char id[3]                               ;
  Color_BK = WINDOW_BK_COLOR                        ;
  Color    = WINDOW_COLOR                           ;
  DrawRectFill(15,y+3,22,21,Color_BK)               ; // ��䱳��ɫ   
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
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 11:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 12:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 13:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 14:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 15:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 16:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 17:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 18:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 19:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    case 20:
      PutStringCN24(15,y+2,"��")                    ;
      break                                         ;
    }
    DrawRectFill(35,y+18,3,4,Color_BK)              ; // ��������ĵ�
  } 
  DrawRect(14,y+2,24,22,Color)                      ;
  if(Selected)
    DrawRectFill(14,y,24,1,MENU_SELE_COLOR)         ; // ����������ſ�Χ�ĵ�
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
//������void Redraw_Menu(UCHAR First_Index,UCHAR Menu_Index,UINT** Menu_List)
//������ˢ�������˵���ʾ����
//������First_Index  ������ʾҳ�ĵ�һ���˵���
//		Menu_Index   ��ǰ����ѡ�õĲ˵���
//		Menu_List    �˵���Դ����ָ��
//=====================================================================//
void Redraw_Menu(unsigned char First_Index,unsigned char Menu_Index)
{
  First_Index_old   = 0xFF                           ;
  y_Index_old       = 0xFF                           ;
  Dis_Menu_Num = (Dis_Y_MAX+1-Y_OFFSET)/Y_WIDTH_MENU ;  
  UpDate_Menu(First_Index,Menu_Index)                ;  
}
//=====================================================================//
//������void Initial_Menu(UINT** Menu_List)
//���ܣ�ˢ�������˵���ʾ����
//������Menu_List - �˵���Դ����ָ��
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
  PutStringCN24(10,294,"ȷ��")                            ; // ����
  if(Menu.MenuIndex>0)
    PutStringCN24(185,294,"����")                         ; 
  else
    DrawRectFill(185,294,54,25,STATUS_BK_COLOR)           ;
}
//***************************************************************************************
//																	                    *
// 		void ActiveEditDig(): �������ֱ༭�ؼ��״̬                     		        *
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
  DrawRectFill(x,y,8,height,WINDOW_BK_COLOR)              ; // ���
  DrawRect(x,y,8,height,WINDOW_COLOR)                     ; 
  temp = y+(unsigned int)(height-44)*position/(total-1)   ;
  DrawRectFill(x,temp,8,44,WINDOW_COLOR)                  ; // ����
}


void LoadMenu(unsigned char Menu_Index,unsigned char Permission)
{
  unsigned char i,j,k                                                                ; // ����˵�������
  Menu.MenuIndex =  Menu_Index                                                       ;
  str_cpy((char *)Menu.MenuConfig,(char *)Menus[Menu_Index].MenuConfig,10)           ; // �����˵�������
  strcpy((char *)Menu.MenuTitle,(char *)Menus[Menu_Index].MenuTitle)                 ; // �����˵�����
  Menu.MenuConfig[0] = 0                                                             ; // �˵�������0
  for(i=1;i< Menus[Menu_Index].MenuConfig[0]+1;i++)                                    // ����˵������ַ���
  {
    if(Permission>=(MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].AccessLevel))
    {
      j = Menu.MenuConfig[0]++                                                       ; // ����˵�����Ŀ
      Menu.ItemIndex[j] = Menus[Menu_Index].ItemIndex[i-1]                           ; // ��¼�˵�������
      for(k=0;k<20;k++)
        Menu.ItemTitle[j][k] = 0x00                                                  ;
      strcpy((char *)Menu.ItemTitle[j],
             (char *)MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].ItemTitle)          ; // �����˵������
      Menu.ItemConfig[j] =  MENU_ITEM[Menus[Menu_Index].ItemIndex[i-1]].Config       ;     // �����˵���������      
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