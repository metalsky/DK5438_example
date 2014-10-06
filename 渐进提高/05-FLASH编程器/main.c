//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板SPI、SST25V016B 实验程序                           //
//         通过SPI口读写SST25V016B                                           //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 备注：  板载SST25V016B内预置多个中/英文字库与图片，编程时请谨慎处理       //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "SST25V.H"

#define FLL_FACTOR 649



//***************************************************************************//
//                                                                           //
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                  ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                              ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                            ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                             ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                  ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                            ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                            ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)  ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &=~OFIFG                                 ; 
  }while(SFRIFG1&OFIFG)                              ; // 检测振荡器错误标志 
}



void main(void)
{
  unsigned char i                                    ;
  char ID[3],buffer[32]                              ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ; 
  MAIN_POWER_ON                                      ;
  __delay_cycles(10000)                              ;
  Init_SSTSPI()                                      ;  
  Read_JEDECID(ID)                                   ;
  ReadFlash(USER_DATA,32,buffer)                     ;
  __delay_cycles(100)                                ;
  EWSR()                                             ; // 寄存器写允许
  WRSR(0x00)                                         ; // 设置保护位
  WREN()                                             ;
  Sector_Erase(USER_DATA)                            ;
  Wait_Busy()                                        ;
  ReadFlash(USER_DATA,32,buffer)                     ;
  for(i=0;i<32;i++)
  {
    WREN()                                           ; // 写允许
    Byte_Program(USER_DATA+i,buffer[i]+1)            ;
    Wait_Busy()                                      ;
  }
  ReadFlash(USER_DATA,32,buffer)                     ;
  LPM3                                               ;
}
