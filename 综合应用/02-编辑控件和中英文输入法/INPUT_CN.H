#ifndef __INPUT_CN_H
#define __INPUT_CN_H


typedef struct
{
  char          *t9PY_T9    ;
  char          *PY         ;
  unsigned char *PY_mb      ;
} t9PY_index                ;


unsigned char T9PY_Get_Match_PY_MB(char *p_PadInput,t9PY_index ** List_match_PY_Mb);
unsigned char Input_CN(char first_key,char* buffer)                           ;
#endif
