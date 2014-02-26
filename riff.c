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
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>
#include <form.h>
#include "big_text.h"
#include "riff.h"


int main(int argc, char *argv[])
{
    // Initial values
    WINDOW *title_win;  // Title window pointer
    //FIELD  *fields[7];
    FORM   *title_form;

    int i;
    for (i = 0; i < 7; i++)
        fields[i] = NULL;

    int row, col;


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
    header(col);

    // Staffs
    staff(row, col);
    refresh();

    // Title Window
    int startx = (col - TITLE_WINDOW_WIDTH) / 2;
    int starty = (row - TITLE_WINDOW_HEIGHT) / 2;
    
    title_win = title_info_win(TITLE_WINDOW_HEIGHT, TITLE_WINDOW_WIDTH, starty, startx);
    keypad(title_win, true);

    int x_mins[8] = {
                        WIN_X_BUFFER + strlen("Project Title: "),
                        WIN_X_BUFFER + strlen("Song Title: "),
                        WIN_X_BUFFER + strlen("Artist:     "),
                        WIN_X_BUFFER + strlen("Tabbed by:  "),
                        WIN_X_BUFFER + strlen("Email:      "),
                        WIN_X_BUFFER + strlen("Number of Strings: "),
                        WIN_X_BUFFER + strlen("Tuning: (1)"),
                        TITLE_WINDOW_WIDTH - WIN_X_BUFFER - strlen("OK") - strlen("CANCEL") - 9,
                    };

    
    
    /* Input fields for windows */
    fields[0] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[0] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER     , x_mins[0], 0, 0); 
    fields[1] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[1] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 4 , x_mins[1], 0, 0); 
    fields[2] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[2] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 6 , x_mins[2], 0, 0); 
    fields[3] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[3] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 8 , x_mins[3], 0, 0); 
    fields[4] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[4] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 10, x_mins[4], 0, 0); 
    fields[5] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[5] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 14, x_mins[5], 0, 0); 
    fields[6] = new_field(1, TITLE_WINDOW_WIDTH - x_mins[6] - 2 * WIN_X_BUFFER, WIN_Y_BUFFER + WIN_HEADER + 16, x_mins[6], 0, 0); 
    fields[7];
    
    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);

    set_field_back(fields[1], A_UNDERLINE);
    field_opts_off(fields[1], O_AUTOSKIP);

    set_field_back(fields[2], A_UNDERLINE);
    field_opts_off(fields[2], O_AUTOSKIP);

    set_field_back(fields[3], A_UNDERLINE);
    field_opts_off(fields[3], O_AUTOSKIP);

    set_field_back(fields[4], A_UNDERLINE);
    field_opts_off(fields[4], O_AUTOSKIP);

    set_field_back(fields[5], A_UNDERLINE);
    field_opts_off(fields[5], O_AUTOSKIP);

    set_field_back(fields[6], A_UNDERLINE);
    field_opts_off(fields[6], O_AUTOSKIP);
    
    /* Create the title window form */
    set_form_win(title_form, title_win);
    title_form = new_form(fields);
    post_form(title_form);
    refresh();

    int movements[8][2] = {
                            {WIN_Y_BUFFER + WIN_HEADER      , x_mins[0]},
                            {WIN_Y_BUFFER + WIN_HEADER + 4  , x_mins[1]},
                            {WIN_Y_BUFFER + WIN_HEADER + 6  , x_mins[2]},
                            {WIN_Y_BUFFER + WIN_HEADER + 8  , x_mins[3]},
                            {WIN_Y_BUFFER + WIN_HEADER + 10 , x_mins[4]},
                            {WIN_Y_BUFFER + WIN_HEADER + 14 , x_mins[5]},
                            {WIN_Y_BUFFER + WIN_HEADER + 16 , x_mins[6]},
                            {WIN_Y_BUFFER + WIN_HEADER + 20 , x_mins[7]},
                          };

    int tuning_moves[8] = { // 8 possible moves for the 8 individual strings
                            WIN_X_BUFFER + strlen("Tuning: (1)"),
                            WIN_X_BUFFER + strlen("Tuning: (1)") +     6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 2 * 6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 3 * 6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 4 * 6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 5 * 6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 6 * 6,
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 7 * 6,
                          };

    int y, x, cur_tune = 0, cur_x = movements[0][1], ch, pos = 0,
        BUTTON_WIDTH = 8;

    wmove(title_win, movements[0][0], movements[0][1]);
    wrefresh(title_win);

    while (true) {
        ch = wgetch(title_win);
        getyx(title_win, y, x);
        cur_x = x;
        if (ch ==  KEY_UP) {
            form_driver(title_form, REQ_PREV_FIELD);
            form_driver(title_form, REQ_END_LINE);
        } else if (ch == KEY_DOWN) {
            form_driver(title_form, REQ_NEXT_FIELD);
            form_driver(title_form, REQ_END_LINE);
        //} else if (ch == KEY_LEFT) {
        //    form_driver(title_form, REQ_NEXT_FIELD);
        //    form_driver(title_form, REQ_END_LINE);
        //} else if (ch == KEY_RIGHT) {
        //    form_driver(title_form, REQ_NEXT_FIELD);
        //    form_driver(title_form, REQ_END_LINE);

        /* ch is a letter, number or special char */
        } else if (((ch >= 'a'&& ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
                    (ch >= 33 && ch <= 46) || ch == ' ' || ch == 64) && x < TITLE_WINDOW_WIDTH - 2 * WIN_X_BUFFER) {

            form_driver(title_form, ch);

            //if (pos != 5 && pos != 6) {
            //    label_push(pos, ch);
            //    mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", ch);
            //    ++movements[pos][1];
            //    wmove(title_win, movements[pos][0], movements[pos][1]);

            //} else if (pos == 5 && (ch >= '4' && ch <= '8') && x == x_mins[pos]) {
            //    strings = ch - '0';

            //    destroy_win(title_win);
            //    title_win = title_info_win(TITLE_WINDOW_HEIGHT, TITLE_WINDOW_WIDTH, starty, startx);
            //    keypad(title_win, true);

            //    mvwprintw(title_win, movements[pos][0], x_mins[pos], "%c", ch);
            //    wmove(title_win, movements[pos][0], x_mins[pos]);

            //} else if (pos == 6  && ((x == tuning_moves[cur_tune] && ((ch >= 'A' && ch <= 'G') || (ch >= 'a' && ch <= 'g'))) ||
            //                         (x == tuning_moves[cur_tune] + 1 && (ch == 'b' || ch == '#')))) {

            //    if (ch >= 'a' && ch <= 'g') // NEED TO ENSURE THAT LOWER CASE B IS IN FACT A NOTE LETTER AND NOT A FLAT 
            //        label_push(pos-1, ch - 'a' + 'A');
            //    else
            //        label_push(pos-1, ch);

            //    if (cur_tune < strings-1) {
            //        if (x == tuning_moves[cur_tune] + 1) {
            //            ++cur_tune;
            //            movements[pos][1] = tuning_moves[cur_tune];
            //        } else
            //            ++movements[pos][1];
            //        wmove(title_win, movements[pos][0], movements[pos][1]);
            //    } else if (x == tuning_moves[strings - 1] || x == tuning_moves[strings-1] + 1) {
            //        ++movements[pos][1];
            //        wmove(title_win, movements[pos][0], movements[pos][1]);
            //    }
            //}

        //} else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {


        //    //TEMP
        //    int i;
        //    for (i = 0; i < 6; i++)
        //        mvwprintw(title_win, WIN_Y_BUFFER + WIN_HEADER + 17 + i, 1, "%s", title_window_labels[i]);
        //    wrefresh(title_win);
        //    //TEMP

        //    if (x > x_mins[pos]) {
        //        if (pos != 5 && pos != 6) {
        //            label_del(pos);
        //            --movements[pos][1];
        //            mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
        //            wmove(title_win, movements[pos][0], movements[pos][1]);
        //        } else if (pos == 6) {
        //            if (x == tuning_moves[cur_tune] + 1 || x == tuning_moves[strings-1] + 2) {
        //                --movements[pos][1];
        //                mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
        //                wmove(title_win, movements[pos][0], movements[pos][1]);   
        //            } else if (x == tuning_moves[cur_tune]) {
        //                --cur_tune;
        //                movements[pos][1] = tuning_moves[cur_tune] + 1;
        //                mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
        //                wmove(title_win, movements[pos][0], movements[pos][1]);   
        //            }
        //        }
        //    } else if (pos == 5) {
        //        strings = 6;
        //        destroy_win(title_win);
        //        title_win = title_info_win(TITLE_WINDOW_HEIGHT, TITLE_WINDOW_WIDTH, starty, startx);
        //        keypad(title_win, true);
        //        mvwprintw(title_win, movements[pos][0], x_mins[pos], "%c", '_');
        //        wmove(title_win, movements[pos][0], x_mins[pos]);
        //    }
        } else if (ch == '\n') {    /* when user presses one of the buttons */
            if (pos != 7) {
                ++pos;
                wmove(title_win, movements[pos][0], movements[pos][1]);   
            } else if (y == movements[7][0] && x == movements[7][1]) { /* "OK" button */
                destroy_win(title_win);
                clear();
                header(col);
                staff(row, col);
                refresh();
                break;
            } else if (y == movements[7][0] && x == movements[7][1] + BUTTON_WIDTH) { /* "CANCEL" button */
                destroy_win(title_win);
                clear();
                header(col);
                staff(row, col);
                refresh();
                break;
            }
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
