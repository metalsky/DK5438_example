//===========================================================================//
//                                                                           //
// 文件：  ICMP.C                                                            //
// 说明：  BW-DK5438开发板以太网数据结构定义头文件                           //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.25                                                        //
//                                                                           //
//===========================================================================//

typedef unsigned char   INT8U ;
typedef unsigned int	INT16U;
typedef unsigned long   INT32U;

#define IP_PACKET           0x0008
#define ARP_PACKET          0x0608
#define RARP_PACKET	    0x3580
#define	MaxLenARPtable	    0x02


typedef struct 
{
	INT32U	ip;					//	IP地址
	INT32U	mask;					//	子网掩码
	INT32U	gate;					//	网关
	INT16U	port;					//	端口号
	INT8U	mac[6];   				//	以太网地址
}net_node	;					//	本机节点信息;

typedef struct 
{
  INT8U	  ipbytes[4];
  INT8U	  maskbytes[4];
  INT8U	  gatebytes[4];
  INT8U	  portbytes[2];
  INT16U  macwords[3] ;
}net_nodebytes;

typedef union 
{
  net_node node             ;
  net_nodebytes nodebytes   ;
}NetNode;
	

#define IP4_ADDR(a,b,c,d) ((INT32U)(a & 0xff) << 24) | ((INT32U)(b & 0xff) << 16) | \
                          ((INT32U)(c & 0xff) << 8) | (INT32U)(d & 0xff)

typedef union {
	INT16U	words[3];
	INT8U	bytes[6];
}ethernet_address_type;

typedef union {
	INT32U	dwords;
	INT16U	words[2];
	INT8U	bytes[4];
}ip_address_type;

typedef union {
	INT8U	bytes[12];
	INT16U	words[6];
	struct{
		INT8U	status;
		INT8U	ttl;
		ip_address_type ip_address;
		ethernet_address_type ethernet_address;
	}arp;							//状态,生存时间,ip地址,以太网地址
}arp_table_type;

//以太网帧                  
typedef struct {
	INT8U	status;				//接收状态
	INT8U	nextpage;			//下一个页
	INT16U	length;				//以太网长度，以字节为单位
	INT8U	destnodeid[6];		        //目的网卡地址
	INT8U	sourcenodeid[6];		//源网卡地址
	INT16U	protocal;			//下一层协议
	INT8U	packet[300];		        //包的内容
}ethernet;

//IP包   仅当IHL=5时用 当IHL!=5时作转换
typedef struct {
	INT16U	head[9];			//以太网包头
	INT8U	verandihl;			//版本与头长度
	INT8U	typeofserver;		        //服务类型
	INT16U	totallength;			//总长度
	INT16U	frameindex;		        //IP帧序号
	INT16U	segment;			//分段标志
	INT8U	ttl;				//生存时间
	INT8U	protocal;			//下一层协议
	INT16U	crc;				//校验和
	INT8U	sourceip[4];			//源IP
	INT8U	destip[4];			//目的IP
	INT8U	packet[280];		        //IP包的内容
}ip;

typedef struct {
	INT16U	head[9];			//以太网包头
	INT8U	ippacket[300];		//IP包的内容
}ippacket;
                
typedef struct {
	INT16U	head[9];  			//以太网头
//arp报文的内容总长28字节
	INT16U	harewaretype;		//以太网为0x0001
	INT16U	protocaltype;		//ip 为0X0800
	INT8U	halength;			//=0X06
	INT8U	palength;			//=0X04
	INT16U	operation;			//操作  0X0001为请求 0X0002为应答 0X0003为反向地址请求 0X0004为反向地址应答
	INT8U	sourcenodeid[6];		//源网卡地址
	INT8U	sourceip[4];			//源IP地址
	INT8U	destnodeid[6];		//目的网卡地址
	INT8U	destip[4];			//目的IP地址
}arp; 

typedef struct {						//包含在IP包中，是IP的上层为0X01的应用
	INT16U	head[9];			//以太网头
	INT16U	iphead[10];			//IP头
	INT8U	type;				//0X08 PING请求 0X00 PING应答
	INT8U	option;				//0X00 PING
	INT16U	crc;      
	INT16U	id;
	INT16U	seq;
	INT8U	icmpdata[272];
}icmp;

typedef struct {
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;			//源端口
	INT16U	destport;			//目的端口
	INT16U	seqnumberH;		//顺序号
	INT16U	seqnumberL;
	INT16U	acknumberH;
	INT16U	acknumberL;		//确认号
	INT8U	offset;				//数据偏移量
	INT8U	control;				//连接控制
	INT16U	window;			//流控
	INT16U	crc;				//校验和 ，包括伪头部，TCP头部，数据
	INT16U	urg;				//紧急指针
	INT8U	tcpdata[260];		//TCP数据
}tcp;

typedef struct {
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	sourceport;		        //源端口
	INT16U	destport;			//目的端口
	INT16U	length;    
	INT16U	crc;				//校验和 ，包括伪头部，udp头部，数据
	INT8U	udpdata[272];	               //udp数据
}udp;

typedef struct {
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	读写代码标志
	INT8U	fileandmode[270];	//	文件名和操作类型
}tftprw;

typedef struct {
	INT16U	head[9];
	INT16U	iphead[10];
	INT16U	udphead[4];
	INT16U	opencode;		//	读写代码标志
	INT16U	number;			//	块编号或者差错码
	INT8U	tftpdata[268];	//	数据或者差错信息
}tftpd;

typedef struct {
	INT8U	bytebuf[336];
}bytes;
             
//所有协议的共用体
typedef union {
	 bytes bytedata;
	 ethernet etherframe;	
	 arp arpframe;
	 icmp icmpframe;
	 tcp tcpframe;  
	 ip ipframe;
	 udp udpframe;
	 ippacket ippacket;
	 tftprw tftprwframe;
	 tftpd tftpdataframe;
}netcard;

typedef struct
{
	ip_address_type	ipaddr;
	ip_address_type	his_sequence;
	ip_address_type	my_sequence;
	ip_address_type	old_sequence;
	ip_address_type	his_ack;
	INT16U	port;
	INT8U	timer;
	INT8U	inactivity;	 
	INT8U	state;
	INT8U	query[23];
}tcpconnection;

