/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : Main_LED.c
Purpose : Sample program for OS running on EVAL-boards with LEDs
--------- END-OF-HEADER --------------------------------------------*/


#include "RTOS.h"
#include "BSP.h"

OS_STACKPTR int StackHP[128], StackLP[128];          /* Task stacks */
OS_TASK TCBHP, TCBLP;                        /* Task-control-blocks */


static void HPTask(void) {
  while (1) {
    BSP_ToggleLED(0);
    OS_Delay (50);
  }
}

static void LPTask(void) {
  while (1) {
    BSP_ToggleLED(1);
    OS_Delay (200);
  }
}

/*********************************************************************
*
*       main
*
*********************************************************************/

int main(void) {
  OS_IncDI();                    // Initially disable interrupts
  OS_InitKern();                 // initialize OS
  OS_InitHW();                   // initialize Hardware for OS
  BSP_Init();                    // initialize LED ports
  // You need to create at least one task before calling OS_Start()
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 100, StackHP);
  OS_CREATETASK(&TCBLP, "LP Task", LPTask,  50, StackLP);
  OS_DecRI();                    // Enable interupts
  OS_Start();                    // Start multitasking
  return 0;
}

