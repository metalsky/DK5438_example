/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : Main_StartTask.c
Purpose : Skeleton program for OS
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.h"

OS_STACKPTR int StackHP[128];      /* Task stacks */
OS_TASK TCBHP;                     /* Task-control-blocks */

static void HPTask(void) {
  while (1) {
    OS_Delay (10);
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
  /* You need to create at least one task here !                    */
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 100, StackHP);
  OS_Start();                      /* Start multitasking            */
  return 0;
}

/****** End of file *************************************************/
