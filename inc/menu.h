#ifndef MENU_H
#define MENU_H

/**
 * Enumeration for the game's difficulty levels.
 */
typedef enum Difficulty {
    Easy, Normal, Hard, Custom
} Difficulty;

/**
 * Structure representing the state of the menu.
 */
typedef struct Menu {
    Cursor cursor;
    Difficulty difficulty;
} Menu;

// Menu Initialization and Management
void initializeMenu();
void manageInputMenu(Menu *menu);

// Difficulty Initialization
void initializeDifficulty(const Difficulty difficulty, Board *board);
void getCustomDifficulty(int *rows, int *cols, int *mines);

// Display and Rendering
void printMainMenu(const Menu *menu);
void printLogo();
void printHelp();

#endif
