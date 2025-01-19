#ifndef GAME_H
#define GAME_H

// Game Initialization and Management
void initializeGame(const uint8_t difficulty);
void initializeFirstMove(Board *board);
void updateTimer(Board *board);

// Cell Display
void printCell(Cell cell, bool cheatMode);

#endif
