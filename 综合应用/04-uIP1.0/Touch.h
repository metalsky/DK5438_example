#include "PIN_DEF.H"
#define Key_Down 0x01
#define Key_Up   0x00

//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 指令集
#define CMD_RDY 0X90  //0B10010000即用差分方式读X坐标
#define CMD_RDX	0XD0  //0B11010000即用差分方式读Y坐标 
#define TEMP_RD	0XF0  //0B11110000即用差分方式读Y坐标 

//触控笔结构体
typedef struct 
{
  unsigned int   X0                                    ; //电气坐标
  unsigned int   Y0                                    ;
  unsigned int   X                                     ; //位置坐标
  unsigned int   Y                                     ;						   	    
  unsigned char  Key_Sta                               ; //笔的状态			  
  float          cx                                    ; //触摸屏校准参数
  float          cy                                    ;
  short          ox                                    ;
  short          oy                                    ;
}Pen_Holder                                            ;	   


void TP_Init(void)                                     ;
void TP_Write_Byte(unsigned char data)                 ; // 向控制芯片写入一个数据
unsigned char Read_TP(unsigned int *x,unsigned int *y) ; // 带舍弃的双方向读取
unsigned char Read_TP_Twice(unsigned int *x,unsigned int *y); // 带加强滤波的双方向坐标读取
unsigned int TP_Read_XY(unsigned char xy)              ; // 带滤波的坐标读取(单方向)
unsigned int TP_Read_AD(unsigned char CMD)             ; // 读取AD转换值
void Drow_Touch_Point(unsigned char x,unsigned int y)  ; // 画一个坐标叫准点
void Draw_Big_Point(unsigned char x,unsigned int y)    ; // 画一个大点
unsigned char TP_Adjust(unsigned int *pos_temp,
                        float *cx,float *ox,
                        float *cy,float *oy)           ; // 触摸屏校准                                               
void Save_Adjdata(void)		                       ; // 保存校准参数
unsigned char Get_Adjdata(void)                        ; // 读取校准参数
void Pen_Int_Set(unsigned char en)                     ; // PEN中断使能/关闭
//void Convert_Pos(void)                                 ; // 结果转换函数	 
void Convert_Pos(unsigned int x,unsigned y, 
                 unsigned *xp,unsigned int *yp)        ; // 结果转换
void Wait_Pen_Up(void)                                 ; // 等待触控笔抬起
















