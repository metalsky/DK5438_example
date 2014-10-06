//===========================================================================//
//                                                                           //
// 文件：  GUIMenu.H                                                         //
// 说明：  BW-DK5438开发板图形用户界面头文件                                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#define MAX_ITEM    25                            // 最大菜单项数，按需要调整

typedef struct 
{
  unsigned char     MenuIndex                   ; // 菜单索引
  unsigned char     MenuTitle[20]               ; // 菜单标题
  unsigned char     MenuConfig[40]              ; // 菜单配置
  unsigned char     ItemTitle[25][20]           ; // 菜单项
  unsigned char     ItemIndex[MAX_ITEM]         ; // 菜单项索引
  unsigned char     ItemConfig[25]              ; // 菜单项配置字
} GUIMENU                                       ; 

typedef struct
{
  unsigned char id                              ; // 识别号
  unsigned int  x                               ; // 起始位置横坐标x
  unsigned int  y                               ; // 起始位置纵坐标y
  unsigned char width                           ; // 宽度
  unsigned char height                          ; // 高度
  unsigned char property                        ; // 属性
  unsigned char status                          ; // 状态
  unsigned char limit                           ; // 最大字符数
  char string[50]                               ; // 字符串
} EDIT_DIG                                      ; // 编辑控件
