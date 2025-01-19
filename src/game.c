#include <board.h>
#include <game.h>
#include <menu.h>
#include <program.h>
#include <stdio.h>
#include <conio.h>

/*
 * TODO: Update the board display every second so the timer updates even without player moves.
 * This may require replacing getch() with another input method.
 */

/**
 * Initializes and runs the game with the specified difficulty level.
 */
void initializeGame(const uint8_t difficulty)
{
    // Declare the game board and initialize board properties
    Board board;
    initializeDifficulty(difficulty, &board);
    const bool cheatMode = true;
    board.cell = allocateBoardCells(&board);
    if (board.cell == NULL)
    {
        fprintf(stderr, "Error: Memory allocation for board cells failed.\n");
        exit(EXIT_FAILURE);
    }
    board.cursor.row = 0;
    board.cursor.col = 0;
    board.flagCounter = board.bombCount;
    board.firstMoveFlag = 1;
    board.timer = 0;

    // Print the initial state of the board
    printBoard(&board, cheatMode);

    // Main game loop
    while (board.isGameRunning || board.firstMoveFlag)
    {
        updateTimer(&board);
        manageInputGame(&board);
        printBoard(&board, cheatMode);
        checkWin(&board);
    }
    if (board.winFlag)
        printf(BLUE"\nYou won!\n"RESET);
    else
        printf(BLUE"\nYou lose!\n"RESET);


    // Free allocated memory for the board after the game ends
    freeBoard(&board);

    // Allow the player to view the board before returning to the main menu
    printf("\nPress any key to return to main menu . . .");
    getch();
    clearConsole();
}

/**
 * Function to handle the first move in the game
 */
void initializeFirstMove(Board *board)
{
    placeBombs(board, board->bombCount);
    calculateNumbers(board);
    board->startTime = (int) time(NULL);
    board->isGameRunning = 1;
}

/**
 * Function to update the game timer
 */
void updateTimer(Board *board)
{
    if (board->isGameRunning && board->timer < 999)
        board->timer = time(NULL) - board->startTime;
    if (board->timer > 999) board->timer = 999;
}

/**
 * This function takes a Cell object, which holds both its state (e.g., Flagged, Closed, Opened)
 * and its type (e.g., Bomb, number of surrounding bombs), and prints a corresponding character
 * or symbol to represent the cell on the console.
 *
 * When cheatMode is enabled, the player can see the location of bombs and identify flagged cells
 * that do not actually contain a bomb.
 *
 * If the cell is not opened, it directly prints a character reflecting its state:
 *   - 'F' for flagged cells,
 *   - '~' for closed cells,
 *   - 'F' with a red background for wrongly flagged bombs,
 *   - 'X' with a red background if a bomb was hit.
 *
 * If the cell is opened, it checks its type and prints:
 *   - '0' for empty cells,
 *   - '1' through '8' with different colors for cells with corresponding bomb counts,
 *   - 'X' for actual bombs.
 *
 * Each printed character have its corresponding style:
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
 * Border   -   #808080
 */
void printCell(const Cell cell, const bool cheatMode)
{
    // Handle cheatMode behavior
    // If cheatMode is active and a cell is flagged incorrectly (not a bomb), display a flagged cell in red
    if (cheatMode && cell.state == Flagged && cell.type != Bomb)
    {
        printf("%sF%s ",RED,RESET);
        return;
    }

    // If cheatMode is active and a hidden bomb is present, show it with "X"
    if (cheatMode && cell.type == Bomb && cell.state != Opened && cell.state != BombHit && cell.state != Flagged)
    {
        printf("X%s ",RESET);
        return;
    }

    // Handle special states where the cell is not opened or is incorrectly flagged
    switch (cell.state)
    {
        case Flagged:
            printf("F%s ",RESET);
            return;
        case WrongBomb:
            printf("%sF%s ", BG_RED,RESET);
            return;
        case BombHit:
            printf("%sX%s ", BG_RED,RESET);
            return;
        case Closed:
            printf("~%s ",RESET);
            return;
        case Opened:
            // If the cell is opened, determine its appearance by its type.
            break;
    }

    // If the cell is opened, print a character or number based on its type.
    switch (cell.type)
    {
        case Empty:
            printf("0");
            break;
        case One:
            printf("%s1", BLUE);
            break;
        case Two:
            printf("%s2", GREEN);
            break;
        case Three:
            printf("%s3", RED);
            break;
        case Four:
            printf("%s4", DBLUE);
            break;
        case Five:
            printf("%s5", BROWN);
            break;
        case Six:
            printf("%s6", TURQUOISE);
            break;
        case Seven:
            printf("%s7", BLACK);
            break;
        case Eight:
            printf("%s8", GREY);
            break;
        case Bomb:
            printf("X");
            break;
        default:
            // Handle unknown cell type
            printf("?");
            break;
    }

    // Reset color at the end
    printf("%s ",RESET);
}
