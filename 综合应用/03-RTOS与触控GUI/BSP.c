/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : BSP.c
Purpose : LED interface for MSP430 Evalboard
--------  END-OF-HEADER  ---------------------------------------------
*/

#define  BSP_C
#include "BSP.h"
#include <msp430.h>

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

/****** Target specific configuration *******************************/

/****** Assign LEDs to Ports ****************************************/

#define _LED0_BIT      (0)                     // LED1 on EXP430 Eval board
#define _LED1_BIT      (1)                     // LED2 on EXP430 Eval board

#define _LED_PORT_DIR  (PADIR)                 // LEDS are connected to Port A
#define _LED_PORT_OUT  (PAOUT)

#define _LED_MASK_ALL  (_LED_MASK_LED0 | _LED_MASK_LED1)
#define _LED_MASK_LED0 (1 << _LED0_BIT)
#define _LED_MASK_LED1 (1 << _LED1_BIT)

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       BSP_Init()
*/
void BSP_Init(void) {
  _LED_PORT_DIR = _LED_MASK_ALL;               // Switch Port to output
  _LED_PORT_OUT = _LED_MASK_ALL;               // Switch LEDs on initially
}

/*********************************************************************
*
*       LED switching routines
*       LEDs are switched on with low level on port lines
*/
void BSP_SetLED(int Index) {
  if (Index == 0) {
    _LED_PORT_OUT |= _LED_MASK_LED0;           // Switch on LED 0
  }
  if  (Index == 1) {
    _LED_PORT_OUT |= _LED_MASK_LED1;           // Switch on LED 1
  }
}

void BSP_ClrLED(int Index) {
  if (Index == 0) {
    _LED_PORT_OUT &= ~_LED_MASK_LED0;          // Switch off LED 0
  }
  if (Index == 1) {
    _LED_PORT_OUT &= ~_LED_MASK_LED1;          // Switch off LED 0
  }
}

void BSP_ToggleLED(int Index) {
  if (Index == 0) {
    _LED_PORT_OUT ^= _LED_MASK_LED0;
  }
  if (Index == 1) {
    _LED_PORT_OUT ^= _LED_MASK_LED1;
  }
}
/****** EOF *********************************************************/

