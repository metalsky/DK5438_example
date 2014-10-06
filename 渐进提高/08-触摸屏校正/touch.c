//===========================================================================//
//                                                                           //
// 文件：  LED.H                                                             //
// 说明：  BW-DK5438开发板触摸控制函数库                                     //
//         TPIC —ADS7843/7846/UH7843/7846/XPT2046/TSC2046                   //
// 编译：  IAR Embedded Workbench IDE for msp430 v4.21                       //
// 版本：  v1.1                                                              //
// 编写：  JASON.ZHANG                                                       //
// 版权：  北京拓普博维电子技术有限公司                                      //
// 日期：  2010.09.19                                                        //
//                                                                           //
//===========================================================================//
#include <msp430x54x.h>
#include "touch.h" 
#include "stdlib.h"
#include "math.h"

extern Pen_Holder Pen_Point                         ;
extern float cx[1],cy[1],ox[1],oy[1]                ;

void TP_Init(void)
{
  P10OUT   &= PSCK                                  ;
  P10DIR   |= PMOSI+PSCK+PNSS                       ;
  P10DIR   &=~PMISO                                 ;
  P10REN   |= PMISO                                 ;
  P3OUT    |= TP_CS                                 ;  
  P3DIR    |= TP_CS                                 ;  
  P5DIR    &=~J60_CS                                ; // 关ENC28J60片选
}

//向TPIC写入1byte数据   
void TP_Write_Byte(unsigned char data)    
{  
  unsigned char temp,i                              ; 
  temp       =  data                                ;  
  for(i=0;i<8;i++)  
  {
    if(temp&0x80)      SET_PMOSI                    ;  
    else               CLR_PMOSI                    ;   
    temp   <<=1                                     ;    
    SET_PSCK                                        ;
    __delay_cycles(5)                               ;
    CLR_PSCK                                        ;	   	 
  } 			    
}

//从TPIC读取ADC值	   
unsigned int TP_Read_AD(unsigned char CMD)	  
{ 
  unsigned char i                                   ; 	  
  unsigned int  rec = 0                             ; 
  CLR_PSCK                                          ;	   	 
  CLR_TP_CS                                         ;	 
  TP_Write_Byte(CMD)                                ; // 发送命令字
  __delay_cycles(200)                               ; // 等待响应
  for(i=0;i<16;i++)  
  { 				  
    SET_PSCK                                        ;	   	 
    __delay_cycles(5)                               ;
    CLR_PSCK                                        ;	   	 
    rec          <<= 1                              ; 	 
    if(PMISO_IS_SET)    rec++                       ; 		 
  }  	
  rec            >>= 4                              ; //高12位有效
  SET_TP_CS                                         ;	 
  return(rec)                                       ;   
}

//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 

#define READ_TIMES 15     //读取次数
#define LOST_VAL    5	  //丢弃值
unsigned int TP_Read_XY(unsigned char xy)
{
  unsigned int  i, j                                ;
  unsigned int  buf[READ_TIMES]                     ;
  unsigned temp,sum = 0                             ;
  for(i=0;i<READ_TIMES;i++)
    buf[i]=TP_Read_AD(xy)                           ;	    
  for(i=0;i<READ_TIMES-1; i++)                        //排序
  {
    for(j=i+1;j<READ_TIMES;j++)
    {
      if(buf[i]>buf[j])                               //升序排列
      {
        temp   = buf[i]                             ;
	buf[i] = buf[j]                             ;
	buf[j] = temp                               ;
      }
    }
  }
  for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
    sum  += buf[i]                                  ;
  sum  /=(READ_TIMES-2*LOST_VAL)                    ;
  return sum                                        ;   
} 
//带滤波的坐标读取
//最小值不能少于100.
unsigned char Read_TP(unsigned int *x,unsigned int *y)
{
  unsigned int xtemp,ytemp                          ;			 	 		  
  xtemp = TP_Read_XY(CMD_RDX)                       ;
  ytemp = TP_Read_XY(CMD_RDY)                       ;	  												   
  if(  xtemp<100 ||ytemp<100
     ||xtemp>4000||ytemp>4000)
    return 0                                        ; //读数失败
  *x    = xtemp                                     ;
  *y    = ytemp                                     ;
  return 1                                          ; //读数成功
}	
//2次读取TPIC,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 50 //误差范围 
unsigned char Read_TP_Twice(unsigned int *x,unsigned int *y) 
{
  unsigned int x1,y1                                ;
  unsigned int x2,y2                                ;
  unsigned char flag                                ;    
  flag = Read_TP(&x1,&y1)                           ;   
  if(flag==0)
    return    0                                     ;
  flag = Read_TP(&x2,&y2)                           ;	   
  if(flag==0)
    return    0                                     ;   
  if(  ((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
     &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
  {
    *x   = (x1+x2)/2                                ;
    *y   = (y1+y2)/2                                ;
     return 1                                       ;
  }
  return 0                                          ;	  
} 
//读取一次坐标值	
//仅仅读取一次,直到PEN松开才返回!					   
unsigned char Read_TP_Once(void)
{
  unsigned char t=0;	    
//  TP_INT_DISABLE                                   ; //关闭中断
  Pen_Point.Key_Sta = Key_Up                        ;
  Read_TP_Twice(&Pen_Point.X,&Pen_Point.Y)          ;
  while(TP_INT_IS_CLR&&t<=250)
  {
    t++                                             ;
    __delay_cycles(200000)                          ;
  }                                                 ;
//  TP_INT_ENABLE                              ;//开启中断		 
  if(t>250)    return 0                             ;//按下2.5s 认为无效
  else         return 1                             ;	
}

//////////////////////////////////////////////////

//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
void Convert_Pos(unsigned int x,unsigned y, unsigned *xp,unsigned int *yp)
{  
  *xp =(unsigned int)(cx[0]*(float)x+ox[0])  ;
  *yp =(unsigned int)(cy[0]*(float)y+oy[0])  ;
}

/*
//中断,检测到PEN脚的一个下降沿.
//置位Pen_Point.Key_Sta为按下状态
//中断线0线上的中断检测
void EXTI1_IRQHandler(void)
{ 		   			 
	Pen_Point.Key_Sta=Key_Down;//按键按下  		  				 
	EXTI->PR=1<<1;  //清除LINE1上的中断标志位 
} 

//PEN中断设置	 
void Pen_Int_Set(unsigned char en)
{
	if(en)EXTI->IMR|=1<<1;   //开启line1上的中断	  	
	else EXTI->IMR&=~(1<<1); //关闭line1上的中断	   
}	  

//////////////////////////////////////////////////////////////////////////
//此部分涉及到使用外部EEPROM,如果没有外部EEPROM,屏蔽此部分即可
#ifdef ADJ_SAVE_ENABLE
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//保存校准参数										    
void Save_Adjdata(void)
{
	s32 temp;			 
	//保存校正结果!		   							  
	temp=Pen_Point.xfac*100000000;//保存x校正因素      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//保存y校正因素    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//保存x偏移量
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//保存y偏移量
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
			     							 
	temp=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);
	temp&=0XF0;
	temp|=0X0A;//标记校准过了
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,temp);			 
}
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
unsigned char Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(52);//第五十二字节的第四位用来标记是否校准过！ 		 
	if((tempfac&0X0F)==0X0A)//触摸屏已经校准过了			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(40,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//得到x校准参数
		tempfac=AT24CXX_ReadLenByte(44,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//得到y校准参数
	    //得到x偏移量
		tempfac=AT24CXX_ReadLenByte(48,2);			   	  
		Pen_Point.xoff=tempfac;					 
	    //得到y偏移量
		tempfac=AT24CXX_ReadLenByte(50,2);				 	  
		Pen_Point.yoff=tempfac;					 
		return 1;	 
	}
	return 0;
}
#endif		
*/

//触摸屏校准代码
//得到四个校准参数
/*
unsigned char TP_Adjust(unsigned int *pos_temp,
                        float *cx,float *ox,
                        float *cy,float *oy)
*/
unsigned char TP_Adjust(unsigned int *pos_temp)
{
  unsigned int d1,d2                             ;
  unsigned long tem1,tem2                        ;
  float fac                                      ; 	   
  
  tem1   = abs(pos_temp[0]-pos_temp[2])          ; // x1-x2
  tem2   = abs(pos_temp[1]-pos_temp[3])          ; // y1-y2
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // 得到1,2的距离					
  tem1   = abs(pos_temp[4]-pos_temp[6])          ; // x3-x4
  tem2   = abs(pos_temp[5]-pos_temp[7])          ; // y3-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // 得到3,4的距离
  fac    = (float)d1/d2                          ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)
    return 0x00                                  ; // 误差超限，返回校正错误
  
  tem1   = abs(pos_temp[0]-pos_temp[4])          ; // x1-x3
  tem2   = abs(pos_temp[1]-pos_temp[5])          ; // y1-y3
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // 得到1,3的距离
  tem1   = abs(pos_temp[2]-pos_temp[6])          ; // x2-x4
  tem2   = abs(pos_temp[3]-pos_temp[7])          ; // y2-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // 得到2,4的距离
  fac    =(float)d1/d2                           ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)               // 不合格
    return 0x00                                  ; // 误差超限，返回校正错误	
  
  tem1   = abs(pos_temp[0]-pos_temp[6])          ; // x1-x4
  tem2   = abs(pos_temp[1]-pos_temp[7])          ; // y1-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // 得到1,4的距离	
  tem1   = abs(pos_temp[2]-pos_temp[4])          ; // x2-x3
  tem2   = abs(pos_temp[3]-pos_temp[5])          ; // y2-y3
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // 得到2,3的距离
  fac    =(float)d1/d2                           ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)
    return 0xFF                                  ; // 误差超限，返回校正错误
//  *cx    =(float)200/(pos_temp[2]-pos_temp[0])   ; // 得到xfac		 
//  *ox    =(240-*cx*(pos_temp[2]+pos_temp[0]))/2  ; // 得到xoff				  
//  *cy    =(float)280/(pos_temp[5]-pos_temp[1])   ; // 得到yfac
//  *oy    =(320-*cy*(pos_temp[5]+pos_temp[1]))/2  ; // 得到yoff 		 
  cx[0]    =200/((float)pos_temp[2]-(float)pos_temp[0])   ; // 得到xfac		  
  ox[0]    =(240-cx[0]*((float)pos_temp[2]+(float)pos_temp[0]))/2  ; // 得到xoff				  
  cy[0]    =280/((float)pos_temp[5]-(float)pos_temp[1])   ; // 得到yfac
  oy[0]    =(320-cy[0]*((float)pos_temp[5]+(float)pos_temp[1]))/2  ; // 得到yoff 
  return 0x01                                    ;
}		

void Wait_Pen_Up(void)
{
  unsigned int x1,y1                             ;
  while(Read_TP(&x1,&y1)){}                      ;
}
