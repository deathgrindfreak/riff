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


char title_window_labels[6][LENGTH];   /* labels for the title window */
int strings = 6;                       /* number of guitar strings */


/* init_labels - initializes all labels to '\0' */
void init_labels(void) {
    int i;
    for (i = 0; i < 6; i++)
        title_window_labels[i][0] = '\0';
}

/* label_push - pushes a character to a specific label */
void label_push(int pos, char ch) {
    int len = strlen(title_window_labels[pos]);
    title_window_labels[pos][len] = ch;
    title_window_labels[pos][len+1] = '\0';
}

/* label_del - deletes the top character from specific label */
void label_del(int pos) {
    int len = strlen(title_window_labels[pos]);
    title_window_labels[pos][len] = '\0';
}

/* tuning_length - finds the true length of a guitar tuning */
int tuning_length(char tuning[]) {
    int i, len = 0;
    for (i = 0; i < strlen(tuning); i++)
        if (tuning[i] != '#' && tuning[i] != 'b')   /* if letter isn't a sharp or a flat */
            len++;
    return len;
}

/* header - prints the header at the top of the screen */
void header(int col) {

    // Set to default values if not specified
    if (strlen(title_window_labels[1]) == 0)
        strcpy(title_window_labels[1], "Untitled");
    if (strlen(title_window_labels[2]) == 0)
        strcpy(title_window_labels[2], "by Artist");
    if (strlen(title_window_labels[3]) == 0)
        strcpy(title_window_labels[3], "Tabbed by: ");
    if (strlen(title_window_labels[4]) == 0)
        strcpy(title_window_labels[4], "Email: ");
    if (strlen(title_window_labels[5]) == 0)
        strcpy(title_window_labels[5], "EADGBE");

    // Print Header
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    print_big_text(title_window_labels[1], Y_BUFFER - 1, (col - text_length(title_window_labels[1])) / 2);
    attroff(COLOR_PAIR(1));

    mvprintw(Y_BUFFER + 3     , (col - strlen(title_window_labels[2])) / 2       , "%s", title_window_labels[2]);
    mvprintw(HEADER_BUFFER - 4, col - (55 + X_BUFFER), "%s", title_window_labels[3]);
    mvprintw(HEADER_BUFFER - 3, col - (55 + X_BUFFER), "%s", title_window_labels[4]);
    mvprintw(HEADER_BUFFER - 3, X_BUFFER             , "%s%s", "Tuning: ", title_window_labels[5]);
    attroff(A_BOLD);

    refresh();
}

/* measure - inserts a measure at (y, x) == (row, col) */
void measure(int row, int col) {
    
    int i;
    for (i = 0; i < strings; i++) {
        mvprintw(row + i, col, "%c", '|');
    }
}

/* staff - prints out the staff with the tuning included */
void staff(int row, int col) {

    int staff_length = strings + 1;
    int printrow = HEADER_BUFFER; 
    while (printrow < row) {
        int i, j, k;
        for (i = 0, j = 0; i < strings; i++) {
            // If tuning contains a flat or a sharp
            if (title_window_labels[5][strlen(title_window_labels[5]) - j - 1] == '#' || title_window_labels[5][strlen(title_window_labels[5]) - j - 1] == 'b') {
                mvprintw(printrow + i, X_BUFFER - 1, "%c", title_window_labels[5][strlen(title_window_labels[5]) - j - 2]);
                mvprintw(printrow + i, X_BUFFER    , "%c", title_window_labels[5][strlen(title_window_labels[5]) - j - 1]);
                j += 2;
            } else {
                mvprintw(printrow + i, X_BUFFER, "%c", title_window_labels[5][strlen(title_window_labels[5]) - j - 1]);
                ++j;
            }
            mvprintw(printrow + i, X_BUFFER + 1, "%c", '|');
            
            // Print the staff
            for (k = 0; k < col - (X_BUFFER + 4); k++) {
                mvprintw(printrow + i, X_BUFFER + k + 2, "%c", '-');
            }
        }
        printrow += staff_length;
    }
}

/* Creates the title info window and prints the associated labels */
WINDOW* title_info_win(int height, int width, int starty, int startx) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    int i;
    char title_welcome[LENGTH];
    char title_strings[LENGTH];
    char project[LENGTH];
    char song[LENGTH];
    char artist[LENGTH];
    char tabbed[LENGTH];
    char tuning[LENGTH];
    char email[LENGTH];

    strcpy(title_welcome, "Welcome to Riff!");


    if (strlen(title_window_labels[0]) == 0) {
        strcpy(project, "Project Title: ________________________________________");
    } else {
        strcpy(project, "Project Title: ");
        strcat(project, title_window_labels[0]);
        int total_length = strlen("Project Title: ________________________________________");
        int end = strlen(project);
        for (i = end; i < total_length; i++)
            strcat(project, "_");
    }
    
    if (!strcmp(title_window_labels[1], "Untitled")) {
        strcpy(song, "Song Title: ___________________________________________");
    } else {
        strcpy(song, "Song Title: ");
        strcat(song, title_window_labels[1]);
        int total_length = strlen("Song Title: ___________________________________________");
        int end = strlen(song);
        for (i = end; i < total_length; i++)
            strcat(song, "_");
    }
    
    if (!strcmp(title_window_labels[2], "by Artist")) {
        strcpy(artist, "Artist:     ___________________________________________");
    } else {
        strcpy(artist, "Artist:     ");
        strcat(artist, title_window_labels[2]);
        int total_length = strlen("Artist:     ___________________________________________");
        int end = strlen(artist);
        for (i = end; i < total_length; i++)
            strcat(artist, "_");
    }
    
    if (!strcmp(title_window_labels[3], "Tabbed by: ")) {
        strcpy(tabbed, "Tabbed by:  ___________________________________________");
    } else {
        strcpy(tabbed, "Tabbed by:  ");
        strcat(tabbed, title_window_labels[3]);
        int total_length = strlen("Tabbed by:  ___________________________________________");
        int end = strlen(tabbed);
        for (i = end; i < total_length; i++)
            strcat(tabbed, "_");
    }
    
    if (!strcmp(title_window_labels[4], "Email: ")) {
        strcpy(email, "Email:      ___________________________________________");
    } else {
        strcpy(email, "Email:      ");
        strcat(email, title_window_labels[4]);
        int total_length = strlen("Email:      ___________________________________________");
        int end = strlen(email);
        for (i = end; i < total_length; i++)
            strcat(email, "_");
    }
    
    strcpy(title_strings, "Number of Strings: ");
    int len = strlen(title_strings);
    title_strings[len] = strings + '0';
    title_strings[len+1] = '\0';

    char s[2]; 
    s[1] = '\0';
    strcpy(tuning, "Tuning: ");
    if (strings >= 4 && strings <= 8) {
        for (i = 1; i <= strings; i++) {
            s[0] = i + '0';
            strcat(tuning, "(");
            strcat(tuning, s);
            strcat(tuning, ")__ ");
        }
    }


    // Print to screen
    wattron(local_win, A_BOLD);
    mvwprintw(local_win, WIN_Y_BUFFER, (width - strlen(title_welcome)) / 2, "%s", title_welcome);
    wattroff(local_win, A_BOLD);

    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER     , WIN_X_BUFFER, "%s", project);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 4 , WIN_X_BUFFER, "%s", song);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 6 , WIN_X_BUFFER, "%s", artist);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 8 , WIN_X_BUFFER, "%s", tabbed);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 10, WIN_X_BUFFER, "%s", email);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 14, WIN_X_BUFFER, "%s", title_strings);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 16, WIN_X_BUFFER, "%s", tuning);
    
    // "Buttons"
    int BUTTON_BUFFER = width - WIN_Y_BUFFER - strlen("OK") - strlen("CANCEL") - 11;

    wattron(local_win, A_REVERSE | A_BOLD);
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 19, BUTTON_BUFFER    , "%s", "      ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 20, BUTTON_BUFFER    , "%s", "  OK  ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 21, BUTTON_BUFFER    , "%s", "      ");
    
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 19, BUTTON_BUFFER + 8, "%s", "          ");
    mvwprintw(local_win, WIN_Y_BUFFER + WIN_HEADER + 20, BUTTON_BUFFER + 8, "%s", "  CANCEL  ");
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

