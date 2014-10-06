//===========================================================================//
//                                                                           //
// 文件：  MenuRes.h                                                         //
// 说明：  BW-DK5438开发板多级文本菜单资源定义                               //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//
#ifndef   __MENURES_H
#define   __MENURES_H

#include "Menu.h"

// 菜单索引定义
#define MENU_MAIN                        0x00  // 主菜单索引
#define MENU_BOARD_SETTINGS              0x01  // 开发板设置菜单索引

// 一级菜单项[0] :　主菜单
#define ITEM_HELP                        0x00  // 帮助信息
#define ITEM_BOARD_INFO                  0x01  // 开发板信息
#define ITEM_BOARD_SETTINGS              0x02  // 开发板设置

// 二级菜单项[4] :　手持机管理菜单
#define ITEM_SET_TIME                    0x03  // 设置手持机时间
#define ITEM_SET_BACKLIGHT               0x04  // 设置手持机背光

// 操作函数定义

#define FUNC_SHOW_HELP                   0x00  // 显示帮助信息
#define FUNC_BOARD_INFO                  0x01  // 开发板信息  
#define FUNC_SET_TIME                    0x02  // 设置时间
#define FUNC_SET_BACKLIGHT               0x03  // 设置背光

extern unsigned char (*Item_OP[ALL_ITEMS])(unsigned int,unsigned char)    ;
void str_cpy(char* Destination, char* Source,unsigned char Num)           ;


//****************************************************************************//
//                                                                            //
//                                 菜单项定义                                 //
//                                                                            //
//****************************************************************************//
const ITEM MENU_ITEM[] = 
{  
//--------------------------------- 主菜单[0] --------------------------------//  
  {ITEM_HELP                    ,
   EMPTY                        ,
   FUNC_SHOW_HELP               ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "帮助信息"                    } ,  // [00]帮助
   
  {ITEM_BOARD_INFO              ,
   EMPTY                        ,
   FUNC_BOARD_INFO              ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "开发板信息"                  } ,  // [01]水表管理
   
  {ITEM_BOARD_SETTINGS          ,
   MENU_BOARD_SETTINGS          ,
   EMPTY                        ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "开发板设置"                  } ,  // [02]数据管理
   

//---------------- 二级菜单项[4] : 开发板设置菜单 -------------------------------//
   
  {ITEM_SET_TIME                ,
   EMPTY                        ,   
   FUNC_SET_TIME                ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "设置时间"                    } ,  // [38]设置时间
   
  {ITEM_SET_BACKLIGHT           ,
   EMPTY                        ,
   FUNC_SET_BACKLIGHT           ,
   ITEM_ACCESS_OPERATOR         ,
   STATIC                       ,
   "设置液晶背光"                } ,  // [39]设置液晶背光   
   
};

//****************************************************************************//
//                                                                            //
//                                 菜单定义                                   //
//                                                                            //
//****************************************************************************//
const MENU_RES Menus[] =                                           
{
//--------------------------- Menus[0]： 主菜单 -----------------------------------//  
  {
    MENU_MAIN                                      ,  // 菜单索引
    EMPTY                                          ,
    EMPTY                                          ,
    "BW-DK5438"                                    ,
    {3,24,24,28,15,SINGLE_PAGE,EMPTY}              ,  
    {
      ITEM_HELP                                    ,  // 水表数据管理
      ITEM_BOARD_INFO                              ,  // 开发板信息
      ITEM_BOARD_SETTINGS                          ,  // 开发板设置
    }                                            
  }                                                ,

//---------------------------Menus[4]： 手持机管理菜单  -------------------------//   
  {
    MENU_BOARD_SETTINGS                            ,  // 菜单索引
    MENU_MAIN                                      ,
    ITEM_BOARD_SETTINGS                            ,
    "开发板设置"                                   ,
    {2,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_SET_TIME                                ,  // 设置开发板时间
      ITEM_SET_BACKLIGHT                           ,  // 设置液晶背光
    }                                            
  }                                                ,
}                                                                     ;


#endif