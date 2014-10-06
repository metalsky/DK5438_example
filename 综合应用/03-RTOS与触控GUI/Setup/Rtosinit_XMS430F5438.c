/*********************************************************************
*  
*   IAR PowerPac
*
*   (c) Copyright IAR Systems 2009.  All rights reserved.
*
**********************************************************************
----------------------------------------------------------------------
File    : RTOSInit_XMS430F5438.c for Texas Instruments TMS430x5
          with IAR compiler
Purpose : Initializes and handles the hardware as far as required by
          the OS.
          Feel free to modify this file if required for your target
--------  END-OF-HEADER  ---------------------------------------------
*/

#include "RTOS.H"
#include <msp430.h>
#include <msp430x54x.h>

/*********************************************************************
*
*       Configuration
*
**********************************************************************

  Define clock frequency for CPU

  Select UART for embOSView, set baudrate,
  If you do not want (or can not due to hardware limitations) to dedicate
  a UART to embOSView, please define OS_UART to -1

*/

#ifndef   OS_FSYS
  #define OS_FSYS 1048576L   /* Assuming, PLL not used, XT=32768 Hz */
#endif

#ifndef   OS_PCLK_TIMER
  #define OS_PCLK_TIMER OS_FSYS*20
#endif

#ifndef   OS_UART
  #define OS_UART 0
#endif

#ifndef   OS_BAUDSRC_SMCLK
  #define OS_BAUDSRC_SMCLK 0
#endif

#ifndef   OS_BAUDRATE
  #define OS_BAUDRATE 9600L
#endif

//#define EMBOS_TIMER_VECTOR TIMER0_A0_VECTOR  // modified by ZDCHUN, 20101023
#define EMBOS_TIMER_VECTOR TIMER1_A0_VECTOR

/****** End of configuration settings *******************************/

#define OS_UART_USED (OS_UART == 1)   // Only USART 1 supported

/*********************************************************************
*
*       OS_InitHW()
*
*       Initialize the hardware (timer) required for embOS to run.
*       May be modified, if an other timer should be used
*/
void OS_InitHW(void) {
  OS_IncDI();                            /*  Ensure, interrupts are disabled */
  
  TA1CTL   = 0                           /*  Reset Timer_A5, division 1      */
           | (1 << 2)                    /*  Clear timer                     */
           | (2 << 8)                    /*  Use SMCLK as timer clock        */
//           |TASSEL_1                    // Use ACLK as timer clock
           ;
  TA1CCR0  = (OS_PCLK_TIMER / 1000) - 1; /*  Set to 1ms                      */
  TA1CCTL0 = 0                           /*  Initilize capture control       */
           | (1 << 4);                   /*  Enable compare interrupt        */
  TA1CTL  |= (1 << 4);                   /*  Start timer in UP-Mode          */
  OS_POWER_UsageInc(OS_POWER_USE_SMCLK); /*  Mark SMCLK as used              */
  OS_COM_Init();                         /*  Initialize UART                 */
  OS_DecRI();                            /*  Restore interrupt context       */
}

/*********************************************************************
*
*       Idle loop  (OS_Idle)
*
*       Please note:
*       This is basically the "core" of the idle loop.
*       This core loop can be changed, but:
*       The idle loop does not have a stack of its own, therefore no
*       functionality should be implemented that relies on the stack
*       to be preserved. However, a simple program loop can be programmed
*       (like toggeling an output or incrementing a counter)
*
*       We just enter low power 0 mode here.
*/
void OS_Idle(void) { /* Idle loop: No task is ready to exec         */
  OS_UINT PowerMask;
  OS_U16  ClkControl;

  OS_DI();
  //
  // Examine which peripherals may be switched off */
  //
//  PowerMask  = OS_POWER_GetMask(); // test
  //
  //  Switch off peripherals which are not needed
  //
//  ClkControl  = UCSCTL8 & ~OS_POWER_USE_ALL;// test
//  PowerMask  &= OS_POWER_USE_ALL;// test
//  UCSCTL8     = ClkControl | PowerMask;// test
  //
  // Nothing to do ... enter low power mode, with interrupts enabled
  //
  _BIS_SR(GIE+CPUOFF+OSCOFF+SCG1+SCG0);
  for (;;);          /* Alternative endless loop, required  */
                     /* when simulator is used !            */
}

/*********************************************************************
*
*       Get time [cycles]
*
*       This routine is required for task-info via embOSView or high
*       resolution time maesurement functions.
*       It returns the system time in timer clock cycles.
*/
OS_U32 OS_GetTime_Cycles(void) {
  unsigned int Tcnt;
  OS_U32       Time;

  Tcnt = TA0R;
  Time = OS_Time;
  if (TA0CCTL0 & (1 << 0)) {   /* If timer interrupt pending, */
    Tcnt = TA0R;             /* correct result              */
    Time++;
  }
  return (OS_PCLK_TIMER / 1000) * Time + Tcnt;
}

/*********************************************************************
*
*       OS_ConvertCycles2us
*
*       Convert Cycles into micro seconds.
*
*       If your clock frequency is not a multiple of 1 MHz,
*       you may have to modify this routine in order to get proper
*       diagonstics.
*
*       This routine is required for profiling or high resolution time
*       measurement only. It does not affect operation of the OS.
*/
OS_U32 OS_ConvertCycles2us(OS_U32 Cycles) {
  #if (OS_PCLK_TIMER >= 1000000)
    return Cycles / (OS_PCLK_TIMER / 1000000);
  #else
    return Cycles * (1000000 / OS_PCLK_TIMER);
  #endif
}

/*********************************************************************
*
*       OS_Tick interrupt Handler
*/
#pragma vector=EMBOS_TIMER_VECTOR
static __interrupt void OS_ISR_Tick(void) {
  OS_EnterNestableInterrupt();
  OS_HandleTick();
  OS_LeaveNestableInterrupt();
}

#if OS_UART_USED
/*********************************************************************
*
*       Communication for embOSView
*
**********************************************************************
*/
#if OS_BAUDSRC_SMCLK
  #define BAUDDIVIDE ((OS_FSYS+(OS_BAUDRATE/2))/OS_BAUDRATE)
  #define UxBR0  (BAUDDIVIDE & 0xFF)
  #define UxBR1  ((BAUDDIVIDE >> 8) & 0xFF)
  #define UxMCTL 0x00
  #define UxTCTL 0x20            /* Use SMCLK for baudrate generator */
#else
  #define UxTCTL 0x10            /* Use ACLK for baudrate generator */
  #if   (OS_BAUDRATE == 9600)
    #define UxBR0  0x03
    #define UxBR1  0x00
    #define UxMCTL 0x4A
  #elif (OS_BAUDRATE == 4800)
    #define UxBR0  0x06
    #define UxBR1  0x00
    #define UxMCTL 0x6F
  #else
    #error Setting of OS_BAUDRATE not supported when using ACLK for baudrate generation
  #endif
#endif

#if (OS_UART == 1)
#define EMBOS_UARTRX_VECTOR (USART1RX_VECTOR)
#define EMBOS_UARTTX_VECTOR (USART1TX_VECTOR)

/*********************************************************************
*
*       OS_COM_Init()
*       Initialize UART for embOSView
*/
void OS_COM_Init(void) {
  U1CTL  = UCSWRST;            /* Reset UART, No parity, 1 stop bit */
  P4SEL |= (BIT0 | BIT1);      /* Set Port function to UART mode    */


  U1CTL |= UC7BIT;             /* Set 8 bit data length             */
  U1TCTL = UxTCTL;             /* Set tx mode                       */
  U1RCTL = 0;                  /* Reset Rx Control, no wake int.    */
  U1BR0  = UxBR0;              /* Setup baudrate generator          */
  U1BR1  = UxBR1;
  U1MCTL = UxMCTL;             /* Initialize Modulation control     */
  ME2   |= (URXE1 | UTXE1);    /* Enable Rx Tx                      */
  U1CTL &= ~UCSWRST;           /* Unlock UART                       */
  IE2   |= (URXIE1 | UTXIE1);  /* Enable Rx Tx interrupts           */
}

/*********************************************************************
*
*       OS_COM_Send1()
*       Never call this function directly from your application
*/
void OS_COM_Send1(OS_U8 c) {
  U1TXBUF = c;
}

/*********************************************************************
*
*       OS_ISR_rx()
*       embOS UART rx interrupt handler
*/
#pragma vector=EMBOS_UARTRX_VECTOR
static __interrupt void OS_ISR_rx(void) {
  IE2   &= ~URXIE1;            /* Avoid nesting itself              */
  OS_EnterNestableInterrupt(); /* We will enable interrupts         */
  OS_OnRx(U1RXBUF);            /* Process data                      */
  OS_DI();
  IE2   |= URXIE1;             /* Re-Enable Rx interrupts           */
  OS_LeaveNestableInterrupt();
}

/*********************************************************************
*
*       OS_ISR_tx()
*       embOS UART tx interrupt handler
*/
#pragma vector=EMBOS_UARTTX_VECTOR
static __interrupt void OS_ISR_tx(void) {
  OS_EnterInterrupt();
  OS_OnTx();
  OS_LeaveInterrupt();
}
#endif // (OS_UART == 1)

#else  /* No communication routines         */
  void OS_COM_Init(void) {}
  void OS_COM_Send1(OS_U8 c) {
    OS_USEPARA(c);
    OS_COM_ClearTxActive();    /* let OS know that tx is not busy   */
  }
#endif /* (OS_UART_USED) */

/*********************************************************************
*
*       __low_level_init()
*/
int __low_level_init(void);
int __low_level_init(void) {
  WDTCTL   = WDTPW + WDTHOLD;            //  Stop watchdog timer
  return 1;
}

/*****  EOF  ********************************************************/



