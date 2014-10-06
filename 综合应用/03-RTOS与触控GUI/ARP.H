//===========================================================================//
//                                                                           //
// 文件：  ARP.C                                                             //
// 说明：  BW-DK5438开发板ARP协议程序头文件                                  //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.25                                                        //
//                                                                           //
//===========================================================================//

void arptab_init(void);
void arp_request(ip_address_type *ip_address);
void arp_answer(void);
void arp_process(void);
void updatearptab(void);
void arp_ip_mac(void);

