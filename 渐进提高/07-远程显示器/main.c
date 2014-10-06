//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������Զ����ʾ������                                     //
//         ������USB�ӿ���PC���ӣ�ͨ��USB�ڽ���PC��ָ������ݣ�����          //
//         ����TFTҺ����ʾ�����ָ������ʾ����                               //
//         ��USB���ڽ��ղ��۲쿪���巢�����ݣ�Ҳ����PC�˷���������         //
//         ͨѶЭ�飺ͷ�ַ�'C'+�ַ�������+�����ַ�+���ݶ�+β�ַ�'T'          //
//         ���������ݶ��壺'A'+2�ֽ���ɫ���� ������ָ����ɫ����              //
//                         'B'+2�ֽ���ɫ���� ��������ǰ����ɫ                //
//                         'C'+2�ֽ���ɫ���� �������ñ�����ɫ                //
//                         'D'+2�ֽ�X����+2�ֽ�Y����+1�ֽ��ַ�               // 
//                                           ������X��Y���괦��ʾӢ���ַ�    //
//                         'E'+2�ֽ�X����+2�ֽ�Y����+2�ֽ�����               // 
//                                           ������X��Y���괦��ʾ����        //
// ʾ����  43 06 41 1F 00 54             �����Դ���ɫ����                    //
//         43 06 42 FF FF 54             �������ð�ɫΪǰ��ɫ                //
//         43 06 43 1F 00 54             �������ô���Ϊ����ɫ                //
//         43 09 44 0A 00 0A 00 44 54    ����������Ϊ(10,10)����ʾ'D'        //
//         43 0A 45 0A 00 0A 00 BA C3 54 ����������Ϊ(10,10)����ʾ'��'       //
// ע�⣺  ÿ�ο���������Դ�ϵ��PC�˴��ڵ������Ӧ���¹ر�/��һ�δ���   //
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
#include "TFTDRV.H"

#define  FLL_FACTOR     549                                          // FLL_FACTOR: DCO��Ƶϵ��    
char     RXBuffer[20]                                              ; // ���ջ���
unsigned int  Color                                                ; // ǰ����ɫ
unsigned int  Color_BK                                             ; // ������ɫ

//***************************************************************************//
//                                                                           //
//  USB�����жϷ������                                                      //
//                                                                           //
//***************************************************************************//
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  static char rx_byte        =  0x00                               ;
  unsigned char temp,event   =  0x00                               ;
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
            event           |= 0x01                                ;
          }
          else
          {
            rx_byte          = 0x00                                ;  
            for(temp=0;temp<20;temp++)
              RXBuffer[temp] = 0x00                                ; // ͨѶ�������������
          }
        }        
      }
      break                                                        ;
  case 4:break                                                     ;  // Vector 4 - TXIFG
  default: break                                                   ;  
  }  
  if(event)  
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
  P5DIR  |= POWER                                                  ; // ����Դ
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

void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  union
  {
    unsigned int  value                                            ;
    unsigned char byte[2]                                          ;
  } d1,d2,d3                                                       ;  
  Init_CLK()                                                       ;
  Init_Port()                                                      ;
  Init_UART()                                                      ;
  LCD_Init()                                                       ;
  _EINT()                                                          ;
  for(;;)
  {    
    LPM3                                                           ;
    _NOP()                                                         ;
    switch(RXBuffer[2])
    {
    case 'A':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Clear_LCD(d1.value)                                          ;
      break                                                        ;
    case 'B':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Color       = d1.value                                       ;
      break                                                        ;
    case 'C':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      Color_BK    = d1.value                                       ;
      break                                                        ;
    case 'D':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      d2.byte[0]  = RXBuffer[5]                                    ;
      d2.byte[1]  = RXBuffer[6]                                    ;
      d3.byte[0]  = RXBuffer[7]                                    ;
      PutCharEN24(d1.value,d2.value,d3.byte)                       ;
      break                                                        ;
    case 'E':
      d1.byte[0]  = RXBuffer[3]                                    ;
      d1.byte[1]  = RXBuffer[4]                                    ;
      d2.byte[0]  = RXBuffer[5]                                    ;
      d2.byte[1]  = RXBuffer[6]                                    ;
      d3.byte[0]  = RXBuffer[7]                                    ;
      d3.byte[1]  = RXBuffer[8]                                    ;
      PutCharCN24(d1.value,d2.value,d3.byte)                       ;
      break                                                        ;
    }
  }
}
