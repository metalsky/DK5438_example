//===========================================================================//
//                                                                           //
// 文件：  TFTDRV.C                                                          //
// 说明：  BW-DK5438开发板TFT驱动程序, 支持uC/OS-II                          //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21/v5.20                 //
// 版本：  v1.2u                                                             //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2011.09.19                                                        //
//                                                                           //
//===========================================================================//

#include <math.h>
#include "TFTDRV.H"
#include "GUI.H"

//#define   HX8347

unsigned int Driver_IC                     ;
extern unsigned int  Color                 ; // 前景颜色
extern unsigned int  Color_BK              ; // 背景颜色
extern const char LED_FONT_2436[10][108]   ;

void Init_SSTSPI(void)
{  
  P8OUT    |= LCD_NCS                      ;
  P8DIR    |= LCD_NCS                      ;
  P3OUT    |= NCS25                        ;
  P3DIR    |= NCS25                        ;
  P3SEL    &= 0xF0                         ;
  P3SEL    |= 0x0E                         ; // // P3.1/2/3功能选择为SPI
  UCB0CTL1 |= UCSWRST                      ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB    ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                     ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x02                         ; // SCK = SMCK/2
  UCB0BR1   = 0                            ; 
  UCB0CTL1 &=~UCSWRST                      ; // SPI状态机使能
}

void Init_LCDSPI(void)
{
  P3OUT    |= 0x0F                         ;
  P3DIR    |= 0x0F                         ;
  P3SEL    &= 0xF0                         ;
  P3SEL    |= 0x08                         ; // P3.3功能选择为SCK
  UCB0CTL1 |= UCSWRST                      ; // 复位SPI状态机
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB    ; // 3-pin, 8-bit SPI master, Clock polarity high, MSB
  UCB0CTL1 |= UCSSEL_2                     ; // 选择SCK参考源为SMCLK
  UCB0BR0   = 0x03                         ; // SCK = SMCK/3
  UCB0BR1   = 0                            ; 
  UCB0CTL1 &=~UCSWRST                      ; // SPI状态机使能
}


/****************************************************************************
* 名    称：u16 CheckController(void)
* 功    能：返回控制器代码
* 入口参数：无
* 出口参数：控制器型号
* 说    明：调用后返回兼容型号的控制器型号
* 调用方法：code=CheckController();
****************************************************************************/
//#pragma location = "BOOTLOADER"
//#pragma optimize=none
unsigned int Read_Reg(unsigned short Reg_Addr)                                         
{
  unsigned char code[2]                        ;
  unsigned int  temp                           ;
  BUS_OUT                                      ;  
//  LCD_WriteReg(0x00, 0x0001)                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(Reg_Addr)                       ;
  HIGH_BYTE_OUT(0x00)                          ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  BUS_IN                                       ;
  CLR_LCD_RD                                   ;
  _NOP();_NOP()                                ;
  code[0] =  TFT_DATA_LOW                      ;
  code[1] =  TFT_DATA_HIGH                     ;
  SET_LCD_RD                                   ;
  temp    = (unsigned int)(code[1]*256+code[0]);
  return  temp                                 ;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
//#pragma location = "BOOTLOADER"
//#pragma optimize=none
void LCD_WriteReg(unsigned short LCD_Reg, unsigned int LCD_RegValue)
{
  unsigned int RegValue                        ;
  RegValue = LCD_RegValue                      ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(LCD_Reg)                        ; 
  HIGH_BYTE_OUT(0x00)                          ; 
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;    
  LOW_BYTE_OUT(RegValue)                       ;
  HIGH_BYTE_OUT(RegValue>>8)                   ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_CS                                   ;
}
//#pragma location = "BOOTLOADER"
//#pragma optimize=none
void  Reset_LCD(void)
{
  BUS_IN                                       ;
  SET_LCD_RST                                  ;
  SET_LCD_WR                                   ;
  SET_LCD_CS                                   ;
  SET_LCD_RS                                   ;
  SET_LCD_RD                                   ;
  __delay_cycles(10240)                        ;      
  CLR_LCD_RST                                  ;
  __delay_cycles(10240)                        ;
  SET_LCD_RST                                  ;
  __delay_cycles(10240)                        ;
}

void WriteIndex(unsigned int data) 
{  
  SET_LCD_CS;
  SET_LCD_WR;
  CLR_LCD_RS;
  LOW_BYTE_OUT(data)                        ; 
  HIGH_BYTE_OUT(data>>8)                    ; 
  CLR_LCD_CS                                   ;
  CLR_LCD_WR ;
  SET_LCD_WR ;
  SET_LCD_CS;
} 

void LCD_Init(void)
{
  __delay_cycles(10240)                        ;
  P3OUT    |= LCD_NRS + LCD_NRD  + LCD_NWR
             +LCD_NCS + LCD_NRST               ;
  P3DIR    |= LCD_NRS + LCD_NRD  + LCD_NWR
             +LCD_NCS + LCD_NRST               ;
  P1OUT    &=~BL_CTR                           ;  
  P1DIR    |= BL_CTR                           ;  
  Reset_LCD()                                  ;
  Driver_IC = Read_Reg(0x00)                   ;
  if(Driver_IC==0x4535)
  {
    // Power On sequence
    LCD_WriteReg(0x15,0x0030);
    LCD_WriteReg(0x9A,0x0010);
    LCD_WriteReg(0x11,0x0020);
    LCD_WriteReg(0x10,0x3428);
    LCD_WriteReg(0x12,0x0002);
    LCD_WriteReg(0x13,0x1038);
    Delay(45)                ;    
    LCD_WriteReg(0x12,0x0012);
    Delay(45)                ;        
    LCD_WriteReg(0x10,0x3420);
    LCD_WriteReg(0x13,0x3038);
    Delay(70)                ;
    // Display mode and Gamma settings    
    LCD_WriteReg(0x30,0x0000);
    LCD_WriteReg(0x31,0x0402);
    LCD_WriteReg(0x32,0x0307);
    LCD_WriteReg(0x33,0x0304);
    LCD_WriteReg(0x34,0x0004);
    LCD_WriteReg(0x35,0x0401);
    LCD_WriteReg(0x36,0x0707);
    LCD_WriteReg(0x37,0x0305);
    LCD_WriteReg(0x38,0x0610);
    LCD_WriteReg(0x39,0x0610);
    LCD_WriteReg(0x01,0x0100);
    LCD_WriteReg(0x02,0x0300);
    LCD_WriteReg(0x03,0x0130);
    LCD_WriteReg(0x08,0x0808);	
    LCD_WriteReg(0x0A,0x0008);
    LCD_WriteReg(0x60,0x2700);	
    LCD_WriteReg(0x61,0x0001);
    LCD_WriteReg(0x90,0x013E);
    LCD_WriteReg(0x92,0x0100);
    LCD_WriteReg(0x93,0x0100);
    LCD_WriteReg(0xA0,0x3000);
    LCD_WriteReg(0xA3,0x0010);
    // Display On sequence
    LCD_WriteReg(0x07,0x0001);
    LCD_WriteReg(0x07,0x0021);
    LCD_WriteReg(0x07,0x0023);
    LCD_WriteReg(0x07,0x0033);
    LCD_WriteReg(0x07,0x0133);
    LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4)) ;
  }
  if(Driver_IC==0xB505)
  {
   //*************8K2666改3.3V液晶*****************	             
  
		LCD_WriteReg(0X0000,0x0000);
		LCD_WriteReg(0X0000,0x0000);
		LCD_WriteReg(0X0000,0x0000);
		LCD_WriteReg(0X0000,0x0000);
		LCD_WriteReg(0X00A4,0X0001);
		Delay(10);
		LCD_WriteReg(0x0060,0x2700);//Driver Output Control NL
		LCD_WriteReg(0x0008,0x0806);//Display Control 2 (R08h) FP BP
		//γ Control
		LCD_WriteReg(0X0030,0X0902);//R30~39h: γ Control
		LCD_WriteReg(0X0031,0X5615);
		LCD_WriteReg(0X0032,0X0603);
		LCD_WriteReg(0X0033,0X1801);
		LCD_WriteReg(0X0034,0X0000);
		LCD_WriteReg(0X0035,0X0118);
		LCD_WriteReg(0X0036,0X5306);
		LCD_WriteReg(0X0037,0X1506);
		LCD_WriteReg(0X0038,0X0209);
		LCD_WriteReg(0X0039,0X3333);

		LCD_WriteReg(0x0090,0x0019);//R90h: RTNI, DIVI 

		LCD_WriteReg(0X000a,0X0008);
		Delay(20);
		LCD_WriteReg(0X0010,0X0410);//BT, AP
		Delay(100);
		LCD_WriteReg(0X0011,0X0247);//DC,VC 
		Delay(100);
		LCD_WriteReg(0x000E,0x0020);Delay(20);//
		LCD_WriteReg(0x0013,0x0E00);Delay(20);//vcom
		LCD_WriteReg(0x002A,0x003D);Delay(20);//vcom2
		LCD_WriteReg(0x0029,0x002d);Delay(20);//vcom1

		LCD_WriteReg(0x0012,0x019a);
		Delay(50);
		LCD_WriteReg(0x0012,0x01b8);
		Delay(200);
		//Window address control instruction
		LCD_WriteReg(0x0050,0x0000); //Window Horizontal RAM Address Start/End (R50h/ R51h)
		LCD_WriteReg(0x0051,0x00EF); 
		LCD_WriteReg(0x0052,0x0000); //Window Vertical RAM Address Start/End (R52h/R53h)
		LCD_WriteReg(0x0053,0x013F); 
		//Base image display control instruction
		LCD_WriteReg(0x0061,0x0001); //Base Image Display Control (R61h)0x0000,changed by phl 
		LCD_WriteReg(0x006A,0x0000); ///Vertical Scroll Control (R6Ah)
		//Partial display control instruction
		LCD_WriteReg(0x0080,0x0000); //Partial Image 1: Display Position (R80h), RAM Address (Start/End Line Address) (R81h/R82h)
		LCD_WriteReg(0x0081,0x0000); //Partial Image 2: Display Position (R83h), RAM Address (Start/End Line Address) (R84h/R85h)
		LCD_WriteReg(0x0082,0x0000); 
		//Panel interface control instruction 
		LCD_WriteReg(0x0092,0x0300); 
		LCD_WriteReg(0x0093,0x0005); 

		LCD_WriteReg(0x0001,0x0100);Delay(20); //Driver Output Control (R01h)
		LCD_WriteReg(0x0002,0x0200);Delay(20); //LCD Driving Wave Control (R02h)
		LCD_WriteReg(0x0003,0x1030);Delay(20); //Entry Mode (R03h) 0x1030  changed byphl 070103

		LCD_WriteReg(0x000C,0x0000);   //External Display Interface Control 1 (R0Ch)0x0000  changed by phl 070103
		LCD_WriteReg(0x000F,0x0000); //External Display Interface Control 2 (R0Fh)
		//RAM access instruction
		LCD_WriteReg(0x0020,0x0000); //RAM Address Set (Horizontal Address) (R20h)
		LCD_WriteReg(0x0021,0x0000); //RAM Address Set (Vertical Address) (R21h)
		//display on
		Delay(20);
		LCD_WriteReg(0X0007,0X0100);
		Delay(80);
		LCD_WriteReg(0X0022,0x0000);
  }

  if(Driver_IC==0x0000)
  {
LCD_WriteReg(0x0000,0x0001); 
    Delay(20)                                  ;
   LCD_WriteReg(0x46,0x91); 
   LCD_WriteReg(0x47,0x11); 
   LCD_WriteReg(0x48,0x00); 
   LCD_WriteReg(0x49,0x66); 
LCD_WriteReg(0x4a,0x37); 
LCD_WriteReg(0x4b,0x04); 
LCD_WriteReg(0x4c,0x11); 
LCD_WriteReg(0x4d,0x77); 
LCD_WriteReg(0x4e,0x00); 
LCD_WriteReg(0x4f,0x1F); 
LCD_WriteReg(0x50,0x0F); 
LCD_WriteReg(0x51,0x00); //以上这段是调整GAMMA值的，也就是调相应亮度区间的，这组是最优的不用动了 
LCD_WriteReg(0x01,0x06); // IDMON=0, INVON=1, NORON=1, PTLON=0     //IC的ID 
//240x320 window setting 
   LCD_WriteReg(0x02,0x00); // Column address start2 
   LCD_WriteReg(0x03,0x00); // Column address start1 
   LCD_WriteReg(0x04,0x00); // Column address end2 
   LCD_WriteReg(0x05,0xef); // Column address end1 
   LCD_WriteReg(0x06,0x00); // Row address start2 
   LCD_WriteReg(0x07,0x00); // Row address start1 
   LCD_WriteReg(0x08,0x01); // Row address end2 
   LCD_WriteReg(0x09,0x3f); // Row address end1 
LCD_WriteReg(0x16,0xc8); // MY=1,MX=1,MV=0,ML=0,BGR=1,TEON=0 //调屏显示方向及RGB顺序 
   // Display Setting   
   LCD_WriteReg(0x23,0x95); // N_DC=1001 0101    
   LCD_WriteReg(0x24,0x95); // PI_DC=1001 0101 
   LCD_WriteReg(0x25,0xFF); // I_DC=1111 1111  以上三个为普通显示，部分显示选择 
   LCD_WriteReg(0x27,0x02); // N_BP=0000 0010 
   LCD_WriteReg(0x28,0x02); // N_FP=0000 0010 
   LCD_WriteReg(0x29,0x02); // PI_BP=0000 0010 
   LCD_WriteReg(0x2a,0x02); // PI_FP=0000 0010 
   LCD_WriteReg(0x2C,0x02); // I_BP=0000 0010 
   LCD_WriteReg(0x2d,0x02); // I_FP=0000 0010  这些值没有实际效果，只设置IC内部一些参数   
   LCD_WriteReg(0x3a,0x01); // N_RTN=0000, N_NW=001    0001 
   LCD_WriteReg(0x3b,0x01); // P_RTN=0000, P_NW=001 
   LCD_WriteReg(0x3c,0xf0); // I_RTN=1111, I_NW=000 以上三个设置N行翻转，这个不用改，解释起来很复杂 
   LCD_WriteReg(0x3d,0x00); // DIV=00 影响刷新频率00是最大，越往上刷新越慢 
   LCD_WriteReg(0x35,0x38); // EQS=38h 
   LCD_WriteReg(0x36,0x78); // EQP=78h 以上两个值为IC产商内部使用，没有开放， 
   LCD_WriteReg(0x3E,0x38); // SON=38h 
   LCD_WriteReg(0x40,0x0F); // GDON=0Fh 
   LCD_WriteReg(0x41,0xf0); // GDOFF 0xf0以上三个为RGB接口方式下的设置DCLK及同步信号 
// Power Supply Setting 
   LCD_WriteReg(0x19,0x49); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz 
   LCD_WriteReg(0x93,0x0F); // RADJ=1111, 100%  这两个直接调刷新频率 请参照IC规格书 
LCD_WriteReg(0x20,0x40); // BT=0100 
LCD_WriteReg(0x1D,0x07); // VC1=111 
LCD_WriteReg(0x1E,0x00); // VC3=000 
LCD_WriteReg(0x1F,0x03); // VRH=0011 以上这部分是IC升压部分的设置 
LCD_WriteReg(0x44,0x50); // VCM=101 0000 
LCD_WriteReg(0x45,0x11); // VDV=1 0001 以上两个调屏的对比度！ 
LCD_WriteReg(0x1C,0x04); // AP=100 
LCD_WriteReg(0x43,0x80); // set VCOMG=1 
LCD_WriteReg(0x1B,0x08); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0 
LCD_WriteReg(0x1B,0x10); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0 
// Display ON Setting 
LCD_WriteReg(0x90,0x7F); // SAP=0111 1111 
LCD_WriteReg(0x26,0x04); //GON=0, DTE=0, D=01  0x04 
LCD_WriteReg(0x26,0x24); //GON=1, DTE=0, D=01 
LCD_WriteReg(0x26,0x2C); //GON=1, DTE=0, D=11 
LCD_WriteReg(0x26,0x3C); //GON=1, DTE=1, D=11 以上这些是屏上电的时序 
// Fixed the read data issue 
LCD_WriteReg(0x57,0x02); // TEST_Mode=1: into TEST mode 
LCD_WriteReg(0x56,0x02); // tune the memory timing 
LCD_WriteReg(0x57,0x00); // TEST_Mode=0: exit TEST mode 
LCD_WriteReg(0x21,0x00); 
WriteIndex(0x22); //0x0022 这条指令很重要，最好在每个显示画面函数前加一条！有IC清零的作用 
    Delay(20)                                  ;

  }
  
  else if(  (Driver_IC==0x9325)
     ||(Driver_IC==0x9328)
     ||(Driver_IC==0x3225))
  {
    LCD_WriteReg(0xE3, 0x3008)                 ; // 设置内部时序
    LCD_WriteReg(0xE7, 0x0010)                 ;      
    LCD_WriteReg(0x00, 0x0001)                 ; //start internal osc
    LCD_WriteReg(0x01, 0x0100)                 ;     
    LCD_WriteReg(0x02, 0x0700)                 ; //power on sequence                     
    LCD_WriteReg(0x03, (1 << 12) | (1 << 5) | (1 << 4));  //65K 
    LCD_WriteReg(0x04, 0x0000)                 ;                                   
    LCD_WriteReg(0x08, 0x0207)                 ;              
    LCD_WriteReg(0x09, 0x0000)                 ;         
    LCD_WriteReg(0x0a, 0x0000)                 ; //display setting         
    LCD_WriteReg(0x0c, 0x0001)                 ; //display setting          
    LCD_WriteReg(0x0d, 0x0000)                 ; //0f3c          
    LCD_WriteReg(0x0f, 0x0000)                 ;
    LCD_WriteReg(0x10, 0x0000)                 ;   
    LCD_WriteReg(0x11, 0x0007)                 ;
    LCD_WriteReg(0x12, 0x0000)                 ;                                                                 
    LCD_WriteReg(0x13, 0x0000)                 ;                 
    __delay_cycles(1024000)                    ;    
    LCD_WriteReg(0x10, 0x1590)                 ;   
    LCD_WriteReg(0x11, 0x0227)                 ;
    Delay(20)                                  ;
    LCD_WriteReg(0x12, 0x009c)                 ;                 
    Delay(20)                                  ;
    LCD_WriteReg(0x13, 0x1900)                 ;   
    LCD_WriteReg(0x29, 0x0023)                 ;
    LCD_WriteReg(0x2b, 0x000e)                 ;
    Delay(20)                                  ;
    LCD_WriteReg(0x20, 0x0000)                 ;                                                            
    LCD_WriteReg(0x21, 0x0000)                 ;                 
    Delay(20)                                  ;
    LCD_WriteReg(0x30, 0x0007)                 ; 
    LCD_WriteReg(0x31, 0x0707)                 ;   
    LCD_WriteReg(0x32, 0x0006)                 ;
    LCD_WriteReg(0x35, 0x0704)                 ;
    LCD_WriteReg(0x36, 0x1f04)                 ; 
    LCD_WriteReg(0x37, 0x0004)                 ;
    LCD_WriteReg(0x38, 0x0000)                 ;        
    LCD_WriteReg(0x39, 0x0706)                 ;     
    LCD_WriteReg(0x3c, 0x0701)                 ;
    LCD_WriteReg(0x3d, 0x000f)                 ;
    Delay(20)                                  ;
    LCD_WriteReg(0x50, 0x0000)                 ;        
    LCD_WriteReg(0x51, 0x00ef)                 ;   
    LCD_WriteReg(0x52, 0x0000)                 ;     
    LCD_WriteReg(0x53, 0x013f)                 ;
    LCD_WriteReg(0x60, 0xa700)                 ;        
    LCD_WriteReg(0x61, 0x0001)                 ; 
    LCD_WriteReg(0x6a, 0x0000)                 ;
    LCD_WriteReg(0x80, 0x0000)                 ;
    LCD_WriteReg(0x81, 0x0000)                 ;
    LCD_WriteReg(0x82, 0x0000)                 ;
    LCD_WriteReg(0x83, 0x0000)                 ;
    LCD_WriteReg(0x84, 0x0000)                 ;
    LCD_WriteReg(0x85, 0x0000)                 ;
    LCD_WriteReg(0x90, 0x0010)                 ;     
    LCD_WriteReg(0x92, 0x0000)                 ;  
    LCD_WriteReg(0x93, 0x0003)                 ;
    LCD_WriteReg(0x95, 0x0110)                 ;
    LCD_WriteReg(0x97, 0x0000)                 ;        
    LCD_WriteReg(0x98, 0x0000)                 ;  
    LCD_WriteReg(0x07, 0x0133)                 ;  
  }
  else if(Driver_IC==0x4531)
  {
    LCD_WriteReg(0x00,0x0001);
    LCD_WriteReg(0x10,0x0628);
    LCD_WriteReg(0x12,0x0006);
    LCD_WriteReg(0x13,0x0A32);
    LCD_WriteReg(0x11,0x0040);
    LCD_WriteReg(0x15,0x0050);
    LCD_WriteReg(0x12,0x0016);
    Delay(15);
    LCD_WriteReg(0x10,0x5660);
    Delay(15);
    LCD_WriteReg(0x13,0x2A4E);
    LCD_WriteReg(0x01,0x0100);
    LCD_WriteReg(0x02,0x0300);
	
    LCD_WriteReg(0x03,0x1030);
//    LCD_WriteReg(0x03,0x1038);
	
    LCD_WriteReg(0x08,0x0202);
    LCD_WriteReg(0x0A,0x0000);
    LCD_WriteReg(0x30,0x0000);
    LCD_WriteReg(0x31,0x0402);
    LCD_WriteReg(0x32,0x0106);
    LCD_WriteReg(0x33,0x0700);
    LCD_WriteReg(0x34,0x0104);
    LCD_WriteReg(0x35,0x0301);
    LCD_WriteReg(0x36,0x0707);
    LCD_WriteReg(0x37,0x0305);
    LCD_WriteReg(0x38,0x0208);
    LCD_WriteReg(0x39,0x0F0B);
    Delay(15);
    LCD_WriteReg(0x41,0x0002);
    LCD_WriteReg(0x60,0x2700);
    LCD_WriteReg(0x61,0x0001);
    LCD_WriteReg(0x90,0x0119);
    LCD_WriteReg(0x92,0x010A);
    LCD_WriteReg(0x93,0x0004);
    LCD_WriteReg(0xA0,0x0100);
//    LCD_WriteReg(0x07,0x0001);
    Delay(15);
//    LCD_WriteReg(0x07,0x0021); 
    Delay(15);
//    LCD_WriteReg(0x07,0x0023);
    Delay(15);
//    LCD_WriteReg(0x07,0x0033);
    Delay(15);
    LCD_WriteReg(0x07,0x0133);
    Delay(15);
    LCD_WriteReg(0xA0,0x0000);
    Delay(20);
  /* Set GRAM write direction and BGR = 1
     I/D=10 (Horizontal : increment, Vertical : increment)
     AM=1 (address is updated in vertical writing direction)                  */
    LCD_WriteReg(0x03, 0x1038);

    LCD_WriteReg(0x07, 0x0173);                 /* 262K color and display ON          */
  //uart0_sendChar('2');  
  }
  else if(Driver_IC==0x5408)
  {
    LCD_WriteReg(0x00e5,0x8000);  
    LCD_WriteReg(0x0000,0x0001);    
      Delay(2);
    LCD_WriteReg(0x0001,0x0000);  
	LCD_WriteReg(0x0002,0x0400);     //0x0700
	LCD_WriteReg(0x0003,0x1000);  
	LCD_WriteReg(0x0004,0x0000);
	
	LCD_WriteReg(0x0008,0x0202);
	LCD_WriteReg(0x0009,0x0000);
	LCD_WriteReg(0x000a,0x0000);
	LCD_WriteReg(0x000C,0x0000);   
	LCD_WriteReg(0x000d,0x0000);   
	LCD_WriteReg(0x000f,0x0000); 
	
	LCD_WriteReg(0x0010,0x0000);
	LCD_WriteReg(0x0011,0x0007);   
	LCD_WriteReg(0x0012,0x0000);
	LCD_WriteReg(0x0013,0x0000);
	Delay(50);	
	LCD_WriteReg(0x0010,0x16b0);   
	LCD_WriteReg(0x0011,0x0034);  
	Delay(50);
	LCD_WriteReg(0x0012,0x011a); 
	Delay(50);   
	LCD_WriteReg(0x0013,0x1700);    
	LCD_WriteReg(0x0029,0x0006);     
    Delay(50);
    LCD_WriteReg(0x0020,0x0000);   
    LCD_WriteReg(0x0021,0x0000);
    
	LCD_WriteReg(0x0030,0x0000);
    LCD_WriteReg(0x0031,0x0404);
    LCD_WriteReg(0x0032,0x0404);
    LCD_WriteReg(0x0035,0x0004);
    LCD_WriteReg(0x0036,0x0404);
    LCD_WriteReg(0x0037,0x0404);
    LCD_WriteReg(0x0038,0x0404);
    LCD_WriteReg(0x0039,0x0707);
    LCD_WriteReg(0x003c,0x0500);
    LCD_WriteReg(0x003d,0x0607);
    LCD_WriteReg(0x0050,0x0000);
    LCD_WriteReg(0x0051,0x00ef);   
    LCD_WriteReg(0x0052,0x0000);
    LCD_WriteReg(0x0053,0x013f);
    
    LCD_WriteReg(0x0060,0xa700);  
    LCD_WriteReg(0x0061,0x0001);
    LCD_WriteReg(0x006a,0x0000);
    
    LCD_WriteReg(0x0080,0x0000);
    LCD_WriteReg(0x0081,0x0000);
    LCD_WriteReg(0x0082,0x0000);
    LCD_WriteReg(0x0083,0x0000);
    LCD_WriteReg(0x0084,0x0000);
    LCD_WriteReg(0x0085,0x0000);
    
    
    LCD_WriteReg(0x0090,0x0010);
    LCD_WriteReg(0x0092,0x0000);
    LCD_WriteReg(0x0093,0x0003);
    LCD_WriteReg(0x0095,0x0110);
    LCD_WriteReg(0x0097,0x0000);
    LCD_WriteReg(0x0098,0x0000);
      
    LCD_WriteReg(0x0007,0x0173);
    
    LCD_WriteReg(0x0020,0x0000);
    LCD_WriteReg(0x0021,0x0000);   
    LCD_WriteReg(0x0022,0x0000);
  }
  else if(Driver_IC==0x5409)
  {
LCD_WriteReg(0X00,0X0001);   
Delay(50);   
LCD_WriteReg(0X10,0X1628);   
LCD_WriteReg(0X12,0X000e);//0x0006    
LCD_WriteReg(0X13,0X0A39);   
Delay (10);   
LCD_WriteReg(0X11,0X0040);   
LCD_WriteReg(0X15,0X0050);   
Delay(40);   
LCD_WriteReg(0X12,0X001e);//16    
Delay(40);   
LCD_WriteReg(0X10,0X1620);   
LCD_WriteReg(0X13,0X2A39);   
Delay(10);   
LCD_WriteReg(0X01,0X0100);   
LCD_WriteReg(0X02,0X0300);   
LCD_WriteReg(0X03,0X1030);   
LCD_WriteReg(0X08,0X0202);   
LCD_WriteReg(0X0A,0X0008);   
LCD_WriteReg(0X30,0X0000);   
LCD_WriteReg(0X31,0X0402);   
LCD_WriteReg(0X32,0X0106);   
LCD_WriteReg(0X33,0X0503);   
LCD_WriteReg(0X34,0X0104);   
LCD_WriteReg(0X35,0X0301);   
LCD_WriteReg(0X36,0X0707);   
LCD_WriteReg(0X37,0X0305);   
LCD_WriteReg(0X38,0X0208);   
LCD_WriteReg(0X39,0X0F0B);   
LCD_WriteReg(0X41,0X0002);   
LCD_WriteReg(0X60,0X2700);   
LCD_WriteReg(0X61,0X0001);   
LCD_WriteReg(0X90,0X0210);   
LCD_WriteReg(0X92,0X010A);   
LCD_WriteReg(0X93,0X0004);   
LCD_WriteReg(0XA0,0X0100);   
LCD_WriteReg(0X07,0X0001);   
LCD_WriteReg(0X07,0X0021);   
LCD_WriteReg(0X07,0X0023);   
LCD_WriteReg(0X07,0X0033);   
LCD_WriteReg(0X07,0X0133);   
LCD_WriteReg(0XA0,0X0000);   
	LCD_WriteReg(0x07,0x0173);
   

//LCD_WriteRegm(0x0022);
  }
  else if(  Driver_IC==0x5400
          ||Driver_IC==0x9320)
  {
    /* Start Initial Sequence ----------------------------------------------------*/
	LCD_WriteReg(0xe5,0x8000);/* Set the internal vcore voltage */ 
	LCD_WriteReg(0x00,0x0001);/* Start internal OSC. */ 
//	LCD_WriteReg(0x2b,0x0010);//?
	LCD_WriteReg(0x01,0x0100);/* set SS and SM bit */ 
	LCD_WriteReg(0x02,0x0700); /* set 1 line inversion */ 
//	LCD_WriteReg(0x03,0x1230); /* set GRAM write direction and BGR=1. */ 
        LCD_WriteReg(0x03,0x1030); /* set GRAM write direction and BGR=1. */ 
	LCD_WriteReg(0x04,0x0000); /* Resize register */ 
	LCD_WriteReg(0x08,0x0202);/* set the back porch and front porch */ 
	LCD_WriteReg(0x09,0x0000);/* set non-display area refresh cycle ISC[3:0] */
	LCD_WriteReg(0x0a,0x0000); /* FMARK function */ 
	LCD_WriteReg(0x0c,0x0000); /* RGB interface setting */ 
	LCD_WriteReg(0x0d,0x0000);/* Frame marker Position */ 
	LCD_WriteReg(0x0f,0x0000);/* RGB interface polarity */ 
        
/* Power On sequence ---------------------------------------------------------*/ 
	LCD_WriteReg(0x10,0x0000);/* SAP, BT[3:0], AP, DSTB, SLP, STB */ 
	LCD_WriteReg(0x11,0x0000);/* DC1[2:0], DC0[2:0], VC[2:0] */ 
	LCD_WriteReg(0x12,0x0000);/* VREG1OUT voltage */ 
	LCD_WriteReg(0x13,0x0000);/* VDV[4:0] for VCOM amplitude */ 
	Delay(200)               ; /* Dis-charge capacitor power voltage (200ms) */ 
	LCD_WriteReg(0x10,0x17b0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */ 
//	LCD_WriteReg(0x11,0x0004); /* DC1[2:0], DC0[2:0], VC[2:0] */ 
	LCD_WriteReg(0x11,0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */ 
	Delay(50);
//	LCD_WriteReg(0x12,0x013e); /* VREG1OUT voltage */ 
	LCD_WriteReg(0x12,0x0139); /* VREG1OUT voltage */ 
	Delay(50);
//	LCD_WriteReg(0x13,0x1f00); /* VDV[4:0] for VCOM amplitude */ 
	LCD_WriteReg(0x13,0x1d00); /* VDV[4:0] for VCOM amplitude */ 

//	LCD_WriteReg(0x29,0x000f); /* VCM[4:0] for VCOMH */ 
	LCD_WriteReg(0x29,0x0013); /* VCM[4:0] for VCOMH */
	Delay(50);
	LCD_WriteReg(0x20,0x0000);/* GRAM horizontal Address */ 
	LCD_WriteReg(0x21,0x0000);/* GRAM Vertical Address */ 

/* Adjust the Gamma Curve ----------------------------------------------------*/ 

	LCD_WriteReg(0x30,0x0204);
	LCD_WriteReg(0x31,0x0001);
	LCD_WriteReg(0x32,0x0000);
	LCD_WriteReg(0x35,0x0206);
	LCD_WriteReg(0x36,0x0600);
	LCD_WriteReg(0x37,0x0500);
	LCD_WriteReg(0x38,0x0505);
	LCD_WriteReg(0x39,0x0407);
	LCD_WriteReg(0x3c,0x0500);
	LCD_WriteReg(0x3d,0x0503);
//*        
	LCD_WriteReg(0x30,0x0006);
	LCD_WriteReg(0x31,0x0101);
	LCD_WriteReg(0x32,0x0003);
	LCD_WriteReg(0x35,0x0106);
	LCD_WriteReg(0x36,0x0b02);
	LCD_WriteReg(0x37,0x0302);
	LCD_WriteReg(0x38,0x0707);
	LCD_WriteReg(0x39,0x0007);
	LCD_WriteReg(0x3c,0x0600);
	LCD_WriteReg(0x3d,0x020b);
//*/
        
        /* Set GRAM area -------------------------------------------------------------*/ 
	LCD_WriteReg(0x50,0x0000);/* Horizontal GRAM Start Address */ 
	LCD_WriteReg(0x51,0x00ef);/* Horizontal GRAM End Address */ 
	LCD_WriteReg(0x52,0x0000);/* Vertical GRAM Start Address */ 
	LCD_WriteReg(0x53,0x013f);/* Vertical GRAM End Address */ 
	LCD_WriteReg(0x60,0x2700);/* Gate Scan Line */ 
	LCD_WriteReg(0x61,0x0001);/* NDL,VLE, REV */ 
	LCD_WriteReg(0x6a,0x0000);/* set scrolling line */ 

/* Partial Display Control ---------------------------------------------------*/	
        LCD_WriteReg(0x80,0x0000);
	LCD_WriteReg(0x81,0x0000);
	LCD_WriteReg(0x82,0x0000);
	LCD_WriteReg(0x83,0x0000);
	LCD_WriteReg(0x84,0x0000);
	LCD_WriteReg(0x85,0x0000);
        
/* Panel Control -------------------------------------------------------------*/	
        LCD_WriteReg(0x90,0x0010);
	LCD_WriteReg(0x92,0x0000);
	LCD_WriteReg(0x93,0x0003);
	LCD_WriteReg(0x95,0x0110);
	LCD_WriteReg(0x97,0x0000);
	LCD_WriteReg(0x98,0x0000);


	
  /* Set GRAM write direction and BGR = 1 */ 
  /* I/D=01 (Horizontal : increment, Vertical : decrement) */ 
  /* AM=1 (address is updated in vertical writing direction) */ 
        LCD_WriteReg(0x03, 0x1018); 
//        LCD_WriteReg(0x03, 0x1038); 

	//display on
	LCD_WriteReg(0x07,0x0173);
  }
}
/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
* Output         : None
* Return         : None
*******************************************************************************/
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos);
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos)
{
    LCD_WriteReg(0x0020, Xpos)                 ;
    LCD_WriteReg(0x0021, Ypos)                 ;
}
/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Clears the hole LCD.
* Input          : Color: the color of the background.
* Output         : None
* Return         : None
*******************************************************************************/
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void Clear_LCD(unsigned int Color)
{
  register unsigned int  x                     ;
//  LCD_WriteReg(0x07, 0x0131)                 ;
  SetDisplayWindow(0,0,240,320)                ;
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  LOW_BYTE_OUT(Color)                          ;
  HIGH_BYTE_OUT(Color>>8)                      ;
  Init_LCDSPI()                                ;
  for(x= 0; x < 240; x++)
  {
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
//*/    
  }
  SET_LCD_CS                                   ;
  P3SEL    &=~0x0F                             ; // P3.1/2/3功能选择为SPI
}

#ifndef HX8347  
unsigned int ReadPixel(unsigned int x,unsigned int y)
{
  unsigned int low_byte,high_byte, result      ;
  LCD_WriteReg(0x20, x)                        ; // 写入像素点位置
  LCD_WriteReg(0x21, y)                        ;
  CLR_LCD_CS                                   ; // Index 寄存器写入
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ; 
  HIGH_BYTE_OUT(0x00)                          ; 
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ; 
  BUS_IN                                       ;
  CLR_LCD_RD                                   ;
  SET_LCD_RD                                   ;
  {
    CLR_LCD_RD                                 ;
    low_byte  = P2IN                           ;
    high_byte = P4IN                           ;
    SET_LCD_RD                                 ;
  }
  SET_LCD_CS                                   ;
  if(Driver_IC==0x4531||Driver_IC==0x4535)
  {
    result= low_byte                           ;
    result=(result<<8)+high_byte               ;
    return result                              ;
  }    
  result  =((high_byte<<3)|(low_byte&0x07))<<8 ;
  result +=((low_byte>>3)|(high_byte&0xE0))    ;
  return result                                ;
}
#endif

#ifdef HX8347
unsigned int ReadPixel(unsigned int x,unsigned int y)
{
  unsigned int low_byte,high_byte, result      ;
  LCD_WriteReg(0x0002, x>>8)                   ;
  LCD_WriteReg(0x0003, x)                      ;
  LCD_WriteReg(0x0006, y>>8)                   ;
  LCD_WriteReg(0x0007, y)                      ;   
  CLR_LCD_CS                                   ; // Index 寄存器写入
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ; 
  HIGH_BYTE_OUT(0x00)                          ; 
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ; 
  BUS_IN                                       ;
  CLR_LCD_RD                                   ;
  SET_LCD_RD                                   ;
  {
    CLR_LCD_RD                                 ;
    low_byte   = P4IN                          ;
    SET_LCD_RD                                 ;
    CLR_LCD_RD                                 ;
    high_byte  = P4IN                          ;
    SET_LCD_RD                                 ;
  }
  SET_LCD_CS                                   ;
//*  
  result  =((high_byte<<3)|(low_byte&0x07))<<8 ;
  result +=((low_byte>>3)|(high_byte&0xE0))    ;
//*/    
//  result  =(high_byte<<8)+low_byte             ;  
  return result                                ;
}
#endif


void DrawPixel(unsigned int x, unsigned int y, unsigned int color)
{
#ifndef HX8347  
   LCD_WriteReg(0x0020, x)                 ;
   LCD_WriteReg(0x0021, y)                 ;  
#endif
#ifdef HX8347
  LCD_WriteReg(0x0002, x>>8)                      ;
  LCD_WriteReg(0x0003, x)                         ;
  LCD_WriteReg(0x0006, y>>8)                      ;
  LCD_WriteReg(0x0007, y)                         ;   
#endif   
   SET_LCD_RD               ;
   CLR_LCD_CS               ;
   CLR_LCD_RS               ;
   LOW_BYTE_OUT(0x22)       ;
   HIGH_BYTE_OUT(0x00)      ;
   CLR_LCD_WR               ;
   SET_LCD_WR               ;
   SET_LCD_RS               ;
   LOW_BYTE_OUT(color)      ;
   HIGH_BYTE_OUT(color>>8)  ;
   CLR_LCD_WR               ;
   SET_LCD_WR               ;
   SET_LCD_CS               ;
}

//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void DrawHorizonLine(unsigned int x, unsigned int y, unsigned int Length,unsigned int Color)               
{
  register int index = 0                       ;
/*  
  LCD_WriteReg(0x0020, x)                      ;
  LCD_WriteReg(0x0021, y)                      ;  
*/
  SetDisplayWindow(x,y,Length,1)               ;
#ifndef HX8347
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  LOW_BYTE_OUT(Color)                          ;
  HIGH_BYTE_OUT(Color>>8)                      ;
  for(index=0;index<Length;index++)
  {
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}

//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void DrawVerticalLine(unsigned int x, unsigned int y, unsigned int Length,unsigned int Color)               
{
  register unsigned index = 0                    ;
/*  
  LCD_WriteReg(0x0020, x)                        ;
  LCD_WriteReg(0x0021, y)                        ;    
*/
  SetDisplayWindow(x,y,1,Length)                 ;  
#ifndef HX8347  
  LCD_WriteReg(0x03,(1<<12)|(1<<5)|(1<<4)|(1<<3));
#endif  
  SET_LCD_RD                                     ;
  CLR_LCD_CS                                     ;
  CLR_LCD_RS                                     ;
  LOW_BYTE_OUT(0x22)                             ;
  HIGH_BYTE_OUT(0x00)                            ;
  BUS_OUT                                        ;
  CLR_LCD_WR                                     ;
  SET_LCD_WR                                     ;
  SET_LCD_RS                                     ;
  LOW_BYTE_OUT(Color)                            ;
  HIGH_BYTE_OUT(Color>>8)                        ;
  for(index=0;index<Length;index++)
  {
    CLR_LCD_WR                                   ;
    SET_LCD_WR                                   ;
  }
  SET_LCD_CS                                     ;
  WindowModeDisable(Color)                       ;
}

/*******************************************************************************
* Function Name  : DrawRect
* Description    : Displays a rectangle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void DrawRect(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height,unsigned Color)
{
  DrawHorizonLine(Xpos, Ypos, Width, Color)            ;
  DrawHorizonLine(Xpos, (Ypos + Height), Width, Color) ;
  DrawVerticalLine(Xpos, Ypos, Height, Color)          ;
  DrawVerticalLine((Xpos+Width-1) ,Ypos, Height, Color);
}

/*******************************************************************************
* Function Name  : LCD_DrawRect
* Description    : Displays a rectangle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void DrawRectFill(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height,unsigned Color)
{
  register unsigned int i,j                    ;
  SetDisplayWindow(Xpos,Ypos,Width,Height)     ;
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  LOW_BYTE_OUT(Color)                          ;
  HIGH_BYTE_OUT(Color>>8)                      ;
  Init_LCDSPI()                                ;
  for(i = Width; i>0; i--)
    for(j=Height/128+1;j>0;j--)
  {
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
     UCB0TXBUF = 0xFF                          ; 
     __delay_cycles(28)                        ;
  }
  SET_LCD_CS                                   ;
  P3SEL    &=~0x0F                             ; // P3.1/2/3功能选择为SPI
  WindowModeDisable(Color)                     ;
}
/*******************************************************************************
* Function Name  : LCD_SetDisplayWindow
* Description    : Sets a display window
* Input          : - Xpos: specifies the X buttom left position.
*                  - Ypos: specifies the Y buttom left position.
*                  - Height: display window height.
*                  - Width: display window width.
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef HX8347
void SetDisplayWindow(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height)
{
  unsigned int temp                                  ;
  LCD_WriteReg(0x0002, Xpos>>8)                      ;
  LCD_WriteReg(0x0003, Xpos)                         ;
  temp = Xpos+Width-1                                ;
  LCD_WriteReg(0x0004, temp>>8)                      ;
  LCD_WriteReg(0x0005, temp)                         ;

  LCD_WriteReg(0x0006, Ypos>>8)                      ;
  LCD_WriteReg(0x0007, Ypos)                         ;
  temp = Ypos + Height-1                             ;
  LCD_WriteReg(0x0008, temp>>8)                      ;
  LCD_WriteReg(0x0009, temp)                         ;
  LCD_WriteReg(0x0016,0x00C8)                        ; 
  WriteIndex(0x0022)                                 ;   
}
#endif

#ifndef HX8347
void SetDisplayWindow(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height)
{
  LCD_WriteReg(0x0050, Xpos)                         ;
  LCD_WriteReg(0x0051, (Xpos+Width-1))               ;
  LCD_WriteReg(0x0052, Ypos)                         ;
  LCD_WriteReg(0x0053,(Ypos + Height-1))             ;
  LCD_WriteReg(0x0020, Xpos)                         ;
  LCD_WriteReg(0x0021, Ypos)                         ; 
}
#endif

/*******************************************************************************
* Function Name  : LCD_WindowModeDisable
* Description    : Disables LCD Window mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WindowModeDisable(unsigned int Color)
{
  SetDisplayWindow(0, 0, 239, 319)                  ;
  LOW_BYTE_OUT(Color)                               ;
  HIGH_BYTE_OUT(Color>>8)                           ;    
}


//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteCN16(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,16,16)                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<32; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}

//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteCN20(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,24,20)                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<60; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}


//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteASCII24(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,16,24)                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<48; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}

//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteASCII16(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,8,16)                   ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<16; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteASCII18(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,16,18)                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<36; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}
//#pragma optimize=none
//#pragma location = "BOOTLOADER"
void WriteASCII20(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix);
void WriteASCII20(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,16,20)                  ;
#ifndef HX8347    
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<40; i++)
  {
    temp = Matrix[i]                           ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}

/*
//#pragma optimize=none
#pragma location = "BOOTLOADER"
void DrawLogo(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color)
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,16,42)                  ;
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<84; i++)
  {
    temp = logo[i]                             ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}
*/

void Draw7Seg2436(unsigned char value,unsigned int x, unsigned int y,
                  unsigned int Color,unsigned int BK_Color           )
{
  register unsigned int i,j                    ;
  register unsigned char temp                  ;
  SetDisplayWindow(x,y,24,36)                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4))    ;
#endif  
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<108; i++)
  {
    temp = (unsigned char)LED_FONT_2436[value][i] ;
    for(j=0;j<8; j++)
    {
      if(temp&0x80)
      {
        LOW_BYTE_OUT(Color)                    ;
        HIGH_BYTE_OUT(Color>>8)                ;  
      }
      else
      {
        LOW_BYTE_OUT(BK_Color)                 ;
        HIGH_BYTE_OUT(BK_Color>>8)             ;  
      }      
      temp <<= 1                               ;
      CLR_LCD_WR                               ;
      SET_LCD_WR                               ;
    }
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}


void SetCursor(unsigned int x,unsigned int y) ; 
void SetCursor(unsigned int x,unsigned int y)  
{
  LCD_WriteReg(0x0020, x)                      ;
  LCD_WriteReg(0x0021, y)                      ;      
}

#pragma optimize=none
//#pragma location = "BOOTLOADER"
void Delay(unsigned int time)
{
  unsigned int i,k                             ;
  for(i=0;i<255;i++)
    for(k=0;k<time;k++)
      _NOP()                                   ;	  
}

//#pragma optimize=none
//#pragma location = "BOOTLOADER"
unsigned int RGB888to565(unsigned long int Color888)
{
  unsigned int Color565                    ;
  union
  {
    unsigned long int Val                  ;
    unsigned char     Byte[4]              ;
  }Color_temp                              ;
  Color_temp.Val       = Color888          ;
  Color_temp.Byte[0] >>= 3                 ;
  Color_temp.Byte[1] >>= 2                 ;
  Color_temp.Byte[2] >>= 3                 ;
  Color565  = Color_temp.Byte[0]
             +Color_temp.Byte[1]*0x0020
	     +Color_temp.Byte[2]*0x0800    ;
  return Color565                          ; 
}

void WriteCN32(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i,j                    ;
  register unsigned char Current               ;
  register unsigned char ColorH,ColorL         ;
  register unsigned char BKColorH,BKColorL     ;  
  SetDisplayWindow(x,y,32,33)                  ;
//  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4)|(1<<3));
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4)); // 非转置字库
#endif  
  ColorL     =  Color                          ;
  ColorH     =  Color>>8                       ;
  BKColorL   =  BK_Color                       ;
  BKColorH   =  BK_Color>>8                    ;
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0;i<4;i++)
    for(j=0;j<33;j++)
    {  
      Current  = Matrix[i*33+j]                 ;
      if(Current>127)
      {
        LOW_BYTE_OUT(ColorL)                     ;
        HIGH_BYTE_OUT(ColorH)                    ;  
      }
      else
      {
        LOW_BYTE_OUT(BKColorL)                   ;
        HIGH_BYTE_OUT(BKColorH)                  ;  
      }      
      Current <<= 1                              ;
      CLR_LCD_WR                                 ;
      SET_LCD_WR                                 ;
      if(Current>127)
      {
        LOW_BYTE_OUT(ColorL)                     ;
        HIGH_BYTE_OUT(ColorH)                    ;  
      }
      else
      {
        LOW_BYTE_OUT(BKColorL)                   ;
        HIGH_BYTE_OUT(BKColorH)                  ;  
      }      
      Current <<= 1                              ;
      CLR_LCD_WR                                 ;
      SET_LCD_WR                                 ;
      if(Current>127)
      {
        LOW_BYTE_OUT(ColorL)                     ;
        HIGH_BYTE_OUT(ColorH)                    ;  
      }
      else
      {
        LOW_BYTE_OUT(BKColorL)                   ;
        HIGH_BYTE_OUT(BKColorH)                  ;  
      }      
      Current <<= 1                              ;
      CLR_LCD_WR                                 ;
      SET_LCD_WR                                 ;
      if(Current>127)
      {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}


/****************************************************************
函数名：LCD写1个24×24 汉字字符函数
入口参数：x,横向坐标，由左到右分别是0~240-1 
          y,纵向坐标，由上到下分别为0~320-1
          CharColaor,字符的颜色 
          CharBackColor,字符背景颜色 
         ASCIICode,相应字符的ASCII码
也就是说，320240分辨率的显示屏，横向能显示240/16个ASCII字符，竖向能显示20行
返回值：无

注意！！！！！如果单独使用此函数则应该加上LCD_Rs_H()和CS_TFT_H();为了优化系统省去了
这个指令，假设此函数执行的上一条语句是写命令，（RS_L情况）则写入将出错
，因为ILI9320认为当RS_L时写入的是命令
*****************************************************************/
#pragma optimize=none
void WriteCN24(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)
{
  register unsigned int i                      ;
  register unsigned char Current               ;
  register unsigned char ColorH,ColorL         ;
  register unsigned char BKColorH,BKColorL     ;  
  SetDisplayWindow(x,y,24,24)                  ;
//  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4)|(1<<3)); // 适用于转置字库
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<5)|(1<<4)); 
#endif  
  ColorL     =  Color                          ;
  ColorH     =  Color>>8                       ;
  BKColorL   =  BK_Color                       ;
  BKColorH   =  BK_Color>>8                    ;
  SET_LCD_RD                                   ;
  CLR_LCD_CS                                   ;
  CLR_LCD_RS                                   ;
  LOW_BYTE_OUT(0x22)                           ;
  HIGH_BYTE_OUT(0x00)                          ;
  BUS_OUT                                      ;
  CLR_LCD_WR                                   ;
  SET_LCD_WR                                   ;
  SET_LCD_RS                                   ;
  for(i=0; i<72;i+=9 )
  {
    Current  = Matrix[i]                       ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;

	

    Current  = Matrix[i+1]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;

	

    Current  = Matrix[i+2]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;

	

    Current  = Matrix[i+3]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
	

    Current  = Matrix[i+4]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
	

    Current  = Matrix[i+5]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;

	

    Current  = Matrix[i+6]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
	

    Current  = Matrix[i+7]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
	

    Current  = Matrix[i+8]                     ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    Current <<= 1                              ;
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
    if(Current>127)
    {
      LOW_BYTE_OUT(ColorL)                     ;
      HIGH_BYTE_OUT(ColorH)                    ;  
    }
    else
    {
      LOW_BYTE_OUT(BKColorL)                   ;
      HIGH_BYTE_OUT(BKColorH)                  ;  
    }      
    CLR_LCD_WR                                 ;
    SET_LCD_WR                                 ;
  }
  SET_LCD_CS                                   ;
  WindowModeDisable(Color)                     ;
}

//***************************************************************************************
//											*
// 		ReadMatrix24: 读取24点阵字模  		                                *
//											*
//***************************************************************************************
void ReadMatrix24(unsigned char addrH,unsigned char addrM,unsigned char addrL,char *Matrix);
void ReadMatrix24(unsigned char addrH,unsigned char addrM,unsigned char addrL,char *Matrix)
{
  register unsigned char i,j;
  Init_SSTSPI()             ;  
  CLR_NCS25                 ;
  _NOP()   		    ; 
  _NOP()      		    ; 
  _NOP()  		    ;  
  UCB0TXBUF = 0x03          ; // 读命令
  __delay_cycles(15)        ;
  UCB0TXBUF = addrH         ; // 地址高字节
  __delay_cycles(15)        ;
  UCB0TXBUF = addrM         ; // 地址中字节
  __delay_cycles(15)        ;
  UCB0TXBUF = addrL         ; // 地址低字节
  __delay_cycles(15)        ;
  for(i=0;i<6;i++)
  {
    j         = i*12        ;
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
    UCB0TXBUF = 0xFF        ;  
    __delay_cycles(15)      ;
    j++                     ; 
    Matrix[j] = UCB0RXBUF   ;  
  }
  SET_NCS25                 ;
  P3SEL    &=~0x0E          ; 
}

//***************************************************************************************
//											*
// 		PutStringCN16: 写16点阵中文字符串                                       *
//											*
//***************************************************************************************
void PutStringCN16(unsigned int x,unsigned int y,unsigned char* string  )
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<14;i++)
  {
    addr.value = 0                                                ;
    Block = string[i*2]                                           ;
    Pos   = string[i*2+1]                                         ;
    if(Block==0) break                                            ;
    addr.value = 32*((Block-161)*94+Pos -161) + TABLE_HZK16       ; 
    ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix)  ; 
    WriteCN16(x+i*16, y, Color,Color_BK,cMatrix)                  ;
  }
} 

//***************************************************************************************
//											*
// 		PutStringCN24: 写24点阵中文字符串	                                *
//											*
//***************************************************************************************
#pragma optimize=none
void PutStringCN24(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<12;i++)
  {
    addr.value = 0                                                ;
    Block = string[i*2]                                           ;
    Pos   = string[i*2+1]                                         ;
    if(Block==0) break                                            ;
//    addr.value = 72*((Block-161-15)*94+Pos -161)                ; // -15 for HZK24 OF UCDOS
    addr.value = 72*((Block-161)*94+Pos -161)                     ; 
    ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix)  ; 
    WriteCN24(x+i*24, y, Color,Color_BK,cMatrix)                  ;
  }
}
//***************************************************************************************
//											*
// 		PutStringCN24: 写24点阵中文字符串	                                *
//											*
//***************************************************************************************
void PutCharCN24(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<1;i++)
  {
    addr.value = 0                                                ;
    Block = string[i*2]                                           ;
    Pos   = string[i*2+1]                                         ;
    if(Block==0) break                                            ;
//    addr.value = 72*((Block-161-15)*94+Pos -161)                ; // -15 for HZK24 OF UCDOS
    addr.value = 72*((Block-161)*94+Pos -161)                     ; 
    ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix)  ; 
    WriteCN24(x+i*24, y, Color,Color_BK,cMatrix)                  ;
  }
}

void PutStringEN24(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  WriteIndex(0x22)                                                ;// ??????FOR HX8347???
  for(i=0;i<24;i++)
  {
    if(string[i]==0) break                                        ;
    addr.value = 48*(string[i]-32) + TABLE_ASCII24                ; 
    ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix)  ; 
    WriteASCII24(x+i*13, y, Color,Color_BK,cMatrix)               ;
  }
}

void PutStringEN16(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<24;i++)
  {
    if(string[i]==0) break                                        ;
    addr.value = 16*(string[i]) + TABLE_ASCII16                   ; 
    ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix)  ; 
    WriteASCII16(x+i*9, y, Color,Color_BK,cMatrix)                ;
  }
} 

//***************************************************************************************
//											*
// 		PutString24: 写24点阵中西文混合字符串	                                *
//											*
//***************************************************************************************
void PutString24(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<24;i++)
  {
    if(string[i]==0) break                                        ;
    if(string[i]>128)
    {
      addr.value = 0                                              ;
      Block = string[i]                                           ;
      Pos   = string[++i]                                         ;
      addr.value = 72*((Block-161)*94+Pos -161)                   ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteCN24(x+(i-1)*13, y, Color,Color_BK,cMatrix)            ;
    }
    else
    {
      addr.value = 16*(string[i]) + TABLE_ASCII16                 ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteASCII16(x+i*9, y, Color,Color_BK,cMatrix)              ;
    }
  }  
}

//***************************************************************************************
//											*
// 		PutString24: 写24点阵中西文混合字符串，显示菜单项用                     *
//											*
//***************************************************************************************
void PutString24M(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<24;i++)
  {
    if(string[i]==0) break                                        ;
    if(string[i]>128)
    {
      addr.value = 0                                              ;
      Block = string[i]                                           ;
      Pos   = string[++i]                                         ;
      addr.value = 72*((Block-161)*94+Pos -161)                   ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteCN24(x+(i-1)*12, y, Color,Color_BK,cMatrix)            ;
    }
    else
    {
      addr.value = 48*(string[i]-32) + TABLE_ASCII24              ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteASCII24(x+i*12, y, Color,Color_BK,cMatrix)             ;
    }
  }  
}

//***************************************************************************************
//											*
// 		PutString16: 写16点阵中西文混合字符串                                   *
//											*
//***************************************************************************************
void PutString16M(unsigned int x,unsigned int y,unsigned char* string)
{
  unsigned char i                                                 ;
  char cMatrix[72]                                                ;
  unsigned long int Block,Pos                                     ;
  union
  {
    unsigned long int  value                                      ;
    unsigned char byte[4]                                         ;
  }addr                                                           ;
  for(i=0;i<24;i++)
  {
    if(string[i]==0) break                                        ;
    if(string[i]>128)
    {
      addr.value = 0                                              ;
      Block = string[i]                                           ;
      Pos   = string[++i]                                         ;
      addr.value = 32*((Block-161)*94+Pos -161) + TABLE_HZK16     ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteCN16(x+(i-1)*8, y, Color,Color_BK,cMatrix)             ;
    }
    else
    {
      addr.value = 16*(string[i]) + TABLE_ASCII16                 ; 
      ReadMatrix24(addr.byte[2],addr.byte[1],addr.byte[0],cMatrix); 
      WriteASCII16(x+i*8, y, Color,Color_BK,cMatrix)              ;
    }
  }  
}

//***************************************************************************************
//											*
// 		Read_JEDECID: 读取SST25VF016B器件ID 		                        *
//											*
//***************************************************************************************
void Read_JEDECID(char *ID);
void Read_JEDECID(char *ID)
{
  Init_SSTSPI()             ;  
  CLR_NCS25                 ;
  _NOP()	   	    ; 
  _NOP()	   	    ; 
  _NOP()   		    ;  
  UCB0TXBUF = 0x9F          ; // 读JEDEC ID  
  __delay_cycles(15)        ;
  UCB0TXBUF = 0xFF          ;  
  __delay_cycles(15)        ;
  ID[0]= UCB0RXBUF          ;  
  UCB0TXBUF = 0xFF          ;  
  __delay_cycles(15)        ;
  ID[1]= UCB0RXBUF          ;  
  UCB0TXBUF = 0xFF          ;  
  __delay_cycles(15)        ;
  ID[2]= UCB0RXBUF          ;  
  SET_NCS25                 ;
  P3SEL    &=~0x0E          ; // P3.1/2/3功能选择为SPI
}

/************************************************************************/   
/* PROCEDURE: Sector_Erase                                              */   
/* This procedure Sector Erases the Chip.                               */   
/* Input:                                                               */   
/*       Dst -        Destination Address 000000H - 07FFFFH             */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void Sector_Erase(unsigned long Dst)   ;
void Sector_Erase(unsigned long Dst)   
{      
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x20                ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst>>16            ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst>>8             ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI
}   

/************************************************************************/   
/* PROCEDURE:   Byte_Program                                            */   
/*                                                                      */   
/* This procedure programs one address of the device.                   */   
/* Assumption:  Address being programmed is already erased and is NOT   */   
/*              block protected.                                        */   
/* Input:                                                               */   
/*       Dst  -  Destination Address 000000H - 07FFFFH                  */   
/*       byte -  byte to be programmed                                  */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void Byte_Program(unsigned long Dst, unsigned char byte);   
void Byte_Program(unsigned long Dst, unsigned char byte)   
{ 
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x02                ; 
  __delay_cycles(15)              ;
  UCB0TXBUF = Dst>>16             ; 
  __delay_cycles(15)              ;
  UCB0TXBUF = Dst>>8              ; 
  __delay_cycles(15)              ;
  UCB0TXBUF = Dst                 ; 
  __delay_cycles(15)              ;
  UCB0TXBUF = byte                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI
}   

/************************************************************************/   
/* PROCEDURE:   Read_Cont                                               */   
/* This procedure reads multiple addresses of the device and stores     */   
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/   
/* Input:                                                               */   
/*       Dst      -   Destination Address 000000H - 07FFFFH             */   
/*       no_bytes -   Number of bytes to read (max = 128)               */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void Read_Cont(unsigned long Dst, unsigned long no_bytes,unsigned char *Buffer) ;  
void Read_Cont(unsigned long Dst, unsigned long no_bytes,unsigned char *Buffer)   
{   
  unsigned long i = 0             ;   
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x03                ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst>>16            ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst>>8             ; 
  __delay_cycles(15)              ;
  UCB0TXBUF =  Dst                ; 
  __delay_cycles(15)              ;
  for (i = 0; i < no_bytes; i++)       
  {   
    UCB0TXBUF    = 0xFF           ; 
    __delay_cycles(15)            ;
    Buffer[i]    = UCB0RXBUF      ;   
  }   
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI   
}   
   
/************************************************************************/   
/* PROCEDURE: Read_Status_Register                                      */   
/* This procedure read the status register and returns the byte.        */   
/* Returns:                                                             */   
/*         status register value                                        */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
unsigned char Read_Status_Register() ;  
unsigned char Read_Status_Register()   
{   
  unsigned char byte = 0          ;   
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  UCB0TXBUF    = 0x05             ; 
  __delay_cycles(15)              ;
  UCB0TXBUF    = 0xFF             ; 
  __delay_cycles(15)              ;
  byte         = UCB0RXBUF        ;
  SET_NCS25                       ;
  P3SEL       &=~0x0E             ; // P3.1/2/3功能选择为SPI   
  return byte                     ;   
}   
   
/************************************************************************/   
/* PROCEDURE: EWSR                                                      */   
/* This procedure Enables Write Status Register.                        */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void EWSR()  ; 
void EWSR()   
{   
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x50                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI   
}   
   
/************************************************************************/   
/* PROCEDURE: WRSR                                                      */   
/* This procedure writes a byte to the Status Register.                 */   
/* Input:                                                               */   
/*        byte                                                          */   
/* Returns:                                                             */   
/*      Nothing                                                         */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void WRSR(char byte)   
{   
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x01                ; 
  __delay_cycles(15)              ;
  UCB0TXBUF = byte                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI   
}   
   
/************************************************************************/   
/* PROCEDURE: WREN                                                      */   
/* This procedure enables the Write Enable Latch.                       */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void WREN()   
{ 
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x06                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI   
}   
   
/************************************************************************/   
/* PROCEDURE: WRDI                                                      */   
/* This procedure disables the Write Enable Latch.                      */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void WRDI()   
{
  Init_SSTSPI()                   ;  
  CLR_NCS25                       ;
  _NOP()	   	          ; 
  _NOP()	   	          ; 
  _NOP()   		          ;  
  UCB0TXBUF = 0x04                ; 
  __delay_cycles(15)              ;
  SET_NCS25                       ;
  P3SEL    &=~0x0E                ; // P3.1/2/3功能选择为SPI   
}   

/************************************************************************/   
/* PROCEDURE: Wait_Busy                                                 */   
/* This procedure waits until device is no longer busy (can be used by  */   
/* Byte-Program, Sector-Erase, Block-Erase, Chip-Erase).                */   
/************************************************************************/   
//#pragma location = "BOOTLOADER"
void Wait_Busy()   
{   
  while(Read_Status_Register()&0x01)  /* waste time until not busy */   
    Read_Status_Register()        ;   
}   



//--------------------------- 拷贝字符串中的指定数目字节 ----------------------------//
//#pragma location = "BOOTLOADER"
void str_cpy(char* Destination, char* Source,unsigned char Num)
{
  unsigned char i                         ;
  for(i=0;i<Num;i++)
    Destination[i] = Source[i]            ;
}


#define PicBufSize  32
//从Flash读1张图片显示
void LCD_WritePic(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr)
{
  unsigned int color[PicBufSize]                             ;
  unsigned long num                                          ;
  register unsigned int  i, j, pack                          ;
  SetDisplayWindow(x,y,w,h)                                  ;
  pack=((unsigned long)w*h+PicBufSize-1 )/PicBufSize         ;
  num=(unsigned long)w*h                                     ;
  Init_SSTSPI()                                              ;
  for(i=0; i<pack; i++)
  {
    Read_Cont(FlashAddr, 2*PicBufSize,(unsigned char *)color);  
    SET_LCD_RD                                               ;
    CLR_LCD_CS                                               ;
    CLR_LCD_RS                                               ;
    LOW_BYTE_OUT(0x22)                                       ;
    HIGH_BYTE_OUT(0x00)                                      ;
    BUS_OUT                                                  ;
    CLR_LCD_WR                                               ;
    SET_LCD_WR                                               ;
    SET_LCD_RS                                               ;
    for(j=0; j<PicBufSize; j++)
    {
      LOW_BYTE_OUT(color[j])                                 ;
      HIGH_BYTE_OUT(color[j]>>8)                             ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
      if(--num==0)        break                              ;
    }
    FlashAddr += 2*PicBufSize                                ;
    SET_LCD_CS                                               ;
  }
  WindowModeDisable(Color)                                   ;
}
/*
void ShowIcon24(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr)
{
  unsigned char color[72]                                   ;
  union 
  {
    unsigned int      val                                    ;
    unsigned char     byte[2]                                ;
  }ctemp                                                     ;
  register unsigned int  i, j                                ;
  FlashAddr  += 54                                           ;
  SetDisplayWindow(x,y,w,h)                                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<4));
#endif
//  
#ifdef HX8347  
  LCD_WriteReg(0x0016,0x00);  
#endif               
//
  SET_LCD_RD                                                 ;
  CLR_LCD_CS                                                 ;
  CLR_LCD_RS                                                 ;
  LOW_BYTE_OUT(0x22)                                         ;
  HIGH_BYTE_OUT(0x00)                                        ;
  BUS_OUT                                                    ;
  CLR_LCD_WR                                                 ;
  SET_LCD_WR                                                 ;
  SET_LCD_RS                                                 ;
  Init_SSTSPI()                                              ;
  for(i=0; i<23; i++)
  {
    SET_LCD_CS                                               ;
    Read_Cont(FlashAddr,    72,(unsigned char *)&color[0] )  ;   
    CLR_LCD_CS                                               ;
    for(j=0; j<24; j++)
    {
      ctemp.byte[0]    = color[j*3]                          ;
      ctemp.byte[1]    = color[j*3+1]                        ;
      ctemp.byte[1]  >>= 2                                   ;
      ctemp.val      >>= 3                                   ;
      ctemp.byte[1]   &= 0x07                                ;
      ctemp.byte[1]   |=(color[j*3+2]&0xF8)                  ;
      LOW_BYTE_OUT(ctemp.byte[0])                            ;
      HIGH_BYTE_OUT(ctemp.byte[1])                           ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
    }
    FlashAddr += 72                                          ;
  }
  WindowModeDisable(Color)                                   ;
  LCD_WriteReg(0x0016,0xC0);  
}
*/

void ShowIcon24(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr)
{
  unsigned char color[72]                                   ;
  union 
  {
    unsigned int      val                                    ;
    unsigned char     byte[2]                                ;
  }ctemp                                                     ;
  register unsigned int  i, j                                ;
  FlashAddr  += 54                                           ;
  SetDisplayWindow(x,y,w,h)                                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<4))                         ;
#endif  
/*  
#ifdef HX8347  
  LCD_WriteReg(0x16,0x00)                                  ;  
  SetDisplayWindow(x,y,w,h)                                  ;
#endif               
*/  
  SET_LCD_RD                                                 ;
  CLR_LCD_CS                                                 ;
  CLR_LCD_RS                                                 ;
  LOW_BYTE_OUT(0x22)                                         ;
  HIGH_BYTE_OUT(0x00)                                        ;
  BUS_OUT                                                    ;
  CLR_LCD_WR                                                 ;
  SET_LCD_WR                                                 ;
  SET_LCD_RS                                                 ;
  Init_SSTSPI()                                              ;
  for(i=0; i<23; i++)
  {
    SET_LCD_CS                                               ;
    Read_Cont(FlashAddr,    72,(unsigned char *)&color[0] )  ;      
    CLR_LCD_CS                                               ;
    for(j=0; j<24; j++)
    {
      ctemp.byte[0]    = color[j*3]                          ;
      ctemp.byte[1]    = color[j*3+1]                        ;
      ctemp.byte[1]  >>= 2                                   ;
      ctemp.val      >>= 3                                   ;
      ctemp.byte[1]   &= 0x07                                ;
      ctemp.byte[1]   |=(color[j*3+2]&0xF8)                  ;
      LOW_BYTE_OUT(ctemp.byte[0])                            ;
      HIGH_BYTE_OUT(ctemp.byte[1])                           ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
    }
    FlashAddr += 72                                          ;
  }
  WindowModeDisable(Color)                                   ;
  LCD_WriteReg(0x0016,0xC0);  
}

void ShowIcon48(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr)
{
  unsigned char color[144]                                   ;
  union 
  {
    unsigned int      val                                    ;
    unsigned char     byte[2]                                ;
  }ctemp                                                     ;
  register unsigned int  i, j                                ;
  FlashAddr  += 54                                           ;
  SetDisplayWindow(x,y,w,h)                                  ;
#ifndef HX8347  
  LCD_WriteReg(0x03, (1<<12)|(1<<4))                         ;
#endif  
/*  
#ifdef HX8347  
  LCD_WriteReg(0x16,0xC8)                                    ;
#endif               
*/
  SET_LCD_RD                                                 ;
  CLR_LCD_CS                                                 ;
  CLR_LCD_RS                                                 ;
  LOW_BYTE_OUT(0x22)                                         ;
  HIGH_BYTE_OUT(0x00)                                        ;
  BUS_OUT                                                    ;
  CLR_LCD_WR                                                 ;
  SET_LCD_WR                                                 ;
  SET_LCD_RS                                                 ;
  Init_SSTSPI()                                              ;
  for(i=0; i<48; i++)
  {
    SET_LCD_CS                                               ;
    Read_Cont(FlashAddr,    96,(unsigned char *)&color[0] )  ;      
    Read_Cont(FlashAddr+96, 48,(unsigned char *)&color[96])  ;
    CLR_LCD_CS                                               ;
    for(j=0; j<48; j++)
    {
      ctemp.byte[0]    = color[j*3]                          ;
      ctemp.byte[1]    = color[j*3+1]                        ;
      ctemp.byte[1]  >>= 2                                   ;
      ctemp.val      >>= 3                                   ;
      ctemp.byte[1]   &= 0x07                                ;
      ctemp.byte[1]   |=(color[j*3+2]&0xF8)                  ;
      LOW_BYTE_OUT(ctemp.byte[0])                            ;
      HIGH_BYTE_OUT(ctemp.byte[1])                           ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
    }
    FlashAddr += 144                                         ;
  }
  WindowModeDisable(Color)                                   ;
  LCD_WriteReg(0x0016,0xC0);  
}

void ShowBMP128128(unsigned int x, unsigned int  y, unsigned long FlashAddr)
{
  unsigned char color[384]                                   ;
  union 
  {
    unsigned int      val                                    ;
    unsigned char     byte[2]                                ;
  }ctemp                                                     ;
  register unsigned int  i, j                                ;
  FlashAddr  += 54                                           ;
  SetDisplayWindow(x,y,128,128)                              ;
#ifndef HX8347    
  LCD_WriteReg(0x03, (1<<12)|(1<<4));
#endif    
  Init_SSTSPI()                                              ;
  for(i=0; i<128; i++)
  {
    Read_Cont(FlashAddr,    128,(unsigned char *)&color[0] ) ;      
    Read_Cont(FlashAddr+128,128,(unsigned char *)&color[128]);
    Read_Cont(FlashAddr+256,128,(unsigned char *)&color[256]);    
    SET_LCD_RD                                               ;
    CLR_LCD_CS                                               ;
    CLR_LCD_RS                                               ;
    LOW_BYTE_OUT(0x22)                                       ;
    HIGH_BYTE_OUT(0x00)                                      ;
    BUS_OUT                                                  ;
    CLR_LCD_WR                                               ;
    SET_LCD_WR                                               ;
    SET_LCD_RS                                               ;
    for(j=0; j<128; j++)
    {
      ctemp.byte[0]    = color[j*3]                          ;
      ctemp.byte[1]    = color[j*3+1]                        ;
      ctemp.byte[1]  >>= 2                                   ;
      ctemp.val      >>= 3                                   ;
      ctemp.byte[1]   &= 0x07                                ;
      ctemp.byte[1]   |=(color[j*3+2]&0xF8)                  ;
      LOW_BYTE_OUT(ctemp.byte[0])                            ;
      HIGH_BYTE_OUT(ctemp.byte[1])                           ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
    }
    FlashAddr += 384                                         ;
    SET_LCD_CS                                               ;
  }
  WindowModeDisable(Color)                                   ;
}

void ShowBMP240320(unsigned long FlashAddr)
{
  unsigned char color[720]                                   ;
  union 
  {
    unsigned int      val                                    ;
    unsigned char     byte[2]                                ;
  }ctemp                                                     ;
  register unsigned int  i, j                                ;
  FlashAddr  += 54                                           ;
  SetDisplayWindow(0,0,240,320)                              ;
#ifndef HX8347    
  LCD_WriteReg(0x03, (1<<12)|(1<<4));
#endif  
#ifdef HX8347  
  LCD_WriteReg(0x16,0x48)                                    ;
//  WriteIndex(0x0022)                                         ;
#endif               
  SET_LCD_RD                                                 ;
  CLR_LCD_CS                                                 ;
  CLR_LCD_RS                                                 ;
  LOW_BYTE_OUT(0x22)                                         ;
  HIGH_BYTE_OUT(0x00)                                        ;
  BUS_OUT                                                    ;
  CLR_LCD_WR                                                 ;
  SET_LCD_WR                                                 ;
  SET_LCD_RS                                                 ;
  Init_SSTSPI()                                              ;
  for(i=0; i<320; i++)
  {
    SET_LCD_CS                                               ;
    Read_Cont(FlashAddr,   120,(unsigned char *)&color[0]  ) ;      
    Read_Cont(FlashAddr+120,120,(unsigned char *)&color[120]);  
    Read_Cont(FlashAddr+240,120,(unsigned char *)&color[240]);  
    Read_Cont(FlashAddr+360,120,(unsigned char *)&color[360]);  
    Read_Cont(FlashAddr+480,120,(unsigned char *)&color[480]);  
    Read_Cont(FlashAddr+600,120,(unsigned char *)&color[600]); 
    CLR_LCD_CS                                               ;
    for(j=0; j<240; j++)
    {
      ctemp.byte[0]    = color[j*3]                          ;
      ctemp.byte[1]    = color[j*3+1]                        ;
      ctemp.byte[1]  >>= 2                                   ;
      ctemp.val      >>= 3                                   ;
      ctemp.byte[1]   &= 0x07                                ;
      ctemp.byte[1]   |=(color[j*3+2]&0xF8)                  ;
      LOW_BYTE_OUT(ctemp.byte[0])                            ;
      HIGH_BYTE_OUT(ctemp.byte[1])                           ;
      CLR_LCD_WR                                             ;
      SET_LCD_WR                                             ;
    }
    FlashAddr += 720                                         ;
  }
  WindowModeDisable(Color)                                   ;
#ifdef HX8347  
  LCD_WriteReg(0x16,0xC8)                                    ;
//  WriteIndex(0x0022)                                         ;
#endif               
}

void Draw_Test_Point(unsigned int x, unsigned int y,unsigned Color)
{
  DrawVerticalLine(x,y-8,17,Color)                           ;
  DrawHorizonLine(x-8,y,17,Color)                            ;
  DrawRect(x-5,y-5,11,10,Color)                              ;
}

void Draw_Line(unsigned int x    ,unsigned int y,
               unsigned int angle,unsigned int length,
               unsigned color                         )
{
  unsigned int xp,yp,i                            ;
  float k,Angle                                   ;
  Angle  = angle*3.1415926/180                    ;
  if(angle>=45&&angle<=135)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))  ;
    k  = cos(Angle)/sin(Angle)                    ;
    for(i=0;i<xp;i++)
      DrawPixel(x+i,(unsigned int)(y-k*i),color)  ;
  }
  if(angle>=225&&angle<=315)
  {
    xp = (unsigned int)fabs((length*sin(Angle)))  ;
    k  = cos(Angle)/sin(Angle)                    ;
    for(i=0;i<xp;i++)
      DrawPixel(x-i,(unsigned int)(y+k*i),color)  ;
  }
  else if(angle<45||angle>315)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))  ;
    k  = sin(Angle)/cos(Angle)                    ;
    for(i=0;i<yp;i++)
      DrawPixel((unsigned int)(x+k*i),y-i,color)  ;
  }
  else if(angle>135&&angle<225)
  {
    yp = (unsigned int)fabs((length*cos(Angle)))  ;
    k  = sin(Angle)/cos(Angle)                    ;
    for(i=0;i<yp;i++)
      DrawPixel((unsigned int)(x-k*i),y+i,color)  ;
  }    
}

void Test_TFT(void)
{
  P1DIR    |= BL_CTR                           ;  
  P1OUT    &=~BL_CTR                           ;  
  P2DIR     = 0xFF                             ;
  P3OUT    |= LCD_NRS + LCD_NRD  + LCD_NWR
             +LCD_NCS + LCD_NRST               ;
  P3DIR    |= LCD_NRS + LCD_NRD  + LCD_NWR
             +LCD_NCS + LCD_NRST               ;
  P4DIR     = 0xFF                             ;
  for(;;)
  {
    
    LCD_POWER_OFF         ;
    LCD_POWER_ON          ; // 背光开关
    SET_LCD_RST           ; // LCD复位
    CLR_LCD_RST           ;
    SET_LCD_CS            ;  // LCD片选
    CLR_LCD_CS            ;
    SET_LCD_RS            ;  // LCD寄存器选择
    CLR_LCD_RS            ;
    SET_LCD_WR            ;  // LCD写信号
    CLR_LCD_WR            ; 
    SET_LCD_RD            ;  // LCD读信号
    CLR_LCD_RD            ;    
    
    P4OUT  |=DB00    ; //
    P4OUT  &=~DB00    ; //
    P4OUT  |=DB01    ; //
    P4OUT  &=~DB01    ; //
    P4OUT  |=DB02    ; //
    P4OUT  &=~DB02    ; //
    P4OUT  |=DB03    ; //
    P4OUT  &=~DB03    ; //
    P4OUT  |=DB04    ; //
    P4OUT  &=~DB04    ; //
    P4OUT  |=DB05    ; //
    P4OUT  &=~DB05    ; //
    P4OUT  |=DB06    ; //
    P4OUT  &=~DB06    ; //
    P4OUT  |=DB07    ; //
    P4OUT  &=~DB07    ; //
    
    P2OUT  |=DB00    ; //
    P2OUT  &=~DB00    ; //
    P2OUT  |=DB01    ; //
    P2OUT  &=~DB01    ; //
    P2OUT  |=DB02    ; //
    P2OUT  &=~DB02    ; //
    P2OUT  |=DB03    ; //
    P2OUT  &=~DB03    ; //
    P2OUT  |=DB04    ; //
    P2OUT  &=~DB04    ; //
    P2OUT  |=DB05    ; //
    P2OUT  &=~DB05    ; //
    P2OUT  |=DB06    ; //
    P2OUT  &=~DB06    ; //
    P2OUT  |=DB07    ; //
    P2OUT  &=~DB07    ; //    
  }
}
              
