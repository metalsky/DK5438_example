char Proc_Show_Clock(char *NC)
{
  unsigned int temp[2]                               ;
  unsigned int i = 0,w,l                             ;
  ShowBMP240320(BMP_CLOCK)                           ;
  w=5;l=60;
  for(;;)
  {
  Gen_Hour_Hand(15)                                  ;
  Draw_Hour_Hand(White)                              ;
  }
//  DrawRectFill(120-l,135-w/2,l+2,w,White)              ;
//  DrawRect(120-l,140-w/2,l+2,w,White)              ;
  l=80;
  DrawRectFill(120-w/2,135-l,w,l+2,White)              ;  
//  DrawRect(120-w/2,140-l,w,l+2,White)              ;  
//  Bresenham_Line(120,140,220,237,Yellow)             ;

  for(;;)
  {
//    Draw_Sencond_Hand(118,138,i,100,White,BK_BUFFER)    ;
//    Draw_Sencond_Hand(119,139,i,100,White,&BK_BUFFER[100])    ;
    Draw_Sencond_Hand(120,135,i,90,White,&BK_BUFFER[200])    ;
//    Draw_Sencond_Hand(121,141,i,90,White,&BK_BUFFER[300])    ;
//    Draw_Sencond_Hand(122,142,i,90,White,&BK_BUFFER[400])    ;
    OS_Delay(1000)                                   ;
//    Draw_Background(122,142,i,90,&BK_BUFFER[400])    ;
//    Draw_Background(121,141,i,90,&BK_BUFFER[300])    ;
    Draw_Background(120,135,i,90,&BK_BUFFER[200])                   ;
//    Draw_Background(119,139,i,100,&BK_BUFFER[100])    ;
//    Draw_Background(118,138,i,100,BK_BUFFER)    ;
    i  += 6                                          ;
    if(i>=360) i = 0                                 ;
    if(Read_TP_Twice(&temp[0],&temp[1])) 
      break                                          ;    
  }
  return 0x00                                        ;
}


void Bresenham_Line(unsigned int x1,unsigned int y1,
                    unsigned int x2,unsigned int y2,
                    unsigned int color              )
{
  int dx,dy,p,dobDy,dobD                                  ;
  unsigned int Px,Py,xt                                   ;
  dx     = x2   - x1                                      ;
  dy     = y2   - y1                                      ;
  p      = 2*dy - dx                                      ;           
  dobD   = 2*(dy-dx)                                      ;
  dobDy  = 2*dy                                           ;
  if( x1 > x2)                                              //判断线段的方向
  { 
    Px   = x2                                             ; //起始坐标X
    Py   = y2                                             ; //起始坐标Y 
    xt   = x1                                             ;
  }
  else
  {
    Px   = x1                                             ;
    Py   = y1                                             ;
    xt   = x2                                             ;
  }
  DrawPixel(Px,Py,color)                                  ;
  while(Px<xt)
  {
    Px++                                                  ;
    if(p < 0)   p += dobDy                                ;
    else
    {
      Py++                                                ;
      p += dobD                                           ;
    }
    DrawPixel(Px,Py,color)                                ;
  }
}


void Draw_Sencond_Hand(unsigned int x,unsigned int y,
               unsigned int angle,unsigned int length,
               unsigned color,unsigned int *bk_buffer )
{
  unsigned int xp,yp,i                                    ;
  float k,Angle                                           ;
  Angle  = angle*3.1415926/180                            ;
  if(angle>=45&&angle<=135)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))          ;
    k  = cos(Angle)/sin(Angle)                            ;
    for(i=0;i<xp;i++)
    {
      bk_buffer[i] = ReadPixel(x+i,(unsigned int)(y-k*i)) ;
      DrawPixel(x+i,(unsigned int)(y-k*i),color)          ;
    }
  }
  if(angle>=225&&angle<=315)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))          ;
    k  = cos(Angle)/sin(Angle)                            ;
    for(i=0;i<xp;i++)
    {      
      bk_buffer[i] = ReadPixel(x-i,(unsigned int)(y+k*i)) ;
      DrawPixel(x-i,(unsigned int)(y+k*i),color)          ;
    }
  }
  else if(angle<45||angle>315)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))          ;
    k  = sin(Angle)/cos(Angle)                            ;
    for(i=0;i<yp;i++)
    {      
      bk_buffer[i] = ReadPixel((unsigned int)(x+k*i),y-i) ;
      DrawPixel((unsigned int)(x+k*i),y-i,color)          ;
    }
  }
  else if(angle>135&&angle<225)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))          ;
    k  = sin(Angle)/cos(Angle)                            ;
    for(i=0;i<yp;i++)
    {
      bk_buffer[i] = ReadPixel((unsigned int)(x-k*i),y+i) ;
      DrawPixel((unsigned int)(x-k*i),y+i,color)          ;
    }
  }
}

void Draw_Background(unsigned int x,unsigned int y,
                     unsigned int angle,unsigned int length,
                     unsigned int* bk_buffer               )
{
  unsigned int xp,yp,i                                    ;
  float k,Angle                                           ;
  Angle  = angle*3.1415926/180                            ;
  if(angle>=45&&angle<=135)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))          ;
    k  = cos(Angle)/sin(Angle)                            ;
    for(i=0;i<xp;i++)
      DrawPixel(x+i,(unsigned int)(y-k*i),bk_buffer[i])   ;
  }
  if(angle>=225&&angle<=315)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))          ;
    k  = cos(Angle)/sin(Angle)                            ;
    for(i=0;i<xp;i++)
      DrawPixel(x-i,(unsigned int)(y+k*i),bk_buffer[i])   ;
  }
  else if(angle<45||angle>315)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))          ;
    k  = sin(Angle)/cos(Angle)                            ;
    for(i=0;i<yp;i++)
      DrawPixel((unsigned int)(x+k*i),y-i,bk_buffer[i])   ;
  }
  else if(angle>135&&angle<225)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))          ;
    k  = sin(Angle)/cos(Angle)                            ;
    for(i=0;i<yp;i++)
      DrawPixel((unsigned int)(x-k*i),y+i,bk_buffer[i])   ;
  }
}

  for(;;)
  {
    Restore_Second_Hand_BK()                         ;
//*    
    Restore_Minu_Hand_BK()                           ; 
    Restore_Hour_Hand_BK()                           ; 
    Gen_Hour_Hand(Hour*30)                           ;
    Gen_Minu_Hand(Minute*6)                          ;
//*/    
    Gen_Second_Hand(Second*6)                        ;
    Save_Second_Hand_BK()                            ; 
//*    
    Save_Hour_Hand_BK()                              ; 
    Save_Minu_Hand_BK()                              ; 
    Draw_Hour_Hand(White)                            ;    
    Draw_Minu_Hand(White)                            ;
//*/    
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



char Proc_Port_Comm(char *NC)
{
  VIEW Rec_View                                      ;
  CHECK_BOX Port_Check[3]                            ;
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
  CreateView(&Rec_View,11,52,160,83)                 ;
  CreateCheck(&Port_Check[0],10 ,268,12,12,"RS232")  ;
  CreateCheck(&Port_Check[1],72 ,268,12,12,"RS485")  ;
  CreateCheck(&Port_Check[2],134,268,12,12,"USB")    ;
  CheckCheck(&Port_Check[0])                         ;
  CreateEditDig(1,11,173,160,22)                     ; 
  IniEditDig(1,1,0,1,20,"1234567890")                ;
  ActiveEditDig(1,1)                                 ;
  CreateEditDig(2,48,235,55,22)                      ; 
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
    OS_Delay(100)                                   ;
//    OS_Use(&SemaLCD)                                      ; /* Make sure nobody else uses LCD */
    SetButtonDown(1);
//    OS_Unuse(&SemaLCD)                               ;      
    OS_Delay(100)                                   ;
//    OS_Use(&SemaLCD)                                 ; /* Make sure nobody else uses LCD */
    SetButtonUp(1);
//    OS_Unuse(&SemaLCD)                               ;          
    if(Read_TP_Twice(&TPoint.pos[0],&TPoint.pos[1]))
    {
      Convert_Pos( TPoint.pos[0], TPoint.pos[1], 
                  &TPoint.pos[0],&TPoint.pos[1] )    ;      
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
    Event  = 0x00                                           ;
    if(++flicker_cnt>50) 
    {
      flicker_cnt = 0x00                                    ;
      OS_Use(&SemaLCD)                                      ; 
      Flicker_Cursor()                                      ;
      OS_Unuse(&SemaLCD)                                    ; 
      event = EVENT_FLICKER                                 ;
//      OS_PutMail1(&MBKeyTP, &event)                         ;
    }
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
          event    |= EVENT_KEY_PRESSED                     ;
//          OS_PutMail1(&MBKeyTP, &event)                     ;
//          OS_PutMail1(&MBKeyTP, &last_key)                  ;
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
          event  |= EVENT_TP_PRESSED                        ;
//          OS_PutMail1(&MBKeyTP, &event)                     ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[0])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[1])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[2])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[3])            ;
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
//          OS_PutMail1(&MBKeyTP, &event)                     ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[0])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[1])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[2])            ;
//          OS_PutMail1(&MBKeyTP, &TPoint.byte[3])            ; 
        }
      }
    }
    if(Event)
    {
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
    OS_Delay(10)                                            ;
  }
}


/*
  for(;;)
  {
//    IrDA_PORT_SEL  |= IrDA_OUT     ; 
    Init_IRUART();
    __delay_cycles(10)             ;
    IrDATX_PROC(tx_buf)            ;
    IrDA_PORT_SEL  &=~(IrDA_OUT+IrDA_IN)     ; // 选择引脚功能
    OS_Delay(80)             ;
    tx_buf[0] = 0x55;
    tx_buf[1] = 0x55;
    tx_buf[2] = 0x00;
    tx_buf[3] = 0xaa;
    tx_buf[4] = 0x00         ;
  }
*/
/*
for(;;)
{
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(1);
        P9OUT   ^= IrDA_OUT                          ;
        OS_Delay(50);  
}
*/  