#ifndef _EDIT_H
#define _EDIT_H

#define    MAX_INTERVAL     150000                // �����������
#define    INPUT_PY           0x01
#define    INPUT_HZ           0x02
#define    EXIT_INPUT         0x03
#define    SHOULD_FLICKER     BIT1                // Ӧ��˸���

#define    PRE_EDIT           0xFE                // �л�����һEdit�ؼ�
#define    NEXT_EDIT          0xFD                // �л�����һEdit�ؼ�
#define    INP_DIG            BIT0                // ����ģʽ ������
#define    INP_EN             BIT1                // ����ģʽ ��Ӣ����ĸ
#define    INP_CN             BIT2                // ����ģʽ ������

// ������ɫ
#define WINDOW_BK_COLOR       0xDFFF              // ���ڱ���ɫ
#define WINDOW_COLOR          0x11FA              // ����ǰ��ɫ

#define TITLE_BK_COLOR        0x11FA              // ����������ɫ
#define TITLE_COLOR           0xDFFF              // ������ǰ��ɫ
#define STATUS_BK_COLOR       0x0014              // ״̬������ɫ
#define STATUS_COLOR          0xDFFF              // ״̬��ǰ��ɫ

#define MENU_BACK_COLOR	      WINDOW_BK_COLOR     // δѡ��˵����ɫ
#define MENU_FONT_COLOR	      WINDOW_COLOR 	  // δѡ��˵�������ɫ
#define MENU_SELE_COLOR	      WINDOW_COLOR        // ѡ��˵����ɫ
#define MENU_SELF_COLOR	      WINDOW_BK_COLOR	  // ѡ��˵�������ɫ


#define EDIT_VALID            BIT7                // �༭�ؼ�״̬        ��1 - ��Ч    0 - ��Ч
#define EDIT_ACTIVE           BIT6                // �༭�ؼ�״̬        ��1 - �    0 - �ǻ 
#define EDIT_FONT             BIT5                // �༭�ؼ�����        ��1 - 24����  0 - 16����
#define EDIT_MODE             BIT4                // �༭�ؼ�ģʽ        ��1 - ɾ��ʽ  0 - ����ʽ
#define EDIT_CURSOR           0x0F                // �༭�ؼ����λ��    
#define EDIT_FRAME            0xE0                // �༭�ؼ��߿�����    : 
#define EDIT_LIMIT            0x1F                // �༭�ؼ�����ַ���  

typedef struct
{
  unsigned char id                              ; // ʶ���
  unsigned int  x                               ; // ��ʼλ�ú�����x
  unsigned int  y                               ; // ��ʼλ��������y
  unsigned char width                           ; // ���
  unsigned char height                          ; // �߶�
  unsigned char property                        ; // ����
  unsigned char status                          ; // ״̬
  unsigned char limit                           ; // ����ַ���
  char string[50]                               ; // �ַ���
} EDIT_DIG                                      ; // �༭�ؼ�

void CreateEditDig(unsigned char ID,unsigned int x_s   ,unsigned int y_s,
                   unsigned char width,unsigned char height)                                 ;
void IniEditDig(unsigned char ID   ,unsigned char type  ,unsigned char font,
                unsigned char frame,unsigned char limit ,char* p             )               ;
void SetEditDig(unsigned char ID,char* p)                                                    ;				
void ActiveEditDig(unsigned char ID   ,unsigned char Active)  ;	
void EditDigMessage(unsigned char Message,char * p)           ;
void RedrawEditDig(unsigned char Index)                       ;                    
void ReadEditDig(unsigned char ID,unsigned char *Ret_Buf)     ;
void DeleteEditDig(unsigned char ID)                          ;
void Flicker_Cursor(void)                                     ;

#endif