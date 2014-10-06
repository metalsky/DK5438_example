//===========================================================================//
//                                                                           //
// 文件：  J60.H                                                             //
// 说明：  BW-DK5438开发板ENC28J60驱动程序头文件                             //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.25                                                        //
//                                                                           //
//===========================================================================//

#include "ne2000.h"
unsigned char encRead(unsigned char add);
void nicSetMacAddress(void);
void encInit(void);
void encPhyWrite(unsigned char add, unsigned int data);
void encPacketSend(ethernet_address_type *pDestAddr, netcard *txdnet, unsigned int buffer_length, unsigned int packet_type);
unsigned int encPacketReceive(netcard *rxdnet);
void ENC_SLEEP(void)                          ;
void ENC_WAKEUP(void)                         ;
void UDP_answer(void)                         ;

