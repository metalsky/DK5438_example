//===========================================================================//
//                                                                           //
// 文件：  Menu_Resource.H                                                   //
// 说明：  BW-DK5438开发板管理程序GUI菜单资源定义                            //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef __MENU_RESOURCE_H
#define __MENU_RESOURCE_H

#include "GUI.H"
#include "TFTDRV.H"

// 菜单常量定义

#define EMPTY                            0xFF  // 无效标识
#define DYNAMIC_OP                       0xFE  // 动态可执行菜单标识
#define DYNAMIC_VIEW                     0xFD  // 动态视图菜单标识
#define STATIC                           0xFC  // 静态菜单
#define SINGLE_PAGE                      0x00
#define MULTI_PAGE                       0x11
#define NEXT_PAGE                        0x11
#define PREVIOUS_PAGE                    0x22
#define CHECK_RECORD                     0x11
#define ITEM_ACCESS_HIDDEN               0x04  // 尚未开放
#define ITEM_ACCESS_SYSTEM               0x03  // 系统级
#define ITEM_ACCESS_MANAGER              0x02  // 管理员级
#define ITEM_ACCESS_OPERATOR             0x01  // 操作员级

// 菜单索引定义
#define MAIN_MENU                        0x00  // 主菜单索引
#define SYSTEM_CFG_MENU                  0x01  // 系统设置菜单


// 一级菜单项[0] :　主菜单
#define ITEM_DISPLAY_CTR                 0x00  // 数码管显示控制
#define ITEM_MY_CLOCK                    0x01  // 我的时钟
#define ITEM_SYSTEM_CFG                  0x02  // 系统设置
#define ITEM_COMM                        0x03  // 端口连接
#define ITEM_IRDA                        0x04  // 红外传输
#define ITEM_INFO                        0x04  // 任务信息
#define ITEM_PAINTER                     0x05  // 画图
#define ITEM_PICTURE                     0x06  // 图片
#define ITEM_ETHERNET                    0x07  // 网络连接
#define ITEM_POWER_OFF                   0x08  // 关闭电源

// 二级菜单项[1] :　系统设置菜单
#define ITEM_BACKLIGHT_SET               0x09  // TFT液晶背光设置
#define ITEM_COMM_SET                    0x0A  // 通讯端口设置
#define ITEM_CLOCK_SET                   0x0B  // 时钟设置
#define ITEM_TP_ADJUST                   0x0C  // 触控校正
#define ITEM_RESERVE_05                  0x0D  // 保留
#define ITEM_RESERVE_06                  0x0E  // 保留
#define ITEM_RESERVE_07                  0x0F  // 保留


// 操作函数定义
#define FUNC_MY_CLOCK                    0x00  // 数码管显示控制
#define FUNC_DISPLAY_CTR                 0x01  // 系统时钟管理
#define FUNC_SYSTEM_CFG                  0x02  // 系统设置
#define FUNC_COMM                        0x03  // 端口连接
#define FUNC_IRDA                        0x04  // 红外传输
#define FUNC_TASK_INFO                   0x04  // 任务信息
#define FUNC_PAINTER                     0x05  // 画图
#define FUNC_PICTURE                     0x06  // 图片
#define FUNC_ETHERNET                    0x07  // 网络连接
#define FUNC_POWER_OFF                   0x08  // 关闭电源
#define FUNC_BACKLIGHT_SET               0x09  // TFT液晶背光设置
#define FUNC_COMM_SET                    0x0A  // 通讯端口设置
#define FUNC_CLOCK_SET                   0x0B  // 时间设置
#define FUNC_TP_ADJUST                   0x0C  // 触控校正

extern char (*Item_OP[MAX_FUNC_ITEM])(char *par)                       ;
void str_cpy(char* Destination, char* Source,unsigned char Num)        ;


//***************************MenuConfig 定义**********************************//
//   MenuConfig[0] ：菜单项数目
//   MenuConfig[1] ：中文字体大小
//   MenuConfig[2] ：西文字体大小
//   MenuConfig[3] ：每一菜单项所占用的Y轴的点数
//   MenuConfig[4] ：X轴固定偏移
//   MenuConfig[5] ：分页模式
//   MenuConfig[6] ：退出菜单调用函数(一般为视图菜单用以保存数据)
//   MenuConfig[7] ：分页函数索引
//   MenuConfig[8] ：菜单页数
//   MenuConfig[9] ：辅助参数，由应用函数定义
//         .                      .
//         .                      .
//         .                      .
//   MenuConfig[15]：多页菜单，菜单页数，供返回时使用
//****************************************************************************//

const ITEM MENU_ITEM[] = 
{ 
//----------------------------- 一级菜单(主菜单)项[0] ------------------------// 
  {ITEM_DISPLAY_CTR             ,
   EMPTY                        ,
   FUNC_TASK_INFO               ,
   ICON_DISPLAY                 ,
   "任务信息"                    } ,  // [00] 红外传输
   
  {ITEM_MY_CLOCK                ,
   EMPTY                        ,
   FUNC_MY_CLOCK                ,
   ICON_CLOCK                   ,
   "我的时钟"                    } ,  // [01] 我的时钟
   
  {ITEM_SYSTEM_CFG              ,
   EMPTY                        ,
   FUNC_SYSTEM_CFG              ,
   ICON_CONFIG                  ,
   "触摸校正"                    } ,  // [02] 系统设置
   
  {ITEM_COMM                    ,
   EMPTY                        ,
   FUNC_COMM                    ,
   ICON_PORT                    ,
   "端口连接"                    } ,  // [03] 端口连接
   
  {ITEM_IRDA                    ,
   EMPTY                        ,
   FUNC_IRDA                    ,
   ICON_IRDA                    ,
   "红外传输"                    } ,  // [04] 红外传输
   
  {ITEM_PAINTER                 ,
   EMPTY                        ,
   FUNC_PAINTER                 ,
   ICON_PAINT                   ,
   "画笔"                        } ,  // [05] 画图
   
  {ITEM_PICTURE                 ,
   EMPTY                        ,
   FUNC_PICTURE                 ,
   ICON_PICTURE                 ,
   "我的相册"                    } ,  // [06] 图片
   
  {ITEM_ETHERNET                ,
   EMPTY                        ,
   FUNC_DISPLAY_CTR             ,
   ICON_NETWORK                 ,
   "显示控制"                    } ,  // [07] 显示控制
   
  {ITEM_POWER_OFF               ,
   EMPTY                        ,
   FUNC_POWER_OFF               ,
   ICON_SLEEP                   ,
   "系统休眠"                    } ,  // [08] 系统休眠
   
  
//------------------------二级菜单项[2]：系统设置菜单 ------------------------//
  {ITEM_BACKLIGHT_SET           ,
   EMPTY                        ,
   FUNC_BACKLIGHT_SET           ,
   ICON_BACKLIGHT               ,     
   "背光设置"                    } ,  // [09] 背光设置
   
  {ITEM_COMM_SET                ,
   EMPTY                        ,
   FUNC_COMM_SET                ,
   0x155BA0                     ,     //***********需修改
   "端口设置"                    } ,  // [0A] 端口设置
   
  {ITEM_CLOCK_SET               ,
   EMPTY                        ,
   FUNC_CLOCK_SET               ,
   0x155BA0                     ,     //***********需修改
   "时钟设置"                    } ,  // [0B] 时钟设置         

  {ITEM_TP_ADJUST               ,
   EMPTY                        ,
   FUNC_TP_ADJUST               ,
   0x155BA0                     ,     //***********需修改
   "触控校正"                    } ,  // [0B] 时钟设置         
}                                                                       ;

const MENU_RES Menus[] =                                          
{
//--------------------------- Menus[0]： 主菜单 -----------------------------------//  
  {
    MAIN_MENU                                      ,                        
    EMPTY                                          ,
    EMPTY                                          ,
    "控制面板"                                     ,
    {9,24,24,28,15,SINGLE_PAGE,EMPTY}              ,  
    {
      ITEM_MY_CLOCK                                , // 我的时钟
      ITEM_DISPLAY_CTR                             , // 任务信息
      ITEM_COMM                                    , // 端口连接
      ITEM_IRDA                                    , // 红外传输
      ITEM_ETHERNET                                , // 显示控制
      ITEM_PAINTER                                 , // 画图
      ITEM_PICTURE                                 , // 图片
      ITEM_SYSTEM_CFG                              , // 触摸校正
      ITEM_POWER_OFF                               , // 关闭电源
    }                                            
  }                                                ,
//--------------------------- Menus[1]： 集中器管理菜单 ---------------------------//    
  {
    SYSTEM_CFG_MENU                                ,                      
    MAIN_MENU                                      ,
    ITEM_SYSTEM_CFG                                ,
    "系统设置"                                     ,
    {3,24,24,28,15,SINGLE_PAGE,EMPTY}              ,      
    {
      ITEM_BACKLIGHT_SET                           , // TFT液晶背光设置
      ITEM_COMM_SET                                , // 通讯端口设置
      ITEM_CLOCK_SET                               , // 时钟设置
    }
  }                                                ,    
}                                                                     ;

#endif