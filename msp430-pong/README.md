# MSP430 Pong Game

This project implements a simple Pong game for the MSP430 microcontroller. The game features a ball that bounces between two paddles controlled by the user. The objective is to keep the ball in play and score points by preventing it from passing your paddle.

## Project Structure

```
msp430-pong
├── lib
│   ├── lcdutils.c       # Implementation of LCD utility functions
│   ├── lcdutils.h       # Header file for LCD utility functions
│   ├── lcddraw.c        # Implementation of drawing functions for the LCD
│   └── lcddraw.h        # Header file for drawing functions
├── src
│   ├── pong.c           # Main logic for the Pong game
│   ├── pong.h           # Header file for Pong game logic
│   ├── main.c           # Entry point of the application
│   └── switch.c         # Implementation of switch handling functions
├── Makefile              # Build instructions for the project
└── README.md             # Documentation for the project
```

## Building the Project

To build the project, navigate to the project directory and run the following command:

```
make
```

This will compile the source files and create an executable for the MSP430.

## Running the Game

After building the project, upload the executable to your MSP430 microcontroller using your preferred method (e.g., using a programmer or development board). Once uploaded, reset the microcontroller to start the game.

## Controls

- Use the switches connected to the MSP430 to control the paddles.
- The left paddle is controlled by one switch, and the right paddle is controlled by another.

## License

This project is open-source and available for modification and distribution under the MIT License.