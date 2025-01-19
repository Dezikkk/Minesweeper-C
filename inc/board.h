#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Enumeration for the state of a cell on the game board.
 */
typedef enum CellState {
    Closed, Opened, Flagged, WrongBomb, BombHit
} CellState;

/**
 * Enumeration for the type of cell on the game board.
 */
typedef enum CellType {
    Empty, One, Two, Three, Four, Five, Six, Seven, Eight, Bomb
} CellType;

/**
 * Structure representing an individual cell on the board.
 */
typedef struct Cell {
    CellType type;
    CellState state;
} Cell;

/**
 * Structure representing the cursor's position and boundaries.
 */
typedef struct Cursor {
    uint8_t row;
    uint8_t col;
    uint8_t maxRows;
    uint8_t maxCols;
} Cursor;

/**
 * Structure representing the Minesweeper game board.
 */
typedef struct Board {
    Cell **cell;
    Cursor cursor;
    unsigned int bombCount;
    unsigned int rows;
    unsigned int cols;
    unsigned int timer; // 3 pos max 999
    unsigned int startTime;
    unsigned int flagCounter;
    bool isGameRunning;
    bool firstMoveFlag;
    bool winFlag;
} Board;

// Memory Management
Cell **allocateBoardCells(const Board *board);
void freeBoard(Board *board);

// Board Initialization
void placeBombs(const Board *board, uint16_t bombCount);
void calculateNumbers(const Board *board);
uint8_t countBombsAround(const Board *board, uint8_t row, uint8_t col);
bool isValid(uint8_t row, uint8_t col, uint8_t maxRows, uint8_t maxCols);

// Game State Management
bool isBombAt(const Board *board, uint8_t row, uint8_t col);
void changeCellState(const Board *board, uint8_t row, uint8_t col, uint8_t newState);
void revealCell(Board *board, uint8_t row, uint8_t col);
void revealEmptyNeighbors(Board *board, uint8_t row, uint8_t col);
void toggleFlag(Board *board);

// Board Display
void printBoardPreview(const Board *board);
void printBoard(const Board *board, bool cheatMode);
void printFlagsAndTimer(const Board *board);

// Game Over Handling
void handleGameOver(Board *board);
void revealBombs(const Board *board);
void checkWin(Board *board);

#endif
