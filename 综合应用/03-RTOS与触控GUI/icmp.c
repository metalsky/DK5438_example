//===========================================================================//
//                                                                           //
// 文件：  ICMP.C                                                            //
// 说明：  BW-DK5438开发板ICMP协议程序                                       //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.25                                                        //
//                                                                           //
//===========================================================================//

#include	"j60.h"

extern  netcard rxdnetbuf;
extern  netcard txdnetbuf;
extern  NetNode myNode;

extern unsigned int chksum(unsigned char *check,unsigned int size);
extern unsigned int Swap16U(unsigned int data16u);

/*==============================================*/
//	函数名称:	PING应答
/*==============================================*/
void ping_answer(void)
{
	unsigned int	i;
	unsigned int	size;
	unsigned int	tosize;

	ethernet_address_type answeraddress;
	
//	size = rxdnetbuf.etherframe.length;
//	if(size == 60)
	{
		tosize = Swap16U(rxdnetbuf.ipframe.totallength) - 28;	//	real icmp data
		size = tosize + 42;
	}

	if(rxdnetbuf.icmpframe.type == 0x08)
	{				//	表示是ping请求
	    	for (i = 16; i < (size+4); i++)	//	ADD form protocal
	    	{			//	将数据复制到发送缓冲区	   
	 	    	txdnetbuf.bytedata.bytebuf[i] = rxdnetbuf.bytedata.bytebuf[i];
	    	}
	 	for(i = 0; i < 6; i++)
	 	{			//	源地址为目标地址
	    		answeraddress.bytes[i] = rxdnetbuf.etherframe.sourcenodeid[i];
	 	}
		txdnetbuf.ipframe.ttl = txdnetbuf.ipframe.ttl - 1;
	    	txdnetbuf.ipframe.crc = 0;
		for(i = 0; i < 4; i++)
		{
			txdnetbuf.ipframe.destip[i] = rxdnetbuf.ipframe.sourceip[i];
			txdnetbuf.ipframe.sourceip[i] = myNode.nodebytes.ipbytes[i];
		}
		txdnetbuf.ipframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 18),20);  
	    txdnetbuf.icmpframe.type = 0x00;		// is icmp answer;
		txdnetbuf.icmpframe.crc = 0;
		txdnetbuf.icmpframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 38),(size -34)); 
	    encPacketSend(&answeraddress,&txdnetbuf,size,IP_PACKET);		
	}  
}


/*==============================================*/
//	函数名称:	PING应答
/*==============================================*/
void UDP_answer(void)
{
	unsigned int	i;
	unsigned int	size;
	unsigned int	tosize;

	ethernet_address_type answeraddress;
	
//	size = rxdnetbuf.etherframe.length;
//	if(size == 60)
	{
		tosize = Swap16U(rxdnetbuf.ipframe.totallength) - 28;	//	real udp data
		size = tosize + 42;
	}

	{				//	表示是ping请求
	    	for (i = 16; i < (size+4); i++)	//	ADD form protocal
	    	{			//	将数据复制到发送缓冲区	   
	 	    	txdnetbuf.bytedata.bytebuf[i] = rxdnetbuf.bytedata.bytebuf[i];
	    	}
	 	for(i = 0; i < 6; i++)
	 	{			//	源地址为目标地址
	    		answeraddress.bytes[i] = rxdnetbuf.etherframe.sourcenodeid[i];
	 	}
		txdnetbuf.ipframe.ttl = txdnetbuf.ipframe.ttl - 1;
	    	txdnetbuf.ipframe.crc = 0;
		for(i = 0; i < 4; i++)
		{
			txdnetbuf.ipframe.destip[i] = rxdnetbuf.ipframe.sourceip[i];
			txdnetbuf.ipframe.sourceip[i] = myNode.nodebytes.ipbytes[i];
		}
		txdnetbuf.ipframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 18),20);  
//	    txdnetbuf.icmpframe.type = 0x00;		// is icmp answer;
                txdnetbuf.udpframe.destport= 8001;
                txdnetbuf.udpframe.sourceport= 8001;
		txdnetbuf.udpframe.crc = 0;
		txdnetbuf.udpframe.crc = 0xFFFF - chksum((txdnetbuf.bytedata.bytebuf + 38),(size -34)); 
	    encPacketSend(&answeraddress,&txdnetbuf,size,0x0002);		
	}  
}
