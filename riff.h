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
#define BUTTON_WIDTH 8          /* Space between the OK and CANCEL buttons */

int tuning_length(char[]);
void header(int);
void measure(int, int);
void staff(int, int);
WINDOW* title_info_win(int, int, int, int);
void destroy_win(WINDOW *);
void print_big_text(char[], int, int);
int text_length(char[]);
