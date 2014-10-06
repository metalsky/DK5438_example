//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������ADCʵ�����                                        //
//         ����ADC12���ɿ��Ź��жϳ���ʱ�������Դ��ѹ����ƽ������         //
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
#define FLL_FACTOR                    649

float     Vmain = 0                                       ; // ����Դ��ѹ


//***************************************************************************//
//                                                                           //
//������void Init_CLK(void)                                                  //
//˵������ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                             //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                       ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                     ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                     ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                     ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                 ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)       ; // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &=~OFIFG                                      ; 
  }while(SFRIFG1&OFIFG)                                   ; // ������������־ 
}

//***************************************************************************//
//                                                                           //
//������void Init_ADC(void)                                                  //
//˵������ʼ��ADC12                                                          //
//                                                                           //
//***************************************************************************//
void Init_ADC(void)
{     //Vbat
  //VbatCON P6.3
  P6DIR   |=  BIT3                               ; //  P6.3 outputs
  P6OUT   |=  BIT3                               ; //  P6.3 set
  //P6OUT   &= ~BIT3                               ; //  P6.3 reset
  //Vbat P6.4
  P6DIR   &= ~BIT4                               ; // P6.4 Input   
  P6SEL   |=  BIT4                               ; // Enable A/D channel A0

  //P7SEL      |= TVBTM                                     ; // Enable A/D channel A0
  ADC12CTL0   = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V; // ��ADC12, ����ʱ��ԴΪ�ڲ����������ο�ԴΪ�ڲ�2.5V
  ADC12CTL1   = ADC12SHP                                  ; // ���ڲ�����ʱ��
  ADC12MCTL0  = ADC12SREF_1+ ADC12INCH_4                 ; // Vr+=Vref+ and Vr-=AVss
  __delay_cycles(20000000)                                ; // Delay for reference start-up
  ADC12CTL0  |= ADC12ENC                                  ; // Enable conversions
}

//***************************************************************************//
//                                                                           //
//������void VmainMon(void)                                                  //
//˵�����������Դ��ѹ������ƫ�����ƽ������                               //
//                                                                           //
//***************************************************************************//
void VmainMon(void)
{
  ADC12CTL0   |= ADC12SC                                  ; 
  while (!(ADC12IFG & BIT0))                              ;
  Vmain     +=(ADC12MEM0*5.0/4095-0.03)                   ; 
  //Vmain        = 0                                      ;
}

void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                              ;
  Init_CLK()                                              ;  
  Init_ADC()                                              ;
  SFRIE1  |= WDTIE                                        ; // ʹ��WDT�ж�
  WDTCTL   = WDT_ADLY_16                                  ; // ����WDTΪINTERVALģʽ���жϼ��100ms
  MAIN_POWER_ON                                           ;
  _EINT()                                                 ;
  LPM3                                                    ;
}

//***************************************************************************//
//                                                                           //
//���Ź��жϷ�����򣬼������ص�ѹ����ƽ������                             //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  static unsigned char i = 0                              ;
  VmainMon()                                              ;
  if(++i==1)
  {
    Vmain     /= 1                                       ;
    i          = 0                                        ;
    Vmain        = 0                                      ;
  }
}

