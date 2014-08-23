#|************************************************************************************ 
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
  ************************************************************************************|#


(declare (unit print))
(require-extension ncurses srfi-13)

;;; Global constants
(define HEADER_BUFFER 12)             ; y buffer spacing for header
(define X_BUFFER 2)                   ; x buffer for side spacing
(define Y_BUFFER 2)                   ; y buffer for top spacing
(define WIN_HEADER 3)                 ; y buffer spacing for window header
(define WIN_X_BUFFER 2)               ; x buffer spacing for window
(define WIN_Y_BUFFER 2)               ; y buffer spacing for window
(define MIN_WIDTH 70)                 ; Minimum width for terminal
(define MIN_HEIGHT 35)                ; Minimum height for terminal
(define TITLE_WINDOW_WIDTH  60)       ; Title window width
(define TITLE_WINDOW_HEIGHT 29)       ; Title window height
(define BUTTON_WIDTH 8)               ; Space between the OK and CANCEL buttons


;;; Fields 
(define strings 6)
(define tuning "EADGBE")
(define song)
(define artist)
(define tabbed)
(define email)
(define tuning)


;;; print-header - prints the header at the top of the screen
(define (print-header col))

#|
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
|#


;;; print-measure - inserts a measure at (y, x) == (row, col)
(define (print-measure row col)
  (do ([i 0 (add1 i)])
      ((>= i strings))
    (mvprintw (+ row i) col "~a" #\|)))

;;; print-staff - prints out the staff with the tuning included
(define (print-staff row col)
  (let loop ([i 0]
             [lst (reverse (string->list tuning))])
    (unless (null? lst)
      (mvprintw (+ row i) (+ X_BUFFER 1) "~a" #\|)

      (do ([k 0 (add1 k)])
          ((>= k (- col (+ X_BUFFER 4))))
        (mvprintw (+ row i) (+ X_BUFFER k 2) "~a" #\-))

      (if (or (char=? (car lst) #\#)
              (char=? (car lst) #\b))
          (begin (mvprintw (+ row i) X_BUFFER "~a" (car lst))
                 (mvprintw (+ row i) (- X_BUFFER 1) "~a" (cadr lst))
                 (loop (+ i 1) (cddr lst)))
          (begin (mvprintw (+ row i) (- X_BUFFER 1) "~a" (car lst))
                 (loop (+ i 1) (cdr lst)))))))

;;; print-multiple-staffs - prints a number of staffs
(define (print-multiple-staffs num col)
  (let ([row HEADER_BUFFER])
    (let loop ([n 0])
      (when (< n num)
        (print-staff (+ row (* n (+ strings 1))) col)
        (loop (+ n 1))))))


#|
int strings = 6;                       /* number of guitar strings */


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
    strcpy(tuning, "Tuning: ");


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

    if (strings >= 4 && strings <= 8) {
        for (i = 0; i < strings; i++) {
            mvprintw(WIN_Y_BUFFER + WIN_HEADER + 16, WIN_X_BUFFER + strlen(tuning) + 6 * i, "(%d)", i+1);
        }
    }
    
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

/* prints the form fields status to the screen */
void print_to_screen(FIELD **fields) {
    char strings[14][LENGTH];

    mvprintw(WIN_Y_BUFFER + WIN_HEADER + 2, WIN_X_BUFFER+20, "%s", "FUCK");

    int i;
    for (i = 0; i < 14; i++) {
            mvprintw(WIN_Y_BUFFER + WIN_HEADER + 2 * i, WIN_X_BUFFER+12, "%d", field_status(fields[i]));
        if (field_status(fields[i])) {
            strcpy(strings[i], field_buffer(fields[i], 0));
            mvprintw(WIN_Y_BUFFER + WIN_HEADER + 2 * i, WIN_X_BUFFER, "%s", strings[i]);
        }
    }
}
|#
