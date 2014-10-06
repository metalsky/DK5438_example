//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板信号发生器程序                                    //
//         通过I2C接口，控制DAC5571输出锯齿波电压                            //
//         输出电压可在开发板上DA口Vo引脚检测                                //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.23                                                        //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "I2C.h"

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

//***************************************************************************//
//                                                                           //
// 函数：Write_DAC(uchar)                                                    //
// 功能：向DAC中写入输出电压数据                                             //
// 参数：wdata  ——输出电压                                                 //
// 返值：写入结果：1--成功，0--失败                                          //
//                                                                           //
//***************************************************************************//
uchar Write_DAC(uchar wdata)
{
    start()                                          ;
    write1byte(0x98)                                 ; //DAC的设备地址
    if(check())   write1byte(wdata >> 4)             ; //写控制模式和电压数据的高四位
    else          return 0                           ;
    if(check())   write1byte(wdata << 4)             ; //写电压数据的低四位
    else          return 0                           ;
    if(check())	  stop()                             ;
    else          return 0                           ;
    return 1                                         ;    
}

void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                         ;
  uchar voltage = 0                                  ;
  Init_CLK()                                         ;   
  Init_I2C()                                         ;
  MAIN_POWER_ON                                      ;
  for(;;)
    Write_DAC(voltage++)                             ;
}
