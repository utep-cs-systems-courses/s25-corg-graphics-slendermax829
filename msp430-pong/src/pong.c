#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "pong.h"
#include <stdio.h>


#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15
#define DEBOUNCE_DELAY 10  

#define BALL_SIZE 3
#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT 20
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#define PADDLE_SPEED 4
#define BALL_SPEED 1

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}


short ballPos[2] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
short ballVelocity[2] = {BALL_SPEED, BALL_SPEED};
short paddlePos[2] = {10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2}; //P1
short paddlePos_2[2] = {SCREEN_WIDTH - 10 - PADDLE_WIDTH, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2}; //P2

void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
}

void buzzer_set_period(short cycles) /* buzzer clock = 2MHz.  (period of 1k results in 2kHz tone) */
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}




void draw_ball() {
    fillRectangle(ballPos[0] - BALL_SIZE, ballPos[1] - BALL_SIZE, BALL_SIZE * 2, BALL_SIZE * 2, COLOR_WHITE);
}
void erase_ball() {
    fillRectangle(ballPos[0] - BALL_SIZE, ballPos[1] - BALL_SIZE, BALL_SIZE * 2, BALL_SIZE * 2, COLOR_BLACK);
}

void draw_paddle() {
    fillRectangle(paddlePos[0], paddlePos[1], PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);

    fillRectangle(paddlePos_2[0], paddlePos_2[1], PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_WHITE);
}
void erase_paddle() {
    fillRectangle(paddlePos[0], paddlePos[1], PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_BLACK);

    fillRectangle(paddlePos_2[0], paddlePos_2[1], PADDLE_WIDTH, PADDLE_HEIGHT, COLOR_BLACK);
}

short P1_score = 0;
short P2_score = 0;

void draw_P1_score(){
    char scoreStr[3];
    sprintf(scoreStr, "%d", P1_score);
    drawString5x7((SCREEN_WIDTH/2)-15, 10, scoreStr, COLOR_WHITE, COLOR_BLACK);
    //drawString8x12((SCREEN_WIDTH/2)-15, SCREEN_HEIGHT - 10, scoreStr, COLOR_WHITE, COLOR_BLACK);
    

}

void draw_P2_score(){
    char scoreStr[3];
    sprintf(scoreStr, "%d", P2_score);
    drawString5x7((SCREEN_WIDTH/2)+15, 10, scoreStr, COLOR_WHITE, COLOR_BLACK);
    //drawString8x12((SCREEN_WIDTH/2)+15, SCREEN_HEIGHT - 10, scoreStr, COLOR_WHITE, COLOR_BLACK);
    
}


void update_ball() {
    erase_ball(); // Erase the ball
    ballPos[0] += ballVelocity[0];
    ballPos[1] += ballVelocity[1];

    // Ball collision with top and bottom walls
    if (ballPos[1] <= BALL_SIZE || ballPos[1] >= SCREEN_HEIGHT - BALL_SIZE) {
        ballVelocity[1] = -ballVelocity[1];
    }

    // Paddle 1 collision detection
    if (ballPos[0] - BALL_SIZE <= paddlePos[0] + PADDLE_WIDTH &&
        ballPos[0] + BALL_SIZE >= paddlePos[0] &&
        ballPos[1] - BALL_SIZE <= paddlePos[1] + PADDLE_HEIGHT &&
        ballPos[1] + BALL_SIZE >= paddlePos[1]) {
        ballVelocity[0] = -ballVelocity[0];
        //generate_sound(); // Play sound on collision
        buzzer_set_period(1000); // Set buzzer frequency
    }

    // Paddle 2 collision detection
    if (ballPos[0] + BALL_SIZE >= paddlePos_2[0] &&
        ballPos[0] - BALL_SIZE <= paddlePos_2[0] + PADDLE_WIDTH &&
        ballPos[1] - BALL_SIZE <= paddlePos_2[1] + PADDLE_HEIGHT &&
        ballPos[1] + BALL_SIZE >= paddlePos_2[1]) {
        ballVelocity[0] = -ballVelocity[0];
        //generate_sound(); // Play sound on collision
        buzzer_set_period(1000); // Set buzzer frequency
    }

    // Reset ball if it goes off screen
    if (ballPos[0] <= 0) {
        // Player 2 scores
        P2_score++;
        ballPos[0] = SCREEN_WIDTH / 2;
        ballPos[1] = SCREEN_HEIGHT / 2;
    } else if (ballPos[0] >= SCREEN_WIDTH) {
        // Player 1 scores
        P1_score++;
        ballPos[0] = SCREEN_WIDTH / 2;
        ballPos[1] = SCREEN_HEIGHT / 2;
    }


    draw_ball(); // Draw the ball in the new position
}

void update_paddle() {
    erase_paddle(); // Erase the paddle

    if (switches & SW1) {
        paddlePos[1] -= PADDLE_SPEED;
        if (paddlePos[1] < 0) {
            paddlePos[1] = 0;
        }
    }
    // Move paddle down if SW2 is pressed
    if (switches & SW2) {
        paddlePos[1] += PADDLE_SPEED;
        if (paddlePos[1] > SCREEN_HEIGHT - PADDLE_HEIGHT) {
            paddlePos[1] = SCREEN_HEIGHT - PADDLE_HEIGHT;
        }
    }

    draw_paddle(); // Draw the paddle in the new position
}

void update_paddle_2() {
    erase_paddle(); // Erase the paddle

    if (switches & SW3) {
        paddlePos_2[1] -= PADDLE_SPEED;
        if (paddlePos_2[1] < 0) {
            paddlePos_2[1] = 0;
        }
    }
    // Move paddle down if SW4 is pressed
    if (switches & SW4) {
        paddlePos_2[1] += PADDLE_SPEED;
        if (paddlePos_2[1] > SCREEN_HEIGHT - PADDLE_HEIGHT) {
            paddlePos_2[1] = SCREEN_HEIGHT - PADDLE_HEIGHT;
        }
    }
    
    draw_paddle(); // Draw the paddle in the new position
}

void start_game() {
  // Initialize game state here
  configureClocks();
  lcd_init();
  clearScreen(COLOR_BLACK);
}

void update_game() {
  update_ball();
  update_paddle();
  update_paddle_2();
    draw_P1_score();
    draw_P2_score();
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

void draw_game() {
  
}