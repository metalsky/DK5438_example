//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��SPI�˿�ʵ�����                              //
//         ����SPI�˿ڹ���ģʽ��ͨ��SPI�˿ڷ�������                          //
//         ����P3.1/P3.3�˿ڹ۲����ݡ�ʱ�Ӳ���                               //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PIN_DEF.H"

#define  FLL_FACTOR     649                                 // FLL_FACTOR: DCO��Ƶϵ��    
char  event, RXBuffer[2]                                  ;


//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // �ؿ��Ź�
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
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // ������������־ 
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
  INTERNAL_PULL_UP                                        ; // ʹ�ܼ��̶˿��ڲ��������� 
  ROW_IN_COL_OUT                                          ; // ���������룬�����0
}

//***************************************************************************//
//                                                                           //
//  Init_SPI(void): ����SPI�˿�                                              //
//                                                                           //
//***************************************************************************//
void Init_SPI(void)
{  
  P8OUT    |= LCD_NCS                                     ; // SPI�˿ڸ��ã�DisableTFTҺ���˿�
  P8DIR    |= LCD_NCS                                     ;
  P3OUT    |= NCS25                                       ; 
  P3DIR    |= NCS25                                       ;
  P3SEL    &= 0xF0                                        ;
  P3SEL    |= 0x0E                                        ; // // P3.1/2/3����ѡ��ΪSPI
  UCB0CTL1 |= UCSWRST                                     ; // ��λSPI״̬��
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB                   ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                                    ; // ѡ��SCK�ο�ԴΪSMCLK
  UCB0BR0   = 0x02                                        ; // SCK = SMCK/2
  UCB0BR1   = 0                                           ; 
  UCB0CTL1 &=~UCSWRST                                     ; // SPI״̬��ʹ��
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                ;
  Init_CLK()                                              ;
  Init_Port()                                             ;
  Init_SPI()                                              ;
  for(;;)
  {
     UCB0TXBUF = 0x55                                     ; 
     __delay_cycles(280)                                  ; 
  }
}
