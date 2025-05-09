// #include <msp430.h>
// #include "switch.h"

// /* Switch on P2 (S2) */
// #define SW1 BIT3  // Paddle 1 Up
// #define SW2 BIT2  // Paddle 1 Down
// #define SW3 BIT1  // Paddle 2 Up
// #define SW4 BIT0  // Paddle 2 Down

// #define SWITCHES (SW1 | SW2 | SW3 | SW4)

// volatile char switch1_state_down = 0;
// volatile char switch2_state_down = 0;
// volatile char switch3_state_down = 0;
// volatile char switch4_state_down = 0;

// static char switch_update_interrupt_sense() {
//     char p2val = P2IN;
//     /* update switch interrupt to detect changes from current buttons */
//     P2IES |= (p2val & SWITCHES);    /* if switch up, sense down */
//     P2IES &= (p2val | ~SWITCHES);   /* if switch down, sense up */
//     return p2val;
// }

// void switch_init() /* setup switch */
// {
//     P2REN |= SWITCHES;      /* enables resistors for switches */
//     P2IE |= SWITCHES;       /* enable interrupts from switches */
//     P2OUT |= SWITCHES;      /* pull-ups for switches */
//     P2DIR &= ~SWITCHES;     /* set switches' bits for input */
//     switch_update_interrupt_sense();
// }

// void switch_interrupt_handler() {
//     static unsigned int last_interrupt_time = 0;
//     unsigned int current_time = TA0R; // Assuming Timer A0 is running

//     if (current_time - last_interrupt_time < 100) { // Debounce delay (adjust as needed)
//         P2IFG &= ~SWITCHES; // Clear interrupt flag
//         return; // Ignore this interrupt
//     }

//     last_interrupt_time = current_time;
//     char p2val = switch_update_interrupt_sense();

//     switch1_state_down = (~p2val & SW1) != 0;
//     switch2_state_down = (~p2val & SW2) != 0;
//     switch3_state_down = (~p2val & SW3) != 0;
//     switch4_state_down = (~p2val & SW4) != 0;
// }

// // Switch interrupt service routine
// #pragma vector=PORT2_VECTOR
// __interrupt void port2_isr() {
//   switch_interrupt_handler();     /* handle switch */
//   P2IFG &= ~SWITCHES;		/* clear P2 interrupt flags */
// }