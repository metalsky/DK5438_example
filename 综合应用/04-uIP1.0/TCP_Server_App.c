//===========================================================================//
//                                                                           //
// �ļ���  TCT_Server.c                                                      //
// ˵����  BW-DK5438������TCP���������                                    //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
//                                                                           //
//===========================================================================//

#include "TCP_Server_App.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GUI.H"
#include "PIN_DEF.H"
#include "DS18B20.H"

extern void *uip_appdata                                              ;               
extern VIEW Conn_View                                                 ;
extern unsigned char all_conns,page                                   ;
unsigned int IP_Connected[8][2]                                       ;

void Update_LED(void)                                                 ;
unsigned char IP_is_Connected(unsigned int ip1,unsigned int ip2)      ;
unsigned char  Add_IP_to_List(unsigned int ip1,unsigned int ip2)      ;   
unsigned char Delete_IP_From_List(unsigned int ip1,unsigned int ip2)  ;        
void Update_View_List(void)                                           ;

void TCP_Server_Init(void) 
{
  uip_listen(HTONS(1020))                                             ;
  Do1Convert()                                                        ; 
}


void TCP_Server_App(void) 
{
    char *hello = "���ã���ӭʹ��BW-DK5438������TCP����������\n"    ;
    char *tcpdata                                                     ;
    char string[100]                                                  ;
    float temperature                                                 ;
    unsigned int  TEMP                                                ;
    tcpdata  = (char *)uip_appdata                                    ;    
    if(uip_connected())
    {
      uip_send(hello,strlen(hello)+1)                                 ; // ����������Ӧ��Ϣ
      if(IP_is_Connected(uip_conn->ripaddr[0],
                         uip_conn->ripaddr[1])==0xFF)                   // IPδ����
      {
        Add_IP_to_List(uip_conn->ripaddr[0],uip_conn->ripaddr[1])     ; // ����ǰ������ӵ������б�
        all_conns++                                                   ;
        Update_LED()                                                  ;
        if(  ((page==1)&&(all_conns<=4))
           ||((page==2)&&(all_conns> 4)))
          Update_View_List()                                          ;
      }
    }
    if(uip_newdata()) 
    {
      if(  ((tcpdata[0]=='w')||(tcpdata[0]=='W'))                       // ��Ӧ"who"��ѯ
         &&((tcpdata[1]=='h')||(tcpdata[1]=='H'))
         &&((tcpdata[2]=='o')||(tcpdata[2]=='O')))
        uip_send("This is BW-DK5438.",18)                             ;
      else 
      if(  ((tcpdata[0]=='t')||(tcpdata[0]=='T'))
         &&((tcpdata[1]=='e')||(tcpdata[1]=='E'))
         &&((tcpdata[2]=='m')||(tcpdata[2]=='M'))
         &&((tcpdata[3]=='p')||(tcpdata[3]=='P')))
      {
        TEMP          = Do1Convert()                                  ; // ��Ӧ"temp"��ѯ
        temperature   = TEMP>>4                                       ; // volt/date/time��ѯ��ʵ�־������������û���
        temperature  += (float)(TEMP&0x000F)/16                       ;
        sprintf(string,"Temperature is %.1f degree.",
                temperature                         )                 ;             
        uip_send(string,strlen(string))                               ;
      }        
      else         
        uip_send("No service.",11)                                    ;
    }
    if(uip_closed())                                                    // Զ�̿ͻ��˹ر�����
    {
      if(IP_is_Connected(uip_conn->ripaddr[0],
                         uip_conn->ripaddr[1])!=0xFF)        
      {
        Delete_IP_From_List(uip_conn->ripaddr[0],uip_conn->ripaddr[1]);
        all_conns--                                                   ;
        Update_LED()                                                  ;
        Update_View_List()                                            ;
      }
    }
}

//***************************************************************************//
//                                                                           //
//  void Update_LED(void): ����������������Ϩ��LED                           //
//                                                                           //
//***************************************************************************//
void Update_LED(void)
{
  P1OUT  |= 0xF0                                                      ;
  P9OUT  |= 0x0F                                                      ;
  switch(all_conns)
  {
  case 0:
    break                                                             ;
  case 1:
    P9OUT  &= 0xFE                                                    ;
    break                                                             ;
  case 2:
    P9OUT  &= 0xFC                                                    ;
    break                                                             ;
  case 3:
    P9OUT  &= 0xF8                                                    ;
    break                                                             ;
  case 4:
    P9OUT  &= 0xF0                                                    ;
    break                                                             ;
  case 5:
    P1OUT  &= 0xEF                                                    ;
    P9OUT  &= 0xF0                                                    ;
    break                                                             ;
  case 6:
    P1OUT  &= 0xCF                                                    ;
    P9OUT  &= 0xF0                                                    ;
    break                                                             ;
  case 7:
    P1OUT  &= 0x8F                                                    ;
    P9OUT  &= 0xF0                                                    ;
    break                                                             ;
  case 8:
    P1OUT  &= 0x0F                                                    ;
    P9OUT  &= 0xF0                                                    ;
    break                                                             ;
  }
}

//***************************************************************************//
//                                                                           //
//  unsigned char IP_Connected(ip1,ip2): ����IP�Ƿ�������                    //
//                                                                           //
//***************************************************************************//
unsigned char IP_is_Connected(unsigned int ip1,unsigned int ip2)
{
  unsigned char i                                                     ;
  for(i=0;i<8;i++)
  {
    if(IP_Connected[i][0]==ip1&&IP_Connected[i][1]==ip2)
      return i                                                        ;
  }
  return 0xFF                                                         ;
}

//***************************************************************************//
//                                                                           //
//  unsigned char Add_IP_to_List(ip1,ip2): ����ǰ������ӵ������б�          //
//                                                                           //
//***************************************************************************//
unsigned char  Add_IP_to_List(unsigned int ip1,unsigned int ip2)         
{
  unsigned int i                                                      ;
  for(i=0;i<8;i++)
  {
    if(IP_Connected[i][0]==0&&IP_Connected[i][1]==0)
    {
      IP_Connected[i][0] = ip1                                        ;
      IP_Connected[i][1] = ip2                                        ;
      return   i                                                      ;
    }
  }
  return  0xFF                                                        ;
}

//***************************************************************************//
//                                                                           //
//  unsigned char Add_IP_to_List(ip1,ip2): ����ǰ������ӵ������б�          //
//                                                                           //
//***************************************************************************//
unsigned char  Delete_IP_From_List(unsigned int ip1,unsigned int ip2)         
{
  unsigned char i,j                                                   ;
  for(i=0;i<8;i++)
  {
    if(IP_Connected[i][0]==ip1&&IP_Connected[i][1]==ip2)
    {
      IP_Connected[i][0]     = 0                                      ;
      IP_Connected[i][1]     = 0                                      ;
      for(j=i+1;j<8;j++)
      {
        IP_Connected[j-1][0] = IP_Connected[j][0]                     ;
        IP_Connected[j-1][1] = IP_Connected[j][1]                     ;
        IP_Connected[j][0]   = 0                                      ;
        IP_Connected[j][1]   = 0                                      ;
      }
      return   i                                                      ;
    }
  }
  return  0xFF                                                        ;
}

//***************************************************************************//
//                                                                           //
//  unsigned char Add_IP_to_List(ip1,ip2): ����ǰ������ӵ������б�          //
//                                                                           //
//***************************************************************************//

void Update_View_List(void)
{
  unsigned char i=0                                                   ;
  char string[100]                                                    ;
  ClearViewContent(&Conn_View)                                        ;
  if(page==2)
    i  = 4                                                            ;
  while(IP_Connected[i][0]!=0||IP_Connected[i][1]!=0)
  {
    sprintf( string,"[%d] %d.%d.%d.%d",i+1         ,
            (IP_Connected[i][0])&0x00FF            ,
            (IP_Connected[i][0])>>8                ,
            (IP_Connected[i][1])&0x00FF            ,
            (IP_Connected[i][1])>>8                 )                 ;
    ViewInsertLine(&Conn_View,(unsigned char *)string)                ;
    if(++i>7) break                                                   ;    
  }
}
