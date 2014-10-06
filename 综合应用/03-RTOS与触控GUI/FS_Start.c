//===========================================================================//
//                                                                           //
// 文件：  MAIN.C                                                            //
// 说明：  BW-DK5438开发板系统程序头文件                                     //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "main.h"

/*********************************************************************
*
*       main
*
*********************************************************************/

int main(void) {
  OS_IncDI()                                         ; // 屏蔽中断
  OS_InitKern()                                      ; // 初始化系统内核
  OS_InitHW()                                        ; // 初始化硬件
//  BSP_Init()                                         ; // Board Support Package
  Init_Func()                                        ;
  //**************** below for system initialize *************//  
                                                       // 与应用相关的初始化
  MAIN_POWER_ON                                      ;
  Ini_LED(8)                                         ;  
  Init_CLK()                                         ;
  Init_RTC()                                         ;
  LCD_Init()                                         ;
  LED_POWER_ON                                       ;
  DisplayWelcome()                                   ;
  OS_CREATERSEMA(&SemaLCD)                           ;
  LED[0].lightness = 3                               ; // 注意：0<lightness<64
  LED[1].lightness = 6                               ;
  LED[2].lightness = 7                               ;
  LED[3].lightness = 12                              ;
  LED[4].lightness = 15                              ;
  LED[5].lightness = 18                              ;
  LED[6].lightness = 54                              ;
  LED[7].lightness = 60                              ;
  
  LED_Disp_Float(3.1415926,7,FIT_ZERO)               ;
  LED_Flicker_Digit(2)                               ; 
  LED_Flicker_Digit(8)                               ;  
  
//*********************图标显示测试************************//  
  Color     = Black                                  ;
  Color_BK  = 0xEF9F                                 ;
//*********************图标显示测试************************//  

//**************** above for system initialize *************//
  
/*  
  OS_CREATETASK(&_TCB, "MainTask", 
                MainTask, 100, 
                _Stack)                              ;
  OS_CREATETASK(&LED_TASK_TCB, 
                "LedTask", LedTask,
                100, LED_TASK_STACK)                 ;
  OS_CREATETASK(&LED_TASK1_TCB, 
                "LedTask1", LedTask1, 
                100, LED_TASK1_STACK)                ;
*/    
  
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

/*********************************************************************
*
*       MainTask
*/
/*
void MainTask(void);      // Forward declaration to avoid "no prototype" warning
void MainTask(void) {
  U32          v;
  FS_FILE    * pFile;
  char         ac[256];
  const char * sVolName = "";

  //
  // Initialize file system
  //
  FS_Init();
  //
  // Check if low-level format is required
  //
  FS_FormatLLIfRequired("");
  //
  // Check if volume needs to be high level formatted.
  //
  if (FS_IsHLFormatted("") == 0) {
    printf("High level formatting\n");
    FS_Format("", NULL);
  }
  printf("Running sample on %s\n", sVolName);
  v = FS_GetVolumeFreeSpace(sVolName);
  printf("  Free space: %u bytes\n", v);
  sprintf(ac, "%s\\File.txt", sVolName);
  printf("  Write test data to file %s\n", ac);
  pFile = FS_FOpen(ac, "w");
  if (pFile) {
    FS_Write(pFile, "Test", 4);
    FS_FClose(pFile);
  } else {
    printf("Could not open file: %s to write.\n", ac);
  }
  v = FS_GetVolumeFreeSpace(sVolName);
  printf("  Free space: %u bytes\n", v);
  printf("  Finished\n");
  while(1);
}
*/

//---------------------------------------------------------------------------//
//                                                                           //
//函数：void Key_TP_Task(void)                                               //
//描述：键盘与触摸屏扫描，编辑控件光标闪烁                                   //
//		                                                             //
//---------------------------------------------------------------------------//
void Key_TP_Task(void)
{
//  int second_bk,time_cnt = 0                                ;
  unsigned char flicker_cnt = 0                             ;
  char key = 0xFF,last_key = 0xFF, down_cnt = 0, tp_cnt = 0 ;
  union 
  {
    unsigned int  pos[2]                                    ;
    char byte[4]                                            ;
  }TPoint                                                   ;
  char event                                                ;
  Init_KeyPad()                                             ; 
  TP_Init()                                                 ;
  OS_CREATEMB(&MBKeyTP, 1, sizeof(MBKeyTPBuffer),
              &MBKeyTPBuffer                     )          ;
//  second_bk = GetRTCSEC()                                   ;
  for(;;)
  {
    key = ReadKey()                                         ; // 按键扫描
    if(key!=last_key&&key!=0xFF)
      down_cnt  =  0                                        ;
    else if(key!=last_key&&key==0xFF)
    {
      if(down_cnt>3)
      {
        if(  !OS_GetSuspendCnt(&LCD_TASK_TCB)
           ||!OS_GetSuspendCnt(&MENU_OP_TASK_TCB))
        {
          event = EVENT_KEY_PRESSED                         ;
          OS_PutMail1(&MBKeyTP, &event)                     ;
          OS_PutMail1(&MBKeyTP, &last_key)                  ;
        }
      }
    }
    else if(key==last_key&&key!=0xFF)
      if(++down_cnt>4) down_cnt = 4                         ;
    last_key = key                                          ;
    if(!Read_TP_Twice(&TPoint.pos[0],&TPoint.pos[1]))                  
    {
      if(tp_cnt>3)
      {
        if(  !OS_GetSuspendCnt(&LCD_TASK_TCB)
           ||!OS_GetSuspendCnt(&MENU_OP_TASK_TCB))
        {
          event   = EVENT_TP_PRESSED                        ;
          OS_PutMail1(&MBKeyTP, &event)                     ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[0])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[1])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[2])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[3])            ;
        }        
      }
      tp_cnt   = 0                                          ;
    }
    else 
    {
      if(++tp_cnt>8)      tp_cnt     = 8                    ;
      if(tp_cnt>4)
      {
        Convert_Pos( TPoint.pos[0], TPoint.pos[1],   
                    &TPoint.pos[0],&TPoint.pos[1] )         ;
        event   = EVENT_TP_TOUCHED                          ;
        if(  !OS_GetSuspendCnt(&LCD_TASK_TCB)
           ||!OS_GetSuspendCnt(&MENU_OP_TASK_TCB))
        {
          OS_PutMail1(&MBKeyTP, &event)                     ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[0])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[1])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[2])            ;
          OS_PutMail1(&MBKeyTP, &TPoint.byte[3])            ; 
        }
      }
    }
    if(++flicker_cnt>50) 
    {
      flicker_cnt = 0x00                                    ;
      OS_Use(&SemaLCD)                                      ; 
      Flicker_Cursor()                                      ;
      OS_Unuse(&SemaLCD)                                    ;      
    }
/*    
    if(++time_cnt>100)
    {
      if(second_bk!=GetRTCSEC())
      {
        second_bk = GetRTCSEC()                             ;
        time_cnt  = 0x00                                    ;
        DisplayTime()                                       ;
      }
    }
*/
    OS_Delay(10)                                            ;
  }
}

void LedTask(void);      
void LedTask(void)
{
  for(;;)
  {
    P1OUT   ^=(DIG5+DIG6+DIG7+DIG8);
    OS_Delay(500);
  }
}
void LedTask1(void);      
void LedTask1(void)
{
  for(;;)
  {
    P9OUT   ^=(DIG1+DIG2+DIG3+DIG4);
    OS_Delay(100);
  }
}

void Seg7LedTask(void);
void LCD_Task(void) 
{
  RunMenu(0)          ;
}

void MENU_OP_Task(void)
{
  char func_index,event                                    ;
  OS_CREATEMB(&MBFunc,1,sizeof(MBFuncBuffer),&MBFuncBuffer);  
  for(;;)
  {
    OS_WaitMail(&MBFunc)                                   ;    
    OS_GetMail1(&MBFunc, &event)                           ;
    if(event&EVENT_OP_STARTED)
    {
      OS_GetMail1(&MBFunc, &func_index)                    ;
      Item_OP[func_index](&func_index)                     ;      
      if(OS_GetSuspendCnt(&LCD_TASK_TCB))
        OS_Resume(&LCD_TASK_TCB)                           ;
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
  SetRTCMON(9)                                       ;
  SetRTCDAY(19)                                      ;
  SetRTCDOW(7)                                       ;
  SetRTCHOUR(17)                                     ;
  SetRTCMIN(2)                                       ;
  SetRTCSEC(35)                                      ;
}

void DisplayTime(void)
{
  unsigned char string[10]                           ;
  int hour,minute,second                             ;
//  for(;RTCCTL01&RTCRDY;)        _NOP()               ; // 等待RTC有效                                
//  for(;!(RTCCTL01&RTCRDY);)     _NOP()               ;                                     
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
      if(Read_TP_Twice(&temp[0],&temp[1]))  break    ;
    DrawRectFill(0,0,40,40,Color_BK)                 ;
    Wait_Pen_Up()                                    ;
  
    Draw_Test_Point(220,20,Yellow)                   ;
    for(;;)
      if(Read_TP_Twice(&temp[2],&temp[3]))  break    ;
    DrawRectFill(200,0,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(20,300,Yellow)                   ;
    for(;;)
      if(Read_TP_Twice(&temp[4],&temp[5]))  break    ;
    DrawRectFill(0,280,40,40,Color_BK)               ;
    Wait_Pen_Up()                                    ;
    
    Draw_Test_Point(220,300,Yellow)                  ;
    for(;;)
      if(Read_TP_Twice(&temp[6],&temp[7]))  break    ;       
    DrawRectFill(200,280,40,40,Color_BK)             ;
    Wait_Pen_Up()                                    ;
    if(TP_Adjust(temp,cx,ox,cy,oy))         break    ;
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
  unsigned char value = 0;
  char string[10],key,Event,temp[1],active_edit      ;
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
  for(active_edit=1;active_edit<9;active_edit++)
  {
    CreateEditDig(active_edit,15+(25*(active_edit-1)),
                  45,26,26)                          ; 
    temp[0] = Find_Data(LED[active_edit-1].data)     ;
    sprintf(string,"%d",temp[0])                     ; 
    IniEditDig(active_edit,1,1,1,4,string)           ;  
  }
  active_edit = 1                                    ;
  ActiveEditDig(active_edit,1)                       ;    
  temp[0]     = 0x00                                 ;
  for(;;)
  {
    OS_WaitMail(&MBKeyTP)                            ;
    OS_GetMail1(&MBKeyTP, &Event)                    ;
    if(Event&EVENT_KEY_PRESSED)
    {
      OS_GetMail1(&MBKeyTP, &key)                    ;  
      switch(key)
      {
      case 1:case 2:case 3:case 4:case 5:
      case 6:case 7:case 8:case 9:case 0:
        EditDigMessage(Backspace,temp)               ;
        EditDigMessage(key,temp)                     ; 
        LED[active_edit-1].data = SMG[key]           ;
        break                                        ;
      case Left:
        if(active_edit>1) 
        {
          ActiveEditDig(active_edit--,0)             ;
          ActiveEditDig(active_edit  ,1)             ;
        }
        break                                        ;
      case Right:
        if(active_edit<8) 
        {
          ActiveEditDig(active_edit++,0)             ;
          ActiveEditDig(active_edit  ,1)             ;
        }
        break                                        ;
      case Enter:
        Draw7Seg2436(value,100, 100,
                     0xF800,0x00    ); // test code
        if(++value>9) value=0        ;
        break                                        ;
      }
    }
    else
    {
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
      if(  (  (TPoint.pos[0]>220&&TPoint.pos[1]<20)
            ||(TPoint.pos[0]<230&&TPoint.pos[0]>200
               &&TPoint.pos[1]>295                 ))
         &&Event&EVENT_TP_PRESSED                    )
        break                                        ;
    }
      
  }
  ActiveEditDig(active_edit,0)                       ;
  for(active_edit=1;active_edit<9;active_edit++)
    DeleteEditDig(active_edit)                       ;
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

char Proc_Show_Clock(char *NC)
{
  unsigned int temp[2]                               ;
  unsigned int Hour = 11,Minute = 0,Second = 1       ;
  Ini_Clock_Contex()                                 ;
  ShowBMP240320(BMP_CLOCK)                           ;
  Gen_Hour_Hand(Hour*30)                             ;
  Save_Hour_Hand_BK()                                ; 
  Draw_Hour_Hand(White)                              ;
  Gen_Minu_Hand(0)                                   ;
  Save_Minu_Hand_BK()                                ; 
  Draw_Minu_Hand(White)                              ;
  Gen_Second_Hand(0)                                 ;
  Save_Second_Hand_BK()                              ; 
  Draw_Second_Hand(White)                            ;
  for(;;)
  {
    Restore_Second_Hand_BK()                         ;
    if(Second%10==0)
      Restore_Minu_Hand_BK()                         ; 
    if(Minute%6==0&&Second==0)
      Restore_Hour_Hand_BK()                         ;
    
    if(Minute%6==0&&Second==0)
      Gen_Hour_Hand(Hour*30+Minute/2)                ;
    if(Second%10==0)
      Gen_Minu_Hand(Minute*6+Second/10)              ;
    Gen_Second_Hand(Second*6)                        ;
    Save_Second_Hand_BK()                            ; 
    
    
    if(Minute%6==0&&Second==0)
      Save_Hour_Hand_BK()                            ; 
    if(Second%10==0)
      Save_Minu_Hand_BK()                            ; 
    if(Minute%6==0&&Second==0)
      Draw_Hour_Hand(White)                          ;    
    if(Second%10==0)
      Draw_Minu_Hand(White)                          ;
    Draw_Second_Hand(White)                          ;
      
    if(++Second==60)
    {
      Second   = 0                                   ;
      if(++Minute==60)
      {
        Minute = 0                                   ; 
        if(++Hour==24)
          Hour = 0                                   ;
      }
    }
    OS_Delay(500)                                    ;
    if(Read_TP_Twice(&temp[0],&temp[1]))
      break                                          ;    
  }
  return 0x00                                        ;
}

char Proc_Port_Comm(char *NC)
{
  char Event,rec_on = 1                              ;
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
  char key,temp[1]                                   ;
  char index,down_id=0xFF,active_id=1,checked_id=1   ;
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
  PutStringCN16(4,4,"端口收发")                      ;
  PutStringCN16(200,300,"退出")                      ;
  DrawPanel(5, 40,230,100,"接收窗口")                ;
  DrawPanel(5,160,230,40,"发送窗口")                 ;
  DrawPanel(5,220,230,65,"参数设置")                 ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  CreateButton(1,"接收",180, 53,50,20)               ;
  CreateButton(2,"保存",180, 83,50,20)               ;
  CreateButton(3,"清空",180,113,50,20)               ;
  CreateButton(4,"发送",180,173,50,20)               ;
  CreateButton(5,"默认",180,228,50,20)               ;
  CreateButton(6,"设置",180,258,50,20)               ;
  CreateView(&Rec_View,68,11,52,160,83)              ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"RS232")  ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RS485")  ;
  CreateCheck(&Port_Check[2],134,268,12,12,"USB")    ;
  CheckCheck(&Port_Check[0])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,16,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,47,235,56,22)                      ; 
  IniEditDig(2,1,0,1,8,"115200")                     ;
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
    OS_WaitMail(&MBKeyTP)                            ;
    OS_GetMail1(&MBKeyTP, &Event)                    ;
    if(Event&EVENT_KEY_PRESSED)
    {
      OS_GetMail1(&MBKeyTP, &key)                    ; 
      switch(key)
      {
      case 1:case 2:case 3:case 4:case 5:
      case 6:case 7:case 8:case 9:case 0:
      case Left: case Right: case Backspace:
        EditDigMessage(key,temp)                     ; 
        break                                        ;    
      case Enter:
        OS_Use(&SemaLCD)                             ; 
        ViewMessage(&Rec_View,EN_INPUT,"k")   ;// test code
        ViewMessage(&Rec_View,CN_INPUT,"好")  ;// test code
        OS_Unuse(&SemaLCD)                           ; 
        break                                 ;
      case Cancel:
        ClearViewContent(&Rec_View)           ;// test code
        break                                        ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
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
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OS_Use(&SemaLCD)                               ; 
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OS_Unuse(&SemaLCD)                             ; 
      switch(index)
      {
      case 1:
        if(rec_on==1)
        {
          rec_on = 0                                 ;
          SetButtonTitle(1,"停止")                   ;
        }
        else
        {
          rec_on = 1                                 ;
          SetButtonTitle(1,"接收")                   ;
        }
        break                                        ;
      case 3:
        ClearViewContent(&Rec_View)                  ;      
        break                                        ;
      }
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;
    }
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
  char Event                                         ;
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
  char key                                           ;
  char index,down_id=0xFF,active_id=1,checked_id=1   ;
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
  PutStringCN16(4,4,"红外传输")                      ;
  PutStringCN16(200,300,"退出")                      ;
  DrawPanel(5, 40,230,100,"接收窗口")                ;
  DrawPanel(5,160,230,40,"发送窗口")                 ;
  DrawPanel(5,220,230,65,"协议设置")                 ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  CreateButton(1,"接收",180, 53,50,20)               ;
  CreateButton(2,"保存",180, 83,50,20)               ;
  CreateButton(3,"清空",180,113,50,20)               ;
  CreateButton(4,"发送",180,173,50,20)               ;
  CreateButton(5,"默认",180,228,50,20)               ;
  CreateButton(6,"设置",180,258,50,20)               ;
  CreateView(&Rec_View,68,11,52,160,83)              ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"RC-5")   ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RC-6")   ;
  CreateCheck(&Port_Check[2],134,268,12,12,"NEC")    ;
  CheckCheck(&Port_Check[0])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,16,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,47,235,56,22)                      ; 
  IniEditDig(2,1,0,1,8,"115200")                     ;
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
    OS_WaitMail(&MBKeyTP)                            ;
    OS_GetMail1(&MBKeyTP, &Event)                    ;
    if(Event&EVENT_KEY_PRESSED)
    {
      OS_GetMail1(&MBKeyTP, &key)                    ; 
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
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
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OS_Use(&SemaLCD)                               ; 
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OS_Unuse(&SemaLCD)                             ;
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                        ;
    }
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

char Proc_Net_Comm(char *NC)
{
  char Event,rec_on = 1                              ;
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
  char key,temp[1]                                   ;
  char index,down_id=0xFF,active_id=1,checked_id=1   ;
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
  PutStringCN16(4,4,"网络连接")                      ;
  PutStringCN16(200,300,"退出")                      ;
  DrawPanel(5, 40,230,100,"接收窗口")                ;
  DrawPanel(5,160,230,40,"发送窗口")                 ;
  DrawPanel(5,220,230,65,"参数设置")                 ;
  Color    = Black                                   ;
  Color_BK = WINDOW_BK_COLOR                         ;
  CreateButton(1,"接收",180, 53,50,20)               ;
  CreateButton(2,"保存",180, 83,50,20)               ;
  CreateButton(3,"清空",180,113,50,20)               ;
  CreateButton(4,"发送",180,173,50,20)               ;
  CreateButton(5,"默认",180,228,50,20)               ;
  CreateButton(6,"设置",180,258,50,20)               ;
  CreateView(&Rec_View,68,11,52,160,83)              ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"UDP")  ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RS485")  ;
  CreateCheck(&Port_Check[2],134,268,12,12,"USB")    ;
  CheckCheck(&Port_Check[0])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,16,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,47,235,56,22)                      ; 
  IniEditDig(2,1,0,1,8,"115200")                     ;
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
    OS_WaitMail(&MBKeyTP)                            ;
    OS_GetMail1(&MBKeyTP, &Event)                    ;
    if(Event&EVENT_KEY_PRESSED)
    {
      OS_GetMail1(&MBKeyTP, &key)                    ; 
      switch(key)
      {
      case 1:case 2:case 3:case 4:case 5:
      case 6:case 7:case 8:case 9:case 0:
      case Left: case Right: case Backspace:
        EditDigMessage(key,temp)                     ; 
        break                                        ;    
      case Enter:
        OS_Use(&SemaLCD)                             ; 
        ViewMessage(&Rec_View,EN_INPUT,"k")   ;// test code
        ViewMessage(&Rec_View,CN_INPUT,"好")  ;// test code
        OS_Unuse(&SemaLCD)                           ; 
        break                                 ;
      case Cancel:
        ClearViewContent(&Rec_View)           ;// test code
        break                                        ;
      }
    }
    else if(Event&EVENT_TP_TOUCHED)
    {
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
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
      OS_GetMail1(&MBKeyTP, &TPoint.byte[0])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[1])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[2])         ; 
      OS_GetMail1(&MBKeyTP, &TPoint.byte[3])         ; 
      index = InScopeButton(TPoint.pos[0],
                            TPoint.pos[1] )          ;
      OS_Use(&SemaLCD)                               ; 
      SetButtonUp(index)                             ;
      down_id  = 0xFF                                ;
      OS_Unuse(&SemaLCD)                             ; 
      switch(index)
      {
      case 1:
        if(rec_on==1)
        {
          rec_on = 0                                 ;
          SetButtonTitle(1,"停止")                   ;
        }
        else
        {
          rec_on = 1                                 ;
          SetButtonTitle(1,"接收")                   ;
        }
        break                                        ;
      case 3:
        ClearViewContent(&Rec_View)                  ;      
        break                                        ;
      }
      if(  (TPoint.pos[0]<230&&TPoint.pos[0]>200&&TPoint.pos[1]>295)
         ||(TPoint.pos[0]>220&&TPoint.pos[1]<20             ))
          break                                      ;
    }
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

//=====================================================================//
//函数：void Display_Welcom(void)                                      //
//描述：显示欢迎界面                                                   //
//=====================================================================//
void DisplayWelcome(void)
{
  unsigned long int i                         ;
  unsigned char dim = 220                     ;
  Color     = 0xDFFF                          ;
  Color_BK  = 0x0000                          ;
  Clear_LCD(Color_BK)                         ;  
  
  PutStringEN24(60,80,"BW-DK5438")            ;
  WriteCN32(20, 130,Color,Color_BK,(char *)BJBWDZ32[0]);
  WriteCN32(53, 130,Color,Color_BK,(char *)BJBWDZ32[1]);
  WriteCN32(86, 130,Color,Color_BK,(char *)BJBWDZ32[2]);
  WriteCN32(119, 130,Color,Color_BK,(char *)BJBWDZ32[3]);
  WriteCN32(152, 130,Color,Color_BK,(char *)BJBWDZ32[4]);
  WriteCN32(185, 130,Color,Color_BK,(char *)BJBWDZ32[5]);

//  PutStringEN16(30,250,"Broad Way to Success");
  PutStringEN16(25,240,"lead to a Broad Way...");
  Start_Sound()                               ;
  Test_LED()                                  ;     
  Delay(20000)                                ;  
  for(i=0;i<20000;i++)
  {
    if(ReadKey()!=0xFF) break                 ;
  }
  for(i=0;i<16;i++)
  {
     TA0CCR1  =  dim                          ; 
     Delay(300)                               ;
     dim     -=  10                           ;
  }
  for(i=0;i<31;i++)
  {
     TA0CCR1  =  dim                          ; 
     Delay(150)                               ;
     dim     -=  2                            ;
  }  
  Delay(8000)                                 ;
}

void Delay_nus(unsigned int time)
{
  unsigned int i              ;
  for(i=time;i>0;i--)
//    __delay_cycles(20*2)   ;    
    __delay_cycles(17*2)   ;    // FLL_FACTOR = 549
  
}

void Start_Sound(void)
{
  unsigned int j;
  P7DIR   |= BUZZER;
  {  
    for(j=0;j<200*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(400)   ; // -5
    }
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(300)   ; //1
    }
  
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(240)   ; //3
    }
    for(j=0;j<500*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(200)   ;//5
    }
    for(j=0;j<350*0.9;j++)
    {
      Delay_nus(200)   ; // PAUSE
    }
  
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(240)   ; // 3
    }
    for(j=0;j<1000*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(200)   ; // 5
    }
    for(j=0;j<2800;j++)
      Delay_nus(300)      ;
  }  
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

