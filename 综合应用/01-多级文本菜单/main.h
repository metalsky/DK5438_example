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
#define    PERMISSION              0x03 // �˵�Ȩ��
#define    RETURN_MASTER           0x33 // ����ִ����ص����˵�
#define    GOTO_SUB                0x44 // ����ִ����ִ���Ӳ˵�


///////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define Power  BIT0

#define    VALID                   0x11
#define    INVALID                 0x22

//------------------------------- �¼����Ͷ���----------------------------------//
#define    SHOULD_DISPLAY_TIME     BIT0  // Ӧ����ʱ����ʾ
#define    SHOULD_FLICKER          BIT1  // Ӧ��˸���
#define    RX_FRAME_RECEIVED       BIT2  // �յ���Ч����֡
#define    TX_FRAME_WAITING        BIT3  // �������ݵȴ�����
#define    LOWPOWER_DETECTED       BIT5  // ���������
#define    SHOULD_UPDATE_WDT       BIT6  // ���¿��Ź���ʱ��
#define    RX_PC_RECEIVED          BIT7  // ��λ�����յ�����
#define    CARD_DETECTED           BIT8  // ��⵽��Ƶ��
#define    SHOULD_AUTO_COUNT       BIT9  // Ӧ�Զ�����
#define    CROSS_DRV_DETECTED      BITA  // ��⵽��������߷���
#define    MOTOR_TIMEOUT           BITB  // ���������ʱ
#define    COMMAND_ISSUED          BITC  // ����������
#define    SHOULD_DISCHARGE        BITD  // ά������
#define    DISCHARGE_OK            BITE  // ά��Ӧ��
#define    AUTO_COUNT_OK           BITF  // �Զ�����Ӧ��

#define    PRE_EDIT                0xFE  // �л�����һEdit�ؼ�
#define    NEXT_EDIT               0xFD  // �л�����һEdit�ؼ�
#define    INP_DIG                 BIT0  // ����ģʽ ������
#define    INP_EN                  BIT1  // ����ģʽ ��Ӣ����ĸ
#define    INP_CN                  BIT2  // ����ģʽ ������


///////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int OP_MODE                           ; // ����ģʽ-ϵͳԱ��/����Ա��/����Ա��

MENU Menu                                      ;

unsigned int  Event                                    ;
unsigned char INP_MODE         = INP_CN                ; // ����ģʽ
unsigned char INP_MODE_LMT     = INP_CN+INP_EN+INP_DIG ; // ����ģʽ���� 
unsigned long int Interval     = 0xFFFFFFFF            ; // ��������
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
