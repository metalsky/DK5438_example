//===========================================================================//
//                                                                           //
// �ļ���  MAIN.H                                                            //
// ˵����  BW-DK5438������ϵͳ����ͷ�ļ�                                     //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "FS.h"
#include "RTOS.h"
#include "BSP.h"
#include "PIN_DEF.H"
#include "LED.H"
#include <msp430.h>
#include <msp430x54x.h>
#include "TFTDRV.H"
#include "TITLE.H"
#include "TOUCH.H"
#include "GUI.H"
#include "KEY.H"
#include <math.h>
#include "CLOCK.H"
#include "RTC.H"
#include "TITLE.H"
#include "7SEG_FONT_2436.H"
#include "enc28j60.h"
#include "ip_arp_udp.h"
#include "timeout.h"
#include "net.h"
#include "DS18B20.H"

#define BUFFER_SIZE              250
#define FLL_FACTOR               449
#define FUNC_MY_CLOCK            0x00  // �������ʾ����
#define PORT_RS232               0x01     
#define PORT_RS485               0x02
#define PORT_USB                 0x03

static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4]  = {192,168,1,103};
static uint16_t myport  = 8001; // listen port for udp
static uint8_t buf[BUFFER_SIZE+1];

unsigned char EVENT                                  ;
/*
float cx[1]={0.0670567926},\
      cy[1]={0.0900232554},\
      ox[1]={-18.3293762 },\
      oy[1]={-18.2325592 }                           ;
*/

float cx[1]={-0.06585446},\
      cy[1]={-0.089657381},\
      ox[1]={252.40039 },\
      oy[1]={348.952941 }                           ;

unsigned char SMG[10]=
{
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF)       ,
  ~(SEGB+SEGC)                           ,
  ~(SEGA+SEGB+SEGD+SEGE+SEGG)            ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGG)            ,
  ~(SEGB+SEGC+SEGF+SEGG)                 ,
  ~(SEGA+SEGC+SEGD+SEGF+SEGG)            ,
  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG)       ,
  ~(SEGA+SEGB+SEGC)                      ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF+SEGG)  ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG)       ,
}                                                    ;

LED_STRUCT LED[8]                                    ;

OS_MAILBOX MBKeyTP                                   ; // ����+��������
OS_MAILBOX MBFunc                                    ; // �˵���������
OS_MAILBOX MBRX                                      ; // ͨѶ��������
char MBRXBuffer[30]                                  ;
char MBKeyTPBuffer[30]                               ;
char MBFuncBuffer[6]                                 ;
extern unsigned int BK_BUFFER[400]                   ;

unsigned int  Color                                  ; // ǰ����ɫ
unsigned int  Color_BK                               ; // ������ɫ

//OS_STACKPTR int _Stack[0x200]                        ;
OS_STACKPTR int LED_TASK_STACK[100]                ;
//OS_STACKPTR int LED_TASK1_STACK[0x100]               ;
OS_STACKPTR int Seg7LED_TASK_STACK[0x200]            ; 
OS_STACKPTR int LCD_TASK_STACK[0x400]                ;
OS_STACKPTR int KEY_TP_TASK_STACK[0x300]             ;
OS_STACKPTR int MENU_OP_TASK_STACK[500]            ;

OS_TASK     LED_TASK_TCB                             ; // ������ƿ�
OS_TASK     LED_TASK1_TCB                            ;
OS_TASK     Seg7LED_TASK_TCB                         ; // �����ˢ��
OS_TASK     LCD_TASK_TCB                             ; // GUI�˵�����
OS_TASK     KEY_TP_TASK_TCB                          ; // ���̼��
OS_TASK     MENU_OP_TASK_TCB                         ; // �˵���������

OS_RSEMA      SemaLCD                                ; // 
OS_RSEMA      SemaSPI                                ; // ��������ENC28J60����SPI

EDIT          Edit_Dig[10]                           ; // �༭�ؼ�
BUTTON        Button[10]                             ; // ��ť�ؼ�
//unsigned int  Event                                  ;
unsigned char DISP_MODE                              ; // �༭�ؼ���ʾģʽ

void Init_RTC(void)                                  ; // 
void DisplayTime(void)                               ;
void Init_CLK(void)                                  ;
void Init_RTC(void)                                  ;
void Init_Func(void)                                 ;
char (*Item_OP[MAX_FUNC_ITEM])(char *par)            ;
char Proc_TP_Adj(char *NC)                           ;
char Proc_Paint(char *NC)                            ;
char Proc_Show_Pic(char *NC)                         ;
char Proc_Show_Clock(char *NC)                       ;
char Proc_Display(char *NC)                          ;
char Proc_Port_Comm(char *NC)                        ;
char Proc_IrDA_Comm(char *NC)                        ;
char Proc_Net_Comm(char *NC)                         ;
char Proc_Go_Sleep(char *NC)                         ;
void GotoSleep(void)                                 ;
void Start_Sound(void)                               ;
void Test_LED(void)                                  ;
unsigned char Find_Data(unsigned char coded)         ;
void DisplayWelcome(void)                            ;
void LCD_Task(void)                                  ;
void MENU_OP_Task(void)                              ;
void Key_TP_Task(void)                               ;
void LedTask(void)                                   ;      
void LedTask1(void)                                  ;      
void Init_J60SPI(void)                               ;
void Init_TPSPI(void)                                ;
void Init_UART(void)                                 ;
void Init_RSUART(void)                               ;
void Init_IRUART(void)                               ;
void Init_ADC(void)                                  ;
void UTX_PROC(char *tx_buf)                          ;
void RS232TX_PROC(char *tx_buf)                      ;
void RS485TX_PROC(char *tx_buf)                      ;
void IrDATX_PROC(char *tx_buf)                       ;
void Init_EtherNet(void)                             ;
void BackLight(void)                                 ; 
void VmainMon(void)                                  ;


