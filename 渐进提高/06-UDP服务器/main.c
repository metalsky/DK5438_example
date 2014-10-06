//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438��������̫��UDPͨѶʾ������                              //
//         ͨ��ENC28J60��չ��̫���ӿڣ���UDPЭ�����/��������                //
//         PC��ͨ����̫�������������UDPЭ���򿪷��巢������                 //
//         ����������յ��ַ�'w'ʱ��Ӧ��"This is BW-DK5438"                  //
//         ����֧��pingӦ���û�����PC��COMMAND��ʽ�·���                   //
//         ��ping 192.163.1.103��,����˹���                                 //
// ע�⣺  PC�����Ҫ��������/�ͻ�������˿ڶ�����Ϊ8001                     //
//         ������IP������Ϊ192.163.1.103�����г�ͻ�����޸�                   //
// ���룺  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.25                                                        //
//                                                                           //
//===========================================================================//

#include "msp430x54x.h"
#include <stdlib.h>
#include <stdio.h>
#include "PIN_DEF.H"
#include "enc28j60.h"
#include "ip_arp_udp.h"
#include "timeout.h"
#include "net.h"

#define FLL_FACTOR   649
#define BUFFER_SIZE 250

static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4]  = {192,168,1,103}                ; // ������IP
static uint16_t myport  = 8001                           ; 
static uint8_t buf[BUFFER_SIZE+1]                        ;


//***************************************************************************//
//                                                                           //
//                 ��ʼ����ʱ��: MCLK = XT1��(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                  ; // �˿�ѡ���ⲿ��Ƶ����XT1
  UCSCTL6   &=~XT1OFF                                ; // ʹ���ⲿ���� 
  UCSCTL6   |= XCAP_3                                ; // �����ڲ����ص���
  UCSCTL3   |= SELREF_2                              ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                            ; // �ر�FLL���ƻ�·
  UCSCTL0    = 0x0000                                ; // ����DCOx, MODx
  UCSCTL1    = DCORSEL_7                             ; // ����DCO�񵴷�Χ
  UCSCTL2    = FLLD__1 + FLL_FACTOR                  ; // Fdco = ( FLL_FACTOR + 1)��FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                            ; // ��FLL���ƻ�·                                                            
  __delay_cycles(1024000)                            ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)  ; // ��� XT2,XT1,DCO �����־                                                            
    SFRIFG1 &=~OFIFG                                 ; 
  }while(SFRIFG1&OFIFG)                              ; // ������������־ 
}

void Init_PSPI(void)
{  
  P3OUT    |=  TP_CS                                 ; // �رմ�����
  P3DIR    |=  TP_CS                                 ;
  P5OUT    |=  J60_CS                                ;
  P5DIR    |=  J60_CS                                ;
  P10DIR   &= ~PMISO                                 ;
  P10REN   |=  PMISO                                 ;
  P10DIR   |=  PMOSI+PSCK                            ;
  P10OUT   &=~(PMOSI+PSCK)                           ;
}


int main(void)
{
  uint16_t plen                                      ;
//  uint8_t payloadlen=0                               ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ;
  Init_PSPI()                                        ; 
  P9DIR   |= IrDA_OUT                                ;
  P9OUT   &=~IrDA_OUT                                ;  
  MAIN_POWER_ON                                      ;
  __delay_cycles(5000000)                            ;                
  enc28j60Init(mymac)                                ;
  delay_ms(20)                                       ;
        
        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
  enc28j60PhyWrite(PHLCON,0x476);
  delay_ms(20);
  init_ip_arp_udp(mymac,myip);//init the ethernet/ip layer:
        while(1){
                // get the next new packet:
                plen = enc28j60PacketReceive(BUFFER_SIZE, buf);

                /*plen will ne unequal to zero if there is a valid 
                 * packet (without crc error) */
                if(plen==0){
                        continue;
                }
                // arp is broadcast if unknown but a host may also
                // verify the mac address by sending it to 
                // a unicast address.
                if(eth_type_is_arp_and_my_ip(buf,plen)){
                        make_arp_answer_from_request(buf,plen);
                        continue;
                }
                // check if ip packets (icmp or udp) are for us:
                if(eth_type_is_ip_and_my_ip(buf,plen)==0){
                        continue;
                }                
                        
                if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V){
                        // a ping packet, let's send pong
                        make_echo_reply_from_request(buf,plen);
                        continue;
                }
         //       if (buf[IP_PROTO_P]==IP_PROTO_UDP_V){
                if (buf[23]==IP_PROTO_UDP_V)
                {
//                  payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
                  if (buf[UDP_DATA_P]=='w')
                        {
                          make_udp_reply_from_request(buf,"This is BW-DK5438",17,myport);
                        }
                }
        }
}
