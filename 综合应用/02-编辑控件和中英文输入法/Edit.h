#ifndef _EDIT_H
#define _EDIT_H

#define    MAX_INTERVAL     150000                // 键盘连击间隔
#define    INPUT_PY           0x01
#define    INPUT_HZ           0x02
#define    EXIT_INPUT         0x03
#define    SHOULD_FLICKER     BIT1                // 应闪烁光标

#define    PRE_EDIT           0xFE                // 切换至上一Edit控件
#define    NEXT_EDIT          0xFD                // 切换至下一Edit控件
#define    INP_DIG            BIT0                // 输入模式 —数字
#define    INP_EN             BIT1                // 输入模式 —英文字母
#define    INP_CN             BIT2                // 输入模式 —汉字

// 界面颜色
#define WINDOW_BK_COLOR       0xDFFF              // 窗口背景色
#define WINDOW_COLOR          0x11FA              // 窗口前景色

#define TITLE_BK_COLOR        0x11FA              // 标题栏背景色
#define TITLE_COLOR           0xDFFF              // 标题栏前景色
#define STATUS_BK_COLOR       0x0014              // 状态栏背景色
#define STATUS_COLOR          0xDFFF              // 状态栏前景色

#define MENU_BACK_COLOR	      WINDOW_BK_COLOR     // 未选择菜单项背景色
#define MENU_FONT_COLOR	      WINDOW_COLOR 	  // 未选择菜单项文字色
#define MENU_SELE_COLOR	      WINDOW_COLOR        // 选择菜单项背景色
#define MENU_SELF_COLOR	      WINDOW_BK_COLOR	  // 选择菜单项文字色


#define EDIT_VALID            BIT7                // 编辑控件状态        ：1 - 有效    0 - 无效
#define EDIT_ACTIVE           BIT6                // 编辑控件状态        ：1 - 活动    0 - 非活动 
#define EDIT_FONT             BIT5                // 编辑控件字体        ：1 - 24点阵  0 - 16点阵
#define EDIT_MODE             BIT4                // 编辑控件模式        ：1 - 删除式  0 - 插入式
#define EDIT_CURSOR           0x0F                // 编辑控件光标位置    
#define EDIT_FRAME            0xE0                // 编辑控件边框属性    : 
#define EDIT_LIMIT            0x1F                // 编辑控件最大字符数  

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

void CreateEditDig(unsigned char ID,unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)                                 ;
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p             )               ;
void SetEditDig(unsigned char ID,char* p)                                                    ;				
void ActiveEditDig(unsigned char ID   ,unsigned char Active)  ;	
void EditDigMessage(unsigned char Message,char * p)           ;
void RedrawEditDig(unsigned char Index)                       ;                    
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)     ;
void DeleteEditDig(unsigned char ID)                          ;
void Flicker_Cursor(void)                                     ;

#endif