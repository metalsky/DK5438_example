//===========================================================================//
//                                                                           //
// 文件：  BSP.C                                                             //
// 说明：  BW-DK5438开发板支持程序文件，支持uC/OS-II                         //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include  <includes.h>
#include  "hal_BW-DK5438.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  BSP_DLY_CONST      (BSP_CPU_CLK_FREQ / 1000000) 
#define  BSP_DLY_MAX        (0xFFFFFFFF / BSP_DLY_CONST)


extern  uint8_t mymac[6]          ;
extern  uint8_t myip[4]           ;
extern  uint16_t myport           ; // listen port for udp


static  void  LCD_Init(void)      ;
void Init_J60SPI(void)            ; // 初始化ENC28J60SPI端口
void Init_TPSPI(void)             ; // 初始化触摸屏SPI端口
void Init_UART(void)              ;
void Init_IRUART(void)            ;
void Init_RSUART(void)            ;
void Init_EtherNet(void)          ;


//---------------------------------------------------------------------------//
//                                                                           //
//函数：void BSP_Init(void)                                                  //
//描述：板初始化                                                             //
//                                                                           //
//---------------------------------------------------------------------------//              

void  BSP_Init (void)
{
//  halBoardSetSystemClock(SYSCLK_25MHZ)               ; 
  halBoardSetSystemClockExt(SYSCLK_25MHZ)            ; 
  halBoardOutputSystemClock()                        ;
  Init_J60SPI()                                      ;
  Init_EtherNet()                                    ;
  ENC_SLEEP()                                        ; // 省电  
  halBoardStartXT1()                                 ; // 通讯口时钟
  Init_UART()                                        ;
  Init_IRUART()                                      ;
  Init_RSUART()                                      ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void BSP_IntDisAll(void)                                             //
//描述：关闭中断                                                             //
//                                                                           //
//---------------------------------------------------------------------------//              

void  BSP_IntDisAll (void)
{
    _DINT();     
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void BSP_IntEnAll(void)                                              //
//描述：打开中断                                                             //
//                                                                           //
//---------------------------------------------------------------------------//              

void  BSP_IntEnAll (void)
{
    _EINT();                                                           
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void BSP_Dly(CPU_INT32U  us)                                         //
//描述：微秒级延时程序                                                       //
//                                                                           //
//---------------------------------------------------------------------------//              

void  BSP_Dly (CPU_INT32U  us)
{
    CPU_INT32U  loops;
    CPU_INT32U  dly;
    
    
    loops = 0;
    
    if (us > BSP_DLY_MAX) {                                            
        loops += us / BSP_DLY_MAX;                                    
        dly    = us % BSP_DLY_MAX;
    }
    
    dly = us * BSP_DLY_CONST;                                       
    
    while (dly--);                                                    
    
    while (loops--) {                                               
        dly = BSP_DLY_MAX;
        while (dly--);
    }
}    

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void TMR_TickInit(void)                                              //
//描述：初始化操作系统时钟                                                   //
//                                                                           //
//---------------------------------------------------------------------------//              

void  TMR_TickInit (void)
{
    WDTCTL  = WDT_MDLY_8                                           ; // 0.32 ms 中断
    SFRIE1 |= 1                                                    ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_J60SPI(void)                                               //
//描述：初始化网口SPI                                                        //
//                                                                           //
//---------------------------------------------------------------------------//              

void Init_J60SPI(void)                       // ENC28J60占用SPI端口
{  
  P3OUT    |=  TP_CS                        ; // 关闭触摸屏
  P3DIR    |=  TP_CS                        ;
  P5OUT    |=  J60_CS                       ;
  P5DIR    |=  J60_CS                       ;
  P10DIR   &= ~PMISO                        ;
  P10REN   |=  PMISO                        ;
  P10DIR   |=  PMOSI+PSCK                   ;
  P10OUT   &=~(PMOSI+PSCK)                  ;
  __delay_cycles(20)                        ;  
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_J60SPI(void)                                               //
//描述：初始化触屏SPI                                                        //
//                                                                           //
//---------------------------------------------------------------------------//              
void Init_TPSPI(void)                       // 触摸屏占用SPI端口
{  
  P10OUT   &= PSCK                         ;
  P10DIR   |= PMOSI+PSCK+PNSS              ;
  P10DIR   &=~PMISO                        ;
  P10REN   |= PMISO                        ;
  P3OUT    |= TP_CS                        ;  
  P3DIR    |= TP_CS                        ;  
  P5DIR    &=~J60_CS                       ; // 关ENC28J60片选
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_UART(void)                                                 //
//描述：初始化USB串口                                                        //
//                                                                           //
//---------------------------------------------------------------------------//              
void Init_UART(void)
{ 
  USB_PORT_SEL   |= TXD_U + RXD_U          ; // 选择引脚功能
  USB_PORT_DIR   |= TXD_U                  ; // 选择引脚功能
  UCA1CTL1        = UCSWRST                ; // 状态机复位
  UCA1CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA1BR0         = 0x03                   ; // 32kHz/9600=3.41 
  UCA1BR1         = 0x00                   ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA1IE         |= UCRXIE                 ; // 允许接收中断
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_IRUART(void)                                               //
//描述：初始化红外串口                                                       //
//                                                                           //
//---------------------------------------------------------------------------//              
void Init_IRUART(void)
{ 
  IrDA_PORT_SEL  |= IrDA_IN + IrDA_OUT     ; // 选择引脚功能
  IrDA_PORT_DIR  |= IrDA_OUT               ; // 选择引脚功能
  UCA2CTL1        = UCSWRST                ; // 状态机复位
  UCA2CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA2BR0         = 27                   ; // 32kHz/1200=27.3 
  UCA2BR1         = 0x00                   ; 
  UCA2MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA2CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA2IE         |= UCRXIE                 ; // 允许接收中断
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_RSUART(void)                                               //
//描述：初始化RS232/485串口                                                  //
//                                                                           //
//---------------------------------------------------------------------------//              
void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD              ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                    ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                ; // 状态机复位
  UCA3CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA3BR0         = 0x03                   ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                   ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA3IE         |= UCRXIE                 ; // 允许接收中断
  RS485_IN                                 ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Init_EtherNet(void)                                             //
//描述：初始化网口参数                                                       //
//                                                                           //
//---------------------------------------------------------------------------//              
void Init_EtherNet(void)
{
  enc28j60Init(mymac)                      ;
  BSP_Dly(20000)                           ;        
  enc28j60PhyWrite(PHLCON,0x476)           ; // 配置指示灯
  BSP_Dly(20000)                           ;
  init_ip_arp_udp(mymac,myip)              ;
}

/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time 
*                       but MUST be less than the maximum measured time; otherwise, timer resolution 
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is 
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured 
*                                       Timer period            Timer's period in some units of 
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same 
*                                                                   units of (fractional) seconds 
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less 
*                           than the maximum measured time; otherwise, timer resolution inadequate to 
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (1);
}
#endif

