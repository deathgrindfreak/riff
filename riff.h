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


#define HEADER_BUFFER 12    /* y buffer spacing for header */
#define X_BUFFER 2          /* x buffer for side spacing */
#define Y_BUFFER 2          /* y buffer for top spacing */
#define LENGTH 100          /* Length for arrays */
#define WIN_HEADER 3        /* y buffer spacing for window header */
#define WIN_X_BUFFER 2      /* x buffer spacing for window */
#define WIN_Y_BUFFER 2      /* y buffer spacing for window */
#define MIN_WIDTH 70        /* Minimum width for terminal */
#define MIN_HEIGHT 35       /* Minimum height for terminal */


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
WINDOW* title_info_win(int height, int width, int starty, int startx, int strings) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    char title_welcome[]       = "Welcome to Riff!";
    char title_project_title[] = "Project Title: ________________________________________ ";
    char title_song_title[]    = "Song Title: ___________________________________________ ";
    char title_artist[]        = "Artist:     ___________________________________________ ";
    char title_tabbed[]        = "Tabbed by:  ___________________________________________ ";
    char title_email[]         = "Email:      ___________________________________________ ";
    char title_strings[]       = "Number of Strings: _";
    char title_tuning[LENGTH];

    int i;
    char s[2]; 
    s[1] = '\0';
    strcpy(title_tuning, "Tuning: ");
    if (strings >= 4 && strings <= 8) {
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
    
    // "Buttons"
    int BUTTON_BUFFER = width - WIN_Y_BUFFER - strlen("OK") - strlen("CANCEL") - 11;

    wattron(local_win, A_REVERSE | A_BOLD);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 19, BUTTON_BUFFER    , "%s", "      ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 19, BUTTON_BUFFER + 8, "%s", "          ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 20, BUTTON_BUFFER    , "%s", "  OK  ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 20, BUTTON_BUFFER + 8, "%s", "  CANCEL  ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 21, BUTTON_BUFFER    , "%s", "      ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 21, BUTTON_BUFFER + 8, "%s", "          ");
    wattroff(local_win, A_REVERSE | A_BOLD);


    wrefresh(local_win);

    return local_win;
}

/* Destroys the title info window */
void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ',' ',' ',' ',' ',' ',' ',' ');

    wrefresh(local_win);
    delwin(local_win);
}

