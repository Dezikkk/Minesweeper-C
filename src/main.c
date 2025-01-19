#include <program.h>
#include <menu.h>
#include <stdio.h>
#include <conio.h>

/**
 * Entry point for the program. Sets up the environment and starts the main menu.
 */
int main(void)
{
    // Seed the random number generator with a predefined seed for mine generation
    srand(SEED);
    enable_ansi_colors();
    initializeMenu();
    return 0;
}
