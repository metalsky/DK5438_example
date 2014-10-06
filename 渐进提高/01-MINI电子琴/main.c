//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������MINI�����ٳ���                                     //
//         �ɼ��̶˿�KPC0~3����͵�ƽ��KPR0~3����Ϊ���벢�ڲ�����            //
//         ������������ʱ����Ƭ����⵽KPR0~3��һ·���·�����ͣ�            //
//         �򰴱����°�������λ�õ�����Ӧ�ķ��������                        //
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
#include "KEY.H"
#include "TONE.H"

#define FLL_FACTOR               649

unsigned int tone                                         ;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  P7OUT   ^= BUZZER                                       ;
  TA0CCR0  = tone                                         ;
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
  P7DIR  |= BUZZER                                        ;
  P7OUT  |= BUZZER                                        ;
  P1OUT   = 0xF0                                          ; // ����LED���ƶ�IO����
  P1DIR   = 0xF0                                          ;
  P9OUT   = 0x0F                                          ; 
  P9DIR   = 0x0F                                          ;
}

//***************************************************************************//
//                                                                           //
//������Init_CLK(void)                                                       //
//���ܣ���ʼ����ʱ��: MCLK = XT2                                             //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // �ؿ��Ź�
  P5SEL     |= 0x0C                                       ; // �˿ڹ���ѡ������
  UCSCTL6   &= ~XT2OFF                                    ; // ����ʹ��
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // ��� XT2,XT1,DCO �����־                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // ������������־
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 ����ģʽ 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}

//***************************************************************************//
//                                                                           //
//  Init_TimerA0(void): ����TimerA0                                              //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{
  TA0CTL   = 0                                               // ��λTimer0_A5, ��Ƶϵ������Ϊ1
           | (1 << 2)                                        // ��������0
           | (2 << 8)                                     ; // ����ʱ����ΪSMCLK    
  TA0CCR0  =   600  - 1                                   ; // SMCK=EX2=16MHz�����ü��������ʱ��Ϊ1ms
  TA0CCTL0 = 0                                              // ��ʼ���������
           | (1 << 4)                                     ; // ʹ�ܱȽ��ж�
  TA0CTL  |= (1 << 4)                                     ; // ���ü�����Ϊ�Ӽ���������
}


//***************************************************************************//
//                                                                           //
//                 ������: ����ʱ�Ӳ������P11.0~2                           //
//                                                                           //
//***************************************************************************//
void main( void )
{
  unsigned char key                                       ;
  WDTCTL = WDTPW + WDTHOLD                                ; // �رտ��Ź�
  Init_CLK()                                              ;
  Init_Port()                                             ;  
  Init_KeyPad()                                           ;
  Init_Timer0_A5()                                        ;
  while(1)
  {
    key   = ReadKey()                                     ;
    switch(key)
    {
     case 1: case 2: case 3:case 4:
     case 5: case 6: case 7:case 8:
      {
        tone       = TONE[key]                            ;
        _EINT()                                           ;
        P1OUT     &=~0xF0                                 ; 
        P9OUT     &=~0x0F                                 ; 
      }
      break                                               ;
    default:
      _DINT()                                             ;
      P1OUT       |= 0xF0                                 ; 
      P9OUT       |= 0x0F                                 ; 
      break                                               ;      
    }
  }
}
