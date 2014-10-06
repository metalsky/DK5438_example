//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板以太网实验程序                                     //
//         通过ENC28J60实现webserver                                         //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.25                                                        //
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
//                 初始化主时钟: MCLK = XT1×(FLL_FACTOR+1)                  //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  P7SEL     |= 0x03                                  ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                              ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                            ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                             ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                  ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (649 + 1) * 32768 = 21.2992MHz
  __bic_SR_register(SCG0)                            ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                            ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)  ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &=~OFIFG                                 ; 
  }while(SFRIFG1&OFIFG)                              ; // 检测振荡器错误标志 
}

void Init_PSPI(void)
{  
  P5OUT    |= J60_CS                       ;
  P5DIR    |= J60_CS                       ;
  P10DIR   &=~PMISO                        ;
  P10REN   |= PMISO                        ;
  P10DIR   |= PMOSI+PSCK                   ;
  P10OUT   &= PSCK                         ;
  P3OUT    |= TP_CS                        ; // 关闭触摸屏
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
      if(rxdnetbuf.etherframe.protocal==ARP_PACKET)    // 收到一个AR请求包
      {			
	if(rxdnetbuf.arpframe.operation==0x0100)
	  arp_answer()                               ; // ARP request,处理arp数据包
	else if(rxdnetbuf.arpframe.operation==0x0200)
	  arp_process()                              ; // ARP answer
      }
      else if(   (rxdnetbuf.etherframe.protocal == IP_PACKET)	// 收到一个IP包
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


