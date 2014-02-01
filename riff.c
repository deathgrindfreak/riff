/************************************************************************************ 
 *
 ************************************************************************************/
 
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>


#define X_BUFFER 2      /* x spacing for header and staff */


void staff(int, int, int, char[]);


main(int argc, char *argv[])
{
    int row, col, strings = 6;
    char tuning[] = "EADGBE";

    initscr();

    getmaxyx(stdscr, row, col);

    staff(strings, 2, col, tuning);
    
    refresh();
    getch();
    endwin();

    return 0;
}


void staff(int strings, int row, int max_col, char tuning[]) {

    int i, j, k;
    for (i = 0; i < strings; i++) {
        mvprintw(row + i, X_BUFFER, "%c", tuning[strlen(tuning) - i - 1]);

        for (j = 0; j < max_col - (X_BUFFER + 2); j++) {
            mvprintw(row + i, X_BUFFER + j + 1, "%c", '-');
        }
    }
}
