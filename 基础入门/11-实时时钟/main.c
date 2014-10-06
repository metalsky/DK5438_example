//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������RTCʵ�����                                        //
//         ����RTCʱ�ӣ�ͨ�����Ź��ж϶�ʱ�鿴RTC�������                    //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "RTC.H"

#define FLL_FACTOR 649

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  RTCCTL01  &=~(RTCRDYIFG + RTCTEVIFG)               ;
}

//***************************************************************************//
//                                                                           //
//���Ź��жϷ�������Ƭ���˳��͹���ģʽ                                 //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  LPM3_EXIT                                          ;
}



//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                  ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                              ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                            ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                             ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                  ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                            ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                            ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)  ; // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &=~OFIFG                                 ; 
  }while(SFRIFG1&OFIFG)                              ; // ������������־ 
}

//***************************************************************************//
//                                                                           //
//                 ��ʼ��RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // ʱ��ģʽ,ÿ�����ж�, BCD��ʽ
  SetRTCYEAR(2010)                                   ; 
  SetRTCMON(9)                                       ;
  SetRTCDAY(19)                                      ;
  SetRTCDOW(7)                                       ;
  SetRTCHOUR(17)                                     ;
  SetRTCMIN(2)                                       ;
  SetRTCSEC(35)                                      ;
}

//***************************************************************************//
//                                                                           //
//                 ��ȡRTCʱ��                                               //
//                                                                           //
//***************************************************************************//
unsigned long GetTime(void)
{
  int hour,minute,second                             ;
  unsigned long time                                 ;
  for(;RTCCTL01&RTCRDY;)        _NOP()               ; // �ȴ�RTC��Ч                                
  for(;!(RTCCTL01&RTCRDY);)     _NOP()               ;                                     
  hour    = GetRTCHOUR()                             ;
  minute  = GetRTCMIN()                              ;
  second  = GetRTCSEC()                              ;
  time    = (unsigned long)hour*1000000
           +(unsigned long)minute*1000
           +(unsigned long)second                    ;
  return time                                        ;
}


void main(void)
{
  unsigned long time                                 ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ; 
  Init_RTC()                                         ;
  SFRIE1  |= WDTIE                                   ; // ʹ��WDT�ж�
  WDTCTL   = WDT_ADLY_1000                           ; // ����WDTΪINTERVALģʽ���жϼ��100ms
  MAIN_POWER_ON                                      ;
  _EINT()                                            ;
  for(;;)
  {
    time = GetTime()                                 ;   
    LPM3                                             ;
    _NOP()                                           ;
  }
}

