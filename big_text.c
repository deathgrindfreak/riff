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

#include "riff.h"

char letters[26][3][6] = {{"____", 
                           "|__|", 
                           "|  |"},
                          
                          {"___ ",
                           "|__]",
                           "|__]"},
                          
                          {"____",
                           "|   ",
                           "|___"},
                          
                          {"___ ",
                           "|  \\",
                           "|__/"},
                          
                          {"____",
                           "|___",
                           "|___"},
                          
                          {"____",
                           "|___",
                           "|   "},
                          
                          {"____",
                           "| __",
                           "|__]"},
                          
                          {"_  _",
                           "|__|",
                           "|  |"},
                          
                          {"_", 
                           "|", 
                           "|"},
                          
                          {" _", 
                           " |", 
                           "_|"},
                                
                          {"_  _", 
                           "|_/ ", 
                           "| \\_"},
                          
                          {"_   ", 
                           "|   ", 
                           "|___"},
                          
                          {"_  _",
                           "|\\/|",
                           "|  |"},
                          
                          {"_  _", 
                           "|\\ |", 
                           "| \\|"},
                          
                          {"____", 
                           "|  |", 
                           "|__|"},
                          
                          {"___ ", 
                           "|__]", 
                           "|   "},
                          
                          {"____", 
                           "|  |", 
                           "|_\\|"},
                          
                          {"____", 
                           "|__/", 
                           "|  \\"},
                          
                          {"____", 
                           "[__ ", 
                           "___]"},
                          
                          {"___", 
                           " | ", 
                           " | "},
                          
                          {"_  _", 
                           "|  |", 
                           "|__|"},
                          
                          {"_  _", 
                           "|  |", 
                           " \\/ "},
                          
                          {"_ _ _", 
                           "| | |",
                           "|_|_|"},
                          
                          {"_  _", 
                           " \\/ ", 
                           "_/\\_"},
                          
                          {"_   _", 
                           " \\_/ ", 
                           "  |  "},
                          
                          {"___ ", 
                           "  / ", 
                           " /__"}};


char numbers[10][3][4] = {{" _,",
                           "|.|",
                           "|_|"},

                          {" , ", 
                           "/| ", 
                           ".|."},
 
                          {" _,", 
                           "'_)", 
                           "/_."},
                          
                          {" _,", 
                           "'_)", 
                           "._)"},
                          
                          {". ,", 
                           "|_|", 
                           "  |"},
                          
                          {"._,",
                           "|_ ",  
                           "._)"},
                          
                          {"._,", 
                           "(_ ", 
                           "(_)"},
                          
                          {"__,",  
                           " / ",  
                           "/  "},
                                                                   
                          {" _,", 
                           "(_)", 
                           "(_)"},
                          
                          {" _,", 
                           "(_)", 
                           "  |"}};


// Special Characters
char exclamation[3][2] = {"|",
                          "|",
                          "."};

char hyphen[3][4] = {"   ",
                     "---",
                     "   "};

char underscore[3][4] = {"   ",
                         "   ",
                         "___"};

char question[3][4] = {"__ ",
                       " _]",
                       " . "};

char ampersand[3][4] = {"[] ",
                        "/\\/",
                        "\\_\\"};

char colen[3][2] = {" ",
                    ".",
                    "."};

char semicolen[3][2] = {".",
                        ",",
                        " "};

char mult[3][4] = {"\\|/",
                   "-|-",
                   "/|\\"};

char dollar[3][6] = {"__|__",
                     "[_|_ ",
                     "__|_]"};

char percent[3][4] = {"0 /",
                      " / ",
                      "/ 0"};

char caret[3][3] = {"/\\",
                    "  ",
                    "  "};

char left_parens[3][2] = {"/",
                          "|",
                          "\\"};

char right_parens[3][2] = {"\\",
                           "|",
                           "/"};

char plus[3][4] = {" | ",
                   "---",
                   " | "};

char equals[3][4] = {"___",
                     "---",
                     "   "};

char hash[3][6] = {"_|_|_",
                   "_|_|_",
                   " | | "};

char apostraphe[3][2] = {"\'",
                         " ",
                         " "};

char double_apostraphe[3][2] = {"\"",
                                " ",
                                " "};


/* print_big_text - prints a big text version of a string */
void print_big_text(char input[], int row, int col) {

    int i, k, len;
    for (i = 0; i < strlen(input); i++) {   // for each letter in input

        // Whitespace
        if (input[i] == ' ') {
            col += 2;

        // Letters
        } else if (input[i] >= 'a' && input[i] <= 'z') {

            len = strlen(letters[input[i] - 'a'][0]);
            
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", letters[input[i] - 'a'][k]);
            }
            col += len + 1;

        } else if (input[i] >= 'A' && input[i] <= 'Z') {

            len = strlen(letters[input[i] - 'A'][0]);

            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", letters[input[i] - 'A'][k]);
            }
            col += len + 1;

        // Numbers
        } else if (input[i] >= '0' && input[i] <= '9') {

            len = strlen(numbers[input[i] - '0'][0]);

            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", numbers[input[i] - '0'][k]);
            }
            col += len + 1;

        // Special Characters
        } else if (input[i] == '!') {
            len = strlen(exclamation[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", exclamation[k]);
            }
            col += len + 1;
        } else if (input[i] == '#') {
            len = strlen(hash[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", hash[k]);
            }
            col += len + 1;
        } else if (input[i] == '$') {
            len = strlen(dollar[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", dollar[k]);
            }
            col += len + 1;
        } else if (input[i] == '%') {
            len = strlen(percent[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", percent[k]);
            }
            col += len + 1;
        } else if (input[i] == '^') {
            len = strlen(caret[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", caret[k]);
            }
            col += len + 1;
        } else if (input[i] == '&') {
            len = strlen(ampersand[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", ampersand[k]);
            }
            col += len + 1;
        } else if (input[i] == '*') {
            len = strlen(mult[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", mult[k]);
            }
            col += len + 1;
        } else if (input[i] == '(') {
            len = strlen(left_parens[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", left_parens[k]);
            }
            col += len + 1;
        } else if (input[i] == ')') {
            len = strlen(right_parens[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", right_parens[k]);
            }
            col += len + 1;
        } else if (input[i] == '-') {
            len = strlen(hyphen[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", hyphen[k]);
            }
            col += len + 1;
        } else if (input[i] == '_') {
            len = strlen(underscore[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", underscore[k]);
            }
            col += len + 1;
        } else if (input[i] == '+') {
            len = strlen(plus[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", plus[k]);
            }
            col += len + 1;
        } else if (input[i] == '=') {
            len = strlen(equals[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", equals[k]);
            }
            col += len + 1;
        } else if (input[i] == ':') {
            len = strlen(colen[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", colen[k]);
            }
            col += len + 1;
        } else if (input[i] == ';') {
            len = strlen(semicolen[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", semicolen[k]);
            }
            col += len + 1;
        } else if (input[i] == '\'') {
            len = strlen(apostraphe[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", apostraphe[k]);
            }
            col += len + 1;
        } else if (input[i] == '\"') {
            len = strlen(double_apostraphe[0]);
            for (k = 0; k < 3; k++) {
                mvprintw(row + k, col, "%s", double_apostraphe[k]);
            }
            col += len + 1;
        } else {
            continue;   // don't print illegal chars
        }
    }
}


/* text_length - returns the total length of the printed big text */
int text_length(char input[]) {

    int i, len = 0;
    for (i = 0; i < strlen(input); i++) {   // for each letter in input
        if (input[i] == ' ') {
            ++len;
        } else if (input[i] >= 'a' && input[i] <= 'z') {
            len += strlen(letters[input[i] - 'a'][0]);
        } else if (input[i] >= 'A' && input[i] <= 'Z') {
            len += strlen(letters[input[i] - 'A'][0]);
        } else if (input[i] >= '0' && input[i] <= '9') {
            len += strlen(numbers[input[i] - '0'][0]);
        } else if (input[i] == '!') {
            len += strlen(exclamation[0]);
        } else if (input[i] == '#') {
            len += strlen(hash[0]);
        } else if (input[i] == '$') {
            len += strlen(dollar[0]);
        } else if (input[i] == '%') {
            len += strlen(percent[0]);
        } else if (input[i] == '^') {
            len += strlen(caret[0]);
        } else if (input[i] == '&') {
            len += strlen(ampersand[0]);
        } else if (input[i] == '*') {
            len += strlen(mult[0]);
        } else if (input[i] == '(') {
            len += strlen(left_parens[0]);
        } else if (input[i] == ')') {
            len += strlen(right_parens[0]);
        } else if (input[i] == '-') {
            len += strlen(hyphen[0]);
        } else if (input[i] == '_') {
            len += strlen(underscore[0]);
        } else if (input[i] == '+') {
            len += strlen(plus[0]);
        } else if (input[i] == '=') {
            len += strlen(equals[0]);
        } else if (input[i] == ':') {
            len += strlen(colen[0]);
        } else if (input[i] == ';') {
            len += strlen(semicolen[0]);
        } else if (input[i] == '\'') {
            len += strlen(apostraphe[0]);
        } else if (input[i] == '\"') {
            len += strlen(double_apostraphe[0]);
        } else {
            continue;   // don't print illegal chars
        }
        ++len;  // Count whitespace
    }
    return len;
}

