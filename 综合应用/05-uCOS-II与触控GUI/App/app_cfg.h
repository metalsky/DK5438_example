//===========================================================================//
//                                                                           //
// �ļ���  APP_CFG.H                                                         //
// ˵����  BW-DK5438������uC/OS-II��ʾ����Ӧ������ͷ�ļ�                     //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// �汾��  v1.2u                                                             //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2011.09.19                                                        //
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
