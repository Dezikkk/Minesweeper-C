#include <board.h>
#include <game.h>
#include <menu.h>
#include <program.h>
#include <stdio.h>

/**
 * This function initializes a 2D array of `Cell` structures based on the board's
 * dimensions. It allocates memory for each row and column, ensuring proper error
 * handling by freeing already allocated memory if an allocation fails.
 */
Cell **allocateBoardCells(const Board *board)
{
    // memory allocation for the array of row pointers
    Cell **outputBoard = calloc(board->rows, sizeof(Cell *));
    if (outputBoard == NULL)
    {
        perror("Błąd alokacji pamięci dla tablicy wierszy");
        return NULL;
    }

    // memory allocation for each column in each row
    for (unsigned int i = 0; i < board->rows; i++)
    {
        outputBoard[i] = (Cell *) calloc(board->cols, sizeof(Cell));
        if (outputBoard[i] == NULL)
        {
            perror("Błąd alokacji pamięci dla tablicy kolumn");

            // free previously allocated memory in case of an error
            for (unsigned int j = 0; j < i; j++)
            {
                free(outputBoard[j]);
            }
            free(outputBoard);
            return NULL;
        }
    }

    return outputBoard;
}

/**
 * This function is responsible for cleaning up the memory that was previously
 * allocated for the cells within a Board structure.
 */
void freeBoard(Board *board)
{
    if (board == NULL)
    {
        return; // Nothing to free
    }

    // Free memory for each row of the cell array
    for (unsigned int i = 0; i < board->rows; i++)
    {
        free(board->cell[i]);
    }

    // Free memory for the main array of pointers
    free(board->cell);

    // Optionally set pointers to NULL for safety
    board->cell = NULL;
}

/**
 * This function randomly assigns bombs to cells on the board.
 * It ensures that each bomb is placed in a unique cell. If a randomly selected
 * cell already contains a bomb, the function retries until all bombs are placed
 */
void placeBombs(const Board *board, uint16_t bombCount)
{
    if (bombCount > (board->rows * board->cols))
    {
        printf("Error: maximum value of bombCount exceeded");
        return;
    }

    if (!bombCount) return;
    while (bombCount > 0)
    {
        // Generate random coordinates
        const uint8_t randX = rand() % board->rows;
        const uint8_t randY = rand() % board->cols;

        // Check if the cell already has a bomb or is the cursor's position
        if (isBombAt(board, randX, randY) == true || board->cursor.row == randX && board->cursor.col == randY)
            continue; // Retry with different coordinates

        // place bomb in selected cell
        board->cell[randX][randY].type = Bomb;

        // Decrease the remaining bomb count
        bombCount--;
    }
}

/**
 * Iterates through all cells on the board. For each cell that is not a bomb,
 * it calculates the number of surrounding bombs and updates the cell's type.
 */
void calculateNumbers(const Board *board)
{
    // iterate all cells on board
    for (int row = 0; row < board->rows; row++)
    {
        for (int col = 0; col < board->cols; col++)
        {
            // if cell is bomb, skip iteration
            if (isBombAt(board, row, col) == true) continue;

            //
            board->cell[row][col].type = countBombsAround(board, row, col);
        }
    }
}

/**
 * This function checks all eight neighboring cells of the given position and returns the count of bombs found.
 */
uint8_t countBombsAround(const Board *board, const uint8_t row, const uint8_t col)
{
    uint8_t count = 0;

    // positions of all surrounding cells
    const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++)
    {
        const uint8_t nx = row + dx[i];
        const uint8_t ny = col + dy[i];

        if (isValid(nx, ny, board->rows, board->cols))
        {
            if (board->cell[nx][ny].type == Bomb)
                count++;
        }
    }

    return count;
}

/**
 * This function verifies whether the checked field is within the board range
 */
bool isValid(const uint8_t row, const uint8_t col, const uint8_t maxRows, const uint8_t maxCols)
{
    // printf("isValid(row: %d, col: %d, maxRows: %d, maxCols: %d) result: %d\n", row,col,maxRows,maxCols, (row < maxRows) && (col < maxCols)); // DEBUG
    return (row < maxRows) && (col < maxCols);
}

/**
 * This function checks the cell at the given row and column
 * on the board to see if it is a bomb and return logic value
 */
bool isBombAt(const Board *board, const uint8_t row, const uint8_t col)
{
    return board->cell[row][col].type == Bomb;
}

/**
 * This function changes cell state
 */
void changeCellState(const Board *board, const uint8_t row, const uint8_t col, const uint8_t newState)
{
    board->cell[row][col].state = newState;
}

/**
 * This function handles logic for revealing cells.
 *  - if cell is 1-8, open only this cell
 *  - if cell is mine, handle game over
 *  - if cell is empty, all surrounding non-mined cells will automatically be opened
 */
void revealCell(Board *board, const uint8_t row, const uint8_t col)
{
    // Check if the specified cell is within the bounds of the board
    if (!isValid(row, col, board->rows, board->cols)) return;

    // Temporary variable for actual cell
    const Cell *cell = &board->cell[row][col];

    // checking if cell is already open or flagged
    if (cell->state == Opened || cell->state == Flagged)
        return;

    // Bomb case handling
    if (cell->type == Bomb)
    {
        board->winFlag = 0;
        handleGameOver(board);
        return;
    }

    // Open current cell
    changeCellState(board, row, col, Opened);

    //If cell is empty, revealNeighbors
    if (cell->type == Empty)
        revealEmptyNeighbors(board, row, col);
}

/**
 * This function ensures that all neighboring cells that are not bombs will be automatically revealed.
 */
void revealEmptyNeighbors(Board *board, const uint8_t row, const uint8_t col)
{
    // Arrays representing the relative positions of the eight neighboring cells
    const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Iterate through all eight possible neighbors
    for (int i = 0; i < 8; i++)
    {
        // Calculate the coordinates of the neighboring cell
        const uint8_t nx = row + dx[i];
        const uint8_t ny = col + dy[i];

        // Check if the neighboring cell is within the bounds of the board
        if (!isValid(nx, ny, board->cols, board->rows)) continue;

        // Temporary cell variable
        const Cell *cell = &board->cell[nx][ny];

        // Check if the neighboring cell is closed and not a bomb
        if (cell->state == Closed && cell->type != Bomb)
            revealCell(board, nx, ny);
    }
}

/**
 * This function toggles the state in Closed/Flagged cell taking position from cursor position
 */
void toggleFlag(Board *board)
{
    const CellState cell_state = board->cell[board->cursor.row][board->cursor.col].state;
    if (cell_state == Closed)
    {
        board->flagCounter--;
        changeCellState(board, board->cursor.row, board->cursor.col, Flagged);
    }
    if (cell_state == Flagged)
    {
        board->flagCounter++;
        changeCellState(board, board->cursor.row, board->cursor.col, Closed);
    }
}

/**
 * Developer function to print a detailed preview of the board's internal state.
 * This function is intended for debugging purposes only and provides detailed
 * information about the state, type, and cursor position of each cell in the board.
 */
void printBoardPreview(const Board *board)
{
    printf("[state, type, isCursorOn]\n");
    for (int row = 0; row < board->rows; row++)
    {
        for (int col = 0; col < board->cols; col++)
        {
            // Print cell details: state, type, and whether the cursor is on the cell
            printf("[%d, %d, %d], ",
                   board->cell[row][col].state,
                   board->cell[row][col].type,
                   (board->cursor.row == row) && (board->cursor.col == col) ? 1 : 0
            );
        }
        printf("\n");
    }
}

/**
 * This function prints the final and colored board in console
 * Prints flag counter and timer above board
 * cheatMode prints mines as if they were Opened
 */
void printBoard(const Board *board, const bool cheatMode)
{
    printLogo();
    printFlagsAndTimer(board);

    // Iterate whole board
    for (unsigned int row = 0; row < board->rows; row++)
    {
        for (unsigned int col = 0; col < board->cols; col++)
        {
            // Temporary cell variable
            const Cell *cell = &board->cell[row][col];

            // Coloring the cell the cursor is on
            if (board->cursor.row == row && board->cursor.col == col)
                printf("%s",BG_TURQUOISE);

            // Printing Cell
            printCell(*cell, cheatMode);
        }
        printf("\n");
    }
}

/**
 * Displays the current number of flags remaining and the elapsed game timer.
 */
void printFlagsAndTimer(const Board *board)
{
    // Print the flag counter with leading zeros
    printf("%03d ", board->flagCounter);

    // Add spacing to align the timer to the right side of the board

    if (board->cols > 4)
        for (unsigned int i = 0; i < board->cols - 4; i++)
            printf("  ");


    // Print the timer with leading zeros
    printf("%03d\n", board->timer);
}

/**
 * This function reveals all bombs
 * and changes state of hitten bomb to BombHit
 */
void handleGameOver(Board *board)
{
    revealBombs(board);
    board->isGameRunning = 0;
}

/**
 * This function changes state of every bomb to open
 * and BombHit for bomb that was clicked
 * Also marks wrongly flagged cells as WrongBomb
 */
void revealBombs(const Board *board)
{
    for (int i = 0; i < board->rows; i++)
    {
        for (int j = 0; j < board->cols; j++)
        {
            Cell *cell = &board->cell[i][j];

            if (cell->type == Bomb)
            {
                // Open current bomb cells
                if (cell->state != Flagged)
                    changeCellState(board, i, j, Opened);
            } else if (cell->state == Flagged) // If cell is not bomb, and it is flagged change state to WrongBomb
                cell->state = WrongBomb;
        }
    }
    // Changing last cell's state to BombHit
    const Cell cursorPos = board->cell[board->cursor.row][board->cursor.col];

    if (cursorPos.type == Bomb && cursorPos.state != Flagged)
        changeCellState(board, board->cursor.row, board->cursor.col, BombHit);
}

/**
 * The function verifies if all non-bomb cells on the board are opened.
 * If true, it declares victory and ends the game.
 */
void checkWin(Board *board)
{
    // Iterate through the entire board
    for (unsigned int row = 0; row < board->rows; row++)
    {
        for (unsigned int col = 0; col < board->cols; col++)
        {
            const Cell cell = board->cell[row][col]; // Access the current cell

            // Skip iteration if the cell is a bomb
            if (cell.type == Bomb) continue;

            // If a non-bomb cell is not opened, the game is not yet won
            if (cell.type != Bomb && cell.state != Opened) return;
        }
    }

    // If all non-bomb cells are opened, the player wins
    board->winFlag = 1;
    handleGameOver(board);
}
