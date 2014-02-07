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
#include "riff.h"


#define HEADER_BUFFER 12    	/* y buffer spacing for header */
#define X_BUFFER 2          	/* x buffer for side spacing */
#define Y_BUFFER 2          	/* y buffer for top spacing */
#define LENGTH 100          	/* Length for arrays */
#define WIN_HEADER 3        	/* y buffer spacing for window header */
#define WIN_X_BUFFER 2      	/* x buffer spacing for window */
#define WIN_Y_BUFFER 2      	/* y buffer spacing for window */
#define MIN_WIDTH 70        	/* Minimum width for terminal */
#define MIN_HEIGHT 35       	/* Minimum height for terminal */
#define TITLE_WINDOW_WIDTH  60	/* Title window width */
#define TITLE_WINDOW_HEIGHT 29	/* Title window height */


int main(int argc, char *argv[])
{
    // Initial values
    WINDOW *title_win;  // Title window pointer

    int row, col;
    int strings = 6, staff_length = strings + 1;    // Default number of strings is 6
    int titles = 6;
    
    char title_window_labels[titles][LENGTH];
    //char proj_title[LENGTH];
    //char tuning[LENGTH];
    //char author[LENGTH];
    //char   song[LENGTH];
    //char tabbed[LENGTH];
    //char  email[LENGTH];

    for (i = 0; i < titles; i++)
	title_window_labels[i][0] = 0;


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
    header(col, title_window_labels[1], title_window_labels[2], title_window_labels[3], // <--- NEEDS TO PASS WHOLE LABEL ARRAY
		title_window_labels[4], title_window_labels[5]);

    // Staffs
    int printrow = HEADER_BUFFER; // <-- SHOULD PROBABLY PUT THIS IN FUNCTION
    while (printrow < row) {
        staff(strings, printrow, col, tuning);
        printrow += staff_length;
    }
    refresh();

    // Title Window
    
    int startx = (col - TITLE_WINDOW_WIDTH) / 2;
    int starty = (row - TITLE_WINDOW_HEIGHT) / 2;
    
    title_win = title_info_win(TITLE_WINDOW_HEIGHT, TITLE_WINDOW_WIDTH, starty, startx, strings);
    keypad(title_win, true);

    int x_mins[8] = {
                        WIN_X_BUFFER + strlen("Project Title: "),
                        WIN_X_BUFFER + strlen("Song Title: "),
                        WIN_X_BUFFER + strlen("Artist:     "),
                        WIN_X_BUFFER + strlen("Tabbed by:  "),
                        WIN_X_BUFFER + strlen("Email:      "),
                        WIN_X_BUFFER + strlen("Number of Strings: "),
                        WIN_X_BUFFER + strlen("Tuning: (1)"),
                        width - WIN_X_BUFFER - strlen("OK") - strlen("CANCEL") - 9,
                    };
    
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
                            WIN_X_BUFFER + strlen("Tuning: (1)") + 6,
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

    while (1) {
        ch = wgetch(title_win);
        getyx(title_win, y, x);
        cur_x = x;
        if (ch ==  KEY_UP) {
            if (pos != 0) {   // Not at top input label
                --pos;
                wmove(title_win, movements[pos][0], movements[pos][1]);
            }

        } else if (ch == KEY_DOWN) {
            if (pos != 7) {   // Not at bottom input label
                ++pos;
                wmove(title_win, movements[pos][0], movements[pos][1]);
            }

        } else if (ch == KEY_LEFT) {
            if (x > movements[pos][1] && x <= width - 2 * WIN_X_BUFFER) {
                if (pos != 5 && pos != 6 && pos != 7) {
                    --cur_x;
                    wmove(title_win, movements[pos][0], cur_x);
                } else if (pos == 6) {
                    if (x == tuning_moves[cur_tune]) {
                        --cur_tune;
                        wmove(title_win, movements[pos][0], tuning_moves[cur_tune] + 1);
                    } else if (x == tuning_moves[cur_tune] + 1) {
                        --cur_x;
                        wmove(title_win, movements[pos][0], cur_x);
                    }
                } else if (pos == 7) {
                    if (x == x_mins[pos] + BUTTON_WIDTH) {
                        cur_x -= BUTTON_WIDTH;
                        wmove(title_win, movements[pos][0], cur_x);
                    }
                }
            }

        } else if (ch == KEY_RIGHT) {
            if (x >= movements[pos][1] && x < width - 2 * WIN_X_BUFFER) {
                if (pos != 5 && pos != 6 && pos != 7) {
                    ++cur_x;
                    wmove(title_win, movements[pos][0], cur_x);
                } else if (pos == 6 && x < tuning_moves[strings-1] + 1) {
                    if (x == tuning_moves[cur_tune]) {
                        ++cur_x;
                        wmove(title_win, movements[pos][0], cur_x);
                    } else if (x == tuning_moves[cur_tune] + 1) {
                        ++cur_tune;
                        wmove(title_win, movements[pos][0], tuning_moves[cur_tune]);
                    }
                } else if (pos == 7) {
                    if (x == x_mins[pos]) {
                        cur_x += BUTTON_WIDTH;
                        wmove(title_win, movements[pos][0], cur_x);
                    }
                }
            }

	// NEED TO RENAME ALL LABEL ARRAYS
        } else if (((ch >= 'a'&& ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
                    (ch >= 33 && ch <= 46)) && x < width - 2 * WIN_X_BUFFER) {   // ch is a letter, number or special char
            // Enter chars into char arrays
            if (pos == 0) {
                proj_title[strlen(proj_title) - 1] = ch;
                proj_title[strlen(proj_title)] = '\0';
            } else if (pos == 1) {
                song[strlen(song)-1] = ch;
                song[strlen(song)] = '\0';
            } else if (pos == 2) {
                author[strlen(author)-1] = ch;
                author[strlen(author)] = '\0';
            } else if (pos == 3) {
                tabbed[strlen(tabbed)-1] = ch;
                tabbed[strlen(tabbed)] = '\0';
            } else if (pos == 4) {
                email[strlen(email)-1] = ch;
                email[strlen(email)] = '\0';
            } else if (pos == 5 && (ch >= '4' && ch <= '8') && x == x_mins[pos]) {
                strings = ch - '0';
                //destroy_win(title_win);
                //title_win = title_info_win(height, width, starty, startx, strings);
            } else if ((pos == 6) && (ch == 'b' || (ch >= 'A' && ch <= 'G') || ch == '#')) {
                tuning[strlen(tuning)-1] = ch;
                tuning[strlen(tuning)] = '\0';
            }
            
            // Movements
            if (pos == 5 && (ch >= '4' && ch <= '8')) {
                mvwprintw(title_win, movements[pos][0], x_mins[pos], "%c", ch);
                wmove(title_win, movements[pos][0], x_mins[pos]);
            } else if (pos == 6  && ((x == tuning_moves[cur_tune] && ch >= 'A' && ch <= 'G') ||
                                     (x == tuning_moves[cur_tune] + 1 && (ch == 'b' || ch == '#')))) {
                mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", ch);
                if (cur_tune < strings-1) {
                    if (x == tuning_moves[cur_tune] + 1) {
                        ++cur_tune;
                        movements[pos][1] = tuning_moves[cur_tune];
                    } else
                        ++movements[pos][1];
                    wmove(title_win, movements[pos][0], movements[pos][1]);
                } else if (x == tuning_moves[strings - 1] || x == tuning_moves[strings-1] + 1) {
                    ++movements[pos][1];
                    wmove(title_win, movements[pos][0], movements[pos][1]);
                }
            } else if (pos != 5 && pos != 6) {
                mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", ch);
                ++movements[pos][1];
                wmove(title_win, movements[pos][0], movements[pos][1]);
            }

        } else if (ch == 127 || ch == 8) {
            if (x > x_mins[pos]) {
                // Delete chars from arrays
                if (pos == 0)
                    proj_title[strlen(proj_title)-1] = '\0';
                else if (pos == 1)
                    song[strlen(song)-1] = '\0';
                else if (pos == 2)
                    author[strlen(author)-1] = '\0';
                else if (pos == 3)
                    tabbed[strlen(tabbed)-1] = '\0';
                else if (pos == 4)
                    email[strlen(email)-1] = '\0';
                else if (pos == 5) {
                    strings = 6;
                    //destroy_win(title_win);
                    //title_win = title_info_win(height, width, starty, startx, strings);
                } else if (pos == 6)
                    tuning[strlen(tuning)-1] = '\0';

                // Movements
                if (pos == 6) {
                    if (x == tuning_moves[cur_tune] + 1 || x == tuning_moves[strings-1] + 2) {
                        --movements[pos][1];
                        mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
                        wmove(title_win, movements[pos][0], movements[pos][1]);   
                    } else if (x == tuning_moves[cur_tune]) {
                        --cur_tune;
                        movements[pos][1] = tuning_moves[cur_tune] + 1;
                        mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
                        wmove(title_win, movements[pos][0], movements[pos][1]);   
                    }
                } else if (pos != 5 && pos != 6){
                    --movements[pos][1];
                    mvwprintw(title_win, movements[pos][0], movements[pos][1], "%c", '_');
                    wmove(title_win, movements[pos][0], movements[pos][1]);
                }
            } else if (pos == 5) {
                mvwprintw(title_win, movements[pos][0], x_mins[pos], "%c", '_');
                wmove(title_win, movements[pos][0], x_mins[pos]);
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
