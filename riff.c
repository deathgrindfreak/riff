/************************************************************************************ 
 *
 ************************************************************************************/
 

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "big_text.h"


#define HEADER_BUFFER 12    /* y buffer spacing for header */
#define X_BUFFER 2          /* x buffer for side spacing */
#define Y_BUFFER 2          /* y buffer for top spacing */


void staff(int, int, int, char[]);
void measure(int, int, int);


main(int argc, char *argv[])
{
    int row, col, strings = 6,
        staff_length = strings + 1;
    char tuning[] = "EADGBE";
    char author[] = "by Artist";
    char song[]   = "Big Long Song Title";
    char tabbed[] = "Tabbed By: _________________________";
    char email[]  = "Email: _________________________";

    initscr();
    getmaxyx(stdscr, row, col);

    
    // Header
    print_big_text(song, Y_BUFFER - 1, (col - text_length(song)) / 2);
    mvprintw(Y_BUFFER + 3     , (col - strlen(author)) / 2       , "%s", author);
    mvprintw(HEADER_BUFFER - 4, col - (strlen(tabbed) + X_BUFFER), "%s", tabbed);
    mvprintw(HEADER_BUFFER - 3, col - (strlen(email) + X_BUFFER) , "%s", email);
    mvprintw(HEADER_BUFFER - 3, X_BUFFER                         , "%s%s", "Tuning: ", tuning);

    int printrow = HEADER_BUFFER;
    while (printrow < row) {
        staff(strings, printrow, col, tuning);
        printrow += staff_length;
    }


    refresh();
    getch();
    endwin();

    return 0;
}


/* measure - inserts a measure at (y, x) == (row, col) */
void measure(int strings, int row, int col) {
    
    int i;
    for (i = 0; i < strings; i++) {
        mvprintw(row + i, col, "%c", '|');
    }
}

/* staff - prints out the staff with the tuning included */
void staff(int strings, int row, int max_col, char tuning[]) {

    int i, j, k;
    for (i = 0, j = 0; i < strings; i++) {
        // If tuning contains a flat or a sharp
        if (tuning[strlen(tuning) - j - 1] == '#' || tuning[strlen(tuning) - j - 1] == 'b') {
            mvprintw(row + i, X_BUFFER - 1, "%c", tuning[strlen(tuning) - j - 2]);
            mvprintw(row + i, X_BUFFER    , "%c", tuning[strlen(tuning) - j - 1]);
            j += 2;
        } else {
            mvprintw(row + i, X_BUFFER, "%c", tuning[strlen(tuning) - j - 1]);
            ++j;
        }
        mvprintw(row + i, X_BUFFER + 1, "%c", '|');
        
        // Print the staff
        for (k = 0; k < max_col - (X_BUFFER + 4); k++) {
            mvprintw(row + i, X_BUFFER + k + 2, "%c", '-');
        }
    }
}
