//===========================================================================//
//                                                                           //
// 文件：  IIC.H                                                             //
// 说明：  BW-DK5438开发板I2C接口函数                                        //
// 编译：  IAR Embedded Workbench IDE for MSP430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.23                                                        //
//                                                                           //
//===========================================================================//

#include <msp430x54x.h>
#include "PIN_DEF.H"
typedef unsigned char uchar;
typedef unsigned int  uint;

#define TRUE    1
#define FALSE   0

void Init_I2C(void);
void delay(void);
void start(void);
void stop(void);
void mack(void);
void mnack(void);
unsigned char check(void);
void write1(void);
void write0(void);
void write1byte(unsigned char wdata);
unsigned char writeNbyte(unsigned char * outbuffer,unsigned char n);
unsigned char read1byte(void);
void readNbyte(unsigned char * inbuffer,unsigned char n);



/*******************************************
函数名称：Init_I2C
功    能：设置IIC对应IO为输出方向并输出高电平
参    数：无
返回值  ：无
********************************************/
void Init_I2C(void)
{
    SCL_out;
    SDA_out;
    SCL_H;
    SDA_H;
    SDA_pull_up;
}
/*******************************************
函数名称：delay
功    能：延时约100us的时间
参    数：无
返回值  ：无
********************************************/
void delay(void)
{
    uchar i;
    
    for(i = 0;i < 15;i++)
        _NOP();
}	
/*******************************************
函数名称：start
功    能：完成IIC的起始条件操作
参    数：无
返回值  ：无
********************************************/
void start(void)
{
    SCL_H;
    SDA_H;
    delay();
    SDA_L;
    delay();
    SCL_L;
    delay();
}
/*******************************************
函数名称：stop
功    能：完成IIC的终止条件操作
参    数：无
返回值  ：无
********************************************/
void stop(void)
{
    SCL_H;
    SDA_L;	
    delay();
    SDA_H;
    delay();	   
}
/*******************************************
函数名称：check
功    能：检查从机的应答操作
参    数：无
返回值  ：从机是否有应答：1--有，0--无
********************************************/
uchar check(void)
{
    uchar slaveack;
    SDA_in;
    SCL_H;
    delay();
    slaveack = SDA_val;   //读入SDA数值
    SCL_L;
    delay();
    SDA_out;
    if(slaveack)    return FALSE;
    else            return TRUE;
}
/*******************************************
函数名称：write1
功    能：向IIC总线发送一个1
参    数：无
返回值  ：无
********************************************/
void write1(void)
{
    SDA_H;
    delay();
    SCL_H;
    delay();
    SCL_L;				
    delay();
}
/*******************************************
函数名称：write0
功    能：向IIC总线发送一个0
参    数：无
返回值  ：无
********************************************/
void write0(void)
{
    SDA_L;
    delay();
    SCL_H;
    delay();			 
    SCL_L;				
    delay();
}
/*******************************************
函数名称：write1byte
功    能：向IIC总线发送一个字节的数据
参    数：wdata--发送的数据
返回值  ：无
********************************************/
void write1byte(uchar wdata)
{
    uchar i;

    for(i = 8;i > 0;i--)
    {
        if(wdata & 0x80)	write1();
        else 			write0();
        wdata <<= 1;
    }                    
}
