//===========================================================================//
//                                                                           //
// 文件：  HAL_BOARD.C                                                       //
// 说明：  BW-DK5438开发板uC/OS-II演示程序板载资源初始化程序文件             //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
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
// 设置核心电压VCore升高                                                     // 
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
// 设置核心电压VCore降低                                                     // 
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
// 关闭SVS模块                                                               // 
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
// 打开SVS模块                                                               // 
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
// 设置核心电压                                                              // 
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
// 生成系统时钟配置数据                                                      // 
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
// 打开XT1低频外振荡器                                                       // 
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
// 设置系统时钟(低频晶振+DCO倍频)                                            // 
//***************************************************************************//
void halBoardSetSystemClock(unsigned char systemClockSpeed)
{
  unsigned char setDcoRange, setVCore;
  unsigned int setMultiplier;

  halBoardGetSystemClockSettings( systemClockSpeed, &setDcoRange, &setVCore, &setMultiplier);
  	
  if (setVCore > (PMMCTL0 & PMMCOREV_3))    // 设置PMM电压为1.85v
    halBoardSetVCore( setVCore )         ;
  UCSCTL0 = 0x00                         ;  // Set lowest possible DCOx, MODx
  UCSCTL1 = setDcoRange;                    // Select suitable range
  
  UCSCTL2 = setMultiplier + FLLD__1;         // Set DCO Multiplier
  UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV  |  SELM__DCOCLKDIV ;
}

//***************************************************************************//
// 设置系统时钟(高频晶振)                                                    // 
//***************************************************************************//
void halBoardSetSystemClockExt(unsigned char systemClockSpeed)
{
  unsigned char setDcoRange, setVCore;
  unsigned int setMultiplier;

  halBoardGetSystemClockSettings( systemClockSpeed, &setDcoRange, &setVCore, &setMultiplier);
  	
  if (setVCore > (PMMCTL0 & PMMCOREV_3))    // 设置PMM电压为1.85v
    halBoardSetVCore( setVCore )         ;
  P5SEL     |= 0x0C                                       ; // 端口功能选择振荡器
  UCSCTL6   &= ~XT2OFF                                    ; // 振荡器使能
  UCSCTL3   |= SELREF_2                                   ; // FLLref = REFO
  UCSCTL4   |= SELA_2                                     ; // ACLK=REFO,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG); // 清除 XT2,XT1,DCO 错误标志                                                          
    SFRIFG1 &= ~OFIFG                                     ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志
  UCSCTL6   |= XT2DRIVE0 + XT2DRIVE1                      ; // XT2 驱动模式 24~32MHz                                            
  UCSCTL4   |= SELS_5 + SELM_5                            ; // SMCLK = MCLK = XT2
}

//***************************************************************************//
// 输出系统时钟至测试端口                                                    // 
//***************************************************************************//
void halBoardOutputSystemClock()
{
  P11SEL |= 0x07;                           // Output ACLK, MCLK, SMCLK on testpoints
  P11DIR |= 0x07;
  
}
//***************************************************************************//
// 关闭系统时钟输出                                                          // 
//***************************************************************************//
void halBoardStopOutputSystemClock()
{  
  P11SEL &= ~0x07;                          // Stop outputing ACLK, MCLK, SMCLK on testpoints
  P11OUT &= ~0x07;
  P11DIR |= 0x07;
  
}

//***************************************************************************//
// 开发板初始化                                                              // 
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

