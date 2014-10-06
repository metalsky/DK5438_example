//===========================================================================//
//                                                                           //
// �ļ���  APP.C                                                             //
// ˵����  BW-DK5438������uC/OS-II��ʾ����                                   //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// �汾��  v1.2u                                                             //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include <includes.h>
#include <app.h>
#include <TITLE.H>
#include <7SEG_FONT_2436.H>
#include "enc28j60.h"
#include "ip_arp_udp.h"
#include "timeout.h"
#include "net.h"
#include <DS18B20.H>

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                       ; // Vector 0 - no interrupt
  case 2:                                                              // Vector 2 - RXIFG
      MBKeyTPBuffer[2]  = UCA1RXBUF                                  ;      
      MBKeyTPBuffer[0] |= RX_RECEIVED                                ;
      OSMboxPost(MBKeyTP,(void *)&MBKeyTPBuffer[0])                  ;
      break                                                          ;
  case 4:break                                                       ;  // Vector 4 - TXIFG
  default: break                                                     ;  
  }  
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  switch(__even_in_range(UCA2IV,4))
  {
  case 0:break                                                        ; // Vector 0 - no interrupt
  case 2:                                                               // Vector 2 - RXIFG
      MBKeyTPBuffer[2]  = UCA2RXBUF                                   ;      
      MBKeyTPBuffer[0] |= RX_RECEIVED                                 ;
      OSMboxPost(MBKeyTP,(void *)&MBKeyTPBuffer[0])                   ;
      break                                                           ;
  case 4:break                                                        ;  // Vector 4 - TXIFG
  default: break                                                      ;  
  }  
}

#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                        ; // Vector 0 - no interrupt
  case 2:                                                               // Vector 2 - RXIFG
      MBKeyTPBuffer[2]  = UCA3RXBUF                                   ;      
      MBKeyTPBuffer[0] |= RX_RECEIVED                                 ;
      OSMboxPost(MBKeyTP,(void *)&MBKeyTPBuffer[0])                   ;
      break                                                           ;
  case 4:break                                                        ;  // Vector 4 - TXIFG
  default: break                                                      ;  
  }
}
  
void  main (void)
{
  WDTCTL   = WDTPW + WDTHOLD                                          ;
  BSP_IntDisAll()                                                     ; // ���ж�
  OSInit()                                                            ; // ��ʼ��uC/OS-II�ں�
  
  OSTaskCreateExt(AppTaskStart,                                         // ������ʼ����
                  (void *)0,
                  (OS_STK *)&AppTaskStartStk[APP_START_TASK_STK_SIZE - 1],
                  APP_START_TASK_PRIO,
                  APP_START_TASK_PRIO,
                  (OS_STK *)&AppTaskStartStk[0],
                  APP_START_TASK_STK_SIZE,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR)          ; // �����ջ���鹦��
  
  Init_RTC()                                                          ;
  MAIN_POWER_ON                                                       ;
  __delay_cycles(50000)                                               ;
  P9DIR   |= IrDA_OUT                                                 ; // �رպ���˿�
  P9OUT   &=~IrDA_OUT                                                 ;
  Init_Func()                                                         ; // ��ʼ����������
  BSP_Init()                                                          ; // ��ʼ������Ӳ����Դ
  Ini_LED(60)                                                         ; // ��ʼ�������
  LED_POWER_ON                                                        ; // ������ܵ�Դ
  LED_Disp_Int(0,NO_ZERO)                                             ; // ��ʼ���������ʾ����
  LCD_Init()                                                          ; // TFTҺ����ʼ��
  OSStart()                                                           ; // ����uC/OS-II
}

//---------------------------------------------------------------------------//
//                                                                           //
//������void AppTaskStart(void *)                                            //
//��������ʼ����                                                             //
//���ܣ���ʼ������ͳ�ƹ��ܣ�������������                                     //
//                                                                           //
//---------------------------------------------------------------------------//              

static  void  AppTaskStart (void *p_arg)
{
  p_arg = p_arg                                                       ; // ������뾯�棬�����ж���õ�������׸��
  TMR_TickInit()                                                      ; // ����OSʱ�ӽ���
#if (OS_TASK_STAT_EN > 0)
  OSStatInit()                                                        ; // ʹ������ͳ�ƹ���
#endif
  AppTaskCreate()                                                     ; // ����������������
  while(1)
  { 
    OSTimeDly(1000)                                                   ; // ���ʹ������ѭ����Ҳ����ɾ������
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//������void LED_TASK(void *)                                                //
//����������ܿ�������                                                       //
//���ܣ���̬ˢ��8λ����ܣ���ʾCPUʹ����                                     //
//                                                                           //
//---------------------------------------------------------------------------//              
void LED_TASK(void *p_arg)
{
  p_arg    = p_arg                                                     ;
  Seg7LedRefresh()                                                     ;  
}

//---------------------------------------------------------------------------//
//                                                                           //
//������void GUI_TASK(void *)                                                //
//������ͼ���û������������                                                 //
//���ܣ����ռ��̺ʹ�������Ϣ�����ͼ���û��������                           //
//                                                                           //
//---------------------------------------------------------------------------//              
void GUI_TASK(void *p_arg)
{
  p_arg    = p_arg                                                     ;
  RunMenu(0)                                                           ;  
}

//---------------------------------------------------------------------------//
//                                                                           //
//������void MENU_OP_TASK(void *)                                            //
//�������˵�����������������                                                 //
//���ܣ�����ͼ���û�����������������Ӧ�ĺ�������                         //
//                                                                           //
//---------------------------------------------------------------------------//              
void MENU_OP_TASK(void *p_arg)
{
  INT8U err[1]                                                         ;
  p_arg    = p_arg                                                     ;
  char *ptmb                                                           ;
  char  func_index,event                                               ;
  SetRTCYEAR(2010)                                                     ; 
  SetRTCMON(10)                                                        ;
  SetRTCDAY(24)                                                        ;
  SetRTCDOW(0)                                                         ;
  SetRTCHOUR(10)                                                       ;
  SetRTCMIN(9)                                                         ;
  SetRTCSEC(15)                                                        ;
  for(;;)
  {
    ptmb       = OSMboxPend(MBFunc,0,err)                              ;    
    event      = *(ptmb++)                                             ;
    func_index = *(ptmb  )                                             ;
    if(event&EVENT_OP_STARTED)
    {
      Item_OP[func_index](&func_index)                                 ; 
      OSTaskResume(GUI_TASK_PRIO)                                      ;
    }
  }
}



//---------------------------------------------------------------------------//
//                                                                           //
//������void CPU_USAGE_TASK(void *)                                          //
//������CPU��������ʾ����                                                    //
//���ܣ���ʾCPU������                         //
//                                                                           //
//---------------------------------------------------------------------------//              
void CPU_USAGE_TASK(void *p_arg)
{
#if      OS_CRITICAL_METHOD == 3
  OS_CPU_SR cpu_sr                                                     ;
#endif
  p_arg    = p_arg                                                     ;
  for(;;)
  {
    OS_ENTER_CRITICAL()                                                ;
#if(OS_TASK_STAT_EN)    
    LED_Disp_CPUUsage(OSCPUUsage)                                      ; // ��ʾCPU������
#endif
    OS_EXIT_CRITICAL()                                                 ;
    OSTimeDly(5)                                                       ;
  }
}

//---------------------------------------------------------------------------//
//                                                                           //
//������void Key_TP_Task(void *)                                             //
//�����������봥��������                                                     //
//���ܣ�ɨ�谴�������ʹ��������������Ʊ༭�ؼ���˸                           //
//                                                                           //
//---------------------------------------------------------------------------//
void KEY_TP_TASK(void *p_arg)
{
  unsigned char flicker_cnt = 0                             ;
  char key = 0xFF,last_key = 0xFF, down_cnt = 0, tp_cnt = 0 ;
  char rx_char                                              ;
  union 
  {
    unsigned int  pos[2]                                    ;
    char byte[4]                                            ;
  }TPoint                                                   ;
  char  event                                               ;
  INT8U err                                                 ;
  p_arg  =  p_arg                                           ;
  Init_KeyPad()                                             ; 
  TP_Init()                                                 ;
  for(;;)
  {
    event  = 0x00                                           ;
    if(++flicker_cnt>50)                                      // ������˸Ԫ��
    {
      flicker_cnt = 0x00                                    ;
      event |= EVENT_FLICKER                                ;
    }
    key = ReadKey()                                         ; // ����ɨ��
    if(key!=last_key&&key!=0xFF)
      down_cnt  =  0                                        ;
    else if(key!=last_key&&key==0xFF)
    {
      if(down_cnt>3)
        event    |= EVENT_KEY_PRESSED                       ;
    }
    else if(key==last_key&&key!=0xFF)
      if(++down_cnt>4) down_cnt = 4                         ;
    OSMutexPend(SPIMutex,0,&err)                            ;
    Init_TPSPI()                                            ;
    if(!Read_TP_Twice(&TPoint.pos[0],&TPoint.pos[1]))         // ������ɨ��         
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
    OSMutexPost(SPIMutex)                                   ;
    if(event)                                                 // ����ϵͳ��Ϣ
    {
      MBKeyTPBuffer[0] = event                              ;
      MBKeyTPBuffer[1] = last_key                           ;
      MBKeyTPBuffer[2] = rx_char                            ;
      MBKeyTPBuffer[3] = TPoint.byte[0]                     ;
      MBKeyTPBuffer[4] = TPoint.byte[1]                     ;
      MBKeyTPBuffer[5] = TPoint.byte[2]                     ;
      MBKeyTPBuffer[6] = TPoint.byte[3]                     ;
      OSMboxPost(MBKeyTP,(void *)&MBKeyTPBuffer[0])         ;
    }
    last_key = key                                          ;
    OSTimeDly(30)                                           ;
  }
}

             
//---------------------------------------------------------------------------//
//                                                                           //
//������void Init_Func(void)                                                 //
//������������ʼ������                                                       //
//���ܣ���ʼ���˵���������ָ��                                               //
//                                                                           //
//---------------------------------------------------------------------------//
void Init_Func(void)
{
  Item_OP[0]   = Proc_Show_Clock                            ;
  Item_OP[1]   = Proc_Task_Info                             ;
  Item_OP[2]   = Proc_Port_Comm                             ;
  Item_OP[3]   = Proc_IrDA_Comm                             ;
  Item_OP[4]   = Proc_Display                               ;
  Item_OP[5]   = Proc_Paint                                 ;
  Item_OP[6]   = Proc_Show_Pic                              ;
  Item_OP[7]   = Proc_TP_Adj                                ;
  Item_OP[8]   = Proc_Go_Sleep                              ;
}

//***************************************************************************//
//                                                                           //
//                 ��ʼ��RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // ʱ��ģʽ,ʮ�����Ƹ�ʽ
}


//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Show_Clock(char *NC)                                       //
//����������ʱ�ӳ���                                                         //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Show_Clock(char *NC)
{
  unsigned int temp[2]                               ;
  unsigned int Hour ,Minute,Second                   ;
  OSTaskSuspend(KEY_TP_TASK_PRIO)                    ;
  Hour    = GetRTCHOUR()                             ;
  while(Hour>=12)
  {
    Hour -= 12                                       ;
  }
  Minute  = GetRTCMIN()                              ;
  Second  = GetRTCSEC()                              ;
  Ini_Clock_Contex()                                 ;
  ShowBMP240320(BMP_CLOCK)                           ;
  Gen_Hour_Hand(Hour*30+Minute/2)                    ;
  Save_Hour_Hand_BK()                                ; 
  Draw_Hour_Hand(White)                              ;
  Gen_Minu_Hand(Minute*6+Second/10)                  ;
  Save_Minu_Hand_BK()                                ; 
  Draw_Minu_Hand(White)                              ;
  Gen_Second_Hand(Second*6)                          ;
  Save_Second_Hand_BK()                              ; 
  Draw_Second_Hand(White)                            ;
  for(;;)
  {
    if(Second==GetRTCSEC())     continue             ;
    Second   = GetRTCSEC()                           ;
    Minute  = GetRTCMIN()                            ;
    Hour    = GetRTCHOUR()                           ;
    while(Hour>=12)
    {
      Hour -= 12                                     ;
    }
    if(Minute%6==0&&Second==0)                        // ʱ��ת��
    {
      Restore_Second_Hand_BK()                       ;
      Restore_Minu_Hand_BK()                         ;
      Restore_Hour_Hand_BK()                         ;
      Gen_Hour_Hand(Hour*30+Minute/2)                ;
      if(Second%10==0)
        Gen_Minu_Hand(Minute*6+Second/10)            ;
      Gen_Second_Hand(Second*6)                      ;
      Save_Hour_Hand_BK()                            ; 
      Draw_Hour_Hand(White)                          ;    
      Save_Minu_Hand_BK()                            ; 
      Draw_Minu_Hand(White)                          ;
      Save_Second_Hand_BK()                          ; 
      Draw_Second_Hand(White)                        ;
    }
    else if(Second%10==0)                             // ����ת��
    {
      Restore_Second_Hand_BK()                       ;
      Restore_Minu_Hand_BK()                         ;      
      Gen_Minu_Hand(Minute*6+Second/10)              ;
      Gen_Second_Hand(Second*6)                      ;
      Save_Minu_Hand_BK()                            ; 
      Draw_Minu_Hand(White)                          ;
      Save_Second_Hand_BK()                          ; 
      Draw_Second_Hand(White)                        ;
    }
    else
    {
      Restore_Second_Hand_BK()                       ;
      Gen_Second_Hand(Second*6)                      ;
      Save_Second_Hand_BK()                          ; 
      Draw_Second_Hand(White)                        ;      
    }
    OSTimeDly(300)                                   ;
    if(Read_TP_Twice(&temp[0],&temp[1]))
      break                                          ;    
  }
  OSTaskResume(KEY_TP_TASK_PRIO)                     ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Display(char *NC)                                          //
//����������ܿ��Ƴ���                                                       //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Display(char *NC)
{
  CHECK_BOX Disp_Check[3]                            ;
  char key,Event,temp[1],active_digit,rx_char        ;
  char index,down_id=0xFF,i,flicker = 0              ;
  union
  {
    unsigned int  pos[2]                             ;
    char byte[4]                                     ;
  }TPoint                                            ;
  INT8U     err[1]                                   ;
  char *ptmb                                         ;
  OSTaskSuspend(CPU_USAGE_TASK_PRIO)                 ;
  Clear_LED()                                        ;
  LED_Disp_Float(3.14159255,7,FIT_ZERO)              ;
  LED_Flicker_Digit(8,DIGIT_FLICKER)                 ;
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"����ܿ���")                    ;
  PutStringCN16(200,300,"�˳�")                      ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  DrawFrame(3,80,232,48)                             ;
  DrawRectFill(4,81,232,46,Black)                    ;
  DrawPanel(6,160,228,72,"")                         ;
  CreateButton(1,"��",15, 175,45,20)                 ;
  CreateButton(2,"��",70, 175,45,20)                 ;
  CreateButton(3,"��",125,175,45,20)                 ;
  CreateButton(4,"��",180,175,45,20)                 ;  
  for(active_digit=1;active_digit<9;active_digit++)
  {
    temp[0] = Find_Data(LED[active_digit-1].data)    ;
    if(!(LED[active_digit-1].mode&DIGIT_OFF))
      Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                   87,Red,Black                  )   ; 
    if(!(LED[active_digit-1].data&SEGDP))
      DrawRectFill((active_digit-1)*29+30,120,
                  3,3,Red)                           ;
  }
  for(active_digit=1;active_digit<9;active_digit++)
  {
    DrawFrame((active_digit-1)*29+14,139,11,11)      ;    
    DrawRectFill((active_digit-1)*29+15,140,10,10,Dark_Grey); 
  }
  DrawRectFill(15,140,10,10,Green)                   ; 
  active_digit = 1                                   ;
  CreateCheck(&Disp_Check[0],25 ,210,12,12,"��˸")   ;
  CreateCheck(&Disp_Check[1],90 ,210,12,12,"����")   ;
  CreateCheck(&Disp_Check[2],150,210,12,12,"С����") ;
  if(LED[active_digit-1].mode&DIGIT_FLICKER)
    CheckCheck(&Disp_Check[0])                       ;
  if(LED[active_digit-1].mode&DIGIT_OFF)
    CheckCheck(&Disp_Check[1])                       ;
  if(!(LED[active_digit-1].data&SEGDP))
    CheckCheck(&Disp_Check[2])                       ;   
  temp[0]      = 0x00                                ;
  for(;;)
  {
    ptmb           = OSMboxPend(MBKeyTP,0,err)       ;
    Event          = *(ptmb++)                       ;
    key            = *(ptmb++)                       ;
    rx_char        = *(ptmb++)                       ;
    TPoint.byte[0] = *(ptmb++)                       ;
    TPoint.byte[1] = *(ptmb++)                       ;
    TPoint.byte[2] = *(ptmb++)                       ;
    TPoint.byte[3] = *(ptmb++)                       ;
    rx_char        = rx_char                         ;    
    key            = key                             ;
    if(Event&EVENT_TP_TOUCHED)
    {
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==0xFF)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        OSMutexPost(SPIMutex)                        ;
      }
      else if(index!=down_id)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        SetButtonDown(down_id)                       ;
        OSMutexPost(SPIMutex)                        ;
      }
    }
    else if(Event&EVENT_TP_PRESSED)  
    {  
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OSMutexPend(SPIMutex,0,err)                    ;
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OSMutexPost(SPIMutex)                          ;
      switch(index)
      {
      case 1:
        if(active_digit>1)
        {
          active_digit--                              ;
          DrawRectFill((active_digit)*29+15,140,10,10,Dark_Grey); 
          DrawRectFill((active_digit-1)*29+15,140,10,10,Green); 
          if(LED[active_digit-1].mode&DIGIT_FLICKER)
            CheckCheck(&Disp_Check[0])               ;
          else
            unCheckCheck(&Disp_Check[0])             ;
          if(LED[active_digit-1].mode&DIGIT_OFF)
            CheckCheck(&Disp_Check[1])               ;
          else
            unCheckCheck(&Disp_Check[1])             ;
          if(!(LED[active_digit-1].data&SEGDP))
            CheckCheck(&Disp_Check[2])               ;
          else
            unCheckCheck(&Disp_Check[2])             ;
        }
        break                                        ;
      case 2:
        if(active_digit<8)
        {
          active_digit++                              ;
          DrawRectFill((active_digit-2)*29+15,140,10,10,Dark_Grey); 
          DrawRectFill((active_digit-1)*29+15,140,10,10,Green); 
          if(LED[active_digit-1].mode&DIGIT_FLICKER)
            CheckCheck(&Disp_Check[0])               ;
          else
            unCheckCheck(&Disp_Check[0])             ;
          if(LED[active_digit-1].mode&DIGIT_OFF)
            CheckCheck(&Disp_Check[1])               ;
          else
            unCheckCheck(&Disp_Check[1])             ;
          if(!(LED[active_digit-1].data&SEGDP))
            CheckCheck(&Disp_Check[2])               ;
          else
            unCheckCheck(&Disp_Check[2])             ;
        }
        break                                        ;
      case 3:        
        temp[0] = Find_Data(LED[active_digit-1].data);
        if(++temp[0]>9)   temp[0] = 0                ;
        LED[active_digit-1].data |= 0x7F             ;
        LED[active_digit-1].data &= SMG[temp[0]]     ;
        Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                     87,Red,Black                 )  ;        
        break                                        ;
      case 4:        
        temp[0] = Find_Data(LED[active_digit-1].data);
        if(--temp[0]>250)   temp[0] = 9              ;
        LED[active_digit-1].data |= 0x7F             ;
        LED[active_digit-1].data &= SMG[temp[0]]     ;
        Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                     87,Red,Black                 )  ;        
        break                                        ;
      }
      for(i=0;i<3;i++)
      {
        if(!InCheckScope(TPoint.pos[0],
                         TPoint.pos[1],
                         &Disp_Check[i]))
        {
          if(Disp_Check[i].status&CHECK_CHECKED)
          {
            unCheckCheck(&Disp_Check[i])             ;
            switch(i)
            {
            case 0:
              LED_Flicker_Digit(active_digit,0)      ; 
              temp[0] = Find_Data(LED[active_digit-1].data)                ;
              if(!(LED[active_digit-1].mode&DIGIT_OFF))
              {
                Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                             87,Red,Black                  )     ; 
                if(!(LED[active_digit-1].data&SEGDP))
                  DrawRectFill((active_digit-1)*29+30,120,3,3,Red)          ;
              }
              break                                  ;
            case 1:
              temp[0] = Find_Data(LED[active_digit-1].data)  ;
              Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                           87,Red,Black                  )   ; 
              if(!(LED[active_digit-1].data&SEGDP))
                  DrawRectFill((active_digit-1)*29+30,120,3,3,Red)    ;
              LED[active_digit-1].mode  &=~DIGIT_OFF ;
              break                                  ;
            case 2:
              LED[active_digit-1].data  |= SEGDP     ;
              DrawRectFill((active_digit-1)*29+30,120,
                            3,3,Black)               ;
              break                                  ;
            }
          }
          else
          {
            CheckCheck(&Disp_Check[i])               ;
            switch(i)
            {
            case 0:
              LED_Flicker_Digit(active_digit,1)      ; 
              break                                  ;
            case 1:
              temp[0] = Find_Data(LED[active_digit-1].data)  ;
              Draw7Seg2436(temp[0],(active_digit-1)*29+5,
                           87,Black,Black                )   ; 
              DrawRectFill((active_digit-1)*29+30,120,
                            3,3,Black)               ;
              LED[active_digit-1].mode  |= DIGIT_OFF ;
              break                                  ;
            case 2:
              LED[active_digit-1].data  &=~SEGDP     ;
              DrawRectFill((active_digit-1)*29+30,120,
                           3,3,Red)                  ;
              break                                  ;
            }
          }
          break                                      ;
        }
      }
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;      
    }
    else if(Event&EVENT_FLICKER)
    {
      if(flicker++>0)
      {
        for(i=1;i<9;i++)
        {
          temp[0] = Find_Data(LED[i-1].data)                ;
          if(  !(LED[i-1].mode&DIGIT_OFF)
             && (LED[i-1].mode&DIGIT_FLICKER))
          {
            Draw7Seg2436(temp[0],(i-1)*29+5,
                         87,Red,Black                  )     ; 
            if(!(LED[i-1].data&SEGDP))
              DrawRectFill((i-1)*29+30,120,3,3,Red)          ;
          }
        }
        flicker  = 0x00                                    ;
      }
      else
      {
        for(i=1;i<9;i++)
        {
          temp[0] = Find_Data(LED[i-1].data)                ;
          if(  !(LED[i-1].mode&DIGIT_OFF)
             && (LED[i-1].mode&DIGIT_FLICKER))
          {
            Draw7Seg2436(temp[0],(i-1)*29+5,
                         87,Black,Black                 )   ; 
            if(!(LED[i-1].data&SEGDP))
              DrawRectFill((i-1)*29+30,120,3,3,Black)      ;
          }
        }
      }                  
    }
  } 
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  DeleteButton(3)                                    ;
  DeleteButton(4)                                    ;
  for(i=0;i<8;i++)
    LED[i].mode = 0x00                               ;
  OSTaskResume(CPU_USAGE_TASK_PRIO)                  ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_TP_Adj(char *NC)                                           //
//������������У������                                                       //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_TP_Adj(char *NC)
{
  unsigned int temp[8]                               ;
  Color     = Green                                  ;
  Color_BK  = Black                                  ;
  OSTaskSuspend(KEY_TP_TASK_PRIO)                    ;
  TP_Init()                                          ;
  Clear_LCD(Color_BK)                                ; 
  PutStringCN24(58,120,"������У��")                 ;
  Color     = Cyan                                   ;
  PutStringCN16(30,165,"���ô��رʵ���Ե�����")     ;
  for(;;)
  {
    Draw_Test_Point(20,20,Yellow)                    ;
    for(;;)
    {
      OSTimeDly(300)                                 ;
      if(Read_TP_Twice(&temp[0],&temp[1]))  break    ;
      if(ReadKey()==Cancel)  goto Exit_TP_Adjust     ;
    }
    DrawRectFill(0,0,40,40,Color_BK)                 ;
    Wait_Pen_Up()                                    ;
  
    Draw_Test_Point(220,20,Yellow)                   ;
    for(;;)
    {
      OSTimeDly(300)                                 ;
      if(Read_TP_Twice(&temp[2],&temp[3]))  break    ;
      if(ReadKey()==Cancel)  goto Exit_TP_Adjust     ;
    }
    DrawRectFill(200,0,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(20,300,Yellow)                   ;
    for(;;)
    {
      OSTimeDly(300)                                 ;
      if(Read_TP_Twice(&temp[4],&temp[5]))  break    ;
      if(ReadKey()==Cancel)  goto Exit_TP_Adjust     ;
    }
    DrawRectFill(0,280,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(220,300,Yellow)                  ;
    for(;;)
    {
      OSTimeDly(300)                                 ;
      if(Read_TP_Twice(&temp[6],&temp[7]))  break    ;       
      if(ReadKey()==Cancel)  goto Exit_TP_Adjust     ;
    }
    DrawRectFill(200,280,40,40,Color_BK)             ;
    Wait_Pen_Up()                                    ;
    if(TP_Adjust(temp))         break                ;
  }
Exit_TP_Adjust:  
  OSTaskResume(KEY_TP_TASK_PRIO)                     ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Paint(char *NC)                                            //
//��������ͼ����                                                             //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Paint(char *NC)
{
  unsigned int pos[2]                                ;             
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  OSTaskSuspend(KEY_TP_TASK_PRIO)                    ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"����")                          ;
  PutStringCN16(4,300,"���")                        ;
  PutStringCN16(200,300,"�˳�")                      ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  for(;;)
  {
    if(Read_TP_Twice(&pos[0],&pos[1]))
    {
      Convert_Pos(pos[0],pos[1], &pos[0],&pos[1] )   ;
      
      if(pos[0]<50&&pos[1]>295)
        DrawRectFill(0,25,240,270,WINDOW_BK_COLOR)   ;
      else 
      if(  (pos[0]<230&&pos[0]>200&&pos[1]>295)
         ||(pos[0]>220&&pos[1]<20             ))
        break                                        ;
      else if(pos[1]<280&&pos[1]>30)
        DrawRectFill(pos[0],pos[1],4,4,Red)          ;
    }
    OSTimeDly(3)                                     ;
  }
  OSTaskResume(KEY_TP_TASK_PRIO)                     ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Task_Info(char *NC)                                          //
//����������ܿ��Ƴ���                                                       //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Task_Info(char *NC)
{
  OS_TCB       TaskData                              ;
  OS_STK_DATA  data                                  ;
  char         key,Event,rx_char                     ;
  char         i,j=0                                 ;
  char         string[10]                            ;
  union
  {
    unsigned int  pos[2]                             ;
    char byte[4]                                     ;
  }TPoint                                            ;
  INT8U     err[1]                                   ;
  char *ptmb                                         ;
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"������Ϣ")                      ;
  PutStringCN16(200,300,"�˳�")                      ;
  CreateButton(1,"����"  ,0  , 30, 60,20)            ;
  CreateButton(2,"ջ�ռ�",60 , 30, 60,20)            ;
  CreateButton(3,"ռ��"  ,120, 30, 60,20)            ;
  CreateButton(4,"����"  ,180, 30, 60,20)            ;
  for(i=9;i<16;i++)
  {
    if(i==10)   continue                             ;
    OSTaskQuery(i,&TaskData)                         ;
    PutStringEN16(2,60+j*25,TaskData.OSTCBTaskName)  ; 

    OSTaskStkChk(i,&data)                            ;
//    Dec2Str(string,data.OSFree+data.OSUsed)          ;
    sprintf(string,"%4ld",data.OSFree+data.OSUsed)   ;
    PutStringEN16( 65,60+j*25,(unsigned char*)string);
    
    Dec2Str(string,data.OSUsed)                      ;
    PutStringEN16(125,60+j*25,(unsigned char*)string);
    
    Dec2Str(string,data.OSFree)                      ;
    PutStringEN16(185,60+j*25,(unsigned char*)string);
    j++                                              ;
  }
  for(;;)
  {    
    ptmb           = OSMboxPend(MBKeyTP,0,err)       ;
    Event          = *(ptmb++)                       ;
    key            = *(ptmb++)                       ;
    rx_char        = *(ptmb++)                       ;
    TPoint.byte[0] = *(ptmb++)                       ;
    TPoint.byte[1] = *(ptmb++)                       ;
    TPoint.byte[2] = *(ptmb++)                       ;
    TPoint.byte[3] = *(ptmb++)                       ;
    rx_char        = rx_char                         ;    
    key            = key                             ;
    if(Event&EVENT_TP_PRESSED)  
    {  
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;      
    }
  }   
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  DeleteButton(3)                                    ;
  DeleteButton(4)                                    ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Port_Comm(char *NC)                                        //
//�������˿ڽ���/���ͳ���/����                                               //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Port_Comm(char *NC)
{
  char Event,rec_on = 0                              ;
  unsigned char rx_char[2],PORT = PORT_USB,i         ;
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
  char key,temp[20]                                  ;
  char index,down_id=0xFF                            ;
  union
  {
    unsigned int  pos[2]                             ;
    char byte[4]                                     ;
  }TPoint                                            ;
  INT8U     err[1]                                   ;
  char *ptmb                                         ;
  rx_char[1]= 0x00                                   ;
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"�˿�����")                      ;
  PutStringCN16(200,300,"�˳�")                      ;
  DrawPanel(5, 40,230,100,"���մ���")                ;
  DrawPanel(5,160,230,40,"���ʹ���")                 ;
  DrawPanel(5,220,230,65,"��������")                 ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  CreateButton(1,"����",180, 53,50,20)               ;
  CreateButton(2,"����",180, 83,50,20)               ;
  CreateButton(3,"���",180,113,50,20)               ;
  CreateButton(4,"����",180,173,50,20)               ;
  CreateButton(5,"Ĭ��",180,228,50,20)               ;
  CreateButton(6,"����",180,258,50,20)               ;
  CreateView(&Rec_View,68,11,52,160,83)              ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"RS232")  ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RS485")  ;
  CreateCheck(&Port_Check[2],134,268,12,12,"USB")    ;
  CheckCheck(&Port_Check[2])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,16,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,47,235,56,22)                      ; 
  IniEditDig(2,1,0,1,8,"9600")                       ;
  CreateEditDig(3,117,235,19,22)                     ; 
  IniEditDig(3,1,0,1,1,"8")                          ;
  CreateEditDig(4,152,235,19,22)                     ; 
  IniEditDig(4,1,0,1,1,"1")                          ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  PutStringEN16(10,238,"BAUD")                       ;
  PutStringEN16(107,238,"D")                         ;
  PutStringEN16(141,238,"S")                         ;

  for(;;)
  {
    ptmb           = OSMboxPend(MBKeyTP,0,err)       ;
    Event          = *(ptmb++)                       ;
    key            = *(ptmb++)                       ;
    rx_char[0]     = *(ptmb++)                       ;
    TPoint.byte[0] = *(ptmb++)                       ;
    TPoint.byte[1] = *(ptmb++)                       ;
    TPoint.byte[2] = *(ptmb++)                       ;
    TPoint.byte[3] = *(ptmb++)                       ;
    key            = key                             ;
    if(Event&RX_RECEIVED&&rec_on)
      ViewMessage(&Rec_View,EN_INPUT,rx_char)        ;
    else if(Event&EVENT_KEY_PRESSED)
    {
      switch(key)
      {
      case 1:case 2:case 3:case 4:case 5:
      case 6:case 7:case 8:case 9:case 0:
      case Left: case Right: case Backspace:
        EditDigMessage(key,temp)                     ; 
        break                                        ;    
      case Enter:
        OSMutexPend(SPIMutex,0,err)                  ;
        ViewMessage(&Rec_View,EN_INPUT,"k")          ;// test code
        ViewMessage(&Rec_View,CN_INPUT,"��")         ;// test code        
        OSMutexPost(SPIMutex)                        ;
        break                                        ;
      case Cancel:
        ClearViewContent(&Rec_View)                  ;// test code
        break                                        ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==0xFF)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        OSMutexPost(SPIMutex)                        ;
      }
      else if(index!=down_id)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        SetButtonDown(down_id)                       ;
        OSMutexPost(SPIMutex)                        ;
      }
    }
    else if(Event&EVENT_TP_PRESSED)  
    {  
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OSMutexPend(SPIMutex,0,err)                    ;
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OSMutexPost(SPIMutex)                          ;
      switch(index)
      {
      case 1:
        if(rec_on==1)
        {
          rec_on = 0                                 ;
          SetButtonTitle(1,"����")                   ;
        }
        else
        {
          rec_on = 1                                 ;
          SetButtonTitle(1,"ֹͣ")                   ;
        }
        break                                        ;
      case 3:
        ClearViewContent(&Rec_View)                  ;      
        break                                        ;
      case 4:
        ReadEditDig(1,(unsigned char*)temp)          ;
        if(PORT==PORT_USB)
          UTX_PROC(temp)                             ;
        else 
        if(PORT==PORT_RS232)
          RS232TX_PROC(temp)                         ;
        else
        if(PORT==PORT_RS485)
          RS485TX_PROC(temp)                         ;          
        break                                        ;
      }
      
      for(i=0;i<3;i++)
      {
        if(!InCheckScope(TPoint.pos[0],
                         TPoint.pos[1],
                         &Port_Check[i]))
        {
          unCheckCheck(&Port_Check[0])               ;
          unCheckCheck(&Port_Check[1])               ;
          unCheckCheck(&Port_Check[2])               ;
          CheckCheck(&Port_Check[i])                 ;
          if(i==0)   PORT  = PORT_RS232              ;
          if(i==1)   PORT  = PORT_RS485              ;
          if(i==2)   PORT  = PORT_USB                                                                                                                                                                                             ;          
        }
      }
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;
    }
    else if(Event&EVENT_FLICKER)          
      Flicker_Cursor()                               ;
  }
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  DeleteButton(3)                                    ;
  DeleteButton(4)                                    ;
  DeleteButton(5)                                    ;
  DeleteButton(6)                                    ;
  DeleteEditDig(1)                                   ;
  DeleteEditDig(2)                                   ;
  DeleteEditDig(3)                                   ;
  DeleteEditDig(4)                                   ;
  return 0x00                                        ;  
}


char Proc_IrDA_Comm(char *NC)
{
  char Event,rx_char[2],rec_on = 0,rec_on_bk         ;
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
  char tx_buf[20]                                    ;
  char key,temp[1]                                   ;
  char index,down_id=0xFF                            ;
  union
  {
    unsigned int  pos[2]                             ;
    char byte[4]                                     ;
  }TPoint                                            ;
  INT8U     err[1]                                   ;
  char *ptmb                                         ;
  rx_char[1]= 0x00                                   ;
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"���⴫��")                      ;
  PutStringCN16(200,300,"�˳�")                      ;
  DrawPanel(5, 40,230,100,"���մ���")                ;
  DrawPanel(5,160,230,40,"���ʹ���")                 ;
  DrawPanel(5,220,230,65,"Э������")                 ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  CreateButton(1,"����",180, 53,50,20)               ;
  CreateButton(2,"����",180, 83,50,20)               ;
  CreateButton(3,"���",180,113,50,20)               ;
  CreateButton(4,"����",180,173,50,20)               ;
  CreateButton(5,"Ĭ��",180,228,50,20)               ;
  CreateButton(6,"����",180,258,50,20)               ;
  CreateView(&Rec_View,68,11,52,160,83)              ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"RC-5")   ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RC-6")   ;
  CreateCheck(&Port_Check[2],134,268,12,12,"NEC")    ;
  CheckCheck(&Port_Check[0])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,16,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,47,235,56,22)                      ; 
  IniEditDig(2,1,0,1,8,"1200")                       ;
  CreateEditDig(3,117,235,19,22)                     ; 
  IniEditDig(3,1,0,1,1,"8")                          ;
  CreateEditDig(4,152,235,19,22)                     ; 
  IniEditDig(4,1,0,1,1,"1")                          ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  PutStringEN16(10,238,"BAUD")                       ;
  PutStringEN16(107,238,"D")                         ;
  PutStringEN16(141,238,"S")                         ;
  IrDA_PORT_DIR   |= IrDA_OUT                        ;
  IrDA_PORT_OUT   &=~IrDA_OUT                        ;

  for(;;)
  {
    ptmb           = OSMboxPend(MBKeyTP,0,err)       ;
    Event          = *(ptmb++)                       ;
    key            = *(ptmb++)                       ;
    rx_char[0]     = *(ptmb++)                       ;
    TPoint.byte[0] = *(ptmb++)                       ;
    TPoint.byte[1] = *(ptmb++)                       ;
    TPoint.byte[2] = *(ptmb++)                       ;
    TPoint.byte[3] = *(ptmb++)                       ;
    key            = key                             ;
    if(Event&RX_RECEIVED&&rec_on)
      ViewMessage(&Rec_View,EN_INPUT,(unsigned char *)rx_char)        ;
    else if(Event&EVENT_KEY_PRESSED)
    {
      switch(key)
      {
      case 1:case 2:case 3:case 4:case 5:
      case 6:case 7:case 8:case 9:case 0:
      case Left: case Right: case Backspace:
        EditDigMessage(key,temp)                     ; 
        break                                        ;    
      case Enter:
        OSMutexPend(SPIMutex,0,err)                  ;
        ViewMessage(&Rec_View,EN_INPUT,"k")          ; // test code
        ViewMessage(&Rec_View,CN_INPUT,"��")         ; // test code        
        OSMutexPost(SPIMutex)                        ; 
        break                                        ;
      case Cancel:
        ClearViewContent(&Rec_View)                  ; // test code
        break                                        ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==0xFF)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        OSMutexPost(SPIMutex)                        ;
      }
      else if(index!=down_id)
      {
        OSMutexPend(SPIMutex,0,err)                  ;
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        SetButtonDown(down_id)                       ;
        OSMutexPost(SPIMutex)                        ;
      }
    }
    else if(Event&EVENT_TP_PRESSED)  
    {  
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OSMutexPend(SPIMutex,0,err)                    ;
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OSMutexPost(SPIMutex)                          ;
      switch(index)
      {
      case 1:
        if(rec_on==1)
        {
          rec_on = 0                                 ;
          SetButtonTitle(1,"����")                   ;
        }
        else
        {
          rec_on = 1                                 ;
          SetButtonTitle(1,"ֹͣ")                   ;
        }
        break                                        ;
      case 3:
        ClearViewContent(&Rec_View)                  ;      
        break                                        ;
      case 4:
        Init_IRUART()                                ;
        ReadEditDig(1,(unsigned char*)tx_buf)        ;
        OSTimeDly(6)                                 ;
        rec_on_bk = rec_on                           ;
        rec_on    = 0                                ;
        IrDATX_PROC(tx_buf)                          ;
        IrDA_PORT_SEL  &=~IrDA_OUT                   ; // ѡ�����Ź���
        rec_on    = rec_on_bk                        ;
        break                                        ;
      }
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;
    }
    else if(Event&EVENT_FLICKER)          
      Flicker_Cursor()                               ;
  }
  DeleteButton(1)                                    ;
  DeleteButton(2)                                    ;
  DeleteButton(3)                                    ;
  DeleteButton(4)                                    ;
  DeleteButton(5)                                    ;
  DeleteButton(6)                                    ;
  DeleteEditDig(1)                                   ;
  DeleteEditDig(2)                                   ;
  DeleteEditDig(3)                                   ;
  DeleteEditDig(4)                                   ;
  return 0x00                                        ;  
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Show_Pic(char *NC)                                         //
//������ͼƬ�鿴����                                                         //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Show_Pic(char *NC)
{
  unsigned int temp[2]                               ;
  OSTaskSuspend(KEY_TP_TASK_PRIO)                    ;
  ShowBMP240320(BMP_PIC1)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
  {
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
    OSTimeDly(300)                                   ;
  }
  ShowBMP240320(BMP_PIC2)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
  {
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
    OSTimeDly(300)                                   ;
  }
  OSTaskResume(KEY_TP_TASK_PRIO)                     ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������char Proc_Show_Pic(char *NC)                                         //
//������ͼƬ�鿴����                                                         //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Stat(char *NC)
{
  unsigned int temp[2]                               ;
  
  OSTaskSuspend(KEY_TP_TASK_PRIO)                    ;
  ShowBMP240320(BMP_PIC1)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
  {
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
    OSTimeDly(300)                                   ;
  }
  ShowBMP240320(BMP_PIC2)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
  {
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
    OSTimeDly(300)                                   ;
  }
  OSTaskResume(KEY_TP_TASK_PRIO)                     ;
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//������unsigned char Find_Data(unsigned char coded)                         //
//���������������ʾ���뷴����ʾ����                                         //
//������NC ����������                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
unsigned char Find_Data(unsigned char coded)
{
  unsigned char i                                    ;
  for(i=0;i<10;i++)
  {
    if(SMG[i]==(coded|(~DPOINT)))
      break                                          ;
  }
  return i                                           ;
}


/*
*********************************************************************************************************
*                                        CREATE APPLICATION TASKS
*
* Description: This function creates the application tasks.
*
* Arguments  : none.
*
* Note(s)    : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
  CPU_INT08U  err[1]                                                  ;
  WDTCTL   = WDTPW + WDTHOLD                                          ; // ֹͣ��ʱ������

  OSTaskCreateExt(LED_TASK                                        ,  
                  (void *)0                                       ,
                  (OS_STK *)&LED_TASK_STK[LED_TASK_STACK_SIZE - 1],
                  LED_TASK_PRIO                                   ,
                  LED_TASK_PRIO                                   ,
                  (OS_STK *)&LED_TASK_STK[0]                      ,
                  LED_TASK_STACK_SIZE                             ,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR        )  ; // ����7�������ˢ������
//OSTaskSuspend(LED_TASK_PRIO)   ;
//OSTimeDly(5000)                ;
  OSTaskCreateExt(GUI_TASK                                        ,  
                  (void *)0                                       ,
                  (OS_STK *)&GUI_TASK_STK[GUI_TASK_STACK_SIZE - 1],
                  GUI_TASK_PRIO                                   ,
                  GUI_TASK_PRIO                                   ,
                  (OS_STK *)&GUI_TASK_STK[0]                      ,
                  GUI_TASK_STACK_SIZE                             ,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR        )  ; // ����ͼ�ν����������  

  OSTaskCreateExt(MENU_OP_TASK                                    ,  
                  (void *)0                                       ,
                  (OS_STK *)&MENU_OP_TASK_STK[MENU_OP_TASK_STACK_SIZE - 1],
                  MENU_OP_TASK_PRIO                               ,
                  MENU_OP_TASK_PRIO                               ,
                  (OS_STK *)&MENU_OP_TASK_STK[0]                  ,
                  MENU_OP_TASK_STACK_SIZE                         ,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR        )  ; // �����˵���������
  
  OSTaskCreateExt(KEY_TP_TASK                                     ,  
                  (void *)0                                       ,
                  (OS_STK *)&KEY_TP_TASK_STK[KEY_TP_TASK_STACK_SIZE - 1],
                  KEY_TP_TASK_PRIO                                   ,
                  KEY_TP_TASK_PRIO                                   ,
                  (OS_STK *)&KEY_TP_TASK_STK[0]                      ,
                  KEY_TP_TASK_STACK_SIZE                             ,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR        )  ; // ����������(����/������)����
                                             
  OSTaskCreateExt(CPU_USAGE_TASK                                      ,  
                  (void *)0                                           ,
                  (OS_STK *)&CPU_USAGE_TASK_STK[CPU_USAGE_TASK_STACK_SIZE - 1],
                  CPU_USAGE_TASK_PRIO                                   ,
                  CPU_USAGE_TASK_PRIO                                   ,
                  (OS_STK *)&CPU_USAGE_TASK_STK[0]                      ,
                  CPU_USAGE_TASK_STACK_SIZE                             ,
                  (void *)0,
                  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR        )  ; // ����CPU��������ʾ����
  
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_START_TASK_PRIO, "START"       , err)           ; // ����������
    OSTaskNameSet(CPU_USAGE_TASK_PRIO, "STAT"        , err)           ; // ����������
    OSTaskNameSet(LED_TASK_PRIO      , "7SEG"        , err)           ; // ����������
    OSTaskNameSet(GUI_TASK_PRIO      , "GUI"         , err)           ;
    OSTaskNameSet(KEY_TP_TASK_PRIO   , "INPUT"       , err)           ;
    OSTaskNameSet(MENU_OP_TASK_PRIO  , "MENU"        , err)           ;
#endif
  MBKeyTP   = OSMboxCreate((void *)0)                                 ; // ��������(����/������)����
  MBFunc    = OSMboxCreate((void *)0)                                 ; // ����������������
  MBRX      = OSMboxCreate((void *)0)                                 ; // �����˿ڽ�������
  SPIMutex  = OSMutexCreate(SPI_MUTEX_PRIO,err)                       ; // ����SPI���߻����ź���
  WDTCTL    = WDT_MDLY_8                                              ; // �ָ���ʱ������
  P7DIR  |= LED_PWR                                                   ;
  P7OUT  &=~LED_PWR                                                   ;    
}


void UTX_PROC(char *tx_buf)
{
#if      OS_CRITICAL_METHOD == 3
  OS_CPU_SR cpu_sr                         ;
#endif
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    OS_ENTER_CRITICAL()                    ;
    UCA1TXBUF = *tx_buf++                  ; 
    while (!(UCA1IFG&UCTXIFG))             ; 
    OS_EXIT_CRITICAL()                     ;
  }
}

void RS232TX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                  ; 
    while (!(UCA3IFG&UCTXIFG))             ; 
  }
}

void RS485TX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  RS485_OUT                                ;
  for(i=0;i<length;i++)
  {
    UCA3TXBUF = *tx_buf++                  ; 
    while (!(UCA3IFG&UCTXIFG))             ; 
  }
  __delay_cycles(100000)                   ;
  RS485_IN                                 ;
}

void IrDATX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    UCA2TXBUF = *tx_buf++                  ; 
    __delay_cycles(200000)                 ;
//    OSTimeDly(30)                          ;
    while (!(UCA2IFG&UCTXIFG))             ; 
  }
}

void Dec2Str(char *string,int data)
{
  int temp                                 ;  
  unsigned char i=1,j=0                    ;
  temp      = data                         ;
  string[0] = temp%10+0x30                 ;
  for(;;)
  {    
    temp     = temp/10                     ;
    if(temp==0)
      break                                ; 
    string[i+1]  = 0                       ;
    for(j=i;j>0;j--)
      string[j]= string[j-1]               ;
    string[0]    = temp%10+0x30            ;
    i++                                    ;
    j++                                    ;
  }
}


char Proc_Go_Sleep(char *NC)
{
  if(!ConfirmBox("������ʾ","ȷʵҪ�ر�ϵͳ��"))
    GotoSleep()                                      ;
  return 0x00                                        ;
}

void GotoSleep(void)
{
  P1DIR  = 0x00  ;
  P2DIR  = 0x00  ;
  P3DIR  = 0x00  ;
  P4DIR  = 0x00  ;
  P5DIR  = 0x00  ;
  P6DIR  = 0x00  ;
  P7DIR  = 0x00  ;
  P8DIR  = 0x00  ;
  P9DIR  = 0x00  ;
  P10DIR = 0x00  ;
  P11DIR = 0x00  ;
  P1REN  = 0x00  ;
  P2REN  = 0x00  ;
  P3REN  = 0x00  ;
  P4REN  = 0x00  ;
  P5REN  = 0x00  ;
  P6REN  = 0x00  ;
  P7REN  = 0x00  ;
  P8REN  = 0x00  ;
  P9REN  = 0x00  ;
  P10REN = 0x00  ;
  P11REN = 0x00  ;
  MAIN_POWER_OFF ;
}
