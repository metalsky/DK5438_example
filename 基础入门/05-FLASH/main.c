//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�����嵥Ƭ��Ƭ��FLASH����/��/дʵ�����                  //
//         �������к�ÿ�ΰ��µ�4����һ�����������ζ���Ƭ��FLASH��ʼ        //
//         ��ַΪ0xE200��512�ֽ����ݺ��1�ݴ棬����0xE200�κ��ݴ���        //
//         ��д��FLASH                                                       //
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
#define  FLL_FACTOR                 649                     // FLL_FACTOR: DCO��Ƶϵ��    


//***************************************************************************//
//                                                                           //
//������void Init_CLK(void)                                                  //
//˵������ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                             //
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
  INTERNAL_PULL_UP                                        ; // ʹ�ܼ��̶˿��ڲ��������� 
  ROW_IN_COL_OUT                                          ; // ���������룬�����0
}

//***************************************************************************//
//                                                                           //
//������void ERASE(unsigned int)                                             //
//˵��������Ƭ��FLASH���ݶ�                                                  //
//������address �����������ݶε�ַ                                           //
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
//������void ERASE(unsigned int)                                             //
//˵��������Ƭ��FLASH����                                                    //
//������address ����������ʼ��ַ                                             //
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
