//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������SPI��SST25V016B ʵ�����                           //
//         ͨ��SPI�ڶ�дSST25V016B                                           //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ��ע��  ����SST25V016B��Ԥ�ö����/Ӣ���ֿ���ͼƬ�����ʱ���������       //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "SST25V.H"

#define FLL_FACTOR 649



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



void main(void)
{
  unsigned char i                                    ;
  char ID[3],buffer[32]                              ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ; 
  MAIN_POWER_ON                                      ;
  __delay_cycles(10000)                              ;
  Init_SSTSPI()                                      ;  
  Read_JEDECID(ID)                                   ;
  ReadFlash(USER_DATA,32,buffer)                     ;
  __delay_cycles(100)                                ;
  EWSR()                                             ; // �Ĵ���д����
  WRSR(0x00)                                         ; // ���ñ���λ
  WREN()                                             ;
  Sector_Erase(USER_DATA)                            ;
  Wait_Busy()                                        ;
  ReadFlash(USER_DATA,32,buffer)                     ;
  for(i=0;i<32;i++)
  {
    WREN()                                           ; // д����
    Byte_Program(USER_DATA+i,buffer[i]+1)            ;
    Wait_Busy()                                      ;
  }
  ReadFlash(USER_DATA,32,buffer)                     ;
  LPM3                                               ;
}
