#ifndef  __ILI9325V430_H_
#define  __ILI9325V430_H_

#include "PIN_DEF.H"
#include "msp430x54x.H"

//---------------------------------- LCD 颜色 --------------------------------------//
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


//---------------------------------- LCD命令定义----------------------------------//
#define  LCD_TYPE          0x9325
#define  START_BYTE        0x70
#define  SET_INDEX         0x00
#define  READ_STATUS       0x01
#define  WRITE_REG         0x02
#define  READ_REG          0x03
#define  WRITE_RAM         0x0022

//---------------------------------- 字库定义----------------------------------//
/*
#define TABLE_HZK24       0x000000                // HZK24字库首地址
#define TABLE_ASCII24     0x0927F0                // ASCII24字库首地址
#define TABLE_HZK16       0x0939F0                // HZK16字库首地址 
#define TABLE_ASCII20     0x0D4F50                // ASCII20字库首地址
#define TABLE_ASCII18     0x0D5E50                // ASCII20字库首地址
#define TABLE_ASCII16     0x0D6BD0                // ASCII20字库首地址
#define TABLE_HZK20       0x0D71D0                // HZK20字库首地址

#define USER_DATA         0x150000                // 用户数据起始地址
*/
#define TABLE_HZK24       0x000000                // HZK24字库首地址
#define TABLE_ASCII24     0x08FC10                // ASCII24字库首地址
#define TABLE_HZK16       0x090E10                // HZK16字库首地址 
#define TABLE_ASCII16     0x0D2370                // ASCII20字库首地址
#define ICON_CLOCK        0x0D3370                // ICON 我的时钟
#define ICON_DISPLAY      0x0D4EB0                // ICON 显示控制
#define ICON_PORT         0x0D69F0                // ICON 端口连接
#define ICON_IRDA         0x0D8530                // ICON 红外传输
#define ICON_NETWORK      0x0DA070                // ICON 网络连接
#define ICON_PAINT        0x0DBBB0                // ICON 画笔
#define ICON_PICTURE      0x0DD6F0                // ICON 我的相册
#define ICON_CONFIG       0x0DF230                // ICON 系统设置
#define ICON_SLEEP        0x0E0D70                // ICON 系统休眠
#define ICON_BACKLIGHT    0x0E28B0                // ICON 背光设置
#define ICON_HELP         0x0E43F0                // ICON 帮助信息
#define ICON_CLOSE        0x0E5F30                // ICON 关闭程序
#define ICON_CALENDER     0x0E65E0                // ICON 日历
#define BMP_CLOCK         0x0E8120                // BMP  钟面
#define BMP_PIC1          0x120560                // BMP  图片1
#define BMP_PIC2          0x1589A0                // BMP  图片2
//#define BMP_PIC3          0x1649E0                // BMP  图片3

//-------------------------------- 写数据到GRAM ------------------------------------//
#define  LCD_WriteRAM(data)    {BUS_OUT               ;\
                                LOW_BYTE_OUT(data)    ;\
                                HIGH_BYTE_OUT(data>>8);\
                                CLR_LCD_WR            ;\
                                SET_LCD_WR            ;}
//-------------------------------- 写索引寄存器 ------------------------------------//
#define  LCD_WriteIndex(index) {CLR_LCD_RS            ;\
                                HIGH_BYTE_OUT(0x00)   ;\
                                LOW_BYTE_OUT(index)   ;\
                                CLR_LCD_WR            ;\
                                SET_LCD_WR            ;\
                                SET_LCD_RS            ;}
//---------------写数据准备-------------------*/
#define  LCD_WriteRAM_Prepare()  {CLR_LCD_CS                 ;\
                                  CLR_LCD_RS                 ;\
                                  LOW_BYTE_OUT(WRITE_RAM)    ;\
                                  HIGH_BYTE_OUT(WRITE_RAM>>8);\
                                  CLR_LCD_WR                 ;\
                                  SET_LCD_WR                 ;\
                                  SET_LCD_RS                 ;}
/*---------------写数据完成-------------------*/
#define  LCD_WriteRAM_Ready() {SET_LCD_CS;\
                               BUS_IN    ;}



void Reset_LCD(void)                                                                         ;
void LCD_WriteReg(unsigned short LCD_Reg, unsigned int LCD_RegValue)                         ;
void WriteIndex(unsigned int data)                                                           ;
unsigned int Read_Reg(unsigned short Reg_Addr)                                               ;
void LCD_Init(void)                                                                          ;
void DrawPixel(unsigned int x, unsigned int y, unsigned int Color)                           ;
void Clear_LCD(unsigned int Color)                                                           ;
unsigned int ReadPixel(unsigned int x,unsigned int y)                                        ;
void DrawHorizonLine(unsigned int x, unsigned int y, unsigned int Length,unsigned int Color) ;              
void DrawVerticalLine(unsigned int x, unsigned int y, unsigned int Length,unsigned int Color);              
void DrawRect(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height,unsigned Color);
void DrawRectFill(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height,unsigned Color);
void SetDisplayWindow(unsigned int Xpos, unsigned int Ypos, unsigned int Width, unsigned int Height)           ;
void WindowModeDisable(unsigned int Color)        ;
void WriteCN24(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)   ;
void WriteCN20(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)   ;
void WriteCN16(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix)   ;
void WriteASCII16(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix);
void WriteASCII18(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix);
void WriteASCII24(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix);
void Delay(unsigned int time)                                                                ;
unsigned int RGB888to565(unsigned long int Color888)                                         ;
void DrawLogo(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color)      ;
void PutStringCN16(unsigned int x,unsigned int y,unsigned char* string  )                    ;
void PutStringCN20(unsigned int x,unsigned int y,unsigned char* string  )                    ;
void PutStringCN24(unsigned int x,unsigned int y,unsigned char* string)                      ;
void SetColor(unsigned long int color)                                       ;
void SetBKColor(unsigned long int color)                                     ;
void ReadMatrix24(unsigned char addrH,unsigned char addrM,unsigned char addrL,char *Matrix);
void PutCharEN24(unsigned int x,unsigned int y,unsigned char* string)        ;
				 
void PutStringEN16(unsigned int x,unsigned int y,unsigned char* string)      ;
void PutString16M(unsigned int x,unsigned int y,unsigned char* string)       ;
void PutCharCN24(unsigned int x,unsigned int y,unsigned char* string)        ;
void PutString24(unsigned int x,unsigned int y,unsigned char* string)        ;
void PutString24M(unsigned int x,unsigned int y,unsigned char* string)       ;
void PutStringEN24(unsigned int x,unsigned int y,unsigned char* string)      ;
void Draw_Test_Point(unsigned int x, unsigned int y,unsigned Color)          ;
void ShowIcon24(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr);
void ShowIcon48(unsigned int x, unsigned int  y, unsigned int w, unsigned int h, unsigned long FlashAddr);
void ShowBMP240320(unsigned long FlashAddr)                                  ;
void ShowBMP128128(unsigned int x, unsigned int  y, unsigned long FlashAddr) ;
void Draw_Line(unsigned int x, unsigned int y,
               unsigned int angle,unsigned int length,
               unsigned color                         )                      ;
void TestEN16(void)                                                          ;
void WriteCN32(unsigned int x, unsigned int y, unsigned int Color,unsigned int BK_Color,char* Matrix);
void Draw7Seg2436(unsigned char value,unsigned int x, unsigned int y,
                  unsigned int Color,unsigned int BK_Color           )       ;


#endif