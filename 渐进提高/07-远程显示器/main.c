//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板远程显示器程序                                     //
//         开发板USB接口与PC连接，通过USB口接收PC机指令和数据，控制          //
//         板载TFT液晶显示屏完成指定的显示操作                               //
//         打开USB串口接收并观察开发板发送数据，也可由PC端发送数据至         //
//         通讯协议：头字符'C'+字符串长度+命令字符+数据段+尾字符'T'          //
//         命令与数据定义：'A'+2字节颜色数据 ——以指定颜色清屏              //
//                         'B'+2字节颜色数据 ——设置前景颜色                //
//                         'C'+2字节颜色数据 ——设置背景颜色                //
//                         'D'+2字节X坐标+2字节Y坐标+1字节字符               // 
//                                           ——在X，Y坐标处显示英文字符    //
//                         'E'+2字节X坐标+2字节Y坐标+2字节内码               // 
//                                           ——在X，Y坐标处显示汉字        //
// 示例：  43 06 41 1F 00 54             ——以纯蓝色清屏                    //
//         43 06 42 FF FF 54             ——设置白色为前景色                //
//         43 06 43 1F 00 54             ——设置纯蓝为背景色                //
//         43 09 44 0A 00 0A 00 44 54    ——在坐标为(10,10)处显示'D'        //
//         43 0A 45 0A 00 0A 00 BA C3 54 ——在坐标为(10,10)处显示'好'       //
// 注意：  每次开发板主电源上电后，PC端串口调试软件应重新关闭/打开一次串口   //
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
#include "TFTDRV.H"

#define  FLL_FACTOR     549                                          // FLL_FACTOR: DCO倍频系数    
char     RXBuffer[20]                                              ; // 接收缓存
unsigned int  Color                                                ; // 前景颜色
unsigned int  Color_BK                                             ; // 背景颜色

//***************************************************************************//
//                                                                           //
//  USB接收中断服务程序                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  static char rx_byte        =  0x00                               ;
  unsigned char temp,event   =  0x00                               ;
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
            event           |= 0x01                                ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<20;temp++)
              RXBuffer[temp] = 0x00                                ; // 通讯有误，清除缓冲区
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event)  
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
  P5DIR  |= POWER                                                  ; // 主电源
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

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  union
  {
    unsigned int  value                                            ;
    unsigned char byte[2]                                          ;
  } d1,d2,d3                                                       ;  
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  LCD_Init()                                                       ;
  _EINT()                                                          ;
  for(;;)
  {    
    LPM3                                                           ;
    _NOP()                                                         ;
    switch(RXBuffer[2])
    {
    case 'A':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Clear_LCD(d1.value)                                          ;
      break                                                        ;
    case 'B':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Color       = d1.value                                       ;
      break                                                        ;
    case 'C':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Color_BK    = d1.value                                       ;
      break                                                        ;
    case 'D':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      d2.byte[0]  = RXBuffer[5]                                    ;
      d2.byte[1]  = RXBuffer[6]                                    ;
      d3.byte[0]  = RXBuffer[7]                                    ;
      PutCharEN24(d1.value,d2.value,d3.byte)                       ;
      break                                                        ;
    case 'E':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      d2.byte[0]  = RXBuffer[5]                                    ;
      d2.byte[1]  = RXBuffer[6]                                    ;
      d3.byte[0]  = RXBuffer[7]                                    ;
      d3.byte[1]  = RXBuffer[8]                                    ;
      PutCharCN24(d1.value,d2.value,d3.byte)                       ;
      break                                                        ;
    }
  }
}
