//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板RTOS+BWGUI演示程序                                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "main.h"

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  char temp = 0                                                    ;
  OS_EnterInterrupt()                                              ;
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break                                                     ; 
  case 2:                                                            
      temp             = UCA1RXBUF                                 ;
      OS_PutMailCond1(&MBRX, &temp)                                ;
      break                                                        ;
  case 4:break                                                     ;  
  default: break                                                   ;  
  }  
  OS_LeaveInterrupt();
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_ISR(void)
{
  char temp = 0                                                    ;
  OS_EnterInterrupt()                                              ;
  switch(__even_in_range(UCA2IV,4))
  {
  case 0:break                                                     ; 
  case 2:                                                            
      temp             = UCA2RXBUF                                 ;
      OS_PutMailCond1(&MBRX, &temp)                                ;
      break                                                        ;
  case 4:break                                                     ; 
  default: break                                                   ;  
  }  
  OS_LeaveInterrupt();
}

#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
  char temp = 0                                                    ;
  OS_EnterInterrupt()                                              ;
  switch(__even_in_range(UCA3IV,4))
  {
  case 0:break                                                     ; 
  case 2:                                                            
      temp             = UCA3RXBUF                                 ;
      OS_PutMailCond1(&MBRX, &temp)                                ;
      break                                                        ;
  case 4:break                                                     ; 
  default: break                                                   ;  
  }  
  OS_LeaveInterrupt();
}

int main(void) 
{
  WDTCTL   = WDTPW + WDTHOLD                         ;
  Init_CLK()                                         ;
  Init_J60SPI()                                      ;
  MAIN_POWER_ON                                      ;
  __delay_cycles(5000000)                            ;
  P9DIR   |= IrDA_OUT                                ;
  P9OUT   &=~IrDA_OUT                                ;
  Init_EtherNet()                                    ;
  ENC_SLEEP()                                        ; // 省电  
    
  OS_IncDI()                                         ; // 屏蔽中断
  OS_InitKern()                                      ; // 初始化系统内核
  OS_InitHW()                                        ; // 初始化硬件
  Init_Func()                                        ;
  Init_UART()                                        ;  
  Init_RSUART()                                      ;
  Ini_LED(8)                                         ;
  Init_ADC()                                         ;
  Init_RTC()                                         ;
  LCD_Init()                                         ;

  LED_POWER_ON                                       ;
  BackLight()                                        ;
  OS_CREATERSEMA(&SemaLCD)                           ;
  OS_CREATERSEMA(&SemaSPI)                           ;
  LED_Disp_Float(3.1415925,7,FIT_ZERO)               ;
  LED_Flicker_Digit(8,1)                             ;  
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  
  OS_CREATETASK(&Seg7LED_TASK_TCB, 
                "Seg7LedTask", 
                Seg7LedRefresh, 100, 
                Seg7LED_TASK_STACK  )                ;
  
  OS_CREATETASK(&LCD_TASK_TCB, 
                "LCD_Task", 
                LCD_Task, 100, 
                LCD_TASK_STACK  )                    ;
  
  OS_CREATETASK(&MENU_OP_TASK_TCB, 
                "MENU_OP_Task", 
                MENU_OP_Task, 100, 
                MENU_OP_TASK_STACK)                  ;

  OS_CREATETASK(&KEY_TP_TASK_TCB, 
                "KEY_TP_Task", 
                Key_TP_Task, 100, 
                KEY_TP_TASK_STACK  )                 ;
  
  
  OS_Start()                                         ; 
  return 0                                           ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Key_TP_Task(void)                                               //
//描述：键盘与触摸屏扫描，编辑控件光标闪烁                                   //
//		                                                             //
//---------------------------------------------------------------------------//
void Key_TP_Task(void)
{
  unsigned char flicker_cnt = 0                             ;
  char key = 0xFF,last_key = 0xFF, down_cnt = 0, tp_cnt = 0 ;
  char rx_char                                              ;
  union 
  {
    unsigned int  pos[2]                                    ;
    char byte[4]                                            ;
  }TPoint                                                   ;
  char event                                                ;
  Init_KeyPad()                                             ; 
  TP_Init()                                                 ;
  OS_CREATEMB(&MBKeyTP, 1, sizeof(MBKeyTPBuffer),
              &MBKeyTPBuffer                     )          ; // 创建键盘、触摸屏消息邮箱
  OS_CREATEMB(&MBRX,1,sizeof(MBRXBuffer),&MBRXBuffer)       ; // 创建端口接收邮箱
  for(;;)
  {
    event  = 0x00                                           ;
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
    OS_Use(&SemaSPI)                                        ;
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
    OS_Unuse(&SemaSPI)                                      ;
    
    if(OS_GetMessageCnt(&MBRX))
    {
      OS_GetMail(&MBRX,&rx_char)                            ;
      event    |= RX_RECEIVED                               ;
    }
    if(event)                                                 // 发送系统消息
    {
      if(  !OS_GetSuspendCnt(&LCD_TASK_TCB)
         ||!OS_GetSuspendCnt(&MENU_OP_TASK_TCB))
      {
        OS_PutMail1(&MBKeyTP, &event)                       ;
        OS_PutMail1(&MBKeyTP, &last_key)                    ;
        OS_PutMail1(&MBKeyTP, &rx_char)                     ;
        OS_PutMail1(&MBKeyTP, &TPoint.byte[0])              ;
        OS_PutMail1(&MBKeyTP, &TPoint.byte[1])              ;
        OS_PutMail1(&MBKeyTP, &TPoint.byte[2])              ;
        OS_PutMail1(&MBKeyTP, &TPoint.byte[3])              ; 
      }
    }
    last_key = key                                          ;
    OS_Delay(10)                                            ;
  }
}


void LCD_Task(void) 
{
  RunMenu(0)                                                ;
}

void MENU_OP_Task(void)
{
  char func_index,event                                     ;
  OS_CREATEMB(&MBFunc,1,sizeof(MBFuncBuffer),&MBFuncBuffer) ;  
  for(;;)
  {
    OS_WaitMail(&MBFunc)                                    ;    
    OS_GetMail1(&MBFunc, &event)                            ;
    if(event&EVENT_OP_STARTED)
    {
      OS_GetMail1(&MBFunc, &func_index)                     ;
      Item_OP[func_index](&func_index)                      ;      
      if(OS_GetSuspendCnt(&LCD_TASK_TCB))
        OS_Resume(&LCD_TASK_TCB)                            ;
    }
  }
}

void Init_Func(void)
{
  Item_OP[0]   = Proc_Show_Clock                     ;
  Item_OP[1]   = Proc_Display                        ;
  Item_OP[2]   = Proc_Port_Comm                      ;
  Item_OP[3]   = Proc_IrDA_Comm                      ;
  Item_OP[4]   = Proc_Net_Comm                       ;
  Item_OP[5]   = Proc_Paint                          ;
  Item_OP[6]   = Proc_Show_Pic                       ;
  Item_OP[7]   = Proc_TP_Adj                         ;
  Item_OP[8]   = Proc_Go_Sleep                       ;
}

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

//***************************************************************************//
//                                                                           //
//                 初始化RTC                                                 //
//                                                                           //
//***************************************************************************//
void Init_RTC(void)
{
  RTCCTL01 = RTCSSEL_0 + RTCMODE                     ; // 时钟模式,十六进制格式
  SetRTCYEAR(2010)                                   ; 
  SetRTCMON(10)                                      ;
  SetRTCDAY(24)                                      ;
  SetRTCDOW(0)                                       ;
  SetRTCHOUR(10)                                     ;
  SetRTCMIN(9)                                       ;
  SetRTCSEC(45)                                      ;
}

void DisplayTime(void)
{
  unsigned char string[10]                           ;
  int hour,minute,second                             ;
  hour    = GetRTCHOUR()                             ;
  minute  = GetRTCMIN()                              ;
  second  = GetRTCSEC()                              ;
  sprintf((char *)string,"%02d-%02d-%02d",
           hour,minute,second             )          ;
  OS_Use(&SemaLCD)                                   ; 
  Color    = White                                   ;
  Color_BK = Blue                                    ;
  PutStringEN16(84,300,string)                       ;
  Color    = WINDOW_COLOR                            ;
  Color_BK = WINDOW_BK_COLOR                         ;
  OS_Unuse(&SemaLCD)                                 ; 
}

void Init_PSPI(void)
{
  P10DIR   |= PMOSI+PSCK+PNSS                        ;
  P10DIR   &=~PMISO                                  ;
}

char Proc_TP_Adj(char *NC)
{
  unsigned int temp[8]                               ;
  Color     = Green                                  ;
  Color_BK  = Black                                  ;
  TP_Init()                                          ;
  Clear_LCD(Color_BK)                                ; 
  PutStringCN24(58,120,"触摸屏校正")                 ;
  Color     = Cyan                                   ;
  PutStringCN16(30,165,"请用触控笔点测试点中心")     ;
  for(;;)
  {
    Draw_Test_Point(20,20,Yellow)                    ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[0],&temp[1]))  break    ;
      if(ReadKey()==Cancel)  return 0x00             ;
    }
    DrawRectFill(0,0,40,40,Color_BK)                 ;
    Wait_Pen_Up()                                    ;
  
    Draw_Test_Point(220,20,Yellow)                   ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[2],&temp[3]))  break    ;
      if(ReadKey()==Cancel)  return 0x00             ;
    }
    DrawRectFill(200,0,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(20,300,Yellow)                   ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[4],&temp[5]))  break    ;
      if(ReadKey()==Cancel)  return 0x00             ;
    }
    DrawRectFill(0,280,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(220,300,Yellow)                  ;
    for(;;)
    {
      if(Read_TP_Twice(&temp[6],&temp[7]))  break    ;       
      if(ReadKey()==Cancel)  return 0x00             ;
    }
    DrawRectFill(200,280,40,40,Color_BK)             ;
    Wait_Pen_Up()                                    ;
    if(TP_Adjust(temp))         break                ;
  }
  return 0x00                                        ;
}


char Proc_Paint(char *NC)
{
  unsigned int pos[2]                                ;             
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"画笔")                          ;
  PutStringCN16(4,300,"清空")                        ;
  PutStringCN16(200,300,"退出")                      ;
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
  }
  return 0x00                                        ;
}

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
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
  Clear_LCD(Color_BK)                                ;
  DrawRectFill(0,0,240,25,Blue)                      ;
  DrawRectFill(0,295,240,25,Blue)                    ;
  ShowIcon24(217,0,24,23,ICON_CLOSE)                 ;
  Color    = 0xFFFF                                  ;
  Color_BK = Blue                                    ;
  PutStringCN16(4,4,"数码管控制")                    ;
  PutStringCN16(200,300,"退出")                      ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  DrawFrame(3,80,232,48)                             ;
  DrawRectFill(4,81,232,46,Black)                    ;
  DrawPanel(6,160,228,72,"")                         ;
  CreateButton(1,"＜",15, 175,45,20)                 ;
  CreateButton(2,"＞",70, 175,45,20)                 ;
  CreateButton(3,"＋",125,175,45,20)                 ;
  CreateButton(4,"－",180,175,45,20)                 ;  
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
  CreateCheck(&Disp_Check[0],25 ,210,12,12,"闪烁")   ;
  CreateCheck(&Disp_Check[1],90 ,210,12,12,"消隐")   ;
  CreateCheck(&Disp_Check[2],150,210,12,12,"小数点") ;
  if(LED[active_digit-1].mode&DIGIT_FLICKER)
    CheckCheck(&Disp_Check[0])                       ;
  if(LED[active_digit-1].mode&DIGIT_OFF)
    CheckCheck(&Disp_Check[1])                       ;
  if(!(LED[active_digit-1].data&SEGDP))
    CheckCheck(&Disp_Check[2])                       ;   
  temp[0]      = 0x00                                ;
  for(;;)
  {
    OS_WaitMail(&MBKeyTP)                            ;
    OS_GetMail1(&MBKeyTP, &Event)                    ;
    OS_GetMail1(&MBKeyTP, &key)                      ; 
    OS_GetMail1(&MBKeyTP, &rx_char)                  ; 
    OS_GetMail1(&MBKeyTP, &TPoint.byte[0])           ; 
    OS_GetMail1(&MBKeyTP, &TPoint.byte[1])           ; 
    OS_GetMail1(&MBKeyTP, &TPoint.byte[2])           ; 
    OS_GetMail1(&MBKeyTP, &TPoint.byte[3])           ;     
    if(Event&EVENT_TP_TOUCHED)
    {
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      if(index==0xFF)
      {
        OS_Use(&SemaLCD)                             ; 
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        OS_Unuse(&SemaLCD)                           ;            
      }
      else if(index!=down_id)
      {
        OS_Use(&SemaLCD)                             ; 
        SetButtonUp(down_id)                         ;
        down_id = index                              ;
        SetButtonDown(down_id)                       ;
        OS_Unuse(&SemaLCD)                           ;            
      }
    }
    else if(Event&EVENT_TP_PRESSED)  
    {  
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OS_Use(&SemaLCD)                               ; 
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OS_Unuse(&SemaLCD)                             ; 
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
  return 0x00                                        ;
}

char Proc_Show_Pic(char *NC)
{
  unsigned int temp[2]                               ;
  ShowBMP240320(BMP_PIC1)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
  ShowBMP240320(BMP_PIC2)                            ;
  __delay_cycles(20000000)                           ;
  for(;;)
    if(Read_TP_Twice(&temp[0],&temp[1]))  break      ;  
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：char Proc_Show_Clock(char *NC)                                       //
//描述：电子时钟程序                                                         //
//参数：NC ——无意义                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Show_Clock(char *NC)
{
  unsigned int temp[2]                               ;
  unsigned int Hour ,Minute,Second                   ;
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
    if(Minute%6==0&&Second==0)                        // 时针转动
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
    else if(Second%10==0)                             // 分针转动
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
    OS_Delay(100)                                    ;
    if(Read_TP_Twice(&temp[0],&temp[1]))
      break                                          ;    
  }
  return 0x00                                        ;
}

//---------------------------------------------------------------------------//
//                                                                           //
//函数：char Proc_Port_Comm(char *NC)                                        //
//描述：端口接收/发送程序/设置                                               //
//参数：NC ——无意义                                                        //
//                                                                           //
//---------------------------------------------------------------------------//
char Proc_Port_Comm(char *NC)
{
  
  MessageBox("信息提示","仅供整板检测使用")          ;
  return 0x00                                        ;  
}


char Proc_IrDA_Comm(char *NC)
{
  MessageBox("信息提示","仅供整板检测使用")          ;
  return 0x00                                        ;  
}

char Proc_Net_Comm(char *NC)
{
  MessageBox("信息提示","仅供整板检测使用")          ;
  return 0x00                                        ;  
}

char Proc_Go_Sleep(char *NC)
{
  if(!ConfirmBox("操作提示","确实要关闭系统吗？"))
    GotoSleep()                                      ;
  return 0x00                                        ;
}

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

void Test_LED(void)
{
  P1DIR  |= 0xF0                   ;
  P1OUT  |= 0xF0                   ; 
  P8DIR  |= 0xFF                   ;
  P8OUT   = 0x00                   ; 
  P9DIR  |= 0x0F                   ;
  P9OUT  |= 0x0F                   ; 
  
  {
    P9OUT  &=~BIT0                 ; // DIG1
    __delay_cycles(2000000)        ;
    P9OUT  |= BIT0                 ; // DIG1
    
    P9OUT  &=~BIT1                 ; // DIG2
    __delay_cycles(2000000)        ;
    P9OUT  |= BIT1                 ; // DIG2
    
    P9OUT  &=~BIT2                 ; // DIG3
    __delay_cycles(2000000)        ;
    P9OUT  |= BIT2                 ; // DIG3
    
    P9OUT  &=~BIT3                 ; // DIG4
    __delay_cycles(2000000)        ;
    P9OUT  |= BIT3                 ; // DIG4
    
    P1OUT  &=~BIT4                 ; // DIG5
    __delay_cycles(2000000)        ;
    P1OUT  |= BIT4                 ; // 
    
    P1OUT  &=~BIT5                 ; // DIG6
    __delay_cycles(2000000)        ;
    P1OUT  |= BIT5                 ; // DIG6
    
    P1OUT  &=~BIT6                 ; // DIG7
    __delay_cycles(2000000)        ;
    P1OUT  |= BIT6                 ; // DIG7
    
    P1OUT  &=~BIT7                 ; // DIG8
    __delay_cycles(2000000)        ;
    P1OUT  |= BIT7                 ; // DIG8       
      }
    __delay_cycles(4000000)        ;
    P8OUT   = 0xFF                 ; 
    P7DIR  |= LED_PWR              ;
    P7OUT  &=~LED_PWR              ;
    P1OUT  &= 0x0F                 ; 
    P9OUT  &= 0xF0                 ; 
    __delay_cycles(6000000)        ;
    P7OUT  |= LED_PWR              ;    
}

void Init_J60SPI(void)                       // ENC28J60占用SPI端口
{  
/*  
  P3OUT    |= TP_CS                        ; // 关闭触摸屏
  P3DIR    |= TP_CS                        ;
  P5OUT    |= J60_CS                       ;
  P5DIR    |= J60_CS                       ;
  P10DIR   &=~PMISO                        ;
  P10REN   |= PMISO                        ;
  P10DIR   |= PMOSI+PSCK                   ;
  P10OUT   &= PSCK                         ;
  __delay_cycles(100)                      ;
*/  
  P3OUT    |=  TP_CS                        ; // 关闭触摸屏
  P3DIR    |=  TP_CS                        ;
  P5OUT    |=  J60_CS                       ;
  P5DIR    |=  J60_CS                       ;
  P10DIR   &= ~PMISO                        ;
  P10REN   |=  PMISO                        ;
  P10DIR   |=  PMOSI+PSCK                   ;
  P10OUT   &=~(PMOSI+PSCK)                  ;
  __delay_cycles(20)                        ;  
}

void Init_TPSPI(void)                       // 触摸屏占用SPI端口
{  
  P10OUT   &= PSCK                         ;
  P10DIR   |= PMOSI+PSCK+PNSS              ;
  P10DIR   &=~PMISO                        ;
  P10REN   |= PMISO                        ;
  P3OUT    |= TP_CS                        ;  
  P3DIR    |= TP_CS                        ;  
  P5DIR    &=~J60_CS                       ; // 关ENC28J60片选
}

void Init_UART(void)
{ 
  USB_PORT_SEL   |= TXD_U + RXD_U          ; // 选择引脚功能
  USB_PORT_DIR   |= TXD_U                  ; // 选择引脚功能
  UCA1CTL1        = UCSWRST                ; // 状态机复位
  UCA1CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA1BR0         = 0x03                   ; // 32kHz/9600=3.41 
  UCA1BR1         = 0x00                   ; 
  UCA1MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA1CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA1IE         |= UCRXIE                 ; // 允许接收中断
}

void Init_IRUART(void)
{ 
  IrDA_PORT_SEL  |= IrDA_IN + IrDA_OUT     ; // 选择引脚功能
  IrDA_PORT_DIR  |= IrDA_OUT               ; // 选择引脚功能
  UCA2CTL1        = UCSWRST                ; // 状态机复位
  UCA2CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA2BR0         = 27                   ; // 32kHz/1200=27.3 
  UCA2BR1         = 0x00                   ; 
  UCA2MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA2CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA2IE         |= UCRXIE                 ; // 允许接收中断
}

void Init_RSUART(void)
{ 
  RS_PORT_SEL    |= TXD + RXD              ; // 选择引脚功能
  RS_PORT_DIR    |= TXD                    ; // 选择引脚功能
  UCA3CTL1        = UCSWRST                ; // 状态机复位
  UCA3CTL1       |= UCSSEL_1               ; // CLK = ACLK
  UCA3BR0         = 0x03                   ; // 32kHz/9600=3.41 
  UCA3BR1         = 0x00                   ; 
  UCA3MCTL        = UCBRS_3 + UCBRF_0      ; // UCBRSx=3, UCBRFx=0
  UCA3CTL1       &= ~UCSWRST               ; // 启动状态机
  UCA3IE         |= UCRXIE                 ; // 允许接收中断
  RS485_IN                                 ;
}

void UTX_PROC(char *tx_buf)
{
  unsigned char i,length                   ;
  length = strlen(tx_buf)                  ;
  for(i=0;i<length;i++)
  {
    UCA1TXBUF = *tx_buf++                  ; 
    while (!(UCA1IFG&UCTXIFG))             ; 
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
    __delay_cycles(200000);
    while (!(UCA2IFG&UCTXIFG))             ; 
  }
}

void Init_EtherNet(void)
{
  enc28j60Init(mymac)                      ;
  delay_ms(20)                             ;        
  enc28j60PhyWrite(PHLCON,0x476)           ; // 配置指示灯
  delay_ms(20)                             ;
  init_ip_arp_udp(mymac,myip)              ;
}

void BackLight(void)                         // 初始化背光亮度
{
  P1DIR   |= BL_CTR                        ;
  P1SEL   |= BL_CTR                        ;
  TA0CCR0  = 250                           ; // PWM周期
  TA0CCTL1 = OUTMOD_3                      ; // 置位/复位模式
  TA0CCR1  = 220                           ; // PWM占空时间
  TA0CTL   = TASSEL_2 + MC_1 + TACLR +ID__8; // SMCLK/8, UP                                   
  TA0EX0  |= 0x07                          ; // SMCLK/8/8
}

void Init_ADC(void)
{
  WDTCTL      = WDTPW+WDTHOLD                             ; 
  P7SEL      |= TVBTM                                     ; 
  ADC12CTL0   = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V; 
  ADC12CTL1   = ADC12SHP                                  ; 
  ADC12MCTL0  = ADC12SREF_1 + ADC12INCH_13                ; 
  __delay_cycles(11200000  )                              ; 
  ADC12CTL0  |= ADC12ENC                                  ; 
}

