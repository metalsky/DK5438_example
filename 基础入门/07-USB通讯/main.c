//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��USB�˿�ʵ�����                              //
//         ���������а���������ͨ��USB�˿ڷ���"This is BW-DK5483" ,        //
//         �ɽ�������USB�ӿ���PC���ӣ�PC�����д��ڴ�ʦ�ȴ��ڵ��������       //
//         ��USB���ڽ��ղ��۲쿪���巢�����ݣ�Ҳ����PC�˷���������         //
//         ������,�����彫�������ݷ���PC                                     //
// ע�⣺  ����������Դ�ϵ��PC�˴��ڵ������Ӧ���¹ر�/��һ�δ���       //
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

#define  FLL_FACTOR     749                                          // FLL_FACTOR: DCO��Ƶϵ��    
char  event, RXBuffer[2]                                           ;

//***************************************************************************//
//                                                                           //
//  USB�����жϷ������                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer[0]      = UCA1RXBUF                                 ;
      event           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

//***************************************************************************//
//                                                                           //
//  RS232/485�����жϷ������                                                //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG
      RXBuffer[0]      = UCA3RXBUF                                 ;
      event           |= 0x01                                      ;
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
}

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
  P5DIR  |= POWER                                                  ; // ����Դ
  MAIN_POWER_ON                                                    ;
  P7DIR  |= LED_PWR                                                ; // ��������ܵ�Դ
  P7OUT  &=~LED_PWR                                                ;
  INTERNAL_PULL_UP                                                 ; // ʹ�ܼ��̶˿��ڲ��������� 
  ROW_IN_COL_OUT                                                   ; // ���������룬�����0
}

//***************************************************************************//
//                                                                           //
//  Init_UART(void): ��ʼ��USB�˿�                                           //
//                                                                           //
//***************************************************************************//
void Init_UART(void)
{ 
  USB_PORT_SEL   |= TXD_U + RXD_U                                  ; // ѡ�����Ź���
  USB_PORT_DIR   |= TXD_U                                          ; // ѡ�����Ź���
  UCA1CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA1CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA1BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA1BR1         = 0x00                                           ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA1IE         |= UCRXIE                                         ; // ��������ж�
}


//***************************************************************************//
//                                                                           //
//  Init_RSUART(void): ��ʼ��RS232/485�˿�                                   //
//                                                                           //
//***************************************************************************//
void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD                                      ; // ѡ�����Ź���
  RS_PORT_DIR    |= TXD                                            ; // ѡ�����Ź���
  UCA3CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA3CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA3BR0         = 0x03                                           ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                                           ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA3IE         |= UCRXIE                                         ; // ��������ж�
  RS485_IN                                                         ;
}

//***************************************************************************//
//                                                                           //
//  UTX_PROC(void): USB�˿ڷ��ͳ���                                          //
//                                                                           //
//***************************************************************************//
void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                                          ; 
//    __delay_cycles(5000)                                           ;
    while (!(UCA1IFG&UCTXIFG))                                     ; 
  }
}

//***************************************************************************//
//                                                                           //
//  RS232TX_PROC(void): RS232�˿ڷ��ͳ���                                    //
//                                                                           //
//***************************************************************************//
void RS232TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
}

//***************************************************************************//
//                                                                           //
//  RS485TX_PROC(void): RS485�˿ڷ��ͳ���                                    //
//                                                                           //
//***************************************************************************//
void RS485TX_PROC(char *tx_buf)
{
  unsigned char i,length                                           ;
  length = strlen(tx_buf)                                          ;
  RS485_OUT                                                        ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                                          ; 
    while (!(UCA3IFG&UCTXIFG))                                     ; 
  }
  __delay_cycles(100000)                                           ;
  RS485_IN                                                         ;
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  _EINT()                                                          ;
  for(;;)
  {
    if(event)
    {
      event   = 0x00                                               ;
      UTX_PROC(RXBuffer)                                           ;
    }
    if((P6IN&0x0F)!=0x0F)                                            // ��ⰴ������
    {
      UTX_PROC("This is BW-DK5438")                                ;
      __delay_cycles(5000000)                                      ;
    }    
  }
}
