//===========================================================================//
//                                                                           //
// 文件：  SST25V.C                                                          //
// 说明：  BW-DK5438开发板SST25V016B操作函数                                 //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.23                                                        //
// 备注：  SST25V016B内预置多个中/英文字库，编程时请谨慎处理                 //
//                                                                           //
//===========================================================================//

#include "SST25V.H"

//***************************************************************************//
//									     //
//函数：void Init_SSTSPI(void)                                               //
//说明: 初始化SPI接口                   		                     //
//									     //
//***************************************************************************//
void Init_SSTSPI(void)
{  
  SPI_PORT_OUT    |= LCD_NCS                         ;
  SPI_PORT_DIR    |= LCD_NCS                         ;
  SPI_PORT_OUT    |= NCS25                           ;
  SPI_PORT_DIR    |= NCS25                           ;
  SPI_PORT_SEL    &= 0xF0                            ;
  SPI_PORT_SEL    |= 0x0E                            ; // 端口功能选择为SPI
  UCB0CTL1        |= UCSWRST                         ; // 复位SPI状态机
  UCB0CTL0        |= UCMST+UCSYNC+UCCKPL+UCMSB       ; // 8-bit数据，SPI主机模式, 时钟相位高, MSB在前
  UCB0CTL1        |= UCSSEL_2                        ; // 选择SCK参考源为SMCLK
  UCB0BR0          = 0x02                            ; // SCK = SMCK/2
  UCB0BR1          = 0                               ; 
  UCB0CTL1        &=~UCSWRST                         ; // SPI状态机使能
}

//***************************************************************************//
//									     //
//函数：void Read_JEDECID(char *)                                            //
//说明: 读取SST25VF016B器件ID         		                             //
//									     //
//***************************************************************************//
void Read_JEDECID(char *ID)
{
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x9F                                   ; // 读JEDEC ID  
  __delay_cycles(15)                                 ;
  UCB0TXBUF = 0xFF                                   ;  
  __delay_cycles(15)                                 ;
  ID[0]= UCB0RXBUF                                   ;  
  UCB0TXBUF = 0xFF                                   ;  
  __delay_cycles(15)                                 ;
  ID[1]= UCB0RXBUF                                   ;  
  UCB0TXBUF = 0xFF                                   ;  
  __delay_cycles(15)                                 ;
  ID[2]= UCB0RXBUF                                   ;  
  SET_NCS25                                          ;
}

//***************************************************************************//
//									     //
//函数：void ReadFlash(long ,int,char *)                                     //
//说明: 读取SST25VF016B数据         		                             //
//参数：addr    ——数据地址                                                 //
//      length  ——读取长度                                                 //
//      Buffer  ——数据缓冲区                                               //
//									     //
//***************************************************************************//
void ReadFlash(long addr,int length,char *Buffer)
{
  unsigned char  i                                   ;
  union
  {
    long int value                                   ;
    char byte[4]                                     ;
  } addr_ch                                          ;
  CLR_NCS25                                          ;
  addr_ch.value = addr                               ;
  _NOP()   		                             ; 
  _NOP()      		                             ; 
  _NOP()  		                             ;  
  UCB0TXBUF = 0x03                                   ; // 读命令
  __delay_cycles(15)                                 ;
  UCB0TXBUF = addr_ch.byte[2]                        ; // 地址高字节
  __delay_cycles(15)                                 ;
  UCB0TXBUF = addr_ch.byte[1]                        ; // 地址高字节
  __delay_cycles(15)                                 ;
  UCB0TXBUF = addr_ch.byte[0]                        ; // 地址高字节
  __delay_cycles(15)                                 ;
  for(i=0;i<length;i++)
  {
    UCB0TXBUF = 0xFF                                 ;  
    __delay_cycles(15)                               ;
    Buffer[i] = UCB0RXBUF                            ;  
  }
  SET_NCS25                                          ;
}

//***************************************************************************//
//									     //
//函数：void Sector_Erase(unsigned long)                                     //
//说明: 扇区擦除-4K              		                             //
//参数：Dst     ——扇区地址                                                 //
//									     //
//***************************************************************************//
void Sector_Erase(unsigned long Dst)   
{      
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x20                                   ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF =  Dst>>16                               ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF =  Dst>>8                                ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF =  Dst                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   

//***************************************************************************//
//									     //
//函数：void Byte_Program(unsigned long, unsigned char)                      //
//说明: 字节写入                 		                             //
//参数：Dst     ——数据地址                                                 //
//      byte    ——写入数据                                                 //
//									     //
//***************************************************************************//
void Byte_Program(unsigned long Dst, unsigned char byte)   
{       
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x02                                   ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF = Dst>>16                                ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF = Dst>>8                                 ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF = Dst                                    ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF = byte                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   

//***************************************************************************//
//									     //
//函数：unsigned char Read_Status_Register(void)                             //
//说明: 读取状态寄存器         	                                             //
//返回：状态寄存器值                                                         //
//									     //
//***************************************************************************//
unsigned char Read_Status_Register(void)   
{   
  unsigned char byte = 0                             ;   
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  UCB0TXBUF    = 0x05                                ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF    = 0xFF                                ; 
  __delay_cycles(15)                                 ;
  byte         = UCB0RXBUF                           ;
  SET_NCS25                                          ;
  return byte                                        ;   
}   
   
//***************************************************************************//
//									     //
//函数：void EWSR(void)                                                      //
//说明: 状态寄存器写使能         	                                     //
//									     //
//***************************************************************************//
void EWSR(void)   
{   
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x50                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   
   
//***************************************************************************//
//									     //
//函数：void WRSR(unsigned char byte)                                        //
//说明: 状态寄存器写入           	                                     //
//参数：byte   ——写入数据                                                  //
//									     //
//***************************************************************************//
void WRSR(unsigned char byte)   
{   
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x01                                   ; 
  __delay_cycles(15)                                 ;
  UCB0TXBUF = byte                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   
   
//***************************************************************************//
//									     //
//函数：void WREN(void)                                                      //
//说明: 芯片写使能            	                                             //
//									     //
//***************************************************************************//
void WREN(void)   
{ 
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x06                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   
   
//***************************************************************************//
//									     //
//函数：void WRDI(void)                                                      //
//说明: 芯片写禁止            	                                             //
//									     //
//***************************************************************************//
void WRDI(void)   
{
  Init_SSTSPI()                                      ;  
  CLR_NCS25                                          ;
  _NOP()	   	                             ; 
  _NOP()	   	                             ; 
  _NOP()   		                             ;  
  UCB0TXBUF = 0x04                                   ; 
  __delay_cycles(15)                                 ;
  SET_NCS25                                          ;
}   

//***************************************************************************//
//									     //
//函数：void Wait_Busy(void)                                                 //
//说明: 芯片忙检测           	                                             //
//									     //
//***************************************************************************//
void Wait_Busy(void)
{   
  while(Read_Status_Register()&0x01)  
    Read_Status_Register()                           ;   
}   

