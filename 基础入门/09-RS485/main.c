//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机RS485端口实验程序                            //
//         主程序检测有按键按下则通过RS485端口发送"This is BW-DK5483" ,      //
//         可将开发板RS485接口与PC连接，PC上运行串口大师等串口调试软件，     //
//         打开RS485串口接收并观察开发板发送数据，也可由PC端发送数据至       //
//         开发板,开发板将接收数据发回PC                                     //
// 注意：  PC端应配置RS232-RS485转接插头以接收485数据                        //
//         开发板主电源上电后，PC端串口调试软件应重新关闭/打开一次串口       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PIN_DEF.H"

#define  FLL_FACTOR     749                                          // FLL_FACTOR: DCO倍频系数    
char  event, RXBuffer[2]                                           ;

//***************************************************************************//
//                                                                           //
//  USB接收中断服务程序                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer[0]      = UCA1RXBUF                                 ;
      event           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

//***************************************************************************//
//                                                                           //
//  RS232/485接收中断服务程序                                                //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer[0]      = UCA3RXBUF                                 ;
      event           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P7SEL     |= 0x03                                       ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                     ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                     ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                     ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                 ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志 
}
//***************************************************************************//
//                                                                           //
//  Init_Port(void): 设置IO端口                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR  |= POWER                                                  ; // 主电源
  MAIN_POWER_ON                                                    ;
  P7DIR  |= LED_PWR                                                ; // 发光二极管电源
  P7OUT  &=~LED_PWR                                                ;
  INTERNAL_PULL_UP                                                 ; // 使能键盘端口内部上拉电阻 
  ROW_IN_COL_OUT                                                   ; // 设置行输入，列输出0
}

//***************************************************************************//
//                                                                           //
//  Init_UART(void): 初始化USB端口                                           //
//                                                                           //
//***************************************************************************//
void Init_UART(void)
{ 
  USB_PORT_SEL   |= TXD_U + RXD_U                                  ; // 选择引脚功能
  USB_PORT_DIR   |= TXD_U                                          ; // 选择引脚功能
  UCA1CTL1        = UCSWRST                                        ; // 状态机复位
  UCA1CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA1BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA1BR1         = 0x00                                           ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA1IE         |= UCRXIE                                         ; // 允许接收中断
}


//***************************************************************************//
//                                                                           //
//  Init_RSUART(void): 初始化RS232/485端口                                   //
//                                                                           //
//***************************************************************************//
void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                                            ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                                        ; // 状态机复位
  UCA3CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA3IE         |= UCRXIE                                         ; // 允许接收中断
  RS485_IN                                                         ;
}

//***************************************************************************//
//                                                                           //
//  UTX_PROC(void): USB端口发送程序                                          //
//                                                                           //
//***************************************************************************//
void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                                          ; 
//    __delay_cycles(5000)                                           ;
    while (!(UCA1IFG&UCTXIFG))                                     ; 
  }
}

//***************************************************************************//
//                                                                           //
//  RS232TX_PROC(void): RS232端口发送程序                                    //
//                                                                           //
//***************************************************************************//
void RS232TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

//***************************************************************************//
//                                                                           //
//  RS485TX_PROC(void): RS485端口发送程序                                    //
//                                                                           //
//***************************************************************************//
void RS485TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  RS485_OUT                                                        ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
  __delay_cycles(100000)                                           ;
  RS485_IN                                                         ;
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  _EINT()                                                          ;
  for(;;)
  {
    if(event)
    {
      event   = 0x00                                               ;
      UTX_PROC(RXBuffer)                                           ;
    }
    if((P6IN&0x0F)!=0x0F)                                            // 检测按键按下
    {
      UTX_PROC("This is BW-DK5438")                                ;
      __delay_cycles(5000000)                                      ;
    }    
  }
}
