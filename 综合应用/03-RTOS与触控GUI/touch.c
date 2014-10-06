//===========================================================================//
//                                                                           //
// �ļ���  LED.H                                                             //
// ˵����  BW-DK5438�����崥�����ƺ�����                                     //
//         TPIC ��ADS7843/7846/UH7843/7846/XPT2046/TSC2046                   //
// ���룺  IAR Embedded Workbench IDE for msp430 v4.21                       //
// �汾��  v1.1                                                              //
// ��д��  JASON.ZHANG                                                       //
// ��Ȩ��  �������ղ�ά���Ӽ������޹�˾                                      //
// ���ڣ�  2010.09.19                                                        //
//                                                                           //
//===========================================================================//
#include <RTOS.H>
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
  P5DIR    &=~J60_CS                                ; // ��ENC28J60Ƭѡ
}

//��TPICд��1byte����   
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

//��TPIC��ȡADCֵ	   
unsigned int TP_Read_AD(unsigned char CMD)	  
{ 
  unsigned char i                                   ; 	  
  unsigned int  rec = 0                             ; 
  CLR_PSCK                                          ;	   	 
  CLR_TP_CS                                         ;	 
  TP_Write_Byte(CMD)                                ; // ����������
  __delay_cycles(200)                               ; // �ȴ���Ӧ
  for(i=0;i<16;i++)  
  { 				  
    SET_PSCK                                        ;	   	 
    __delay_cycles(5)                               ;
    CLR_PSCK                                        ;	   	 
    rec          <<= 1                              ; 	 
    if(PMISO_IS_SET)    rec++                       ; 		 
  }  	
  rec            >>= 4                              ; //��12λ��Ч
  SET_TP_CS                                         ;	 
  return(rec)                                       ;   
}

//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 

#define READ_TIMES 15     //��ȡ����
#define LOST_VAL    5	  //����ֵ
unsigned int TP_Read_XY(unsigned char xy)
{
  unsigned int  i, j                                ;
  unsigned int  buf[READ_TIMES]                     ;
  unsigned temp,sum = 0                             ;
  for(i=0;i<READ_TIMES;i++)
    buf[i]=TP_Read_AD(xy)                           ;	    
  for(i=0;i<READ_TIMES-1; i++)                        //����
  {
    for(j=i+1;j<READ_TIMES;j++)
    {
      if(buf[i]>buf[j])                               //��������
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
//���˲��������ȡ
//��Сֵ��������100.
unsigned char Read_TP(unsigned int *x,unsigned int *y)
{
  unsigned int xtemp,ytemp                          ;			 	 		  
  xtemp = TP_Read_XY(CMD_RDX)                       ;
  ytemp = TP_Read_XY(CMD_RDY)                       ;	  												   
  if(  xtemp<100 ||ytemp<100
     ||xtemp>4000||ytemp>4000)
    return 0                                        ; //����ʧ��
  *x    = xtemp                                     ;
  *y    = ytemp                                     ;
  return 1                                          ; //�����ɹ�
}	
//2�ζ�ȡTPIC,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 50 //��Χ 
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
  if(  ((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
     &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
  {
    *x   = (x1+x2)/2                                ;
    *y   = (y1+y2)/2                                ;
     return 1                                       ;
  }
  return 0                                          ;	  
} 
//��ȡһ������ֵ	
//������ȡһ��,ֱ��PEN�ɿ��ŷ���!					   
unsigned char Read_TP_Once(void)
{
  unsigned char t=0;	    
//  TP_INT_DISABLE                                   ; //�ر��ж�
  Pen_Point.Key_Sta = Key_Up                        ;
  Read_TP_Twice(&Pen_Point.X,&Pen_Point.Y)          ;
  while(TP_INT_IS_CLR&&t<=250)
  {
    t++                                             ;
    __delay_cycles(200000)                          ;
  }                                                 ;
//  TP_INT_ENABLE                              ;//�����ж�		 
  if(t>250)    return 0                             ;//����2.5s ��Ϊ��Ч
  else         return 1                             ;	
}

//////////////////////////////////////////////////

//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
void Convert_Pos(unsigned int x,unsigned y, unsigned *xp,unsigned int *yp)
{  
  *xp =(unsigned int)(cx[0]*(float)x+ox[0])  ;
  *yp =(unsigned int)(cy[0]*(float)y+oy[0])  ;
}

/*
//�ж�,��⵽PEN�ŵ�һ���½���.
//��λPen_Point.Key_StaΪ����״̬
//�ж���0���ϵ��жϼ��
void EXTI1_IRQHandler(void)
{ 		   			 
	Pen_Point.Key_Sta=Key_Down;//��������  		  				 
	EXTI->PR=1<<1;  //���LINE1�ϵ��жϱ�־λ 
} 

//PEN�ж�����	 
void Pen_Int_Set(unsigned char en)
{
	if(en)EXTI->IMR|=1<<1;   //����line1�ϵ��ж�	  	
	else EXTI->IMR&=~(1<<1); //�ر�line1�ϵ��ж�	   
}	  

//////////////////////////////////////////////////////////////////////////
//�˲����漰��ʹ���ⲿEEPROM,���û���ⲿEEPROM,���δ˲��ּ���
#ifdef ADJ_SAVE_ENABLE
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//����У׼����										    
void Save_Adjdata(void)
{
	s32 temp;			 
	//����У�����!		   							  
	temp=Pen_Point.xfac*100000000;//����xУ������      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=Pen_Point.yfac*100000000;//����yУ������    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//����xƫ����
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,Pen_Point.xoff,2);		    
	//����yƫ����
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,Pen_Point.yoff,2);	
			     							 
	temp=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);
	temp&=0XF0;
	temp|=0X0A;//���У׼����
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,temp);			 
}
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
unsigned char Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(52);//����ʮ���ֽڵĵ���λ��������Ƿ�У׼���� 		 
	if((tempfac&0X0F)==0X0A)//�������Ѿ�У׼����			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(40,4);		   
		Pen_Point.xfac=(float)tempfac/100000000;//�õ�xУ׼����
		tempfac=AT24CXX_ReadLenByte(44,4);			          
		Pen_Point.yfac=(float)tempfac/100000000;//�õ�yУ׼����
	    //�õ�xƫ����
		tempfac=AT24CXX_ReadLenByte(48,2);			   	  
		Pen_Point.xoff=tempfac;					 
	    //�õ�yƫ����
		tempfac=AT24CXX_ReadLenByte(50,2);				 	  
		Pen_Point.yoff=tempfac;					 
		return 1;	 
	}
	return 0;
}
#endif		
*/

//������У׼����
//�õ��ĸ�У׼����
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
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�1,2�ľ���					
  tem1   = abs(pos_temp[4]-pos_temp[6])          ; // x3-x4
  tem2   = abs(pos_temp[5]-pos_temp[7])          ; // y3-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�3,4�ľ���
  fac    = (float)d1/d2                          ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)
    return 0x00                                  ; // ���ޣ�����У������
  
  tem1   = abs(pos_temp[0]-pos_temp[4])          ; // x1-x3
  tem2   = abs(pos_temp[1]-pos_temp[5])          ; // y1-y3
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�1,3�ľ���
  tem1   = abs(pos_temp[2]-pos_temp[6])          ; // x2-x4
  tem2   = abs(pos_temp[3]-pos_temp[7])          ; // y2-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�2,4�ľ���
  fac    =(float)d1/d2                           ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)               // ���ϸ�
    return 0x00                                  ; // ���ޣ�����У������	
  
  tem1   = abs(pos_temp[0]-pos_temp[6])          ; // x1-x4
  tem2   = abs(pos_temp[1]-pos_temp[7])          ; // y1-y4
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d1     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�1,4�ľ���	
  tem1   = abs(pos_temp[2]-pos_temp[4])          ; // x2-x3
  tem2   = abs(pos_temp[3]-pos_temp[5])          ; // y2-y3
  tem1  *= tem1                                  ;
  tem2  *= tem2                                  ;
  d2     =(unsigned int)sqrt(tem1+tem2)          ; // �õ�2,3�ľ���
  fac    =(float)d1/d2                           ;
  if(fac<0.9||fac>1.1||d1==0||d2==0)
    return 0xFF                                  ; // ���ޣ�����У������
//  *cx    =(float)200/(pos_temp[2]-pos_temp[0])   ; // �õ�xfac		 
//  *ox    =(240-*cx*(pos_temp[2]+pos_temp[0]))/2  ; // �õ�xoff				  
//  *cy    =(float)280/(pos_temp[5]-pos_temp[1])   ; // �õ�yfac
//  *oy    =(320-*cy*(pos_temp[5]+pos_temp[1]))/2  ; // �õ�yoff 		 
  cx[0]    =200/((float)pos_temp[2]-(float)pos_temp[0])   ; // �õ�xfac		  
  ox[0]    =(240-cx[0]*((float)pos_temp[2]+(float)pos_temp[0]))/2  ; // �õ�xoff				  
  cy[0]    =280/((float)pos_temp[5]-(float)pos_temp[1])   ; // �õ�yfac
  oy[0]    =(320-cy[0]*((float)pos_temp[5]+(float)pos_temp[1]))/2  ; // �õ�yoff 
  return 0x01                                    ;
}		

void Wait_Pen_Up(void)
{
  unsigned int x1,y1                             ;
  while(Read_TP(&x1,&y1)){}                      ;
}
