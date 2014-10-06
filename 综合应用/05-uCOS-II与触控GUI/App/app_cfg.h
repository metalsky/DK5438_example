//===========================================================================//
//                                                                           //
// 文件：  APP_CFG.H                                                         //
// 说明：  BW-DK5438开发板uC/OS-II演示程序应用配置头文件                     //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//


#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#define  APP_START_TASK_PRIO                   9
#define  SPI_MUTEX_PRIO                       10
#define  LED_TASK_PRIO                        11
#define  GUI_TASK_PRIO                        12
#define  MENU_OP_TASK_PRIO                    13
#define  KEY_TP_TASK_PRIO                     14
#define  CPU_USAGE_TASK_PRIO                  15

#define  APP_START_TASK_STK_SIZE             128
#define  LED_TASK_STACK_SIZE                  80
#define  GUI_TASK_STACK_SIZE                 192
#define  MENU_OP_TASK_STACK_SIZE             512
#define  KEY_TP_TASK_STACK_SIZE              128
#define  CPU_USAGE_TASK_STACK_SIZE            80
#endif
