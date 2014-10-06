/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : PerformanceTest.c
Purpose : Performance test program for OS
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.H"
#include <stdio.h>
#include <stdlib.h>

OS_STACKPTR int StackHP[128];         // Task stacks
OS_TASK TCBHP;                        // Task-control-blocks

char aIsPrime[1000];
int NumPrimes;

/*********************************************************************
*
*       _CalcPrimes
*/
static void _CalcPrimes(int NumItems) {
  int i;
  //
  // Mark all as potential prime numbers
  //
  memset(aIsPrime, 1, NumItems);
  //
  // Cross out multiples of every prime
  //
  for (i = 2; i < NumItems; i++) {
    if (aIsPrime[i]) {
      int j;
      for (j = 2 * i; j < NumItems; j += i) {
        aIsPrime[j] = 0;     // Cross it out: not a prime
      }
    }
  }
  //
  // Count remaining prime numbers
  //
  NumPrimes = 0;
  for (i = 2; i < NumItems; i++) {
    if (aIsPrime[i]) {
      NumPrimes++;
    }
  }
}

/*********************************************************************
*
*       _PrintCnt
*/
static void _PrintCnt(unsigned Cnt) {
  int Digit;
  int r;

  Digit = 10;
  puts("Loops:");
  while (Digit < Cnt) {
    Digit *= 10;
  }
  while (Cnt) {
    Digit /= 10;
    r = Cnt / Digit;
    Cnt -= r * Digit;
    putchar(r + '0');
  }
  puts("\n");
}


/*********************************************************************
*
*       HPTask
*/
static void HPTask(void) {
  int Cnt;
  int TestTime;

  while(1) {
    Cnt = 0;
    TestTime = OS_GetTime32() + 1000;
    while ((TestTime - OS_GetTime()) >= 0) {
      _CalcPrimes(sizeof(aIsPrime));
      Cnt++;
    }
    _PrintCnt(Cnt);
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
  OS_CREATETASK(&TCBHP, "HP Task", HPTask, 100, StackHP);
  OS_Start();                      /* Start multitasking            */
  return 0;
}

