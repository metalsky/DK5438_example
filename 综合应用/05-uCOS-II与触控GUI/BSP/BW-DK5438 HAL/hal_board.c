//===========================================================================//
//                                                                           //
// �ļ���  HAL_BOARD.C                                                       //
// ˵����  BW-DK5438������uC/OS-II��ʾ���������Դ��ʼ�������ļ�             //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// �汾��  v1.2u                                                             //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include "hal_BW-DK5438.h"

//***************************************************************************//
// ���ú��ĵ�ѹVCore����                                                     // 
//***************************************************************************//


void halBoardSetVCoreUp (unsigned char VCore)
{
  PMMCTL0_H = 0xA5;                         // Open PMM module registers for write access
  PMMCTL0 = 0xA500 + (VCore * PMMCOREV0);   // Set VCore to new VCore
  SVSMLCTL = (SVSMLCTL & ~(3 * SVSMLRRL0)) + SVMLE + (VCore * SVSMLRRL0);  // Set SVM new Level    
  while ((PMMIFG & SVSMLDLYIFG) == 0);      // Wait till SVM is settled (Delay)
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);        // Clear already set flags
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);     // Wait till level is reached
  SVSMLCTL &= ~SVMLE;                       // Disable Low side SVM
  PMMCTL0_H = 0x00;                         // Lock PMM module registers for write access
}


//***************************************************************************//
// ���ú��ĵ�ѹVCore����                                                     // 
//***************************************************************************//
void halBoardSetVCoreDown (unsigned char VCore)
{
  PMMCTL0_H = 0xA5;                         // Open PMM module registers for write access
  SVSMLCTL = (SVSMLCTL & ~(3 * SVSMLRRL0)) + SVMLE + (VCore * SVSMLRRL0);  // Set SVM new Level    
  while ((PMMIFG & SVSMLDLYIFG) == 0);      // Wait till SVM is settled (Delay)
  PMMCTL0 = 0xA500 + (VCore * PMMCOREV0);   // Set VCore to 1.85 V for Max Speed.
  SVSMLCTL &= ~SVMLE;                       // Disable Low side SVM
  PMMCTL0_H = 0x00;                         // Lock PMM module registers for write access
}

//***************************************************************************//
// �ر�SVSģ��                                                               // 
//***************************************************************************//
void halBoardDisableSVS()
{
  PMMCTL0_H = 0xA5                                ; // Open PMM module registers for write access
  SVSMLCTL &= ~( SVMLE + SVSLE + SVSLFP + SVMLFP ); // Disable Low side SVM
  SVSMHCTL &= ~( SVMHE + SVSHE + SVSHFP + SVMHFP ); // Disable High side SVM
  PMMCTL1 = (PMMREFMD + PMMREFACC)                ;
  PMMCTL0_H = 0x00                                ; // Lock PMM module registers for write access	
}

//***************************************************************************//
// ��SVSģ��                                                               // 
//***************************************************************************//
void halBoardEnableSVS()
{
  PMMCTL0_H = 0xA5;                         // Open PMM module registers for write access
  SVSMHCTL &= ~(SVSHFP+SVMHFP);             // Disable full-performance mode 
  SVSMLCTL &= ~(SVSLFP+SVMLFP);             // Disable full-performance mode
  SVSMLCTL |= ( SVMLE + SVSLE);             // Enable Low side SVM
  SVSMHCTL |= ( SVMHE + SVSHE);             // Enable High side SVM
  PMMCTL1 &= ~(PMMREFMD + PMMREFACC);
  PMMCTL0_H = 0x00;                         // Lock PMM module registers for write access  
}

//***************************************************************************//
// ���ú��ĵ�ѹ                                                              // 
//***************************************************************************//
void halBoardSetVCore (unsigned char VCore)
{
  unsigned int currentVCore;

  currentVCore = PMMCTL0 & PMMCOREV_3;      // Get actual VCore  
  while (VCore != currentVCore) 
  {   
    if (VCore > currentVCore) 
      halBoardSetVCoreUp(++currentVCore);
    else
      halBoardSetVCoreDown(--currentVCore);
  }
  
}

//***************************************************************************//
// ����ϵͳʱ����������                                                      // 
//***************************************************************************//
void halBoardGetSystemClockSettings(unsigned char systemClockSpeed, 
				    unsigned char *setDcoRange    ,
				    unsigned char *setVCore       ,																		
				    unsigned int *setMultiplier    )
{
	switch (systemClockSpeed)
	{
		case SYSCLK_1MHZ: 
	    *setDcoRange = DCORSEL_1MHZ;
	    *setVCore = VCORE_1MHZ;
	    *setMultiplier = DCO_MULT_1MHZ;
	    break;
		case SYSCLK_4MHZ: 
	    *setDcoRange = DCORSEL_4MHZ;
	    *setVCore = VCORE_4MHZ;
	    *setMultiplier = DCO_MULT_4MHZ;
	    break;
		case SYSCLK_8MHZ: 
	    *setDcoRange = DCORSEL_8MHZ;
	    *setVCore = VCORE_8MHZ;
	    *setMultiplier = DCO_MULT_8MHZ;
	    break;
		case SYSCLK_12MHZ: 
	    *setDcoRange = DCORSEL_12MHZ;
	    *setVCore = VCORE_12MHZ;
	    *setMultiplier = DCO_MULT_12MHZ;
	    break;
		case SYSCLK_16MHZ: 
	    *setDcoRange = DCORSEL_16MHZ;
	    *setVCore = VCORE_16MHZ;
	    *setMultiplier = DCO_MULT_16MHZ;
	    break;
		case SYSCLK_20MHZ: 
	    *setDcoRange = DCORSEL_20MHZ;
	    *setVCore = VCORE_20MHZ;
	    *setMultiplier = DCO_MULT_20MHZ;
	    break;
		case SYSCLK_25MHZ: 
	    *setDcoRange = DCORSEL_25MHZ;
	    *setVCore = VCORE_25MHZ;
	    *setMultiplier = DCO_MULT_25MHZ;
	    break;	     
	}	
}

//***************************************************************************//
// ��XT1��Ƶ������                                                       // 
//***************************************************************************//
void halBoardStartXT1()
{
	// Set up XT1 Pins to analog function, and to lowest drive	
  P7SEL |= 0x03;                            
  UCSCTL6 &= ~XT1DRIVE_3;
  UCSCTL6 |= XCAP_3 ;
  while ( (SFRIFG1 &OFIFG))
  {    
    UCSCTL7 &= ~(XT1LFOFFG + DCOFFG);
    SFRIFG1 &= ~OFIFG;
  }
}
//***************************************************************************//
// ����ϵͳʱ��(��Ƶ����+DCO��Ƶ)                                            // 
//***************************************************************************//
void halBoardSetSystemClock(unsigned char systemClockSpeed)
{
  unsigned char setDcoRange, setVCore;
  unsigned int setMultiplier;

  halBoardGetSystemClockSettings( systemClockSpeed, &setDcoRange, &setVCore, &setMultiplier);
  	
  if (setVCore > (PMMCTL0 & PMMCOREV_3))    // ����PMM��ѹΪ1.85v
    halBoardSetVCore( setVCore )         ;
  UCSCTL0 = 0x00                         ;  // Set lowest possible DCOx, MODx
  UCSCTL1 = setDcoRange;                    // Select suitable range
  
  UCSCTL2 = setMultiplier + FLLD__1;         // Set DCO Multiplier
  UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV  |  SELM__DCOCLKDIV ;
}

//***************************************************************************//
// ����ϵͳʱ��(��Ƶ����)                                                    // 
//***************************************************************************//
void halBoardSetSystemClockExt(unsigned char systemClockSpeed)
{
  unsigned char setDcoRange, setVCore;
  unsigned int setMultiplier;

  halBoardGetSystemClockSettings( systemClockSpeed, &setDcoRange, &setVCore, &setMultiplier);
  	
  if (setVCore > (PMMCTL0 & PMMCOREV_3))    // ����PMM��ѹΪ1.85v
    halBoardSetVCore( setVCore )         ;
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
// ���ϵͳʱ�������Զ˿�                                                    // 
//***************************************************************************//
void halBoardOutputSystemClock()
{
  P11SEL |= 0x07;                           // Output ACLK, MCLK, SMCLK on testpoints
  P11DIR |= 0x07;
  
}
//***************************************************************************//
// �ر�ϵͳʱ�����                                                          // 
//***************************************************************************//
void halBoardStopOutputSystemClock()
{  
  P11SEL &= ~0x07;                          // Stop outputing ACLK, MCLK, SMCLK on testpoints
  P11OUT &= ~0x07;
  P11DIR |= 0x07;
  
}

//***************************************************************************//
// �������ʼ��                                                              // 
//***************************************************************************//
void halBoardInit()
{  
  //Tie unused ports
  PAOUT  = 0;
  PADIR  = 0xFFFF;  
  PASEL  = 0;
  PBOUT  = 0;  
  PBDIR  = 0xFFFF;
  PBSEL  = 0;
  PCOUT  = 0;    
  PCDIR  = 0xFFFF;
  PCSEL  = 0;  
  PDOUT  = 0;  
  PDDIR  = 0xFFFF;
  PDSEL  = 0;  
  PEOUT  = 0;  
  PEDIR  = 0xFFFF;
  PESEL  = 0;  
  P11OUT = 0;
  P11DIR = 0xFF;
  PJOUT = 0;    
  PJDIR = 0xFF;
  P11SEL = 0;
}

