#ifndef PONG_H
#define PONG_H

#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define PADDLE_WIDTH 5
#define PADDLE_HEIGHT 20
#define BALL_SIZE 3

typedef struct {
    short x;
    short y;
    short velocityX;
    short velocityY;
} Ball;

typedef struct {
    short x;
    short y;
} Paddle;

void initGame();
void updateGame();
void drawGame();
void moveBall();
void checkCollision();
void resetBall();

#endif // PONG_H