#include <board.h>
#include <game.h>
#include <menu.h>
#include <program.h>
#include <conio.h>
#include <stdio.h>

/**
 *  Initializes the main menu and manages user input for menu navigation.
 */
void initializeMenu()
{
    Menu menu;

    // Set default menu settings
    menu.difficulty = Normal;
    menu.cursor.row = 0;
    menu.cursor.col = 0;
    menu.cursor.maxRows = 4;
    menu.cursor.maxCols = 0;
    printMainMenu(&menu);

    // Main menu loop
    while (1)
    {
        manageInputMenu(&menu);
        printMainMenu(&menu);
    }
}

/**
 * Handles user input and updates the menu state accordingly.
 */
void manageInputMenu(Menu *menu)
{
    const int input = getch();
    clearConsole();

    switch (input)
    {
        // Move cursor up
        case 'w':
        case 'W':
            moveCursor(&menu->cursor, -1, 0);
            break;
        // Move cursor down
        case 's':
        case 'S':
            moveCursor(&menu->cursor, 1, 0);
            break;
        // Decrease difficulty
        case 'a':
        case 'A':
            if (menu->cursor.row == 1 && menu->difficulty > 0)
                menu->difficulty--;
            break;
        // Increase difficulty
        case 'd':
        case 'D':
            if (menu->cursor.row == 1 && menu->difficulty < 3)
                menu->difficulty++;
            break;
        // Confirm selection
        case ' ':
            switch (menu->cursor.row)
            {
                case 0:
                    initializeGame(menu->difficulty);
                    break;
                case 1:
                    break;
                case 2:
                    printHelp();
                    break;
                case 3:
                    exit(0);
                default:
                    printf("Error: Invalid menu cursor position.");
            }
            break;
        // Quit the program
        case 'q':
        case 'Q':
            exit(0);
        default:
            printf("Wrong key! Use W/A/S/D/SPACE.\n\n");
    }
}

/**
 * Initializes the game board based on the selected difficulty level.
 */
void initializeDifficulty(const Difficulty difficulty, Board *board)
{
    switch (difficulty)
    {
        case 0: // Easy
            board->rows = 9;
            board->cols = 9;
            board->bombCount = 10;
            break;
        case 1: // Normal
            board->rows = 16;
            board->cols = 16;
            board->bombCount = 40;
            break;
        case 2: // Hard
            board->rows = 16;
            board->cols = 30;
            board->bombCount = 99;
            break;
        case 3: // Custom
            int rows, cols, mines;
            getCustomDifficulty(&rows, &cols, &mines);
            board->rows = rows;
            board->cols = cols;
            board->bombCount = mines;
    }
    board->cursor.maxRows = board->rows;
    board->cursor.maxCols = board->cols;
}

/**
 * Gets and validates custom difficulty settings from the user.
 */
void getCustomDifficulty(int *rows, int *cols, int *mines)
{
    char buffer[100];

    // Prompt the user to enter the number of rows (0-100)
    while (1)
    {
        printf("Enter the number of rows (0-100): ");
        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", rows) == 1 && *rows > 0 && *rows <= 100)
        {
            break; // Valid input, exit the loop
        }
        printf("Invalid input. Please enter a number between 1 and 100.\n");
    }

    // Prompt the user to enter the number of columns (0-100)
    while (1)
    {
        printf("Enter the number of columns (0-100): ");
        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", cols) == 1 && *cols > 0 && *cols <= 100)
        {
            break; // Valid input, exit the loop
        }
        printf("Invalid input. Please enter a number between 1 and 100.\n");
    }

    // Calculate the maximum number of mines (total cells - 1)
    const int maxMines = (*rows) * (*cols) - 1;

    // Prompt the user to enter the number of mines (0-maxMines)
    while (1)
    {
        printf("Enter the number of mines (0-%d): ", maxMines);
        if (fgets(buffer, sizeof(buffer), stdin) &&
            sscanf(buffer, "%d", mines) == 1 && *mines >= 0 && *mines <= maxMines)
        {
            break; // Valid input, exit the loop
        }
        printf("Invalid input. Please enter a number between 0 and %d.\n", maxMines);
    }
}

/**
 * Displays the main menu
 */
void printMainMenu(const Menu *menu)
{
    printLogo();
    const char *difficultyOptions[] = {GREEN"Easy", BLUE"Normal", RED"Hard", BROWN"Custom"};

    if (menu->cursor.row == 0)
        printf("%s",BG_DBLUE);
    printf("Start Game%s\n\n",RESET);
    if (menu->cursor.row == 1)
        printf("%s",BG_DBLUE);
    printf("Difficulty: %s%s\n\n", difficultyOptions[menu->difficulty], RESET);
    if (menu->cursor.row == 2)
        printf("%s",BG_DBLUE);
    printf("Help%s\n\n",RESET);
    if (menu->cursor.row == 3)
        printf("%s",BG_DBLUE);
    printf("Quit%s\n",RESET);
}

/**
 * Prints the logo of the game.
 */
void printLogo()
{
    FILE *fr = fopen("../resources/logo.txt", "r");
    if (fr == NULL)
    {
        perror("Error opening logo file");
        exit(EXIT_FAILURE);
    }
    char buff[100];

    // Read and display the logo line by line
    while (fgets(buff, sizeof(buff), fr) != NULL)
    {
        printf(BLUE"%s", buff);
    }
    printf(RESET"\n\n");

    fclose(fr);
}

/**
 * Displays the help information for the game.
 */
void printHelp()
{
    clearConsole();
    FILE *fr = fopen("../resources/help.txt", "r");
    if (fr == NULL)
    {
        perror("Error opening logo file");
        exit(EXIT_FAILURE);
    }
    char buff[100];

    // Read and display the logo line by line
    while (fgets(buff, sizeof(buff), fr) != NULL)
    {
        printf("%s", buff);
    }
    printf("\n\n");

    fclose(fr);
    getch();
    clearConsole();
}
