#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "pong.h"


void main()
{
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
  configureClocks();
  lcd_init();
  switch_init(); // Initialize switches
  __enable_interrupt(); // Enable global interrupts

  clearScreen(COLOR_BLACK); // Clear the screen to black
  start_game(); // Start the Pong game

  while (1) {
    // Main game loop
    update_game(); // Update game state
    draw_game();   // Draw game state on the LCD
    __delay_cycles(50000); // Increased delay to slow down the game
  }
}