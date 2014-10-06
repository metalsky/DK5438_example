//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板uIP1.0示例程序                                     //
//         TCP客户端连接后，LED灯亮起指示连接数量，提供WHO/TEMP/查询服务     //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
//                                                                           //
//===========================================================================//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PIN_DEF.H"
#include <msp430.h>
#include <msp430x54x.h>
#include "TFTDRV.H"
#include "TOUCH.H"
#include "GUI.H"
#include "KEY.H"
#include "enc28j60.h"
#include "DS18B20.H"
#include "TCP_Server_App.h"

#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include "uipopt.h"
#include "clock-arch.h"

#define FLL_FACTOR   549

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

//--------------------------- LCD 颜色 ----------------------//
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Grey2          0xF79E
#define Dark_Grey      0x6B4D
#define Dark_Grey2     0x52AA
#define Light_Grey     0xE71C
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

float cx[1]={0.0670567926},\
      cy[1]={0.0900232554},\
      ox[1]={-18.3293762 },\
      oy[1]={-18.2325592 }                                         ;

unsigned Color,Color_BK                                            ;
char          MBRX[30]                                             ;
char          MBKeyTP[30]                                          ;
EDIT          Edit_Dig[10]                                         ; // 编辑控件
BUTTON        Button[10]                                           ; // 按钮控件
char          Event                                                ; // 事件
unsigned char INP_MODE_LMT = INP_EN+INP_DIG                        ; // 输入模式限制 
unsigned char DISP_MODE                                            ; // 编辑控件显示模式
unsigned char all_conns,page  = 1                                  ; // 连接数，连接信息当前显示页数
VIEW Conn_View                                                     ; // 连接信息视图
static uint8_t mymac[6] = {UIP_ETHADDR0,
                           UIP_ETHADDR1,
                           UIP_ETHADDR2,
                           UIP_ETHADDR3,
                           UIP_ETHADDR4,
                           UIP_ETHADDR5 }                          ; // 物理地址
static uint8_t myip[4]  = {UIP_IPADDR0 ,
                           UIP_IPADDR1 ,
                           UIP_IPADDR2 ,
                           UIP_IPADDR3  }                          ; // IP地址
clock_time_t sys_ticks                                             ; // 系统时钟

struct timer periodic_timer, arp_timer                             ;

char Key_TP_Task(void)                                             ;
void TCP_Server(void)                                              ;
void TCPP_Server(void);
extern void Update_View_List(void)                                 ;


#pragma vector=TIMER1_A0_VECTOR                                      // uIP时钟中断
static __interrupt void ISR_Tick(void) 
{
  sys_ticks++                                                      ;
  TA0CCR0  = (18000) - 1                                           ;  
}

//***************************************************************************//
//                                                                           //
//  Init_Timer0_A5(void): 初始化Timer0_A5                                    //
//                                                                           //
//***************************************************************************//
void Init_Timer0_A5(void)
{ 
  TA1CTL   = 0                                              // 复位Timer_A5,分频比为1
           | (1 << 2)                                       // 计数器清0                    
           | (2 << 8)                                     ; // 设置SMCLK为定时器时钟        
           
  TA1CCR0  = 18000-1                                      ; // 中断时间1ms
  TA1CCTL0 = 0                                             
            |(1 << 4)                                     ; // 允许比较中断
  TA1CTL  |= (1 << 4)                                     ; // 向上计数  
}

//***************************************************************************//
//                                                                           //
//函数：Init_CLK(void)                                                       //
//功能：初始化主时钟: MCLK = XT2                                             //
//                                                                           //
//***************************************************************************//
void Init_CLK(void)
{
  WDTCTL     = WDTPW + WDTHOLD                            ; // 关看门狗
  P7SEL     |= 0x03                                       ; // 端口选择外部低频晶振XT1
  UCSCTL6   &=~XT1OFF                                     ; // 使能外部晶振 
  UCSCTL6   |= XCAP_3                                     ; // 设置内部负载电容
  UCSCTL3   |= SELREF_2                                   ; // DCOref = REFO
  UCSCTL4   |= SELA_0                                     ; // ACLK   = XT1  
  __bis_SR_register(SCG0)                                 ; // 关闭FLL控制回路
  UCSCTL0    = 0x0000                                     ; // 设置DCOx, MODx
  UCSCTL1    = DCORSEL_7                                  ; // 设置DCO振荡范围
  UCSCTL2    = FLLD__1 + FLL_FACTOR                       ; // Fdco = ( FLL_FACTOR + 1)×FLLRef = (549 + 1) * 32768 = 18MHz
  __bic_SR_register(SCG0)                                 ; // 打开FLL控制回路                                                            
  __delay_cycles(1024000)                                 ; 
  do
  {
    UCSCTL7 &=~(XT2OFFG+XT1LFOFFG+XT1HFOFFG+DCOFFG)       ; // 清除 XT2,XT1,DCO 错误标志                                                            
    SFRIFG1 &=~OFIFG                                      ; 
  }while(SFRIFG1&OFIFG)                                   ; // 检测振荡器错误标志   
}

//***************************************************************************//
//                                                                           //
//  Init_Port(void): 设置IO端口                                              //
//                                                                           //
//***************************************************************************//
void Init_Port(void)
{
  P9DIR   |= IrDA_OUT                                     ; // 禁止红外发射调制
  P9OUT   &=~IrDA_OUT                                     ;  
  P7DIR  |= LED_PWR                                       ; // 发光二极管电源
  P7OUT  &=~LED_PWR                                       ;
  P8DIR   = 0xFF                                          ;
  P8OUT   = 0xFF                                          ;
  P1OUT   = 0xF0                                          ; // 设置LED控制端IO属性
  P1DIR   = 0xF0                                          ;
  P9OUT   = 0x0F                                          ; 
  P9DIR   = 0x0F                                          ;
  OW_DIO_PULLUP                                           ; // DS18B20  
}


void TCP_Server(void)
{
  char key,temp[2],i                                      ;
  char index,down_id=0xFF                                 ;
  char string[30]                                         ;
  union
  {
    unsigned int  pos[2]                                  ;
    char byte[4]                                          ;
  }TPoint                                                 ;
  unsigned char ntx_buf[20]                               ;
  CHECK_BOX Protocol_Check                                ;
  char Event,con_on = 1                                   ;
  static int cnt = 0                                      ;
  temp[1]   = 0x00                                        ;
  Color     = Black                                       ;
  Color_BK  = 0xEF9F                                      ;
  Clear_LCD(Color_BK)                                     ;
  DrawRectFill(0,0,240,25,Blue)                           ;
  DrawRectFill(0,295,240,25,Blue)                         ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                      ;
  Color    = 0xFFFF                                       ;
  Color_BK = Blue                                         ;
  PutString16M(4,4,"TCP服务器")                           ;
  PutStringEN16(45,300,"Powered by uIP1.0")               ;
  DrawPanel(5, 40,230,100,"连接信息")                     ;
  DrawPanel(5,160,230,40,"查询命令")                      ;
  DrawPanel(5,220,230,65,"参数设置")                      ;
  Color    = Black                                        ;
  Color_BK = WINDOW_BK_COLOR                              ;
  
  CreateButton(1,"关闭",180, 53,50,20)                    ;
  CreateButton(2,"上翻",180, 83,50,20)                    ;
  CreateButton(3,"下翻",180,113,50,20)                    ;
  CreateButton(5,"默认",180,228,50,20)                    ;
  CreateButton(6,"设置",180,258,50,20)                    ;
  CreateView(&Conn_View,68,11,52,160,83)                  ;
  CreateCheck(&Protocol_Check,132 ,265,12,12,"TCP")       ;
  CheckCheck(&Protocol_Check)                             ;
  CreateEditDig(1,11,173,218,22)                          ; 
  IniEditDig(1,1,0,1,26,"who/temp/volt/date/time")        ;
  Color    = Black                                        ;
  Color_BK = WINDOW_BK_COLOR                              ;
  PutString16M(9,238,"IP:")                               ;
  PutString16M(69,238,".")                                ;
  PutString16M(109,238,".")                               ;
  PutString16M(129,238,".")                               ;
  PutString16M(9,263,"端口:")                             ;
  PutString16M(90,263,"协议:")                            ;
  CreateEditDig(2,38,235,30,20)                           ; 
  sprintf(string,"%d",myip[0])                            ;
  IniEditDig(2,1,0,1,3,string)                            ;
  CreateEditDig(3,78,235,30,20)                           ; 
  sprintf(string,"%d",myip[1])                            ;
  IniEditDig(3,1,0,1,3,string)                            ;
  CreateEditDig(4,118,235,10,20)                          ; 
  sprintf(string,"%d",myip[2])                            ;
  IniEditDig(4,1,0,1,1,string)                            ;
  CreateEditDig(5,138,235,30,20)                          ; 
  sprintf(string,"%d",myip[3])                            ;
  IniEditDig(5,1,0,1,3,string)                            ;
  CreateEditDig(6,50,261,38,20)                           ; 
  IniEditDig(6,1,0,1,4,"1020")                            ;
  Init_J60SPI()                                           ; // 

  for(;;) 
  {
    if(con_on)
      uip_len = enc28j60PacketReceive(UIP_BUFSIZE + 2,
                                      uip_buf)            ;
    else 
      uip_len = 0x00                                      ;
    if(uip_len > 0) 
    {      
      if(BUF->type == htons(UIP_ETHTYPE_IP)) 
      {
	uip_arp_ipin()                                    ;
	uip_input()                                       ;
	if(uip_len > 0)                                     // 若需发送数据，则uip_len>0
        {
	  uip_arp_out()                                   ;
	  enc28j60PacketSend(uip_len,uip_buf)             ;
	}
      } 
      else if(BUF->type == htons(UIP_ETHTYPE_ARP)) 
      {
	uip_arp_arpin();
	if(uip_len > 0) 
        {
	  enc28j60PacketSend(uip_len,uip_buf)             ;
	}
      }
    } 
    else if(timer_expired(&periodic_timer)) 
    {
      timer_reset(&periodic_timer)                        ;
      for(i = 0; i < UIP_CONNS; i++) 
      {
	uip_periodic(i);
	if(uip_len > 0) 
        {
	  uip_arp_out();
	  enc28j60PacketSend(uip_len,uip_buf);
	}
      }

#if UIP_UDP
      for(i = 0; i < UIP_UDP_CONNS; i++) 
      {
	uip_udp_periodic(i);
	if(uip_len > 0) 
        {
	  uip_arp_out();
	  enc28j60PacketSend(uip_len,uip_buf);
	}
      }
#endif /* UIP_UDP */
      
      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer)) {
	timer_reset(&arp_timer);
	uip_arp_timer();
      }
    }
    if(cnt++>500)
    {
      cnt = 0                                                 ;
      Event  =  Key_TP_Task()                                 ;
      key            = MBKeyTP[1]                             ;
      TPoint.byte[0] = MBKeyTP[3]                             ;
      TPoint.byte[1] = MBKeyTP[4]                             ;
      TPoint.byte[2] = MBKeyTP[5]                             ;
      TPoint.byte[3] = MBKeyTP[6]                             ;
      if(Event&EVENT_KEY_PRESSED)
      {
        _NOP();
        switch(key)
        {
        case 1:case 2:case 3:case 4:case 5:
        case 6:case 7:case 8:case 9:case 0:
        case Left: case Right: case Backspace:
          EditDigMessage(key,temp)                            ; 
          break                                               ;    
        case Enter:
          break                                               ;
        case Cancel:
          break                                               ;
        }
      }
      else if(Event&EVENT_TP_TOUCHED)
      {
        index = InScopeButton(TPoint.pos[0],
                              TPoint.pos[1] )                 ;
        if(index==0xFF)
        {
          SetButtonUp(down_id)                                ;
          down_id = index                                     ;
        }
        else if(index!=down_id)
        {
          SetButtonUp(down_id)                                ;
          down_id = index                                     ;
          SetButtonDown(down_id)                              ;
        }
      }
      else if(Event&EVENT_TP_PRESSED)  
      {  
        index = InScopeButton(TPoint.pos[0],
                              TPoint.pos[1] )                 ;
        SetButtonUp(index)                                    ;
        down_id  = 0xFF                                       ;
        switch(index)
        {
        case 1:
          if(con_on==1)
          {
            con_on = 0                                        ;
            SetButtonTitle(1,"开放")                          ;
          }
          else
          {
            con_on = 1                                        ;
            SetButtonTitle(1,"关闭")                          ;
          }
          break                                               ;
        case 2:
          if(page>1)
          {
            page = 1                                          ;
            Update_View_List()                                ;
          }
          break                                               ;
        case 3:
          if(all_conns>5&&page<2)
          {  
            page = 2                                          ;
            Update_View_List()                                ;
          }
          break                                               ;
        case 4:
          ReadEditDig(1,ntx_buf)                              ;
          break                                               ;
        }
        if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
           ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
           break                                              ;
      }
      Event   = 0                                             ;
    }
  }  
}

/*---------------------------------------------------------------------------*/
void main(void)
{
  uip_ipaddr_t ipaddr                                ;

  Init_CLK()                                         ; // 初始化时钟
  Init_Port()                                        ; // 初始化端口 
  MAIN_POWER_ON                                      ; // 打开电源
  LCD_Init()                                         ; // 初始化LCD
  Init_Timer0_A5()                                   ; // 初始化定时器
  _EINT()                                            ; 
  
  __delay_cycles(5000000)                            ;    
  Init_J60SPI()                                      ;
  enc28j60Init(mymac)                                ;
  delay_ms(20)                                       ;
  enc28j60PhyWrite(PHLCON,0x476)                     ; // 配置网口指示灯，绿灯：连接状态，黄灯：数据传输

  timer_set(&periodic_timer, CLOCK_SECOND / 2)       ;
  timer_set(&arp_timer, CLOCK_SECOND * 10)           ;
  uip_init()                                         ;
  uip_arp_init()                                     ;
  uip_ipaddr(ipaddr, 192,168,1,103)                  ; // 本地IP
  uip_sethostaddr(ipaddr)                            ; 
  uip_ipaddr(ipaddr, 192,168,1,1)                    ; // 网关IP，根据网络情况自行设置
  uip_setdraddr(ipaddr)                              ;
  uip_ipaddr(ipaddr, 255,255,255,0)                  ; // 子网掩码
  uip_setnetmask(ipaddr)                             ;
  TCP_Server_Init()                                  ;
  TCP_Server()                                       ;  
}

/*---------------------------------------------------------------------------*/

void
uip_log(char *m)
{
  printf("uIP log message: %s\n", m);
}
void
resolv_found(char *name, u16_t *ipaddr)
{
//  u16_t *ipaddr2;
  
  if(ipaddr == NULL) {
    printf("Host '%s' not found.\n", name);
  } else {
    printf("Found name '%s' = %d.%d.%d.%d\n", name,
	   htons(ipaddr[0]) >> 8,
	   htons(ipaddr[0]) & 0xff,
	   htons(ipaddr[1]) >> 8,
	   htons(ipaddr[1]) & 0xff);
    /*    webclient_get("www.sics.se", 80, "/~adam/uip");*/
  }
}
 
#ifdef __DHCPC_H__
void
dhcpc_configured(const struct dhcpc_state *s)
{
  uip_sethostaddr(s->ipaddr);
  uip_setnetmask(s->netmask);
  uip_setdraddr(s->default_router);
  resolv_conf(s->dnsaddr);
}
#endif /* __DHCPC_H__ */

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Key_TP_Task(void)                                               //
//描述：键盘与触摸屏扫描，编辑控件光标闪烁                                   //
//		                                                             //
//---------------------------------------------------------------------------//
char Key_TP_Task(void)
{
  static unsigned char flicker_cnt = 0                      ;
  static char key      = 0xFF, last_key = 0xFF              ;
  static char down_cnt = 0   , tp_cnt   = 0                 ;
  static union 
  {
    unsigned int  pos[2]                                    ;
    char byte[4]                                            ;
  }TPoint                                                   ;
  char event = 0                                            ;
  Init_KeyPad()                                             ; 
  TP_Init()                                                 ;
  {
    if(++flicker_cnt>50)                                      // 界面闪烁元素
    {
      flicker_cnt = 0x00                                    ;
      event = EVENT_FLICKER                                 ;
    }
    key = ReadKey()                                         ; // 按键扫描
    if(key!=last_key&&key!=0xFF)
      down_cnt  =  0                                        ;
    else if(key!=last_key&&key==0xFF)
    {
      if(down_cnt>3)
        event    |= EVENT_KEY_PRESSED                       ;
    }
    else if(key==last_key&&key!=0xFF)
      if(++down_cnt>4) down_cnt = 4                         ;
    Init_TPSPI()                                            ;
    if(!Read_TP_Twice(&TPoint.pos[0],&TPoint.pos[1]))         // 触摸屏扫描         
    {
      if(tp_cnt>3)
        event  |= EVENT_TP_PRESSED                          ;
      tp_cnt   = 0                                          ;
    }
    else 
    {
      if(++tp_cnt>8)      tp_cnt     = 8                    ;
      if(tp_cnt>4)
      {
        Convert_Pos( TPoint.pos[0], TPoint.pos[1],   
                    &TPoint.pos[0],&TPoint.pos[1] )         ;
        event  |= EVENT_TP_TOUCHED                          ;
      }
    }  
    Init_J60SPI()                                           ;
    if(event)
    {
      Event        = event                                  ;
      MBKeyTP[0]   = event                                  ;
      MBKeyTP[1]   = last_key                               ;
      MBKeyTP[3]   = TPoint.byte[0]                         ;
      MBKeyTP[4]   = TPoint.byte[1]                         ;
      MBKeyTP[5]   = TPoint.byte[2]                         ;
      MBKeyTP[6]   = TPoint.byte[3]                         ;
    }
    last_key = key                                          ;
  }
  return  event                                             ;
}