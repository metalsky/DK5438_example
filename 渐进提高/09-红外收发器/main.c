//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ�������շ�����                                 //
//         PC�����д��ڴ�ʦ�ȴ��ڵ��������ͨ��USB�ӿ��򿪷��巢�����ݣ�     //
//         �������յ�����֡�󣬽����ɺ���˿ڷ��ͣ��������ܽ��յ��Լ���      //
//         �͵ĺ����źţ�������˿ڽ��յ���������֡�󣬿����彫������USB     // 
//         ������PC��                                                        // 
//         ������δ����NEC��Philips�ĺ���Э�飬ּ��չʾһ����ʹ�ô���һ��    //
//         ʹ�ú���ӿڵķ�ʽ��ʵ���������һ����ʮ����Ч����1200bps��3��    // 
//         ��������������зǳ��õ��շ�Ч����ʵ��Ӧ���У��ɼ����Ҫ������    //
//         ��У�顣                                                          //
// ʾ����  �������� ����43 0D 12 34 56 78 90 98 76 54 32 10 54               //
// ע�⣺  ����������Դ�ϵ��PC�˴��ڵ������Ӧ���¹ر�/��һ�δ���       //
//         PC�˴��ڵ������������ʽӦѡ��Ϊ��ʮ�����ơ�                      //
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

#define  FLL_FACTOR    649                                           // FLL_FACTOR: DCO��Ƶϵ��    
#define  URECEIVED    0x01                                           // USB���յ�����֡
#define  IRECEIVED    0x02                                           // ������յ�����֡
char  event, RXBuffer[30],IRXBuffer[30]                            ;

//***************************************************************************//
//                                                                           //
//  USB�����жϷ������                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  static char rx_byte        =  0x00                               ;
  unsigned char   temp                                             ;
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
            event           |= URECEIVED                           ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<30;temp++)
              RXBuffer[temp] = 0x00                                ; // ͨѶ�������������
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event&URECEIVED)  
    LPM3_EXIT                                                      ;
}

//***************************************************************************//
//                                                                           //
//  ��������жϷ������                                                     //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  unsigned char temp                                               ;
  static unsigned char rx_byte                                     ; // ���ռ�����
  switch(__even_in_range(UCA2IV,4))
  {
  case 0:break                                                     ; // Vector 0 - no interrupt
  case 2:                                                            // Vector 2 - RXIFG    
      temp                   = UCA2RXBUF                           ;
      if(rx_byte==0)
      {
        if(temp=='C')
        {
          IRXBuffer[0]       = UCA2RXBUF                           ;
          rx_byte++                                                ;
        }
      }
      else
      {
        IRXBuffer[rx_byte++] = temp                                ;
        if(rx_byte>=IRXBuffer[1])
        {
          if(IRXBuffer[rx_byte-1]=='T')
          {
            rx_byte          = 0x00                                ;
            event           |= IRECEIVED                           ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<30;temp++)
              IRXBuffer[temp] = 0x00                               ; // ͨѶ�������������
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event&IRECEIVED)  
    LPM3_EXIT                                                      ;
}


//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                                     ; // �ؿ��Ź�
  P7SEL     |= 0x03                                                ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                              ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                              ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                                            ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                              ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                          ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                              ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                                           ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                                ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                                          ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                                          ; 
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG)         ; // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &= ~OFIFG                                              ; 
  }while(SFRIFG1&OFIFG)                                            ; // ������������־ 
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): ����IO�˿�                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P5DIR     |= POWER                                               ; // ����Դ
  MAIN_POWER_ON                                                    ;
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
//  Init_IRUART(void): ���ú���˿�                                          //
//                                                                           //
//***************************************************************************//
void Init_IRUART(void)
{ 
  IrDA_PORT_SEL  |= IrDA_IN + IrDA_OUT                             ; // ѡ�����Ź���
  IrDA_PORT_DIR  |= IrDA_OUT                                       ; // ѡ�����Ź���
  UCA2CTL1        = UCSWRST                                        ; // ״̬����λ
  UCA2CTL1       |= UCSSEL_1                                       ; // CLK = ACLK
  UCA2BR0         = 27                                             ; // 32kHz/1200=27.3 
  UCA2BR1         = 0x00                                           ; 
  UCA2MCTL        = UCBRS_3 + UCBRF_0                              ; // UCBRSx=3, UCBRFx=0
  UCA2CTL1       &= ~UCSWRST                                       ; // ����״̬��
  UCA2IE         |= UCRXIE                                         ; // ��������ж�
}


//***************************************************************************//
//                                                                           //
//  UTX_PROC(void): ����IO�˿�                                               //
//                                                                           //
//***************************************************************************//
void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                  ; 
    while (!(UCA1IFG&UCTXIFG))             ; 
  }
}


//***************************************************************************//
//                                                                           //
//  IrDATX_PROC(void): ����IO�˿�                                            //
//                                                                           //
//***************************************************************************//
void IrDATX_PROC(char *tx_buf)
{          
  unsigned char i,length                                           ;  
  Init_IRUART()                                                    ;
  length = strlen(tx_buf)                                          ;
  for(i=0;i<length;i++)
  {
    UCA2TXBUF     = *tx_buf++                                      ; 
    __delay_cycles(280000)                                         ;
  }
  IrDA_PORT_SEL  &=~IrDA_OUT                                       ; // ѡ�����Ź���
}

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  Init_IRUART()                                                    ;
  P9DIR                      |= IrDA_OUT                           ;
  P9OUT                      &=~IrDA_OUT                           ;
  IrDATX_PROC("This is BW-DK5438")                                 ;
  _EINT()                                                          ;
  for(;;)
  {
    LPM3                                                           ;
    _NOP()                                                         ;
    if(event&URECEIVED)
    {
      RXBuffer[RXBuffer[1]]   = 0x00                               ;
      IrDATX_PROC(RXBuffer)                                        ;
      event      &=~URECEIVED                                      ;
    }
    if(event&IRECEIVED)
    {
      IRXBuffer[IRXBuffer[1]] = 0x00                               ;
      UTX_PROC(IRXBuffer)                                          ;
      event      &=~IRECEIVED                                      ;
    }
  }
}
