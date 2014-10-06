//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机红外收发程序                                 //
//         PC上运行串口大师等串口调试软件，通过USB接口向开发板发送数据，     //
//         开发板收到数据帧后，将其由红外端口发送；开发板能接收到自己发      //
//         送的红外信号，当红外端口接收到完整数据帧后，开发板将数据由USB     // 
//         口送至PC。                                                        // 
//         本程序未采用NEC或Philips的红外协议，旨在展示一种像使用串口一样    //
//         使用红外接口的方式，实验表明，这一方法十分有效，在1200bps、3米    // 
//         距离的条件下仍有非常好的收发效果。实际应用中，可加入必要的冗余    //
//         与校验。                                                          //
// 示例：  发送数据 ——43 0D 12 34 56 78 90 98 76 54 32 10 54               //
// 注意：  开发板主电源上电后，PC端串口调试软件应重新关闭/打开一次串口       //
//         PC端串口调试软件发送形式应选择为‘十六进制’                      //
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

#define  FLL_FACTOR    649                                           // FLL_FACTOR: DCO倍频系数    
#define  URECEIVED    0x01                                           // USB接收到数据帧
#define  IRECEIVED    0x02                                           // 红外接收到数据帧
char  event, RXBuffer[30],IRXBuffer[30]                            ;

//***************************************************************************//
//                                                                           //
//  USB接收中断服务程序                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  static char rx_byte        =  0x00                               ;
  unsigned char   temp                                             ;
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      temp                   = UCA1RXBUF                           ;
      if(rx_byte==0)
      {
        if(temp=='C')
        {
          RXBuffer[0]        = UCA1RXBUF                           ;
          rx_byte++                                                ;
        }
      }
      else
      {
        RXBuffer[rx_byte++]  = temp                                ;
        if(rx_byte>=RXBuffer[1])
        {
          if(RXBuffer[rx_byte-1]=='T')
          {
            rx_byte          = 0x00                                ;
            event           |= URECEIVED                           ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<30;temp++)
              RXBuffer[temp] = 0x00                                ; // 通讯有误，清除缓冲区
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event&URECEIVED)  
    LPM3_EXIT                                                      ;
}

//***************************************************************************//
//                                                                           //
//  红外接收中断服务程序                                                     //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  unsigned char temp                                               ;
  static unsigned char rx_byte                                     ; // 接收计数器
  switch(__even_in_range(UCA2IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG    
      temp                   = UCA2RXBUF                           ;
      if(rx_byte==0)
      {
        if(temp=='C')
        {
          IRXBuffer[0]       = UCA2RXBUF                           ;
          rx_byte++                                                ;
        }
      }
      else
      {
        IRXBuffer[rx_byte++] = temp                                ;
        if(rx_byte>=IRXBuffer[1])
        {
          if(IRXBuffer[rx_byte-1]=='T')
          {
            rx_byte          = 0x00                                ;
            event           |= IRECEIVED                           ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<30;temp++)
              IRXBuffer[temp] = 0x00                               ; // 通讯有误，清除缓冲区
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event&IRECEIVED)  
    LPM3_EXIT                                                      ;
}


//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                                     ; // 关看门狗
  P7SEL     |= 0x03                                                ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                              ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                              ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                            ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                              ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                          ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                              ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                           ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                                ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                          ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                          ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG)         ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &= ~OFIFG                                              ; 
  }while(SFRIFG1&OFIFG)                                            ; // 检测振荡器错误标志 
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): 设置IO端口                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR     |= POWER                                               ; // 主电源
  MAIN_POWER_ON                                                    ;
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
//  Init_IRUART(void): 设置红外端口                                          //
//                                                                           //
//***************************************************************************//
void Init_IRUART(void)
{ 
  IrDA_PORT_SEL  |= IrDA_IN + IrDA_OUT                             ; // 选择引脚功能
  IrDA_PORT_DIR  |= IrDA_OUT                                       ; // 选择引脚功能
  UCA2CTL1        = UCSWRST                                        ; // 状态机复位
  UCA2CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA2BR0         = 27                                             ; // 32kHz/1200=27.3 
  UCA2BR1         = 0x00                                           ; 
  UCA2MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA2CTL1       &= ~UCSWRST                                       ; // 启动状态机
  UCA2IE         |= UCRXIE                                         ; // 允许接收中断
}


//***************************************************************************//
//                                                                           //
//  UTX_PROC(void): 设置IO端口                                               //
//                                                                           //
//***************************************************************************//
void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                  ; 
    while (!(UCA1IFG&UCTXIFG))             ; 
  }
}


//***************************************************************************//
//                                                                           //
//  IrDATX_PROC(void): 设置IO端口                                            //
//                                                                           //
//***************************************************************************//
void IrDATX_PROC(char *tx_buf)
{          
  unsigned char i,length                                           ;  
  Init_IRUART()                                                    ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA2TXBUF     = *tx_buf++                                      ; 
    __delay_cycles(280000)                                         ;
  }
  IrDA_PORT_SEL  &=~IrDA_OUT                                       ; // 选择引脚功能
}

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  Init_IRUART()                                                    ;
  P9DIR                      |= IrDA_OUT                           ;
  P9OUT                      &=~IrDA_OUT                           ;
  IrDATX_PROC("This is BW-DK5438")                                 ;
  _EINT()                                                          ;
  for(;;)
  {
    LPM3                                                           ;
    _NOP()                                                         ;
    if(event&URECEIVED)
    {
      RXBuffer[RXBuffer[1]]   = 0x00                               ;
      IrDATX_PROC(RXBuffer)                                        ;
      event      &=~URECEIVED                                      ;
    }
    if(event&IRECEIVED)
    {
      IRXBuffer[IRXBuffer[1]] = 0x00                               ;
      UTX_PROC(IRXBuffer)                                          ;
      event      &=~IRECEIVED                                      ;
    }
  }
}
