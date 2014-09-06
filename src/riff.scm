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


(declare (uses print big-text))
(use ncurses)


  ;; Global constants
(define MIN_WIDTH 70)        	; Minimum width for terminal
(define MIN_HEIGHT 35)       	; Minimum height for terminal
(define TITLE_WINDOW_WIDTH  60)         ; Title window width
(define TITLE_WINDOW_HEIGHT 29)         ; Title window height


;;; screen-size-too-small - Ensure the terminal is large enough
(define (screen-size-too-small row col)
  (cond
   [(or (< row MIN_HEIGHT) (< col MIN_WIDTH))
    (let ((error_one "Your terminal is too small!")
          (error_two (conc "Resize to at least "
                           (number->string MIN_WIDTH)
                           " X "
                           (number->string MIN_HEIGHT)
                           " and try again.")))
      
      (mvprintw (- (quotient row 2) 1)
                (quotient (- col (string-length error_one)) 2)
                "~a" error_one)
      (mvprintw (quotient row 2)
                (quotient (- col (string-length error_two)) 2)
                "~a" error_two))
    (getch)
    '#t]
   [else #f]))

;;; Init the screen for the program
(define (init-riff)
  (initscr)
  (clear)
  (noecho)
  (cbreak))

;;; Close the program
(define (close-riff win)
  (clear)
  (echo)
  (delwin win)
  (nocbreak)
  (endwin))


(define (main)
  
  ;; Start screen
  (init-riff)

  ;; Set options for stdscr
  (nodelay (stdscr) #t)
  (keypad (stdscr) #t)
  (scrollok (stdscr) #t)
  
  ;; Get the size of the terminal
  (define MAX_ROWS (LINES))
  (define MAX_COLS (COLS))

  ;; Check if terminal has color
  (when (has_colors)
        (start_color))

  ;; Create the title window
  (define startx (quotient (- MAX_COLS TITLE_WINDOW_WIDTH) 2))
  (define starty (quotient (- MAX_ROWS TITLE_WINDOW_HEIGHT) 2))
  (define title-win (newwin TITLE_WINDOW_HEIGHT TITLE_WINDOW_WIDTH starty startx))
  (box title-win 0 0)

  (print-header MAX_COLS)
  (print-multiple-staffs 4 MAX_COLS)
  (print-window title-win)
  (refresh)
  (wrefresh title-win)

  
  ;; Main program loop
  (let loop ()
    (unless (screen-size-too-small MAX_ROWS MAX_COLS)
      (unless (= (char->integer (getch)) 27)

        (loop))))

  ;; Close the program
  (close-riff title-win))
  
(main)
  #|
  FIELD  *fields[14];


  int main(int argc, char *argv[])
  {

  // Initial values
  WINDOW *title_win;  // Title window pointer
  FORM   *title_form;


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
  fields[5] = new_field(1, 1                                                , WIN_Y_BUFFER + WIN_HEADER + 14, x_mins[5], 0, 0); 

  int i;
  for (i = 0; i < strings; i++)
  fields[6 + i]  = new_field(1, 2, WIN_Y_BUFFER + WIN_HEADER + 16, x_mins[6] + i * 6, 0, 0);

  for (i = 0; i < 8 - strings; i++)
  fields[6 + strings + i] = NULL;
  
  for (i = 0; i < 12; i++) {  // Needs to be able to change with string change
  set_field_back(fields[i], A_UNDERLINE);
  field_opts_off(fields[i], O_AUTOSKIP);
  }

  
  /* Create the title window form */
  title_form = new_form(fields);
  set_form_win(title_form, title_win);
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

  int y, x;
  char ch;

  wmove(title_win, movements[0][0], movements[0][1]);
  wrefresh(title_win);

  while (true) {
  ch = wgetch(title_win);
  getyx(title_win, y, x);
  if (ch ==  KEY_UP) {
  if (y == movements[7][0])
  move(movements[6][0], movements[6][1]);
  else if (y == movements[6][0] && x == movements[6][1]) {
  form_driver(title_form, REQ_PREV_FIELD);
  form_driver(title_form, REQ_END_LINE);
  } else if (y != movements[7][0] && y != movements[6][0]){
  form_driver(title_form, REQ_PREV_FIELD);
  form_driver(title_form, REQ_END_LINE);
  }
  } else if (ch == KEY_DOWN) {
  if (y == movements[6][0])
  move(movements[7][0], movements[7][1]);
  else if (y != movements[7][0] && y != movements[6][0]){
  form_driver(title_form, REQ_NEXT_FIELD);
  form_driver(title_form, REQ_END_LINE);
  }
  } else if (ch == KEY_LEFT) {
  if (y == movements[6][0]) {
  form_driver(title_form, REQ_PREV_FIELD);
  form_driver(title_form, REQ_END_LINE);
  } else if (y == movements[7][0] && x == (movements[7][1] + BUTTON_WIDTH)){  // Is at CANCEL button
  move(movements[7][0], movements[7][1]);
  }
  } else if (ch == KEY_RIGHT) {
  if (y == movements[6][0]) {
  form_driver(title_form, REQ_NEXT_FIELD);
  form_driver(title_form, REQ_END_LINE);
  } else if (y == movements[7][0] && x == movements[7][1]) {   // Is at OK button
  move(movements[7][0], movements[7][1] + BUTTON_WIDTH);
  }
  } else if (ch == '\n' && y == movements[7][0]) {
  if (x == movements[7][1]) { // OK
  clear();
  print_to_screen(fields);
  refresh();
  unpost_form(title_form);
  free_form(title_form);

  for (i = 0; i < 14; i++)
  free_field(fields[i]);

  getch();
  break;

  } else {    // CANCEL
  clear();
  endwin();
  break;
  }

  /* ch is a letter, number or special char */
  } else if (((ch >= 'a'&& ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
  (ch >= 33 && ch <= 46) || ch == ' ' || ch == 64)) {
  form_driver(title_form, ch);
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
  |#

