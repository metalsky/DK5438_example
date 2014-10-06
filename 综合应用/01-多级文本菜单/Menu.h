//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������༶�ı��˵����ݽṹ���弰��������ͷ�ļ�           //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//
#ifndef _MENU_H
#define _MENU_H

#include "TFTDRV430.H"
#include "string.h"

// �˵���������
#define MAX_ITEM                               18  
#define ALL_ITEMS                              60
#define EMPTY                                0xFF // ��Ч��ʶ
#define DYNAMIC_OP                           0xFE // ��̬��ִ�в˵���ʶ
#define DYNAMIC_VIEW                         0xFD // ��̬��ͼ�˵���ʶ
#define STATIC                               0xFC // ��̬�˵�
#define SINGLE_PAGE                          0x00
#define MULTI_PAGE                           0x11
#define NEXT_PAGE                            0x11
#define PREVIOUS_PAGE                        0x22
#define CHECK_RECORD                         0x11
#define ITEM_ACCESS_HIDDEN                   0x04 // ��δ����
#define ITEM_ACCESS_SYSTEM                   0x03 // ϵͳ��
#define ITEM_ACCESS_MANAGER                  0x02 // ����Ա��
#define ITEM_ACCESS_OPERATOR                 0x01 // ����Ա��

typedef  struct 
{
  unsigned char  ItemIndex                      ; // �˵���Ŀ����
  unsigned char  SubMenuIndex                   ; // �Ӳ˵�����
  unsigned char  CallBackFuncIndex              ; // �ص���������
  unsigned char  AccessLevel                    ; // �˵���Ȩ��
  unsigned char  Config                         ; // �˵������
  char           ItemTitle[20]                  ; // �˵�������
} ITEM                                          ;

typedef struct                                    // �����ڴ�����
{
  unsigned char     MenuIndex                   ;
  unsigned char     MenuConfig[40]              ; // �˵�����
  unsigned char     MenuTitle[20]               ; // �˵�����
  unsigned char     ItemTitle[25][20]           ; // �˵���
  unsigned char     ItemIndex[25]               ; // �˵�������
  unsigned char     ItemConfig[25]              ; // �˵���������
} MENU                                          ; 

typedef struct                                    // �˵���Դ
{
  unsigned char     MenuIndex                   ; // �˵����� 
  unsigned char     MasterMenu                  ; // ���˵�����  
  unsigned char     MasterItem                  ; // ���˵�������
  unsigned char     MenuTitle[20]               ; // �˵�����
  unsigned char     MenuConfig[40]              ; // �˵�����
  unsigned char     ItemIndex[MAX_ITEM]         ; // �˵�������
} MENU_RES                                      ; // �˵���Դ

//***************************MenuConfig ����**********************************//
//   MenuConfig[0] ���˵�����Ŀ
//   MenuConfig[1] �����������С
//   MenuConfig[2] �����������С
//   MenuConfig[3] ��ÿһ�˵�����ռ�õ�Y��ĵ���
//   MenuConfig[4] ��X��̶�ƫ��
//   MenuConfig[5] ����ҳģʽ
//   MenuConfig[6] ���˳��˵����ú���(һ��Ϊ��ͼ�˵����Ա�������)
//   MenuConfig[7] ����ҳ��������
//   MenuConfig[8] ���˵�ҳ��
//   MenuConfig[9] ��������������Ӧ�ú�������
//         .                      .
//         .                      .
//         .                      .
//   MenuConfig[15]����ҳ�˵����˵�ҳ����������ʱʹ��
//****************************************************************************//

#define MAX_ITEM                    18  
#define ALL_ITEMS                   60
#define EMPTY                     0xFF
#define Y_OFFSET                    72       // ��ʾ�˵���Y��ƫ����
#define Y_WIDTH_MENU                30       // �˵���߶�
#define X_SPACE_FRONT               15       // �˵���������

#define SHOULD_FLICKER            BIT1       // Ӧ��˸
#define MENU_LOADED               BIT2       // �˵�������

//*
// ������ɫ
#define WINDOW_BK_COLOR         0xDFFF       // ���ڱ���ɫ
#define WINDOW_COLOR            0x11FA       // ����ǰ��ɫ

#define TITLE_BK_COLOR          0x11FA       // ����������ɫ
#define TITLE_COLOR             0xDFFF       // ������ǰ��ɫ
#define STATUS_BK_COLOR         0x0014       // ״̬������ɫ
#define STATUS_COLOR            0xDFFF       // ״̬��ǰ��ɫ

#define MENU_BACK_COLOR		WINDOW_BK_COLOR // δѡ��˵����ɫ
#define MENU_FONT_COLOR		WINDOW_COLOR 	// δѡ��˵�������ɫ
#define MENU_SELE_COLOR		WINDOW_COLOR    // ѡ��˵����ɫ
#define MENU_SELF_COLOR		WINDOW_BK_COLOR	// ѡ��˵�������ɫ
//*/
/*
// ������ɫ
#define WINDOW_BK_COLOR         Black           // ���ڱ���ɫ
#define WINDOW_COLOR            White           // ����ǰ��ɫ

#define TITLE_BK_COLOR          Dark_Grey2      // ����������ɫ
#define TITLE_COLOR             White           // ������ǰ��ɫ
#define STATUS_BK_COLOR         0x3186          // ״̬������ɫ
#define STATUS_COLOR            0xF7BE          // ״̬��ǰ��ɫ

#define MENU_BACK_COLOR		WINDOW_BK_COLOR // δѡ��˵����ɫ
#define MENU_FONT_COLOR		WINDOW_COLOR 	// δѡ��˵�������ɫ
//#define MENU_SELE_COLOR		0xFD6A  // ѡ��˵����ɫ
#define MENU_SELE_COLOR		Yellow          // ѡ��˵����ɫ
#define MENU_SELF_COLOR		WINDOW_BK_COLOR	// ѡ��˵�������ɫ
*/
/*							
#define TITLE_BK_COLOR          0xF800       // ����������ɫ
#define TITLE_COLOR             0xDFFF       // ������ǰ��ɫ
#define STATUS_BK_COLOR         0xF800       // ״̬������ɫ
#define STATUS_COLOR            0xDFFF       // ״̬��ǰ��ɫ
*/
#define Dis_X_MAX		       228
#define Dis_Y_MAX		       289

void LoadMenu(unsigned char Menu_Index,unsigned char Permission) ;
void Initial_Menu(void)                                          ;
void ShowMenu_Item(unsigned int y,unsigned char index,
                   unsigned char Selected            )           ;
void Redraw_Menu(unsigned char First_Index,
                 unsigned char Menu_Index  )                     ;
void UpDate_Menu(unsigned char First_Index,
                 unsigned char Menu_Index  )                     ;
void Update_ScrollBar(unsigned int x,unsigned int y,
                      unsigned char height,
                      unsigned char position,
		      unsigned char total           )            ;
void MenuMessage(unsigned char Message)                          ;                    
void str_cpy(char* Destination, char* Source,unsigned char Num)  ;
unsigned char GetFirstItem(void)                                 ;
unsigned char GetActiveItem(void)                                ;
unsigned char GetActiveItemIndex(void)                           ;
unsigned char GetMenuIndex(void)                                 ;

#endif