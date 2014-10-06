//===========================================================================//
//                                                                           //
// 文件：  APP.H                                                             //
// 说明：  BW-DK5438开发板uC/OS-II演示程序头文件                             //
// 编译：  IAR Embedded Workbench IDE for msp430 v5.20                       //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//


#define BUFFER_SIZE              250
#define PORT_RS232               0x01     
#define PORT_RS485               0x02
#define PORT_USB                 0x03

#pragma data_alignment = 8

uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24}                    ; // MAC地址
uint8_t myip[4]  = {192,168,1,103}                                    ; // IP地址
uint16_t myport  = 8001                                               ; // UDP监听端口
static uint8_t buf[BUFFER_SIZE+1]                                     ; // UDP接收缓冲区

OS_EVENT *MBKeyTP                                                     ; // 键盘+触控邮箱
OS_EVENT *MBFunc                                                      ; // 菜单操作邮箱
OS_EVENT *MBRX                                                        ; // 通讯接收邮箱
OS_EVENT *SPIMutex                                                    ; // SPI总线互斥信号量
char MBRXBuffer[30]                                                   ; // 通讯邮箱缓冲区
char MBKeyTPBuffer[30]                                                ; // 键盘/触摸屏邮箱缓冲区
char MBFuncBuffer[6]                                                  ; // 函数操作邮箱缓冲区
unsigned int  Color                                                   ; // 前景颜色
unsigned int  Color_BK                                                ; // 背景颜色
EDIT          Edit_Dig[10]                                            ; // 编辑控件
BUTTON        Button[10]                                              ; // 按钮控件
unsigned char DISP_MODE                                               ; // 编辑控件显示模式

float cx[1]={-0.065854461},\
      cy[1]={-0.089657381},\
      ox[1]={252.40039321},\
      oy[1]={348.95294111}                                            ;

extern unsigned int BK_BUFFER[400]                                    ;
INT32U exe_time                                                       ;


//***********************************************************//
#pragma data_alignment = 8
static  OS_STK      AppTaskStartStk[APP_START_TASK_STK_SIZE]          ; // 起始任务栈
#pragma data_alignment = 8
static  OS_STK      LED_TASK_STK[LED_TASK_STACK_SIZE]                 ; // 数码管刷新任务栈
#pragma data_alignment = 8
static  OS_STK      GUI_TASK_STK[GUI_TASK_STACK_SIZE]                 ; // 图形界面任务栈
#pragma data_alignment = 8
static  OS_STK      MENU_OP_TASK_STK[MENU_OP_TASK_STACK_SIZE]         ; // 输入检测(键盘/触摸屏)任务栈
#pragma data_alignment = 8
static  OS_STK      KEY_TP_TASK_STK[KEY_TP_TASK_STACK_SIZE]           ; // 输入检测(键盘/触摸屏)任务栈
#pragma data_alignment = 8
static  OS_STK      CPU_USAGE_TASK_STK[CPU_USAGE_TASK_STACK_SIZE]     ; // 显示CPU利用率任务栈

#if (uC_TCPIP_MODULE > 0)
        NET_IP_ADDR     AppNetIP                                      ;
        NET_IP_ADDR     AppNetMsk                                     ;
        NET_IP_ADDR     AppNetGateway                                 ;
#endif
        
//static  CPU_CHAR    AppLCDTxt[(LCD_LINE_BUF * LCD_NUM_LINES)] = "  0  Micrium      000 uCOS-II  00   0 ---------    CPU Usage                         Time Ticks                        Context Switches                  ";
static  void  AppTaskCreate (void)                                    ;
static  void  AppTaskStart  (void *p_arg)                             ;
static void LED_TASK(void *p_arg)                                     ;
static void GUI_TASK(void *p_arg)                                     ;
static void KEY_TP_TASK(void *p_arg)                                  ;
static void MENU_OP_TASK(void *p_arg)                                 ;
void Init_Func(void)                                                  ;
void Init_RTC(void)                                                   ;
void GotoSleep(void)                                                  ;
void UTX_PROC(char *tx_buf)                                           ;
void RS232TX_PROC(char *tx_buf)                                       ;
void RS485TX_PROC(char *tx_buf)                                       ;
void IrDATX_PROC(char *tx_buf)                                        ;
unsigned char Find_Data(unsigned char coded)                          ;
void Dec2Str(char *string,int data)                                   ;

char (*Item_OP[MAX_FUNC_ITEM])(char *par)                             ;
char Proc_Show_Clock(char *NC)                                        ;
char Proc_Display(char *NC)                                           ;
char Proc_Task_Info(char *NC)                                         ;
char Proc_TP_Adj(char *NC)                                            ;
char Proc_Paint(char *NC)                                             ;
char Proc_Show_Pic(char *NC)                                          ;
char Proc_Port_Comm(char *NC)                                         ;
char Proc_IrDA_Comm(char *NC)                                         ;
char Proc_Net_Comm(char *NC)                                          ;
char Proc_Go_Sleep(char *NC)                                          ;
