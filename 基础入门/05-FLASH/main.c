//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板单片机片内FLASH擦除/读/写实验程序                  //
//         程序运行后，每次按下第4行任一按键，则依次读出片内FLASH起始        //
//         地址为0xE200的512字节数据后加1暂存，擦除0xE200段后将暂存数        //
//         据写会FLASH                                                       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#define  FLL_FACTOR                 649                     // FLL_FACTOR: DCO倍频系数    


//***************************************************************************//
//                                                                           //
//函数：void Init_CLK(void)                                                  //
//说明：初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                             //
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
  P5DIR  |= POWER                                         ; // 主电源
  MAIN_POWER_ON                                           ;
  INTERNAL_PULL_UP                                        ; // 使能键盘端口内部上拉电阻 
  ROW_IN_COL_OUT                                          ; // 设置行输入，列输出0
}

//***************************************************************************//
//                                                                           //
//函数：void ERASE(unsigned int)                                             //
//说明：擦除片内FLASH数据段                                                  //
//参数：address ——擦除数据段地址                                           //
//                                                                           //
//***************************************************************************//
void FERASE(unsigned int address)
{
  unsigned int *Erase_address                             ;
  Erase_address  = (unsigned int *)address                ;
  FCTL1          = FWKEY + ERASE                          ;
  FCTL3          = FWKEY                                  ;
  *Erase_address = 0                                      ;
  FCTL1          = FWKEY                                  ;
  FCTL3          = FWKEY + LOCK                           ;
}

//***************************************************************************//
//                                                                           //
//函数：void ERASE(unsigned int)                                             //
//说明：擦除片内FLASH数据                                                    //
//参数：address ——擦除起始地址                                             //
//                                                                           //
//***************************************************************************//
void FWRITE(unsigned int address, unsigned char* data,unsigned int length)
{
  unsigned int   i                                        ;
  unsigned char* Wr_Addr                                  ;
  Wr_Addr =(unsigned char*)address                        ;
  FCTL1   = FWKEY + WRT                                   ; // Set WRT bit for write operation
  FCTL3   = FWKEY                                         ;
  for(i=0;i<length;i++)
  {
    *Wr_Addr= data[i]                                     ;
    Wr_Addr++                                             ;
  }
  FCTL1   = FWKEY                                         ;
  FCTL3   = FWKEY + LOCK                                  ;  
}

int main( void )
{
  unsigned char temp[1000]                                ;
  unsigned int  i                                         ;
  char *addr                                              ;
  WDTCTL = WDTPW + WDTHOLD                                ;
  Init_CLK()                                              ;
  Init_Port()                                             ;
  for(;;)
  {
    if(!(P6IN&KPR0))
    {
      addr   = (char *)0xE200                             ;
      for(i=0;i<512;i++)
      {
        temp[i]  = *addr++                                ; 
        temp[i]++                                         ;
      }
      FERASE(0xE200)                                      ;
      FWRITE(0xE200,temp,512)                             ;
    }
  }
}
