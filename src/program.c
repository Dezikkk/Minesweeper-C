#include <board.h>
#include <game.h>
#include <program.h>
#include <stdio.h>
#include <conio.h>

/**
 * This function clears console depending on OS
 * WIN32 systems uses console command "cls"
 * UNIX type systems uses "clear" command
 */
void clearConsole()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
     system("clear");  // Linux/macOS
#endif
}

/**
 * This function enables color handling in console on Windows type systems
 */
void enable_ansi_colors()
{
#ifdef _WIN32
    const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
#else
    // On Unix-type systems ANSI colours are usually supported by default.
#endif
}

/**
 * Controls
 * WSAD - up/down/left/right
 * F - flag
 * Space - reveal
 * Q - quit
 */
void manageInputGame(Board *board)
{
    const int input = getch();
    clearConsole();

    switch (input)
    {
        case 'w':
        case 'W':
            moveCursor(&board->cursor, -1, 0);
            break;
        case 's':
        case 'S':
            moveCursor(&board->cursor, 1, 0);
            break;
        case 'a':
        case 'A':
            moveCursor(&board->cursor, 0, -1);
            break;
        case 'd':
        case 'D':
            moveCursor(&board->cursor, 0, 1);
            break;
        case 'f':
        case 'F':
            toggleFlag(board);
            break;
        case ' ':
            if (board->firstMoveFlag)
            {
                initializeFirstMove(board);
                board->firstMoveFlag = 0;
            }
            revealCell(board, board->cursor.row, board->cursor.col);
            break;
        case 'q':
        case 'Q':
            freeBoard(board);
            exit(0);
        default:
            printf("Wrong key! Use W/A/S/D/Q/F/SPACE.\n\n");
    }
}

/**
 * Function handles moving cursor over UI
 */
void moveCursor(Cursor *cursor, const uint8_t dx, const uint8_t dy)
{
    // new position variable
    const uint8_t nx = cursor->row + dx;
    const uint8_t ny = cursor->col + dy;

    // Ensure new position is within bounds
    if (nx < cursor->maxRows)
    {
        cursor->row = nx;
    }

    if (ny < cursor->maxCols)
    {
        cursor->col = ny;
    }
}
