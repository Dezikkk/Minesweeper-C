#ifndef PROGRAM_H
#define PROGRAM_H

#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <board.h>

// Macro defining the seed value for random number generation.
#define SEED time(NULL)

/*
 * Empty    -   c0c0c0
 * One  -   #0000ff
 * Two  -   #008000
 * Three    -   #ff0000
 * Four    -   #000080
 * Five    -   #800000
 * Six    -   #008080
 * Seven    -   #000000
 * Eight    -   #808080
 * Bomb    -   #    X
 * WrongBomb    -   red X
 * BombHit    -   X with red background
 * border   -   #808080
 */
#define RESET           "\x1b[0m"                   // Reset style
#define BLUE            "\x1b[38;2;0;0;255m"        // 1 - #0000ff
#define GREEN           "\x1b[38;2;0;128;0m"        // 2 - #008000
#define RED             "\x1b[38;2;255;0;0m"        // 3 - #ff0000
#define DBLUE           "\x1b[38;2;0;0;128m"        // 4 - #000080
#define BROWN           "\x1b[38;2;128;0;0m"        // 5 - #800000
#define TURQUOISE       "\x1b[38;2;0;128;128m"      // 6 - #008080
#define BLACK           "\x1b[38;2;0;0;0m"          // 7 - #000000
#define GREY            "\x1b[38;2;128;128;128m"    // 8 - #808080

// Kolory tła
#define BG_BLUE         "\x1b[48;2;0;0;255m"        // #0000ff
#define BG_GREEN        "\x1b[48;2;0;128;0m"        // #008000
#define BG_RED          "\x1b[48;2;255;0;0m"        // #ff0000
#define BG_DBLUE        "\x1b[48;2;0;0;128m"        // #000080
#define BG_BROWN        "\x1b[48;2;128;0;0m"        // #800000
#define BG_TURQUOISE    "\x1b[48;2;0;128;128m"      // #008080
#define BG_BLACK        "\x1b[48;2;0;0;0m"          // #000000
#define BG_GREY         "\x1b[48;2;128;128;128m"    // #808080

// Console Management
void clearConsole();
void enable_ansi_colors();

// Game Input Management
void manageInputGame(Board *board);
void moveCursor(Cursor *cursor, const uint8_t dx, const uint8_t dy);

#endif
