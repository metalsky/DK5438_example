//===========================================================================//
//                                                                           //
// �ļ���  IP_ARP_UDP.H                                                      //
// ˵����  BW-DK5438��������̫��IP_ARP_UDPЭ�����                           //
//         ��Microchip ENC28J60 Ethernet Interface Driver ATMEGA8����ֲ      //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.25                                                        //
//                                                                           //
//===========================================================================//
//@{
#ifndef IP_ARP_UDP_H
#define IP_ARP_UDP_H
// you must call this function once before you use any of the other functions:
extern void init_ip_arp_udp(uint8_t *mymac,uint8_t *myip);
//
extern uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf,uint8_t len);
extern uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint8_t len);
extern void make_arp_answer_from_request(uint8_t *buf,uint8_t len);
extern void make_echo_reply_from_request(uint8_t *buf,uint8_t len);
extern void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port);


#endif /* IP_ARP_UDP_H */
//@}
