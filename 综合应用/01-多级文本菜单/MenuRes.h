//===========================================================================//
//                                                                           //
// �ļ���  MenuRes.h                                                         //
// ˵����  BW-DK5438������༶�ı��˵���Դ����                               //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//
#ifndef   __MENURES_H
#define   __MENURES_H

#include "Menu.h"

// �˵���������
#define MENU_MAIN                        0x00  // ���˵�����
#define MENU_BOARD_SETTINGS              0x01  // ���������ò˵�����

// һ���˵���[0] :�����˵�
#define ITEM_HELP                        0x00  // ������Ϣ
#define ITEM_BOARD_INFO                  0x01  // ��������Ϣ
#define ITEM_BOARD_SETTINGS              0x02  // ����������

// �����˵���[4] :���ֳֻ�����˵�
#define ITEM_SET_TIME                    0x03  // �����ֳֻ�ʱ��
#define ITEM_SET_BACKLIGHT               0x04  // �����ֳֻ�����

// ������������

#define FUNC_SHOW_HELP                   0x00  // ��ʾ������Ϣ
#define FUNC_BOARD_INFO                  0x01  // ��������Ϣ  
#define FUNC_SET_TIME                    0x02  // ����ʱ��
#define FUNC_SET_BACKLIGHT               0x03  // ���ñ���

extern unsigned char (*Item_OP[ALL_ITEMS])(unsigned int,unsigned char)    ;
void str_cpy(char* Destination, char* Source,unsigned char Num)           ;


//****************************************************************************//
//                                                                            //
//                                 �˵����                                 //
//                                                                            //
//****************************************************************************//
const ITEM MENU_ITEM[] = 
{  
//--------------------------------- ���˵�[0] --------------------------------//  
  {ITEM_HELP                    ,
   EMPTY                        ,
   FUNC_SHOW_HELP               ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "������Ϣ"                    } ,  // [00]����
   
  {ITEM_BOARD_INFO              ,
   EMPTY                        ,
   FUNC_BOARD_INFO              ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "��������Ϣ"                  } ,  // [01]ˮ�����
   
  {ITEM_BOARD_SETTINGS          ,
   MENU_BOARD_SETTINGS          ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "����������"                  } ,  // [02]���ݹ���
   

//---------------- �����˵���[4] : ���������ò˵� -------------------------------//
   
  {ITEM_SET_TIME                ,
   EMPTY                        ,   
   FUNC_SET_TIME                ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "����ʱ��"                    } ,  // [38]����ʱ��
   
  {ITEM_SET_BACKLIGHT           ,
   EMPTY                        ,
   FUNC_SET_BACKLIGHT           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "����Һ������"                } ,  // [39]����Һ������   
   
};

//****************************************************************************//
//                                                                            //
//                                 �˵�����                                   //
//                                                                            //
//****************************************************************************//
const MENU_RES Menus[] =                                           
{
//--------------------------- Menus[0]�� ���˵� -----------------------------------//  
  {
    MENU_MAIN                                      ,  // �˵�����
    EMPTY                                          ,
    EMPTY                                          ,
    "BW-DK5438"                                    ,
    {3,24,24,28,15,SINGLE_PAGE,EMPTY}              ,  
    {
      ITEM_HELP                                    ,  // ˮ�����ݹ���
      ITEM_BOARD_INFO                              ,  // ��������Ϣ
      ITEM_BOARD_SETTINGS                          ,  // ����������
    }                                            
  }                                                ,

//---------------------------Menus[4]�� �ֳֻ�����˵�  -------------------------//   
  {
    MENU_BOARD_SETTINGS                            ,  // �˵�����
    MENU_MAIN                                      ,
    ITEM_BOARD_SETTINGS                            ,
    "����������"                                   ,
    {2,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_SET_TIME                                ,  // ���ÿ�����ʱ��
      ITEM_SET_BACKLIGHT                           ,  // ����Һ������
    }                                            
  }                                                ,
}                                                                     ;


#endif