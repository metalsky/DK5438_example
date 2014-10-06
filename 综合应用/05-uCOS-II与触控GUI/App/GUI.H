//===========================================================================//
//                                                                           //
// 文件：  GUIMenu.H                                                         //
// 说明：  BW-DK5438开发板图形用户界面头文件                                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef  __GUI_H_
#define  __GUI_H_

#define MAX_MENU_ITEM    25                       // 最大菜单项数量，按需要调整
#define MAX_FUNC_ITEM    20                       // 最大操作函数数量，按需要调整
#define CN_INPUT        129                       // EDIT\VIEW控件输入中文字符
#define EN_INPUT        128 

//---------------------------------- LCD 颜色 --------------------------------------//
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Grey2          0xF79E
#define Dark_Grey      0x6B4D
#define Dark_Grey2     0x52AA
#define Light_Grey     0xE71C
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

// 界面颜色
#define WINDOW_BK_COLOR         0xEF9F            // 窗口背景色
#define WINDOW_COLOR            Black             // 窗口前景色
#define TITLE_BK_COLOR          Blue              // 标题栏背景色
#define TITLE_COLOR             White             // 标题栏前景色
#define STATUS_BK_COLOR         Blue              // 状态栏背景色
#define STATUS_COLOR            White             // 状态栏前景色


#define EVENT_KEY_PRESSED       0x01
#define EVENT_TP_PRESSED        0x02
#define EVENT_TP_TOUCHED        0x04
#define EVENT_OP_STARTED        0x08
#define EVENT_FLICKER           0x10
#define RX_RECEIVED             0x20

#define EDIT_VALID              BIT7              // 编辑控件状态        ：1 - 有效    0 - 无效
#define EDIT_ACTIVE             BIT6              // 编辑控件状态        ：1 - 活动    0 - 非活动 
#define EDIT_FONT               BIT5              // 编辑控件字体        ：1 - 24点阵  0 - 16点阵
#define EDIT_MODE               BIT4              // 编辑控件模式        ：1 - 删除式  0 - 插入式
#define EDIT_CURSOR             0x0F              // 编辑控件光标位置    
#define EDIT_FRAME              0xE0              // 编辑控件边框属性    : 
#define EDIT_LIMIT              0x1F              // 编辑控件最大字符数  
#define VIEW_LIM                 150              // 视图控件最大字符数

#define BUTTON_VALID            BIT7              // 按钮控件状态        ：1 - 有效    0 - 无效
#define BUTTON_ACTIVE           BIT6              // 编辑控件状态        ：1 - 活动    0 - 非活动 
#define BUTTON_DOWN             BIT5              // 按钮控件状态        ：1 - 按下    0 - 未按下
#define CHECK_CHECKED           BIT7              // 选择框状态          : 1 - 选择    0 - 未选择
#define SHOULD_FLICKER          BIT1              // 应闪烁

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
  char string[50]                               ; // 字符缓冲区
} EDIT                                          ; // 编辑控件

typedef struct
{
  unsigned char id                              ; // 识别号
  unsigned int  x                               ; // 起始位置横坐标x
  unsigned int  y                               ; // 起始位置纵坐标y
  unsigned char width                           ; // 宽度
  unsigned char height                          ; // 高度
  unsigned char length                          ; // 当前字符串长度
  unsigned char status                          ; // 状态
  unsigned int  positionx                       ; // 下一字符显示位置
  unsigned int  positiony                       ; // 下一字符显示位置
  unsigned int  limit                           ; // 最大字符数
  char string[VIEW_LIM]                         ; // 字符串
} VIEW                                          ; // 视图控件


typedef struct
{
  unsigned char id                              ; // 识别号
  unsigned int  x                               ; // 起始位置横坐标x
  unsigned int  y                               ; // 起始位置纵坐标y
  unsigned char width                           ; // 宽度
  unsigned char height                          ; // 高度
  unsigned char property                        ; // 属性
  unsigned char status                          ; // 状态
  char          Title[10]                       ; // 选择框标题
} CHECK_BOX                                     ; // 选择框控件

typedef struct
{
  unsigned char id                              ; // 识别号
  unsigned int  x                               ; // 起始位置横坐标x
  unsigned int  y                               ; // 起始位置纵坐标y
  unsigned char width                           ; // 宽度
  unsigned char height                          ; // 高度
  unsigned char property                        ; // 属性
  unsigned char status                          ; // 状态
  char          Title[10]                       ; // 按钮文字
} BUTTON                                        ; // 按钮控件


typedef struct 
{
  unsigned char     MenuIndex                   ; // 菜单索引 
  unsigned char     MasterMenu                  ; // 父菜单索引  
  unsigned char     MasterItem                  ; // 父菜单项索引
  unsigned char     MenuTitle[20]               ; // 菜单标题
  unsigned char     MenuConfig[40]              ; // 菜单配置
  unsigned char     ItemIndex[MAX_MENU_ITEM]    ; // 菜单项索引
} MENU_RES                                      ; // 菜单资源

typedef  struct 
{
  unsigned char  ItemIndex                      ; // 菜单项目索引
  unsigned char  SubMenuIndex                   ; // 子菜单索引
  unsigned char  CallBackFuncIndex              ; // 回调函数索引
  unsigned long  IconAddr                       ; // 图标地址
  char           ItemTitle[20]                  ; // 菜单项名称
} ITEM                                          ;

extern char (*Item_OP[MAX_FUNC_ITEM])(char *par);
extern float cx[1],cy[1],ox[1],oy[1]            ;

void str_cpy(char* Destination, char* Source,unsigned char Num)                ;
void ShowMenu_Item(unsigned char index,unsigned char no,unsigned char mode)    ;
void ActivateMenu_Item(unsigned char no,unsigned char mode)                    ;
void RunMenu(unsigned char index)                                              ;
void CreateEditDig(unsigned char ID,unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)                   ;
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p             ) ;
void SetEditDig(unsigned char ID,char* p)                                      ;				
void ActiveEditDig(unsigned char ID   ,unsigned char Active)                   ;	
void EditDigMessage(unsigned char Message,char * p)                            ;
void RedrawEditDig(unsigned char Index)                                        ;                    
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)                      ;
void DeleteEditDig(unsigned char ID)                                           ;
void Flicker_Cursor(void)                                                      ;
void Sweep_Cursor(unsigned char Index)                                         ;
void CreateCheckBox(unsigned char ID,unsigned int x_s   ,unsigned int y_s,
                    unsigned char width,unsigned char height              )    ;
void Select(unsigned char ID)                                                  ;
void Deselect(unsigned char ID)                                                ;
void CreateButton( unsigned char ID   ,char *title     ,
                   unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)                   ;
void SetButtonDown(unsigned char Index)                                        ;
void SetButtonUp(unsigned char Index)                                          ;
void SetButtonTitle(unsigned char ID,char *new_title)                          ;
void RedrawButton(unsigned char Index)                                         ;
void DeleteButton(unsigned char ID)                                            ;
unsigned char InScopeButton(unsigned int x,unsigned int y)                     ;
void DrawPanel(unsigned int x_s   ,unsigned int y_s,
               unsigned char width,unsigned char height,char *Title)           ;
void DrawFrame(unsigned int x_s   ,unsigned int y_s,
               unsigned char width,unsigned char height)                       ;
void CreateView(VIEW *view         ,unsigned int limit,
                unsigned int x_s   ,unsigned int y_s,
                unsigned char width,unsigned char height)                      ;
void RedrawView(VIEW *view)                                                    ;
void ViewMessage(VIEW *view,char Message,unsigned char *p)                     ;
void ClearViewContent(VIEW *view)                                              ;
void CreateCheck(CHECK_BOX *check  ,
                unsigned int x_s   ,unsigned int y_s,
                unsigned char width,unsigned char height,
                char *title                              )                     ;
void Redrawcheck(CHECK_BOX *check)                                             ;
void CheckCheck(CHECK_BOX *check)                                              ;
void unCheckCheck(CHECK_BOX *check)                                            ;
unsigned char InCheckScope(unsigned int x,unsigned int y,CHECK_BOX *check)     ;
void MessageBox(char *title,char *string)                                      ;
unsigned char ConfirmBox(char *title,char *string)                             ;
#endif