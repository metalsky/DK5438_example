
#include "init.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//系统初始化
void Init(void)
{
  Init_MCLK()                         ;
//GotoSleep()       ;// FOR DEBUG   
  BackLight()                         ;
  Init_KeyPad()                       ;
//  InitRF();
  Init_ADC()                          ;
  Init_timer()                        ;
  Init_RTC()                          ;  
  Init_UART()                         ;
//  Init_Port()                         ;
}
/*
//初始化主时钟 - XT2 = 32MHz
void Init_MCLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P5SEL     |= 0x0C                                       ; // 端口功能选择振荡器
  UCSCTL6   &= ~XT2OFF                                    ; // 振荡器使能
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 驱动模式 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}
*/

//*
//初始化主时钟 - DCO = 28MHz
#pragma optimize=none
void Init_MCLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
//  P8DIR      = 0xFF                                       ;
//=============== DEBUG
  P7SEL     |= 0x03                                       ; // Port select XT1
  UCSCTL6   &=~XT1OFF                                     ;  
  UCSCTL6   |= XCAP_3                                     ; // Internal load cap
//=============== DEBUG  
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  
//  UCSCTL4   |= SELA_2                                     ; // ACLK   = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  
  __bis_SR_register(SCG0)                                 ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                     ; // Set lowest possible DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // Select DCO range 16MHz operation
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( N + 1) * FLLRef = (749 + 1) * 32768 = 24.576MHz
  __bic_SR_register(SCG0)                                 ; // Enable the FLL control loop
                                                            // Set FLL Div = fDCOCLK/2

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 32.768 MHz / 32768 Hz = 1024000 = MCLK cycles for DCO to settle
  __delay_cycles(1024000)                                 ;

  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // Clear  XT2,XT1,DCO fault flags                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // Test oscillator fault flag 
//  UCSCTL4   |= SELM__DCOCLK                               ; // MCLK  = DCO  
//  UCSCTL4   |= SELS__DCOCLK                               ; // SMCLK  = DCO  
}
//*/

#pragma optimize=none
void Init_Port(void)
{
//  P8DIR  |= GPIO09_TA00         ;
//  P8OUT  |= CLED_PWR            ;
//  P8DIR  |= CLED_PWR            ;
//  P8OUT  &=~GPIO09_TA00         ; // 打开RS232接口电源
  P6DIR  |= POWER               ; 
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(2600)          ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(2600)          ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(2600)          ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(2600)          ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  &=~POWER               ; // 打开电源
  __delay_cycles(26000)         ;
  P6OUT  |= POWER               ; // 打开电源
  __delay_cycles(26000)         ;
/*  
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  |= POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  |= POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  |= POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  &=~POWER               ; // 打开电源
  P6OUT  |= POWER               ; // 打开电源
*/
//  __delay_cycles(26000000)      ;
}
//定时器初始化
#pragma optimize=none
void Init_timer(void)
{
  TBCTL =  TBSSEL_1 + MC_1 
         + TBCLR    + TBIE      ; // 时钟源 = ACLK
  TBCCR0 =32768                 ; //1.0s
  TBCCR0 =16384                 ; //0.5s
}

//外围电路关闭
void CutOff(void)
{
/*  
  BCSCTL1 |= XT2OFF;
  P1OUT=0x00;
  P2OUT=0x00;
  P3DIR=0x00;
  P3SEL=0x00;
  P3OUT=0x00;
  P4OUT=0x00;
  P5OUT=0x00;
  P6SEL=0X00;
  P6OUT=0x00;
  U0CTL=0;
  U0TCTL=0;
  ADC12CTL0 &=~ENC; 
  ADC12CTL0=0;
  ADC12CTL1=0;
  ADC12MCTL0=0;
  ADC12IE=0;
  ADC12CTL0 |= ENC; 
  P4OUT|=Power;   
*/
}

#pragma optimize=none
void Init_ADC(void)
{
  WDTCTL      = WDTPW+WDTHOLD                             ; // Stop watchdog timer
  P7SEL      |= TVBTM                                     ; // Enable A/D channel A13
  ADC12CTL0   = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V; // Turn on ADC12, Sampling time on Reference Generator and set to 2.5V
  ADC12CTL1   = ADC12SHP                                  ; // Use sampling timer
  ADC12MCTL0  = ADC12SREF_1 + ADC12INCH_13                ; // Vr+=Vref+ and Vr-=AVss
  __delay_cycles(11200000  )                              ; // Delay for reference start-up
  ADC12CTL0  |= ADC12ENC                                  ; // Enable conversions
}



#pragma optimize=none
void BackLight(void)                         // 初始化背光亮度
{
  P1DIR   |= BL_CTR                        ;
  P1SEL   |= BL_CTR                        ;
  TA0CCR0  = 250                           ; // PWM周期
  TA0CCTL1 = OUTMOD_3                      ; // 置位/复位模式
  TA0CCR1  = 220                           ; // PWM占空时间
  TA0CTL   = TASSEL_2 + MC_1 + TACLR +ID__8; // SMCLK/8, UP                                   
  TA0EX0  |= 0x07                          ; // SMCLK/8/8
}
/*
//初始化键盘端口
#pragma optimize=none
void Init_KeyPad(void)
{
  KEY_PORT_REN      =  0xFF            ;
  KEY_PORT_OUT      =  0x00            ;
}

#pragma optimize=none
#pragma location = "BOOTLOADER"
void Init_SSTSPI(void)
{  
  P8OUT    |= LCD_NCS                      ;
  P8DIR    |= LCD_NCS                      ;
  P3OUT    |= NCS25                        ;
  P3DIR    |= NCS25                        ;
  P3SEL    &= 0xF0                         ;
  P3SEL    |= 0x0E                         ; // // P3.1/2/3功能选择为SPI
  UCB0CTL1 |= UCSWRST                      ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB    ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                     ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x02                         ; // SCK = SMCK/2
  UCB0BR1   = 0                            ; 
  UCB0CTL1 &=~UCSWRST                      ; // SPI状态机使能
}

#pragma optimize=none
#pragma location = "BOOTLOADER"
void Init_LCDSPI(void)
{
  P3OUT    |= 0x0F                         ;
  P3DIR    |= 0x0F                         ;
  P3SEL    &= 0xF0                         ;
  P3SEL    |= 0x08                         ; // P3.3功能选择为SCK
  UCB0CTL1 |= UCSWRST                      ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB    ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                     ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x03                         ; // SCK = SMCK/3
  UCB0BR1   = 0                            ; 
  UCB0CTL1 &=~UCSWRST                      ; // SPI状态机使能
}
void Init_PSPI(void)
{  
  P10OUT   |= PNSS                         ;
  P10DIR   |= PNSS                         ;
  P5OUT   |= NCS25                        ;
  P5DIR   |= NCS25                        ;
  P10SEL   &= 0xF0                         ;
  P10SEL   |= 0x0E                         ; // // P10.1/2/3功能选择为SPI
  UCB3CTL1 |= UCSWRST                      ; // 复位SPI状态机
  UCB3CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB    ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB3CTL1 |= UCSSEL_2                     ; // 选择SCK参考源为SMCLK
  UCB3BR0   = 0x02                         ; // SCK = SMCK/2
  UCB3BR1   = 0                            ; 
  UCB3CTL1 &=~UCSWRST                      ; // SPI状态机使能
}

#pragma optimize=none
void Init_RTC(void)
{
  RTCCTL01 =  RTCTEVIE + RTCSSEL_0 
            + RTCTEV_0 + RTCMODE           ; // 时钟模式,每分钟中断, BCD格式
  RTCMIN   =  58                           ;
  RTCHOUR  =  5                            ;  
  RTCDAY   =  1                            ;
  RTCMON   =  3                            ;
  RTCYEARL =  10                           ;
  RTCYEARH =  20                           ;  
  RTCHOUR  =  5                            ;  
}
#pragma optimize=none
void Init_UART(void)
{
  P3SEL     = TXD + RXD                    ; // 选择引脚功能
  UCA0CTL1  = UCSWRST                      ; // 状态机复位
  UCA0CTL1 |= UCSSEL_1                     ; // CLK = ACLK
  UCA0BR0   = 0x03                         ; // 32kHz/9600=3.41 
  UCA0BR1   = 0x00                         ; 
  UCA0MCTL  = UCBRS_3 + UCBRF_0            ; // UCBRSx=3, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST                     ; // 启动状态机
  UCA0IE   |= UCRXIE                       ; // 允许接收中断
}
*/

#pragma optimize=none
void Init_UART(void)
{ 
  USB_PORT_SEL|= TXD_U + RXD_U             ; // 选择引脚功能
  USB_PORT_DIR|= TXD_U                     ; // 选择引脚功能
  UCA1CTL1     = UCSWRST                   ; // 状态机复位
  UCA1CTL1    |= UCSSEL_1                  ; // CLK = ACLK
  UCA1BR0      = 0x03                      ; // 32kHz/9600=3.41 
  UCA1BR1      = 0x00                      ; 
  UCA1MCTL     = UCBRS_3 + UCBRF_0         ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1    &= ~UCSWRST                  ; // 启动状态机
  UCA1IE      |= UCRXIE                    ; // 允许接收中断
}

#pragma optimize=none
void Init_UART_115200(void)
{
  P3SEL     = TXD + RXD                    ; // 选择引脚功能
  UCA0CTL1  = UCSWRST                      ; // 状态机复位
  UCA0CTL1 |= UCSSEL__SMCLK                ; // CLK = SMCLK
  UCA0BR0   =  213                         ; // SMCLK/115200=213 
  UCA0BR1   = 0x00                         ; 
  UCA0MCTL  = UCBRS_3 + UCBRF_0            ; // UCBRSx=3, UCBRFx=0
  UCA0CTL1 &= ~UCSWRST                     ; // 启动状态机
  UCA0IE   |= UCRXIE                       ; // 允许接收中断
}


