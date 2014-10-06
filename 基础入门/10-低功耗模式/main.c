//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������͹���ģʽ����ʵ�����                             //
//         ����MSP430F5438ΪLPM3ģʽ����WDTÿ4�����������������һ��         //
//         �����������ڿ�����J0����ɶ���������ĵ�������                  //
//         �˳�����ƽ������Ϊ5uA����                                       //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
   LPM3_EXIT                                     ; // �˳��͹���ģʽ
}


int main( void )
{
  WDTCTL   = WDTPW + WDTHOLD                     ;
  P7SEL   |= 0x03                                ; // ʹ��XT1
  UCSCTL6 &=~XT1OFF                              ;  
  UCSCTL6 |= XCAP_3                              ; 
  UCSCTL4 |= SELA_0                              ; // ACLKʱ��Դѡ��XT1  
  UCSCTL5 |= DIVA1                               ; // ACLK = XT1/4
  P1DIR    = 0xFF                                ; // All P1.x outputs
  P1OUT    = 0x00                                ; // All P1.x reset
  P2DIR    = 0xFF                                ; // All P2.x outputs
  P2OUT    = 0x00                                ; // All P2.x reset
  P3DIR    = 0xFF                                ; // All P3.x outputs
  P3OUT    = 0x00                                ; // All P3.x reset
  P4DIR    = 0xFF                                ; // All P4.x outputs
  P4OUT    = 0x00                                ; // All P4.x reset
  P5DIR    = 0xFF                                ; // All P5.x outputs
  P5OUT    = 0x00                                ; // All P5.x reset
  P6DIR    = 0xFF                                ; // All P6.x outputs
  P6OUT    = 0x00                                ; // All P6.x reset
  P7DIR    = 0xFF                                ; // All P6.x outputs
  P7OUT    = 0x00                                ; // All P6.x reset
  P8DIR    = 0xFF                                ; // All P6.x outputs
  P8OUT    = 0xFF                                ; // All P6.x reset
  P9DIR    = 0xFF                                ; // All P6.x outputs
  P9OUT    = 0x00                                ; // All P6.x reset
  P10DIR   = 0xFF                                ; // All P6.x outputs
  P10OUT   = 0x00                                ; // All P6.x reset
  P11DIR   = 0xFF                                ; // All P6.x outputs
  P11OUT   = 0x00                                ; // All P6.x reset
  SFRIE1  |= WDTIE                               ; // ʹ��WDT�ж�
  WDTCTL   = WDT_ADLY_1000                       ; // ����WDTΪINTERVALģʽ���жϼ��1000ms
  _EINT()                                        ;
  for(;;)
  {
    LPM3                                         ;
    _NOP()                                       ;    
    MAIN_POWER_ON                                ;
    __delay_cycles(100)                          ;
    MAIN_POWER_OFF                               ;
  }
}
