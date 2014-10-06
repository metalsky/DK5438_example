//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����忴�Ź���ʱ������ʵ�����                           //
//         ���ÿ��Ź���ʱ��ÿ���ж�һ�Σ���תLED�����������                 //
//         ����LED��ÿ2��һ����˸                                            //
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
  P1OUT   ^= 0xF0                                         ; 
  P9OUT   ^= 0x0F                                         ; 
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): ����IO�˿�                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR  |= POWER                                         ; // ����Դ
  MAIN_POWER_ON                                           ;
  P7DIR  |= LED_PWR                                       ; // ��������ܵ�Դ
  P7OUT  &=~LED_PWR                                       ;
  P1OUT   = 0xF0                                          ; // ����LED���ƶ�IO����
  P1DIR   = 0xF0                                          ;
  P9OUT   = 0x0F                                          ; 
  P9DIR   = 0x0F                                          ;
}


int main( void )
{
  WDTCTL   = WDTPW + WDTHOLD                              ;
  Init_Port()                                             ;
  P7SEL   |= 0x03                                         ; // ʹ��XT1
  UCSCTL6 &=~XT1OFF                                       ;  
  UCSCTL6 |= XCAP_3                                       ; 
  UCSCTL4 |= SELA_0                                       ; // ACLKʱ��Դѡ��XT1  
  SFRIE1  |= WDTIE                                        ; // ʹ��WDT�ж�
  WDTCTL   = WDT_ADLY_1000                                ; // ����WDTΪINTERVALģʽ���жϼ��1000ms
  _EINT()                                                 ;
  LPM3                                                    ;
}
