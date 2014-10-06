//===========================================================================//
//                                                                           //
// �ļ���  J60.H                                                             //
// ˵����  BW-DK5438������ENC28J60��������ͷ�ļ�                             //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.25                                                        //
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

