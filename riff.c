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
WINDOW *title_info_win(int, int, int, int);
void destroy_win(WINDOW *local_win);


main(int argc, char *argv[])
{
    WINDOW *title_win;

    int row, col, strings = 6,
        staff_length = strings + 1;
    char tuning[] = "EADGBE";
    char author[] = "by Artist";
    char song[]   = "Big Long Song Title";
    char tabbed[] = "Tabbed By: _________________________";
    char email[]  = "Email: _________________________";

    initscr();

    if (has_colors() == TRUE)   // Test if terminal has color
        start_color();

    getmaxyx(stdscr, row, col);
    
    int width  = 60;
    int height = 20;
    int startx = (col - width) / 2;
    int starty = (row - height) / 2;
    
    // Header
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    print_big_text(song, Y_BUFFER - 1, (col - text_length(song)) / 2);
    attroff(COLOR_PAIR(1));

    mvprintw(Y_BUFFER + 3     , (col - strlen(author)) / 2       , "%s", author);
    mvprintw(HEADER_BUFFER - 4, col - (strlen(tabbed) + X_BUFFER), "%s", tabbed);
    mvprintw(HEADER_BUFFER - 3, col - (strlen(email) + X_BUFFER) , "%s", email);
    mvprintw(HEADER_BUFFER - 3, X_BUFFER                         , "%s%s", "Tuning: ", tuning);
    attroff(A_BOLD);


    // Staffs
    int printrow = HEADER_BUFFER;
    while (printrow < row) {
        staff(strings, printrow, col, tuning);
        printrow += staff_length;
    }
    refresh();
    

    // Title Window
    title_win = title_info_win(height, width, starty, startx);
    wrefresh(title_win);


    getch();
    destroy_win(title_win);
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

/* Creates the title info window and prints the associated labels */
WINDOW *title_info_win(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    // Default labels
    int HEADER = 3;
    int Y_BUFF = 2;
    int X_BUFF = 2;
    
    int LENGTH = 100;   // Length for tuning array

    int strings = 8;

    char title_welcome[]      = "Welcome to Riff!";
    char title_song_title[]   = "Song Title: ___________________________________________ ";
    char title_artist[]       = "Artist:     ___________________________________________ ";
    char title_tabbed[]       = "Tabbed by:  ___________________________________________ ";
    char title_email[]        = "Email:      ___________________________________________ ";
    char title_strings[]      = "Number of Strings: ";
    char title_tuning[LENGTH];

    int i;
    char s[2]; 
    s[1] = '\0';
    strcpy(title_tuning, "Tuning: ");
    if (strings >= 4 && strings <=8) {
        for (i = 1; i <= strings; i++) {
            s[0] = i + '0';
            strcat(title_tuning, "(");
            strcat(title_tuning, s);
            strcat(title_tuning, ")__ ");
        }
    }


    // Print to screen
    attron(A_BOLD);
    mvwprintw(local_win, Y_BUFF, (width - strlen(title_welcome)) / 2, "%s", title_welcome);
    attroff(A_BOLD);

    mvwprintw(local_win, Y_BUFF + HEADER     , X_BUFF, "%s", title_song_title);
    mvwprintw(local_win, Y_BUFF + HEADER + 2 , X_BUFF, "%s", title_artist);
    mvwprintw(local_win, Y_BUFF + HEADER + 4 , X_BUFF, "%s", title_tabbed);
    mvwprintw(local_win, Y_BUFF + HEADER + 6 , X_BUFF, "%s", title_email);
    mvwprintw(local_win, Y_BUFF + HEADER + 10, X_BUFF, "%s", title_strings);
    mvwprintw(local_win, Y_BUFF + HEADER + 12, X_BUFF, "%s", title_tuning);
    
    wrefresh(local_win);


    return local_win;
}

/* Destroys the title info window */
void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ',' ',' ',' ',' ',' ',' ',' ');

    wrefresh(local_win);
    delwin(local_win);
}
