//===========================================================================//
//                                                                           //
// �ļ���  MAIN.C                                                            //
// ˵����  BW-DK5438��������̫��ʵ�����                                     //
//         ͨ��ENC28J60ʵ��webserver                                         //
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
#include "J60.H"
#include "ARP.H"
#include "ICMP.H"
#include "REG.H"

#define FLL_FACTOR   649

extern  NetNode myNode                          ;
extern  netcard rxdnetbuf                       ;
extern  netcard txdnetbuf                       ;

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
  P5OUT    |= J60_CS                       ;
  P5DIR    |= J60_CS                       ;
  P10DIR   &=~PMISO                        ;
  P10REN   |= PMISO                        ;
  P10DIR   |= PMOSI+PSCK                   ;
  P10OUT   &= PSCK                         ;
  P3OUT    |= TP_CS                        ; // �رմ�����
  P3DIR    |= TP_CS                        ;
}


int main(void)
{
  unsigned int num_bytes                             ;
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ;
  Init_PSPI()                                        ;
 
  P9DIR   |= IrDA_OUT;
  P9OUT   &=~IrDA_OUT;
  
  MAIN_POWER_ON                                      ;
/*  
  for(;;){
  for(i=0;i<40;i++)
  {
  P9OUT   |= IrDA_OUT;
  __delay_cycles(20000);
  P9OUT   &=~IrDA_OUT;
  __delay_cycles(20000);  
  
  }
  __delay_cycles(5000000);
  }
  
*/  
  
/*  
  for(;;){
  for(j=0;j<40;j++){
  for(i=0;i<50;i++)
  {
  P9OUT   &=~IrDA_OUT;
  __delay_cycles(270);  
  P9OUT   |= IrDA_OUT;
  __delay_cycles(270);
  }
  __delay_cycles(20000);
  }
  __delay_cycles(5000000);
  }
*/  
  myNode.node.ip     = IP4_ADDR(103,1,168,192)       ;
  myNode.node.mask   = IP4_ADDR(0,255,255,255)       ;
  myNode.node.gate   = IP4_ADDR(1,1,168,192)         ;
  myNode.node.mac[0] = 0x00                          ;
  myNode.node.mac[1] = 0x60                          ;
  myNode.node.mac[2] = 0x97                          ;
  myNode.node.mac[3] = 0x48                          ;
  myNode.node.mac[4] = 0xF4                          ;
  myNode.node.mac[5] = 0xAB                          ;
  encInit()                                          ;
  nicSetMacAddress()                                 ;
  encPhyWrite(0x14,0x0470)                           ;
  arptab_init()                                      ;
  ENC_SLEEP() ;// TEST
  ENC_WAKEUP();// TEST
  
  while(1)
  {
    num_bytes = encPacketReceive(&rxdnetbuf);
    if(num_bytes > 0)
    {
      if(rxdnetbuf.etherframe.protocal==ARP_PACKET)    // �յ�һ��AR�����
      {			
	if(rxdnetbuf.arpframe.operation==0x0100)
	  arp_answer()                               ; // ARP request,����arp���ݰ�
	else if(rxdnetbuf.arpframe.operation==0x0200)
	  arp_process()                              ; // ARP answer
      }
      else if(   (rxdnetbuf.etherframe.protocal == IP_PACKET)	// �յ�һ��IP��
	     && ((rxdnetbuf.ipframe.verandihl&0xf0) == 0x40)	// IP V4
             && (rxdnetbuf.ipframe.destip[0] == myNode.nodebytes.ipbytes[0])
	     && (rxdnetbuf.ipframe.destip[1] == myNode.nodebytes.ipbytes[1])
	     && (rxdnetbuf.ipframe.destip[2] == myNode.nodebytes.ipbytes[2])
             && (rxdnetbuf.ipframe.destip[3] == myNode.nodebytes.ipbytes[3]))	
      {
        arp_ip_mac()                                 ;
	if(rxdnetbuf.ipframe.protocal == 1)		//	ICMP
	{
          if(rxdnetbuf.icmpframe.type == 8)		//	ECHO
            ping_answer()                            ;
        }
        else if(rxdnetbuf.ipframe.protocal == 17)	//	UDP
        {
//          net_udp_rcv()                              ;
          _NOP();
//          Clear_UPD_Buf()                            ;
        }
/*        
	else if(rxdnetbuf.ipframe.protocal == 6)	//	TCP
          tcp_rcve(&rxdnetbuf)                       ;
        updatearptab()                               ;
*/        
      }
    }
  }
}


