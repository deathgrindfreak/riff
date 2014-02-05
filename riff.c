/************************************************************************************ 
 * Copyright (c) 2014, John Cooper Bell                                             *
 * All rights reserved.                                                             *
 *                                                                                  *
 * Redistribution and use in source and binary forms, with or without               *
 * modification, are permitted provided that the following conditions are met:      *
 *                                                                                  *
 * 1. Redistributions of source code must retain the above copyright notice, this   *
 *    list of conditions and the following disclaimer.                              *
 * 2. Redistributions in binary form must reproduce the above copyright notice,     *
 *    this list of conditions and the following disclaimer in the documentation     *
 *    and/or other materials provided with the distribution.                        *
 *                                                                                  *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND  *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED    *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR  *
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES   *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;     *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND      *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS    *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     *
 *                                                                                  *
 * The views and conclusions contained in the software and documentation are those  *
 * of the authors and should not be interpreted as representing official policies,  *
 * either expressed or implied, of the FreeBSD Project.                             *
 ************************************************************************************/


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "big_text.h"


#define HEADER_BUFFER 12    /* y buffer spacing for header */
#define X_BUFFER 2          /* x buffer for side spacing */
#define Y_BUFFER 2          /* y buffer for top spacing */
#define LENGTH 100          /* Length for arrays */
#define WIN_HEADER 3        /* y buffer spacing for window header */
#define WIN_X_BUFFER 2      /* x buffer spacing for window */
#define WIN_Y_BUFFER 2      /* y buffer spacing for window */
#define MIN_WIDTH 70        /* Minimum width for terminal */
#define MIN_HEIGHT 35       /* Minimum height for terminal */


void header(int, char[], char[], char[], char[], char[]);
void staff(int, int, int, char[]);
void measure(int, int, int);
WINDOW* title_info_win(int, int, int, int);
void destroy_win(WINDOW *);


main(int argc, char *argv[])
{
    // Initial values
    WINDOW *title_win;  // Title window pointer

    int row, col;
    int strings = 6, staff_length = strings + 1;
    
    char tuning[LENGTH];
    char author[LENGTH];
    char   song[LENGTH];
    char tabbed[LENGTH];
    char  email[LENGTH];

         tuning[0] = 0;
         author[0] = 0;
           song[0] = 0;
         tabbed[0] = 0;
          email[0] = 0;   


    // Start screen
    initscr();
    clear();
    noecho();
    cbreak();

    if (has_colors() == TRUE)   // Test if terminal has color
        start_color();

    getmaxyx(stdscr, row, col);

    // Check if terminal is a proper size
    if (row < MIN_HEIGHT || col < MIN_WIDTH) {
        char term_size_err1[] = "Your terminal is too small!";
        char term_size_err2[] = "Resize and try again.";
        mvprintw(row / 2 - 1, (col - strlen(term_size_err1)) / 2, "%s", term_size_err1);
        mvprintw(row / 2    , (col - strlen(term_size_err2)) / 2, "%s", term_size_err2);
        getch();
        endwin();
        return 0;
    }

    
    // Header
    header(col, tuning, author, song, tabbed, email);

    // Staffs
    int printrow = HEADER_BUFFER;
    while (printrow < row) {
        staff(strings, printrow, col, tuning);
        printrow += staff_length;
    }
    refresh();

    // Title Window
    int width  = 60;
    int height = 25;
    int startx = (col - width) / 2;
    int starty = (row - height) / 2;
    
    title_win = title_info_win(height, width, starty, startx);
    keypad(title_win, true);
    
    int movements[7][2] = {
                            {WIN_Y_BUFFER + WIN_HEADER      , WIN_X_BUFFER + strlen("Project Title: ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 4  , WIN_X_BUFFER + strlen("Song Title: ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 6  , WIN_X_BUFFER + strlen("Artist:     ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 8  , WIN_X_BUFFER + strlen("Tabbed by:  ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 10 , WIN_X_BUFFER + strlen("Email:      ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 14 , WIN_X_BUFFER + strlen("Number of Strings: ")},
                            {WIN_Y_BUFFER + WIN_HEADER + 16 , WIN_X_BUFFER + strlen("Tuning: (1)")},
                          };

    int y, x, cur_x = movements[0][1], c, pos = 0;
    wmove(title_win, movements[0][0], movements[0][1]);
    wrefresh(title_win);

    while (1) {
        c = wgetch(title_win);
        getyx(title_win, y, x);
        cur_x = x;
        switch (c) {
            case KEY_UP:
                if (pos != 0) {   // Not at top input label
                    --pos;
                    wmove(title_win, movements[pos][0], movements[pos][1]);
                }
                break;

            case KEY_DOWN:
                if (pos != 6) {   // Not at bottom input label
                    ++pos;
                    wmove(title_win, movements[pos][0], movements[pos][1]);
                }
                break;

            case KEY_LEFT:
                if (x > movements[pos][1] && x <= width - 2 * WIN_X_BUFFER) {
                    if (pos != 5 && pos != 6) {
                        --cur_x;
                        wmove(title_win, movements[pos][0], cur_x);
                    } else if (pos == 6) {
                        cur_x -= 6;
                        wmove(title_win, movements[pos][0], cur_x);
                    }
                }
                break;

            case KEY_RIGHT:
                if (x >= movements[pos][1] && x < width - 2 * WIN_X_BUFFER) {
                    if (pos != 5 && pos != 6) {
                        ++cur_x;
                        wmove(title_win, movements[pos][0], cur_x);
                    } else if (pos == 6) {
                        cur_x += 6;
                        wmove(title_win, movements[pos][0], cur_x);
                    }
                }
                break;
        }
    }




    getch();
    destroy_win(title_win);
    clear();
    refresh();
    getch();
    endwin();

    return 0;
}


/* header - prints the header at the top of the screen */
void header(int col, char tuning[], char author[], char song[], char tabbed[], char email[]) {

    // Set to default values if not specified
    if (tuning[0] == 0)
        strcpy(tuning, "EADGBE");
    if (author[0] == 0)
        strcpy(author, "by Artist");
    if (song[0] == 0)
        strcpy(song, "Untitled");
    if (tabbed[0] == 0)
        strcpy(tabbed, "Tabbed By: _________________________");
    if (email[0] == 0)
        strcpy(email, "Email: _________________________");


    // Print Header
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

    refresh();
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
WINDOW* title_info_win(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    int strings = 8;

    char title_welcome[]       = "Welcome to Riff!";
    char title_project_title[] = "Project Title: ________________________________________ ";
    char title_song_title[]    = "Song Title: ___________________________________________ ";
    char title_artist[]        = "Artist:     ___________________________________________ ";
    char title_tabbed[]        = "Tabbed by:  ___________________________________________ ";
    char title_email[]         = "Email:      ___________________________________________ ";
    char title_strings[]       = "Number of Strings: ";
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
    wattron(local_win, A_BOLD);
    mvwprintw(local_win, WIN_Y_BUFFER, (width - strlen(title_welcome)) / 2, "%s", title_welcome);
    wattroff(local_win, A_BOLD);

    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER     , WIN_X_BUFFER, "%s", title_project_title);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 4 , WIN_X_BUFFER, "%s", title_song_title);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 6 , WIN_X_BUFFER, "%s", title_artist);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 8 , WIN_X_BUFFER, "%s", title_tabbed);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 10, WIN_X_BUFFER, "%s", title_email);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 14, WIN_X_BUFFER, "%s", title_strings);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 16, WIN_X_BUFFER, "%s", title_tuning);
    
    wrefresh(local_win);

    return local_win;
}

/* Destroys the title info window */
void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ',' ',' ',' ',' ',' ',' ',' ');

    wrefresh(local_win);
    delwin(local_win);
}

