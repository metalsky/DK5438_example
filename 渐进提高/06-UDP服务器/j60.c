//===========================================================================//
//                                                                           //
// 文件：  J60.H                                                             //
// 说明：  BW-DK5438开发板ENC28J60驱动程序文件                               //
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
#include "reg.h"
#include "ne2000.h"

NetNode myNode;
netcard rxdnetbuf;
netcard txdnetbuf;

unsigned char Enc28j60Bank;
unsigned int NextPacketPtr;

void delay_ms(unsigned char ms)
{
  unsigned char i                    ;
  for(i=0;i<ms;i++)
    __delay_cycles(20000)            ;
}

unsigned char SPI_RW_Char(unsigned char data)
{
  unsigned char temp,i,cRec=0        ;
  temp = data                        ;
  for(i=0;i<8;i++)
  {
    if(P10IN&PMISO)
      cRec    |= (1<<(7-i))          ;        
    if(temp&0x80)
      P10OUT  |= PMOSI               ;
    else             
      P10OUT  &=~PMOSI               ;
    P10OUT    |= PSCK                ;
    P10OUT    &=~PSCK                ;
    temp     <<=  1                  ;
  }  
  return cRec                        ;
}

/*==========================================================
//	function:	write data to add
//	Parameter:	op			command code
//				add			parameter regsiter address
//				data		send data
//	return:		void
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void encWriteOp(unsigned char op,unsigned char add,unsigned char data)
{
  CLR_J60_CS                                  ;                         
//  __delay_cycles(10)                          ;
  SPI_RW_Char(op | (add & ADDR_MASK))         ;
//  __delay_cycles(80)                          ;
  SPI_RW_Char(data)                           ;
//  __delay_cycles(80)                          ;
  SET_J60_CS                                  ;                         
//  __delay_cycles(100)                         ;
}

/*==========================================================
//	function:	read data from add
//	Parameter:	op			command code
//				add			parameter regsiter address
//	return:		unsigned char	read data
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
unsigned char encReadOp(unsigned char op,unsigned char add)
{
  unsigned char data                          ;  
  CLR_J60_CS                                  ;                         
//  __delay_cycles(10)                          ;
  SPI_RW_Char(op | (add & ADDR_MASK))         ;
//  __delay_cycles(80)                          ;
  data=SPI_RW_Char(0x00)                      ;
//  __delay_cycles(80)                          ;
  if(add & 0x80)
  {
    data = SPI_RW_Char(0x00)                  ;
//    __delay_cycles(80)                        ;
  }
  SET_J60_CS                                  ;                         
//  __delay_cycles(100)                         ;
  return data                                 ;
}

/*==========================================================
//	function:	read data buffer form enc28j60
//	Parameter:	len			read size
//				*data		data buffer pointer
//	return:		void
//	date:		2006/11/26
//	note:		none
//	write:		han
==========================================================*/
void encReadBuffer(unsigned int len,unsigned char *data)
{
  CLR_J60_CS                                  ;                         
//  __delay_cycles(10)                          ;
  SPI_RW_Char(ENC28J60_READ_BUF_MEM)          ;
//  __delay_cycles(80)                          ;
  while(len--)
  {
    *data++   = SPI_RW_Char(0x00)             ;
//    __delay_cycles(80)                        ;
  }		
  SET_J60_CS                                  ;                         
//  __delay_cycles(100)                         ;
}

/*==========================================================
//	function:	write data to enc28j60
//	Parameter:	len			write size
//				*data		data buffer pointer
//	return:		void
//	date:		2006/11/26
//	note:		none
//	write:		han
==========================================================*/
void encWriteBuffer(unsigned int len,unsigned char *data)
{
  CLR_J60_CS                                  ;                         
//  __delay_cycles(10)                          ;
  SPI_RW_Char(ENC28J60_WRITE_BUF_MEM)         ;
//  __delay_cycles(80)                          ;
  while(len--)
  {
    SPI_RW_Char(*data++)                      ;
//    __delay_cycles(80)                        ;
  }
  SET_J60_CS                                  ;                         
//  __delay_cycles(100)                         ;
}

/*==========================================================
//	function:	set enc28j60 register bank
//	Parameter:	add			register address
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 17
//	write:		han
==========================================================*/
void encSetBank(unsigned char add)
{
  if((add & BANK_MASK) != Enc28j60Bank)
  {
    encWriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
    encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, (add & BANK_MASK)>>5)     ;
    Enc28j60Bank = (add & BANK_MASK)                                    ;
  }
}

/*==========================================================
//	function:	read regsiter
//	Parameter:	add			register address
//	return:		unsigned char	return data
//	date:		2006/11/27
//	note:		datasheet page 28
//	write:		han
==========================================================*/
unsigned char encRead(unsigned char add)
{
  encSetBank(add)                              ;
  return encReadOp(ENC28J60_READ_CTRL_REG, add);
}

/*==========================================================
//	function:	write regsiter
//	Parameter:	add			register address
//				data		data to write
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void encWrite(unsigned char add,unsigned char data)
{
  encSetBank(add)                               ; // set the bank	
  encWriteOp(ENC28J60_WRITE_CTRL_REG, add, data); // do the write
}

/*==========================================================
//	function:	set MAC address
//	Parameter:	void		data in myNode
//	return:		void
//	date:		2006/11/26
//	note:		datasheet page 28
//	write:		han
==========================================================*/
void nicSetMacAddress(void)
{
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	encWrite(MAADR0, myNode.node.mac[5]);
	encWrite(MAADR1, myNode.node.mac[4]);
	encWrite(MAADR2, myNode.node.mac[3]);
	encWrite(MAADR3, myNode.node.mac[2]);
	encWrite(MAADR4, myNode.node.mac[1]);
	encWrite(MAADR5, myNode.node.mac[0]);
}

/*==========================================================
//	function:	write PHY regsiter
//	Parameter:	add			register address
//				data		data to write
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 21
//	write:		han
==========================================================*/
void encPhyWrite(unsigned char add, unsigned int data)
{
	// set the PHY register address
	encWrite(MIREGADR, add);
	
	// write the PHY data
	encWrite(MIWRL, data);	
	encWrite(MIWRH, data>>8);

	// wait until the PHY write completes
	while(encRead(MISTAT) & MISTAT_BUSY);
}

/*==========================================================
//	function:	chip init
//	Parameter:	void
//	return:		void
//	date:		2006/11/27
//	note:		datasheet page 35
//	write:		han
==========================================================*/
void encInit(void)
{
  encWriteOp(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET)    ;
  delay_ms(10)                                               ;
  while(!(encRead(ESTAT) & ESTAT_CLKRDY))                    ;
  NextPacketPtr = RXSTART_INIT                               ;
  encWrite(ERXSTL, RXSTART_INIT&0xFF)                        ;
  encWrite(ERXSTH, RXSTART_INIT>>8)                          ;
  encWrite(ERXRDPTL, RXSTART_INIT&0xFF)                      ;
  encWrite(ERXRDPTH, RXSTART_INIT>>8)                        ;
  encWrite(ERXNDL, RXSTOP_INIT&0xFF)                         ;
  encWrite(ERXNDH, RXSTOP_INIT>>8)                           ;
  encWrite(ETXSTL, TXSTART_INIT&0xFF)                        ;
  encWrite(ETXSTH, TXSTART_INIT>>8)                          ;
  encWrite(MACON1, MACON1_MARXEN|MACON1_TXPAUS|MACON1_RXPAUS);
  encWrite(MACON2, 0x00)                                     ;
  encWriteOp(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0|MACON3_TXCRCEN|MACON3_FRMLNEN);

	// set inter-frame gap (non-back-to-back)
  encWrite(MAIPGL, 0x12);
  encWrite(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
  encWrite(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
  encWrite(MAMXFLL, MAX_FRAMELEN&0xFF);	
  encWrite(MAMXFLH, MAX_FRAMELEN>>8);

	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
  nicSetMacAddress();

	// no loopback of transmitted frames
	//modified by zhang
	//encPhyWrite(PHCON2, PHCON2_HDLDIS);

	// switch to bank 0
  encSetBank(ECON1);
	// enable interrutps
  encWriteOp(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE|EIE_PKTIE);
	// enable packet reception
  encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);

}

void encPacketSend(ethernet_address_type *pDestAddr, netcard *txdnet, 
                  unsigned int buffer_length, unsigned int packet_type)
{
	unsigned char i;
	for(i = 0; i < 6; i++)
	{
    	txdnet->etherframe.sourcenodeid[i] = myNode.node.mac[i];
   	}
	// Step 3b: Load the destination address
    for(i = 0; i < 6; i++)
    {
    	txdnet->etherframe.destnodeid[i] = pDestAddr->bytes[i];
    }
    txdnet->etherframe.protocal = packet_type;

	// Set the write pointer to start of transmit buffer area
	encWrite(EWRPTL, TXSTART_INIT);
	encWrite(EWRPTH, TXSTART_INIT>>8);
	// Set the TXND pointer to correspond to the packet size given
	encWrite(ETXNDL, (TXSTART_INIT+buffer_length));
	encWrite(ETXNDH, (TXSTART_INIT+buffer_length)>>8);

	// write per-packet control byte
	encWriteOp(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

	// copy the packet into the transmit buffer
	encWriteBuffer(buffer_length, txdnet->bytedata.bytebuf+4);
	
	// send the contents of the transmit buffer onto the network
	encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
}

unsigned int encPacketReceive(netcard *rxdnet)
{
	unsigned int rxstat;
	unsigned int len;
//        unsigned char result;
//        result = encRead(EIR);
	// check if a packet has been received and buffered
	if( !(encRead(EIR) & EIR_PKTIF) )
		return 0;
	
	// Make absolutely certain that any previous packet was discarded	
	//if( WasDiscarded == FALSE)
	//	MACDiscardRx();

	// Set the read pointer to the start of the received packet
	encWrite(ERDPTL, (NextPacketPtr));
	encWrite(ERDPTH, (NextPacketPtr)>>8);
	// read the next packet pointer
	NextPacketPtr  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	NextPacketPtr |=(encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8);
	// read the packet length
	len  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	len |=(encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8);
	// read the receive status
	rxstat  = encReadOp(ENC28J60_READ_BUF_MEM, 0);
	rxstat |=(encReadOp(ENC28J60_READ_BUF_MEM, 0)<<8);

	// limit retrieve length
	// (we reduce the MAC-reported length by 4 to remove the CRC)
	if(len > 336)
		len = 336;

	// copy the packet from the receive buffer
	encReadBuffer(len, rxdnet->bytedata.bytebuf+4);

	// Move the RX read pointer to the start of the next received packet
	// This frees the memory we just read out
	encWrite(ERXRDPTL, (NextPacketPtr));
	encWrite(ERXRDPTH, (NextPacketPtr)>>8);

	// decrement the packet counter indicate we are done with this packet
	encWriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

	return len;
}


void ENC_SLEEP(void)
{
  encWriteOp(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXEN) ;
  encWriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_VRPS) ;
  encWriteOp(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PWRSV);  
}

void ENC_WAKEUP(void)
{
  encWriteOp(ENC28J60_BIT_FIELD_CLR, ECON2, ECON2_PWRSV);
  delay_ms(1)                                           ;  
  encWriteOp(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN) ;  
}