//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板多级文本菜单数据结构定义及函数声明头文件           //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//
#ifndef _MENU_H
#define _MENU_H

#include "TFTDRV430.H"
#include "string.h"

// 菜单常量定义
#define MAX_ITEM                               18  
#define ALL_ITEMS                              60
#define EMPTY                                0xFF // 无效标识
#define DYNAMIC_OP                           0xFE // 动态可执行菜单标识
#define DYNAMIC_VIEW                         0xFD // 动态视图菜单标识
#define STATIC                               0xFC // 静态菜单
#define SINGLE_PAGE                          0x00
#define MULTI_PAGE                           0x11
#define NEXT_PAGE                            0x11
#define PREVIOUS_PAGE                        0x22
#define CHECK_RECORD                         0x11
#define ITEM_ACCESS_HIDDEN                   0x04 // 尚未开放
#define ITEM_ACCESS_SYSTEM                   0x03 // 系统级
#define ITEM_ACCESS_MANAGER                  0x02 // 管理员级
#define ITEM_ACCESS_OPERATOR                 0x01 // 操作员级

typedef  struct 
{
  unsigned char  ItemIndex                      ; // 菜单项目索引
  unsigned char  SubMenuIndex                   ; // 子菜单索引
  unsigned char  CallBackFuncIndex              ; // 回调函数索引
  unsigned char  AccessLevel                    ; // 菜单项权限
  unsigned char  Config                         ; // 菜单项类别
  char           ItemTitle[20]                  ; // 菜单项名称
} ITEM                                          ;

typedef struct                                    // 调入内存运行
{
  unsigned char     MenuIndex                   ;
  unsigned char     MenuConfig[40]              ; // 菜单配置
  unsigned char     MenuTitle[20]               ; // 菜单标题
  unsigned char     ItemTitle[25][20]           ; // 菜单项
  unsigned char     ItemIndex[25]               ; // 菜单项索引
  unsigned char     ItemConfig[25]              ; // 菜单项配置字
} MENU                                          ; 

typedef struct                                    // 菜单资源
{
  unsigned char     MenuIndex                   ; // 菜单索引 
  unsigned char     MasterMenu                  ; // 父菜单索引  
  unsigned char     MasterItem                  ; // 父菜单项索引
  unsigned char     MenuTitle[20]               ; // 菜单标题
  unsigned char     MenuConfig[40]              ; // 菜单配置
  unsigned char     ItemIndex[MAX_ITEM]         ; // 菜单项索引
} MENU_RES                                      ; // 菜单资源

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

#define MAX_ITEM                    18  
#define ALL_ITEMS                   60
#define EMPTY                     0xFF
#define Y_OFFSET                    72       // 显示菜单的Y轴偏移量
#define Y_WIDTH_MENU                30       // 菜单项高度
#define X_SPACE_FRONT               15       // 菜单项左缩进

#define SHOULD_FLICKER            BIT1       // 应闪烁
#define MENU_LOADED               BIT2       // 菜单已下载

//*
// 界面颜色
#define WINDOW_BK_COLOR         0xDFFF       // 窗口背景色
#define WINDOW_COLOR            0x11FA       // 窗口前景色

#define TITLE_BK_COLOR          0x11FA       // 标题栏背景色
#define TITLE_COLOR             0xDFFF       // 标题栏前景色
#define STATUS_BK_COLOR         0x0014       // 状态栏背景色
#define STATUS_COLOR            0xDFFF       // 状态栏前景色

#define MENU_BACK_COLOR		WINDOW_BK_COLOR // 未选择菜单项背景色
#define MENU_FONT_COLOR		WINDOW_COLOR 	// 未选择菜单项文字色
#define MENU_SELE_COLOR		WINDOW_COLOR    // 选择菜单项背景色
#define MENU_SELF_COLOR		WINDOW_BK_COLOR	// 选择菜单项文字色
//*/
/*
// 界面颜色
#define WINDOW_BK_COLOR         Black           // 窗口背景色
#define WINDOW_COLOR            White           // 窗口前景色

#define TITLE_BK_COLOR          Dark_Grey2      // 标题栏背景色
#define TITLE_COLOR             White           // 标题栏前景色
#define STATUS_BK_COLOR         0x3186          // 状态栏背景色
#define STATUS_COLOR            0xF7BE          // 状态栏前景色

#define MENU_BACK_COLOR		WINDOW_BK_COLOR // 未选择菜单项背景色
#define MENU_FONT_COLOR		WINDOW_COLOR 	// 未选择菜单项文字色
//#define MENU_SELE_COLOR		0xFD6A  // 选择菜单项背景色
#define MENU_SELE_COLOR		Yellow          // 选择菜单项背景色
#define MENU_SELF_COLOR		WINDOW_BK_COLOR	// 选择菜单项文字色
*/
/*							
#define TITLE_BK_COLOR          0xF800       // 标题栏背景色
#define TITLE_COLOR             0xDFFF       // 标题栏前景色
#define STATUS_BK_COLOR         0xF800       // 状态栏背景色
#define STATUS_COLOR            0xDFFF       // 状态栏前景色
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