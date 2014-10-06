//===========================================================================//
//                                                                           //
// 文件：  LED.C                                                             //
// 说明：  BW-DK5438开发板数码管显示函数库文件                               //
// 功能：  8位数码管动态显示，独立亮度调节、闪烁、小数点显示                 //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include "LED.H"
unsigned char SMG[10]=
{
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF)       ,
  ~(SEGB+SEGC)                           ,
  ~(SEGA+SEGB+SEGD+SEGE+SEGG)            ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGG)            ,
  ~(SEGB+SEGC+SEGF+SEGG)                 ,
  ~(SEGA+SEGC+SEGD+SEGF+SEGG)            ,
  ~(SEGA+SEGC+SEGD+SEGE+SEGF+SEGG)       ,
  ~(SEGA+SEGB+SEGC)                      ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGE+SEGF+SEGG)  ,
  ~(SEGA+SEGB+SEGC+SEGD+SEGF+SEGG)       ,
}                                                          ; // 数码管段选码

LED_STRUCT LED[8]                                          ; // 数码管显示缓冲区


//***************************************************************************//
//                                                                           //
// 函数：Ini_LED(unsigned char lightness)                                    //
// 功能：初始化数码管显示                                                    //
// 参数：lightness  —— 显示亮度, 1~63                                      //
//                                                                           //
//***************************************************************************//
void Ini_LED(unsigned char lightness)
{
  unsigned char i                                          ;
  Clear_LED()                                              ;
  if(lightness==0||lightness>63)
    lightness          = 10                                ;
  for(i=0;i<8;i++)
    LED[i].lightness   = lightness                         ;
  SET_DIGITS_OUT                                           ; // 设置位选IO
  SET_SEGS_OUT                                             ; // 设置段选IO
}


//***************************************************************************//
//                                                                           //
// 函数：LED_Disp_Int(long,unsigned char)                                    //
// 功能：数码管显示整数                                                      //
// 参数：iValue ——显示数值                                                 //
//       mode   ——显示模式, FIT_ZERO: 高位填零 NO_ZERO: 高位无零           //              
//                                                                           //
//***************************************************************************//
void LED_Disp_Int(long iValue,unsigned char mode)
{
  unsigned char i,l                                        ;
  char string[10]                                          ;
  long value                                               ;
  value = iValue                                           ;
  if(value<0)  value = -value                              ;
  Clear_LED()                                              ;
  if(value<100000000)  
    l = 8                                      ;
  if(value<10000000)  
    l = 7                                      ;
  if(value<1000000)  
    l = 6                                      ;
  if(value<100000)  
    l = 5                                      ;
  if(value<10000)  
    l = 4                                      ;
  if(value<1000)  
    l = 3                                      ;
  if(value<100)  
    l = 2                                      ;
  if(value<10)  
    l = 1                                      ;
  for(i=0;i<l;i++)
  {
    string[l-i-1] = value%10                   ;
    value         = value/10                   ;
  }
  if(iValue>=0)
  {
    for(i=0;i<l;i++)
      LED[7-i].data  = SMG[string[l-i-1]]      ;
  }
  else
  {
    for(i=0;i<l-1;i++)
      LED[7-i].data  = SMG[string[l-i-1]]      ;
  }
  if(iValue<0)
    LED[8-l].data  = MINUS                     ; 
  if(l<8)
  {
    for(i=0;i<8-l;i++)
      LED[i].mode |= DIGIT_OFF                 ;
  }
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Disp_CPUUsage(long)                                             //
// 功能：数码管显示CPU利用率                                                 //
// 参数：iValue ——显示数值                                                 //
//                                                                           //
//***************************************************************************//
void LED_Disp_CPUUsage(long iValue)
{
  unsigned char i,l                            ;
  char string[10]                              ;
  long value                                   ;
  for(i=0;i<8;i++)
    LED[i].mode &=~DIGIT_OFF                   ;
  LED[0].data  = ~(SEGA+SEGD+SEGE+SEGF)        ; // 显示'C'
  LED[1].data  = ~(SEGA+SEGB+SEGE+SEGF+SEGG)   ; // 显示'P'
  LED[2].data  = ~(SEGB+SEGC+SEGD+SEGE+SEGF)   ; // 显示'U'  
  LED[3].data  = ~(SEGG)                       ; // 显示'-'  
  LED[7].data  = ~(SEGA+SEGB+SEGE+SEGF+SEGG)   ; // 显示'P'  
  value   = iValue                             ;
  if(value<100)  
  {
    l          = 2                             ;
    LED[4].data= SMG[0]                        ;     
  }
  if(value<10)  
  {
    l          = 1                             ;
    LED[5].data= SMG[0]                        ;     
  }
  for(i=0;i<l;i++)
  {
    string[l-i-1] = value%10                   ;
    value         = value/10                   ;
  }
  for(i=0;i<l;i++)
    LED[6-i].data  = SMG[string[l-i-1]]        ;
}


//***************************************************************************//
//                                                                           //
// 函数：LED_Disp_Float(float,unsigned char,unsigned char)                   //
// 功能：数码管显示浮点数                                                    //
// 参数：fValue   ——显示数值                                               //
//       dpLength ——小数点后位数                                           //
//       mode     ——显示模式, FIT_ZERO: 高位填零                           //
//                                                                           //
//***************************************************************************//
void LED_Disp_Float(double fValue,unsigned char dpLength,unsigned char mode)
{
  char string[10]                                          ;
  long int lvTempH,lvTempL,lvTemp                          ;
  Clear_LED()                                              ;
  lvTempH      =(long int)fValue                           ;
  lvTempL      =(long int)(fValue-lvTempH)                 ;
  lvTempL      =lvTempL*(long int)pow(10,dpLength)         ;
  lvTempL      =(fValue-lvTempH)*pow(10,dpLength)          ;  
  sprintf(string,"%ld%ld",lvTempH,lvTempL)                 ;
  if(fValue<0)
    sprintf(string,"%ld%ld",lvTempH,-lvTempL)              ;
  lvTemp = atol(string)                                    ;
  LED_Disp_Int(lvTemp,mode)                                ;
  LED[7-dpLength].data  &= DPOINT                          ;
}

//***************************************************************************//
//                                                                           //
// 函数：Clear_LED(void)                                                     //
// 功能：清除LED显示缓冲区                                                   //
// 参数：无                                                                  //
//                                                                           //
//***************************************************************************//
void Clear_LED(void)
{
  unsigned char i                                          ;
  for(i=0;i<8;i++)
  {
    LED[i].data   = 0xFF                                   ;
    LED[i].mode  &=~DIGIT_OFF                              ;
  }
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Hide_Digit(unsigned char)                                       //
// 功能：消隐LED指定位                                                       //
// 参数：digit  —— 消隐位                                                  //
//                                                                           //
//***************************************************************************//
void LED_Hide_Digit(unsigned char digit)
{
  if(digit<=8)
    LED[digit-1].mode  |= DIGIT_OFF                        ;
}

//***************************************************************************//
//                                                                           //
// 函数：LED_Flicker_Digit(unsigned char)                                    //
// 功能：闪烁LED指定位                                                       //
// 参数：digit  —— 闪烁位                                                  //
//       mode   —— 0: 不闪烁  1: 闪烁  2: 翻转闪烁状态                     //
//                                                                           //
//***************************************************************************//
void LED_Flicker_Digit(unsigned char digit,unsigned char mode)
{
  if(digit<=8)
  {
    if(mode==1)
      LED[digit-1].mode  |= DIGIT_FLICKER                  ;
    else if(mode==0)
      LED[digit-1].mode  &=~DIGIT_FLICKER                  ;
    else if(mode==2)
      LED[digit-1].mode  ^= DIGIT_FLICKER                  ;
  }
}

//***************************************************************************//
//                                                                           //
// 函数：Seg7LedRefresh(void)                                                //
// 功能：数码管动态显示                                                      //
// 参数：无                                                                  //
//                                                                           //
//***************************************************************************//
void Seg7LedRefresh(void)
{
  unsigned char on_off         = 1                     ;
  unsigned char digit          = 1                     ; 
  unsigned int  flicker_timer  = 0                     ;
  P8OUT                        = 0xFF                  ;  
  for(;;)
  {    
    switch(digit)
    {
    case 1: case 2: case 3: case 4:
      MASK_DIGITS                                      ;
      if(LED[digit-1].mode&DIGIT_FLICKER)
        if(flicker_timer>250) 
          break                                        ;
      if(LED[digit-1].mode&DIGIT_OFF)
        break                                          ;
      if(on_off)
        LOW_DIGITS_OUT(~(0x01<<(digit-1)))             ;
      break                                            ;
    case 5: case 6: case 7: case 8:
      MASK_DIGITS                                      ;
      if(LED[digit-1].mode&DIGIT_FLICKER)
        if(flicker_timer>250) 
          break                                        ;
      if(LED[digit-1].mode&DIGIT_OFF)
        break                                          ;
      if(on_off)
        HIGH_DIGITS_OUT(~(0x10<<(digit-5)))            ;
      break                                            ;
    }
    P8OUT           = LED[digit-1].data                ;  
    if(on_off) 
      if(++flicker_timer>=500) 
        flicker_timer = 0                              ; 
/*  
    if(on_off)       
      TBCCR0          = LED[digit-1].lightness         ; // 亮度调节
    else
      TBCCR0          = 64 - LED[digit-1].lightness    ; 
*/  
    if(on_off)
      if(++digit>8)     
        digit         = 1                              ;
    on_off           ^= 0x01                           ;
    OSTimeDly(3)                                       ;
  }
}