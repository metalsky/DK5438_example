//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438�������źŷ���������                                    //
//         ͨ��I2C�ӿڣ�����DAC5571�����ݲ���ѹ                            //
//         �����ѹ���ڿ�������DA��Vo���ż��                                //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.23                                                        //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "I2C.h"

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

//***************************************************************************//
//                                                                           //
// ������Write_DAC(uchar)                                                    //
// ���ܣ���DAC��д�������ѹ����                                             //
// ������wdata  ���������ѹ                                                 //
// ��ֵ��д������1--�ɹ���0--ʧ��                                          //
//                                                                           //
//***************************************************************************//
uchar Write_DAC(uchar wdata)
{
    start()                                          ;
    write1byte(0x98)                                 ; //DAC���豸��ַ
    if(check())   write1byte(wdata >> 4)             ; //д����ģʽ�͵�ѹ���ݵĸ���λ
    else          return 0                           ;
    if(check())   write1byte(wdata << 4)             ; //д��ѹ���ݵĵ���λ
    else          return 0                           ;
    if(check())	  stop()                             ;
    else          return 0                           ;
    return 1                                         ;    
}

void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                         ;
  uchar voltage = 0                                  ;
  Init_CLK()                                         ;   
  Init_I2C()                                         ;
  MAIN_POWER_ON                                      ;
  for(;;)
    Write_DAC(voltage++)                             ;
}
