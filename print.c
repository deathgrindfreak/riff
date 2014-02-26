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


#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <form.h>
#include "riff.h"


int strings = 6;                       /* number of guitar strings */
FIELD *fields[7];


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

    // Strings for printing to header
    char song[LENGTH];
    char artist[LENGTH];
    char tabbed[LENGTH];
    char email[LENGTH];
    char tuning[LENGTH];

    // Set to default values if not specified
    // Otherwise, copy the labels
    if (fields[1] == NULL)
        strcpy(song, "Untitled");
    else
        strcpy(song, field_buffer(fields[1], 0));

    if (fields[2] == NULL)
        strcpy(artist, "by Artist");
    else {
        strcpy(artist, "by ");
        strcat(artist, field_buffer(fields[2], 0));
    }

    if (fields[3] == NULL)
        strcpy(tabbed, "Tabbed by: ________________________");
    else {
        strcpy(tabbed, "Tabbed by: ");
        strcat(tabbed, field_buffer(fields[3], 0));
    }

    if (fields[4] == NULL)
        strcpy(email, "Email: ____________________________");
    else {
        strcpy(tabbed, "Email: ");
        strcat(tabbed, field_buffer(fields[4], 0));
    }

    strcpy(tuning, "Tuning: ");
    strcat(tuning, field_buffer(fields[6], 0));


    // Print Header
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    print_big_text(song, Y_BUFFER - 1, (col - text_length(song)) / 2);
    attroff(COLOR_PAIR(1));

    int tab_offset = strlen("Tabbed by: ________________________") + X_BUFFER;
    if (strlen(tabbed) > tab_offset)
        tab_offset = strlen(tabbed);
    if (strlen(email) > tab_offset)
        tab_offset = strlen(email);

    mvprintw(Y_BUFFER + 3     , (col - strlen(artist)) / 2       , "%s", artist);
    mvprintw(HEADER_BUFFER - 4, col - tab_offset, "%s", tabbed);
    mvprintw(HEADER_BUFFER - 3, col - tab_offset, "%s", email);
    mvprintw(HEADER_BUFFER - 3, X_BUFFER                         , "%s", tuning);
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

    char stringage[LENGTH];
    char tunage[LENGTH];
    strcpy(stringage, field_buffer(fields[5], 0));
    strcpy(tunage, field_buffer(fields[6], 0));
    (fields[5] == NULL) ? (strings = 6) : (strings = stringage[0]);


    int staff_length = strings + 1;
    int printrow = HEADER_BUFFER; 
    while (printrow < row) {
        int i, j, k;
        for (i = 0, j = 0; i < strings; i++) {
            // If tuning contains a flat or a sharp
            if (tunage[strlen(tunage) - j - 1] == '#' || tunage[strlen(tunage) - j - 1] == 'b') {
                mvprintw(printrow + i, X_BUFFER - 1, "%c", tunage[strlen(tunage) - j - 2]);
                mvprintw(printrow + i, X_BUFFER    , "%c", tunage[strlen(tunage) - j - 1]);
                j += 2;
            } else {
                mvprintw(printrow + i, X_BUFFER, "%c", tunage[strlen(tunage) - j - 1]);
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
    char email[LENGTH];
    char tuning[LENGTH];

    strcpy(title_welcome, "Welcome to Riff!");


    strcpy(project, "Project Title: ");
    strcpy(song, "Song Title: ");
    strcpy(artist, "Artist: ");
    strcpy(tabbed, "Tabbed by: ");
    strcpy(email, "Email: ");
    strcpy(title_strings, "Number of Strings: ");

    char s[2]; 
    s[1] = '\0';
    strcpy(tuning, "Tuning: ");
    if (strings >= 4 && strings <= 8) {
        for (i = 1; i <= strings; i++) {
            s[0] = i + '0';
            strcat(tuning, "(");
            strcat(tuning, s);
            strcat(tuning, ")   ");
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

