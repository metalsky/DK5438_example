//===========================================================================//
//                                                                           //
// 文件：  HAL_BOARD.C                                                       //
// 说明：  BW-DK5438开发板uC/OS-II演示程序板载资源初始化程序头文件           //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#ifndef HAL_BOARD_H
#define HAL_BOARD_H

#define CLK_PORT_DIR      P11DIR
#define CLK_PORT_OUT      P11OUT
#define CLK_PORT_SEL      P11SEL

#define ACLK_PIN          BIT0
#define MCLK_PIN          BIT1
#define SMCLK_PIN         BIT2

#define XT1_XTAL_DIR      P7DIR
#define XT1_XTAL_SEL      P7SEL
#define XT1_XTAL_OUT      P7OUT



#define SYSCLK_1MHZ               0
#define SYSCLK_4MHZ               1
#define SYSCLK_8MHZ               2
#define SYSCLK_12MHZ              3
#define SYSCLK_16MHZ              4
#define SYSCLK_20MHZ              5
#define SYSCLK_25MHZ              6

#define DCO_MULT_1MHZ            30
#define DCO_MULT_4MHZ           122
#define DCO_MULT_8MHZ           244
#define DCO_MULT_12MHZ          366
#define DCO_MULT_16MHZ          488
#define DCO_MULT_20MHZ          610
//#define DCO_MULT_25MHZ          763 // 32768×763=25.001984MHz
#define DCO_MULT_25MHZ          999 // 32768×999=32.735232MHz, 测试用

#define DCORSEL_1MHZ            DCORSEL_2
#define DCORSEL_4MHZ            DCORSEL_3
#define DCORSEL_8MHZ            DCORSEL_4
#define DCORSEL_12MHZ           DCORSEL_5
#define DCORSEL_16MHZ           DCORSEL_5
#define DCORSEL_20MHZ           DCORSEL_6
#define DCORSEL_25MHZ           DCORSEL_7

#define VCORE_1MHZ              PMMCOREV_0
#define VCORE_4MHZ              PMMCOREV_0
#define VCORE_8MHZ              PMMCOREV_0
#define VCORE_12MHZ             PMMCOREV_0
#define VCORE_16MHZ             PMMCOREV_1
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

#define VCORE_1_35V             PMMCOREV_0
#define VCORE_1_55V             PMMCOREV_1
#define VCORE_1_75V             PMMCOREV_2
#define VCORE_1_85V             PMMCOREV_3

void halBoardInit();

void halBoardStartXT1();

void halBoardSetVCore(unsigned char VCore);

void halBoardSetSystemClock(unsigned char systemClockSpeed)   ;
void halBoardSetSystemClockExt(unsigned char systemClockSpeed);

void halBoardDisableSVS();
void halBoardEnableSVS();
void halBoardOutputSystemClock();
void halBoardStopOutputSystemClock();
#endif
