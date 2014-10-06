#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Menu.h"
#include "MenuRes.h"
//#include "Edit.h"
#include "TFTDRV.H"
#include "PIN_DEF.H"
#include "RTC.H"

#define    MAX_INTERVAL          150000
#define    PERMISSION              0x03 // 菜单权限
#define    RETURN_MASTER           0x33 // 函数执行完回到主菜单
#define    GOTO_SUB                0x44 // 函数执行完执行子菜单


///////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define Power  BIT0

#define    VALID                   0x11
#define    INVALID                 0x22

//------------------------------- 事件类型定义----------------------------------//
#define    SHOULD_DISPLAY_TIME     BIT0  // 应更新时间显示
#define    SHOULD_FLICKER          BIT1  // 应闪烁光标
#define    RX_FRAME_RECEIVED       BIT2  // 收到有效数据帧
#define    TX_FRAME_WAITING        BIT3  // 串口数据等待发送
#define    LOWPOWER_DETECTED       BIT5  // 电池容量低
#define    SHOULD_UPDATE_WDT       BIT6  // 更新看门狗定时器
#define    RX_PC_RECEIVED          BIT7  // 上位机接收到数据
#define    CARD_DETECTED           BIT8  // 检测到射频卡
#define    SHOULD_AUTO_COUNT       BIT9  // 应自动抄表
#define    CROSS_DRV_DETECTED      BITA  // 检测到电机驱动线反接
#define    MOTOR_TIMEOUT           BITB  // 电机操作超时
#define    COMMAND_ISSUED          BITC  // 集中器命令
#define    SHOULD_DISCHARGE        BITD  // 维护阀门
#define    DISCHARGE_OK            BITE  // 维护应答
#define    AUTO_COUNT_OK           BITF  // 自动抄表应答

#define    PRE_EDIT                0xFE  // 切换至上一Edit控件
#define    NEXT_EDIT               0xFD  // 切换至下一Edit控件
#define    INP_DIG                 BIT0  // 输入模式 —数字
#define    INP_EN                  BIT1  // 输入模式 —英文字母
#define    INP_CN                  BIT2  // 输入模式 —汉字


///////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int OP_MODE                           ; // 操作模式-系统员级/管理员级/操作员级

MENU Menu                                      ;

unsigned int  Event                                    ;
unsigned char INP_MODE         = INP_CN                ; // 输入模式
unsigned char INP_MODE_LMT     = INP_CN+INP_EN+INP_DIG ; // 输入模式限制 
unsigned long int Interval     = 0xFFFFFFFF            ; // 输入键间隔
float    VmainAvr              = 0                     ;
unsigned int Color,Color_BK                            ;

unsigned char (*Item_OP[ALL_ITEMS])(unsigned int,unsigned char)               ;
unsigned char View_Board_Info(unsigned int NC1,unsigned char NC2)             ;
unsigned char Set_Time(unsigned int NC1,unsigned char NC2)                    ;
unsigned char Set_Backlight(unsigned int NC1,unsigned char NC2)               ;

void Init_Func(void)                                                          ;
void Wait_Key(unsigned char key_waited)                                       ;
void Display_Time(void)                                                       ;
void VmainMon(void)                                                           ;



extern void Init_MCLK(void)                                                   ;
extern void Init_KeyPad(void)                                                 ;
extern void Init_RTC(void)                                                    ;
extern void Init_timer(void)                                                  ;
extern void Init_Port(void)                                                   ;
extern void Init_ADC(void)                                                    ;
extern void BackLight(void)                                                   ;
extern void Init_UART(void)                                                   ;
extern void Init_UART_115200(void)                                            ;
extern void Init(void)                                                        ;

extern void str_cpy(char* Destination,char* Source,unsigned char Num)         ;

//////////////////////////////////////////////////////////////////////////////////////////////////////

extern void Init_SPI(void)                                                     ;
void RunMenu(void)                                                             ;
unsigned char ReadKey(void)                                                    ;
unsigned char Input_CN(char first_key,char* buffer)                            ;
unsigned char GetKeyPress(void)                                                ;
void DrawBattery(float voltage)                                                ;
void DisplayTime(void)                                                         ;
unsigned char Read_Edit_Val(unsigned char Edit_Index,char* string,
                            unsigned char Renew_Edit_Index        )            ;
unsigned char Valve_Mantain_Open(unsigned int NC1,unsigned char NC2)           ;

void MessageBox(char *title,char *string)                                      ;
