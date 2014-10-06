#include "PIN_DEF.H"
#define Key_Down 0x01
#define Key_Up   0x00

//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ�
#define CMD_RDY 0X90  //0B10010000���ò�ַ�ʽ��X����
#define CMD_RDX	0XD0  //0B11010000���ò�ַ�ʽ��Y���� 
#define TEMP_RD	0XF0  //0B11110000���ò�ַ�ʽ��Y���� 

//���رʽṹ��
typedef struct 
{
  unsigned int   X0                                    ; //��������
  unsigned int   Y0                                    ;
  unsigned int   X                                     ; //λ������
  unsigned int   Y                                     ;						   	    
  unsigned char  Key_Sta                               ; //�ʵ�״̬			  
  float          cx                                    ; //������У׼����
  float          cy                                    ;
  short          ox                                    ;
  short          oy                                    ;
}Pen_Holder                                            ;	   


void TP_Init(void)                                     ;
void TP_Write_Byte(unsigned char data)                 ; // �����оƬд��һ������
unsigned char Read_TP(unsigned int *x,unsigned int *y) ; // ��������˫�����ȡ
unsigned char Read_TP_Twice(unsigned int *x,unsigned int *y); // ����ǿ�˲���˫���������ȡ
unsigned int TP_Read_XY(unsigned char xy)              ; // ���˲��������ȡ(������)
unsigned int TP_Read_AD(unsigned char CMD)             ; // ��ȡADת��ֵ
void Drow_Touch_Point(unsigned char x,unsigned int y)  ; // ��һ�������׼��
void Draw_Big_Point(unsigned char x,unsigned int y)    ; // ��һ�����
unsigned char TP_Adjust(unsigned int *pos_temp,
                        float *cx,float *ox,
                        float *cy,float *oy)           ; // ������У׼                                               
void Save_Adjdata(void)		                       ; // ����У׼����
unsigned char Get_Adjdata(void)                        ; // ��ȡУ׼����
void Pen_Int_Set(unsigned char en)                     ; // PEN�ж�ʹ��/�ر�
//void Convert_Pos(void)                                 ; // ���ת������	 
void Convert_Pos(unsigned int x,unsigned y, 
                 unsigned *xp,unsigned int *yp)        ; // ���ת��
void Wait_Pen_Up(void)                                 ; // �ȴ����ر�̧��
















