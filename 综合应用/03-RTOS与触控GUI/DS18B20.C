#include "DS18B20.H"

/*
延时函数
in:  count  延时的时间长度 uS
out:无*/


void OW_Delay(unsigned int count)
{   
  unsigned int i,j       ;
  for(i=2;i>0;i--)
    for(j=count;j>0;j--)
      _NOP()             ;  
}

/*******************************************
函数名称：Init_18B20
功    能：对DS18B20进行复位操作
参    数：无
返回值  ：初始化状态标志：1--失败，0--成功
********************************************/
unsigned char Init_18B20(void)
{
  unsigned char flag;
  _DINT();
  
  OW_OUT;  
  OW_OUT_0;
  __delay_cycles(RstL_time);
  OW_IN;
  __delay_cycles(Wait_time);
  if(OW_IN_DATA) 
  {
    flag = 1;          //初始化失败
  }
  else
  {
    flag = 0;          //初始化成功
  }
  OW_OUT;
  OW_OUT_1;
  __delay_cycles(RstR_time);
  _EINT();  
  return (flag);
}
/*******************************************
函数名称：Write_18B20
功    能：向DS18B20写入一个字节的数据
参    数：wdata--写入的数据
返回值  ：无
********************************************/
void Write_18B20(unsigned char data)
{
  unsigned char i;
  
  _DINT();

  for(i = 0; i < 8;i++)
  {
    OW_OUT_0;
    if(data & 0x01)    
    {
      OW_OUT_1;
    }
    else 
    {
      OW_OUT_0;
    }
    data >>= 1;
    __delay_cycles(Write_time);           //延时50us
    OW_OUT_1;
  }
  
  _EINT();
}
/*******************************************
函数名称：Read_18B20
功    能：从DS18B20读取一个字节的数据
参    数：无
返回值  ：读出的一个字节数据
********************************************/
unsigned char Read_18B20(void)
{
  unsigned char i;
  unsigned char temp = 0;
  
  _DINT();
  
  for(i = 0;i < 8;i++)
  {
    temp >>= 1;
    OW_OUT_0;
    OW_OUT_1;
    OW_IN;
    _NOP();
    if(OW_IN_DATA)  
    {
      temp |= 0x80;
    }
    __delay_cycles(Read_time);           //延时70us
    OW_OUT;
    OW_OUT_1;
  }
  _EINT();
  
  return  temp;
}

/*******************************************
函数名称：Skip
功    能：发送跳过读取产品ID号命令
参    数：无
返回值  ：无
********************************************/
void Skip(void)
{
  Write_18B20(0xcc);
}
/*******************************************
函数名称：Convert
功    能：发送温度转换命令
参    数：无
返回值  ：无
********************************************/
void Convert(void)
{
  Write_18B20(0x44);
}
/*******************************************
函数名称：Read_SP
功    能：发送读ScratchPad命令
参    数：无
返回值  ：无
********************************************/
void Read_SP(void)
{
  Write_18B20(0xbe);
}
/*******************************************
函数名称：ReadTemp
功    能：从DS18B20的ScratchPad读取温度转换结果
参    数：无
返回值  ：读取的温度数值
********************************************/
unsigned int ReadTemp(void)
{
  unsigned char temp_low;
  unsigned int  temp;
  
  temp_low = Read_18B20();      //读低位
  temp = Read_18B20();          //读高位
  temp = (temp<<8) | temp_low;
  
  return  temp;
}
/*******************************************
函数名称：ReadTemp
功    能：控制DS18B20完成一次温度转换
参    数：无
返回值  ：测量的温度数值
********************************************/
unsigned int Do1Convert(void)
{
  unsigned char i,j; 
  j=0;
  do
  {
    i = Init_18B20();
    j++;
    if(j>200) 
      return 0x0000;  //复位超时返回0
  }
  while(i);
  Skip();
  Convert();
  for(i = 20;i > 0;i--)  
  {
    __delay_cycles(22000); //延时800ms以上
  }
  j=0;
  do
  {
    i = Init_18B20();
    j++;
    if(j>200) return 0x0000;  //复位超时返回0
  }
  while(i);
  Skip();
  Read_SP();
  return ReadTemp();
}
