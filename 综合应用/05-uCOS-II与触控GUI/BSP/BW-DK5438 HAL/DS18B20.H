#ifndef __DS18B20H_
#define __DS18B20H_

#include "MCU_TYPE.H"
#ifdef    MSP430F5438
#include "MSP430x54x.h"
#endif
#ifdef    MSP430F5438A
#include "MSP430x54xA.h"
#endif
#include "PIN_DEF.H"

#define OW_DIO_PULLUP P10REN |= DIO_B20
#define OW_OUT_1      P10OUT |= DIO_B20
#define OW_OUT_0      P10OUT &=~DIO_B20
#define OW_IN         P10DIR &=~DIO_B20
#define OW_OUT        P10DIR |= DIO_B20
#define OW_IN_DATA   (P10IN  &  DIO_B20)>>6

/*软件延时时间定义*/
//系统MCLK主时钟为1M，可根据单片机主频调以下延时
#define Read_time      70*22  //写一位延时
#define Write_time     50*22  //读一位延时
#define RstL_time      480*22 //复位低电平延时
#define RstH_time      70*22  //复位高电平延时
#define RstR_time      360*22 //复位释放延时
#define Wait_time      40*22  // 等待响应时间

unsigned int Do1Convert(void)                          ;

#endif