
#define    PERMISSION              0x03 // 菜单权限
#define    RETURN_MASTER           0x33 // 函数执行完回到主菜单
#define    GOTO_SUB                0x44 // 函数执行完执行子菜单

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define    VALID                   0x11
#define    INVALID                 0x22

//------------------------------- 事件类型定义----------------------------------//
#define    SHOULD_DISPLAY_TIME     BIT0  // 应更新时间显示

///////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int OP_MODE                                   ; // 操作模式-系统员级/管理员级/操作员级
EDIT_DIG Edit_Dig[6]                                   ; // 数字编辑控件

unsigned int  Event                                    ;
unsigned char INP_MODE         = INP_CN                ; // 输入模式
unsigned char INP_MODE_LMT     = INP_CN+INP_EN+INP_DIG ; // 输入模式限制 
unsigned char DISP_MODE                                ; // 编辑控件显示模式
float    VmainAvr              = 0                     ;
unsigned int Color,Color_BK                            ;
unsigned long int Interval     = 0xFFFFFFFF            ;

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

extern void Init_SPI(void)                                                    ;
void RunMenu(void)                                                            ;
unsigned char ReadKey(void)                                                   ;
unsigned char GetKeyPress(void)                                               ;
void DrawBattery(float voltage)                                               ;
void DisplayTime(void)                                                        ;
unsigned char Read_Edit_Val(unsigned char Edit_Index,char* string,
                            unsigned char Renew_Edit_Index        )           ;
unsigned char Valve_Mantain_Open(unsigned int NC1,unsigned char NC2)          ;

void MessageBox(char *title,char *string)                                     ;
void Input_OP_Password(void)                                                  ;
