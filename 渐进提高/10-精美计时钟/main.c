//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438������ָ��ʽ����ʱ�ӳ���                                 //
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
#include "RTC.H"
#include "CLOCK.H"

#define  FLL_FACTOR     649                                          // FLL_FACTOR: DCO��Ƶϵ��   
//--------------------------- LCD ��ɫ ----------------------//
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Grey2          0xF79E
#define Dark_Grey      0x6B4D
#define Dark_Grey2     0x52AA
#define Light_Grey     0xE71C
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned Color,Color_BK                                            ;

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
//                 ��ʼ��RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // ʱ��ģʽ,ʮ�����Ƹ�ʽ
  SetRTCYEAR(2010)                                   ; 
  SetRTCMON(10)                                      ;
  SetRTCDAY(24)                                      ;
  SetRTCDOW(0)                                       ;
  SetRTCHOUR(20)                                     ;
  SetRTCMIN(24)                                      ;
  SetRTCSEC(35)                                      ;
}


//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Show_Clock(char *NC)                                       //
//����������ʱ�ӳ���                                                         //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
void Show_Clock(void)
{
  unsigned int Hour ,Minute,Second                                 ;
  Hour    = GetRTCHOUR()                                           ;
  while(Hour>=12)
  {
    Hour -= 12                                                     ;
  }
  Minute  = GetRTCMIN()                                            ;
  Second  = GetRTCSEC()                                            ;
  Ini_Clock_Contex()                                               ;
  ShowBMP240320(BMP_CLOCK)                                         ;
  Gen_Hour_Hand(Hour*30+Minute/2)                                  ;
  Save_Hour_Hand_BK()                                              ; 
  Draw_Hour_Hand(White)                                            ;
  Gen_Minu_Hand(Minute*6+Second/10)                                ;
  Save_Minu_Hand_BK()                                              ; 
  Draw_Minu_Hand(White)                                            ;
  Gen_Second_Hand(Second*6)                                        ;
  Save_Second_Hand_BK()                                            ; 
  Draw_Second_Hand(White)                                          ;
  for(;;)
  {
    if(Second==GetRTCSEC())     continue                           ;
    Second   = GetRTCSEC()                                         ;
    Minute  = GetRTCMIN()                                          ;
    Hour    = GetRTCHOUR()                                         ;
    while(Hour>=12)
    {
      Hour -= 12                                                   ;
    }
    if(Minute%6==0&&Second==0)                                      // ʱ��ת��
    {
      Restore_Second_Hand_BK()                                     ;
      Restore_Minu_Hand_BK()                                       ;
      Restore_Hour_Hand_BK()                                       ;
      Gen_Hour_Hand(Hour*30+Minute/2)                              ;
      if(Second%10==0)
        Gen_Minu_Hand(Minute*6+Second/10)                          ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Hour_Hand_BK()                                          ; 
      Draw_Hour_Hand(White)                                        ;    
      Save_Minu_Hand_BK()                                          ; 
      Draw_Minu_Hand(White)                                        ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;
    }
    else if(Second%10==0)                                           // ����ת��
    {
      Restore_Second_Hand_BK()                                     ;
      Restore_Minu_Hand_BK()                                       ;      
      Gen_Minu_Hand(Minute*6+Second/10)                            ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Minu_Hand_BK()                                          ; 
      Draw_Minu_Hand(White)                                        ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;
    }
    else
    {
      Restore_Second_Hand_BK()                                     ;
      Gen_Second_Hand(Second*6)                                    ;
      Save_Second_Hand_BK()                                        ; 
      Draw_Second_Hand(White)                                      ;      
    }
    Delay(100)                                                     ;
  }
}


void main( void )
{
  WDTCTL = WDTPW + WDTHOLD                                         ;
  Init_CLK()                                                       ;
  Init_RTC()                                                       ;
  Init_Port()                                                      ;
  LCD_Init()                                                       ;
  Show_Clock()                                                     ;
  for(;;) _NOP();
}
