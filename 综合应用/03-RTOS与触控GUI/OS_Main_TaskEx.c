/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : Main_TaskEx.c
Purpose : Sample program for embOS using OC_CREATETASK_EX
--------- END-OF-HEADER --------------------------------------------*/

#include "RTOS.h"

OS_STACKPTR int StackHP[128], StackLP[128];          /* Task stacks */
OS_TASK TCBHP, TCBLP;                        /* Task-control-blocks */

/********************************************************************/

static void TaskEx(void* pData) {
  while (1) {
    OS_Delay ((OS_TIME) pData);
  }
}

/*********************************************************************
*
*       main
*
*********************************************************************/

int main(void) {
  OS_IncDI();                      /* Initially disable interrupts  */
  OS_InitKern();                   /* initialize OS                 */
  OS_InitHW();                     /* initialize Hardware for OS    */
  /* You need to create at least one task before calling OS_Start() */
  OS_CREATETASK_EX(&TCBHP, "HP Task", TaskEx, 100, StackHP, (void*) 50);
  OS_CREATETASK_EX(&TCBLP, "LP Task", TaskEx,  50, StackLP, (void*) 200);
  OS_Start();                      /* Start multitasking            */
  return 0;
}

