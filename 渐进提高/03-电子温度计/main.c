//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438����������¶ȼƳ���                                     //
//         �����������ʾ��⵽���¶�ֵ                                      //
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
#include "LED.H"
#include "DS18B20.H"

#define FLL_FACTOR 649

unsigned char SMG[10]=
{
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF)       ,
  ~(SEGB+SEGC)                           ,
  ~(SEGA+SEGB+SEGD+SEGE+SEGG)            ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGG)            ,
  ~(SEGB+SEGC+SEGF+SEGG)                 ,
  ~(SEGA+SEGC+SEGD+SEGF+SEGG)            ,
  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG)       ,
  ~(SEGA+SEGB+SEGC)                      ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF+SEGG)  ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG)       ,
}                                                    ;

LED_STRUCT LED[8]                                    ;


//***************************************************************************//
//                                                                           //
//���Ź��жϷ������ˢ����ʾ����                                           //
//                                                                           //
//***************************************************************************//
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  LPM3_EXIT                                          ;
}


//***************************************************************************//
//                                                                           //
//    TIMERB1�жϷ�������������ʾ                                        //
//                                                                           //
//***************************************************************************//
#pragma vector=TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void)
{
  static unsigned char on_off         = 1            ;
  static unsigned char digit          = 1            ; 
  static unsigned int  flicker_timer  = 0            ;
  P8OUT           = LED[digit-1].data                ;  
  switch(digit)
  {
  case 1: case 2: case 3: case 4:
    MASK_DIGITS                                      ;
    if(LED[digit-1].mode&DIGIT_FLICKER)
      if(flicker_timer>256) 
        break                                        ;
    if(LED[digit-1].mode&DIGIT_OFF)
      break                                          ;
    if(on_off)
    {
      LOW_DIGITS_OUT(~(0x01<<(digit-1)))             ;
      P8OUT           = LED[digit-1].data            ;        
    }      
    break                                            ;
  case 5: case 6: case 7: case 8:
    MASK_DIGITS                                      ;
    if(LED[digit-1].mode&DIGIT_FLICKER)
      if(flicker_timer>256) 
        break                                        ;
    if(LED[digit-1].mode&DIGIT_OFF)
      break                                          ;
    if(on_off)
    {
      HIGH_DIGITS_OUT(~(0x10<<(digit-5)))            ;
      P8OUT           = LED[digit-1].data            ;        
    }
    break                                            ;
  }
  
  if(on_off) 
    if(++flicker_timer>=512) 
      flicker_timer = 0                              ; 
  if(on_off)       
    TBCCR0          = LED[digit-1].lightness         ; // ���ȵ���
  else
    TBCCR0          = 64 - LED[digit-1].lightness    ; 
  if(on_off)
    if(++digit>8)     
      digit         = 1                              ;
  on_off           ^= 0x01                           ;
  TBCTL            &= ~TBIFG                         ;
}

unsigned int Do1Convert(void)                        ;

int TEMP                                             ;

void Delay(unsigned int cnt)
{
  unsigned int i=0;
  for(i=0;i<cnt;i++)
  {
    _NOP();
  }
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



void main(void)
{
  WDTCTL   = WDTPW + WDTHOLD                         ;
  float  temp                                        ;
  Init_CLK()                                         ; 
  Ini_LED(10)                                        ;  
  OW_DIO_PULLUP                                      ;  
  SFRIE1  |= WDTIE                                   ; // ʹ��WDT�ж�
  WDTCTL   = WDT_ADLY_1000                           ; // ����WDTΪINTERVALģʽ���жϼ��100ms
  MAIN_POWER_ON                                      ;
  TBCTL    = TBSSEL_1 + MC_1 + TBCLR + TBIE          ; // ʱ��Դ   = ACLK
  TBCCR0   = 64                                      ; // ˢ��Ƶ�� = 32768/64/8=64Hz  
  _EINT()                                            ;
  TEMP=Do1Convert()                                  ; //��ȡ�¶�ת������
  __delay_cycles(10000)                              ;
  for(;;)
  {
    TEMP=Do1Convert()                                ; //��ȡ�¶�ת������
    temp = TEMP>>4                                   ;
    temp+= (float)(TEMP&0x000F)/16                   ;
    LED_Disp_Float(temp,1,NO_ZERO)                   ;
    LPM3                                             ;
    _NOP()                                           ;
  }
}

