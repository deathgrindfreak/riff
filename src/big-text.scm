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


(declare (unit big-text))
(use ncurses)


;;; Vector of lists for alphabetic characters
(define alpha
  (vector '("____" "|__|" "|  |") '("___ " "|__]" "|__]") '("____" "|   " "|___")
          '("___ " "|  \\" "|__/") '("____" "|___" "|___") '("____" "|___" "|   ")
          '("____" "| __" "|__]") '("_  _" "|__|" "|  |") '("_" "|" "|")
          '(" _" " |" "_|") '("_  _" "|_/ " "| \\_") '("_   " "|   " "|___")
          '("_  _","|\\/|" "|  |") '("_  _" "|\\ |" "| \\|") '("____" "|  |" "|__|")
          '("___ " "|__]" "|   ") '("____" "|  |" "|_\\|") '("____" "|__/" "|  \\")
          '("____" "[__ " "___]") '("___" " | " " | ") '("_  _" "|  |" "|__|")
          '("_  _" "|  |" " \\/ ") '("_ _ _" "| | |" "|_|_|") '("_  _" " \\/ " "_/\\_")
          '("_   _" " \\_/ " "  |  ") '("___ " "  / " " /__")))


;;; Vector of lists for alphabetic characters
(define numbers
  (vector '(" _," "|.|" "|_|") '(" , " "/| " ".|.") '(" _," "'_)" "/_.")
          '(" _," "'_)" "._)") '(". ," "|_|" "  |") '("._," "|_ " "._)")
          '("._," "(_ " "(_)") '("__," " / " "/  ") '(" _," "(_)" "(_)")
          '(" _," "(_)" "  |")))


;;; Special Characters (define exclamation '("|" "|" "."))
(define hyphen '("   " "---" "   "))
(define exclamation '("|" "|" "."))
(define underscore '("   " "   " "___"))
(define question '("__ " " _]" " . "))
(define ampersand '("[] " "/\\/" "\\_\\"))
(define colen '(" " "." "."))
(define semicolen '("." "," " "))
(define mult '("\\|/" "-|-" "/|\\"))
(define dollar '("__|__" "[_|_ " "__|_]"))
(define percent '("0 /" " / " "/ 0"))
(define caret '("/\\" "  " "  "))
(define left_parens '("/" "|" "\\"))
(define right_parens'("\\" "|" "/"))
(define plus '(" | " "---" " | "))
(define equals '("___" "---" "   "))
(define hash-sign '("_|_|_" "_|_|_" " | | "))
(define apostraphe '("\'" " " " "))
(define double_apostraphe '("\"" " " " "))
(define space '(" " " " " "))


;;; Convert char to big-char
(define (char->big-char char)
  (let ((ch (char-downcase char)))
    (cond
     ((char=? ch #\!) exclamation) 
     ((char=? ch #\-) hyphen)
     ((char=? ch #\_) underscore)
     ((char=? ch #\?) question)
     ((char=? ch #\&) ampersand)
     ((char=? ch #\:) colen)
     ((char=? ch #\;) semicolen)
     ((char=? ch #\*) mult)
     ((char=? ch #\$) dollar)
     ((char=? ch #\%) percent)
     ((char=? ch #\^) caret)
     ((char=? ch #\() left_parens)
     ((char=? ch #\)) right_parens)
     ((char=? ch #\+) plus)
     ((char=? ch #\=) equals)
     ((char=? ch #\#) hash-sign)
     ((char=? ch #\') apostraphe)
     ((char=? ch #\") double_apostraphe)
     ((char-alphabetic? ch) (let ((base (- (char->integer ch)
                                           (char->integer #\a))))
                              (vector-ref alpha base)))
     ((char-numeric? ch) (let ((base (- (char->integer ch)
                                        (char->integer #\0))))
                           (vector-ref numbers base)))
     (else space))))


;;; Converts a string to a list of big chars
(define (big-text-string text)
  (let ((char-list (string->list text)))
    (map (lambda (ch) (char->big-char ch)) char-list)))


;;; Prints a string of big charactes
(define (print-big-string str row col)
  (let ([str-lst (big-text-string str)])
    (let ([first-row (foldr (lambda (x y) (string-append (car x) " " y)) "" str-lst)]
          [second-row (foldr (lambda (x y) (string-append (cadr x) " " y)) "" str-lst)]
          [third-row (foldr (lambda (x y) (string-append (caddr x) " " y)) "" str-lst)])
      (mvprintw row col "~a " first-row)
      (mvprintw (+ row 1) col "~a " second-row)
      (mvprintw (+ row 2) col "~a " third-row))))


;;; Returns the width of a big string
(define (big-string-length str)
  (let ([big-str (big-text-string str)])
    (- (foldr + 0
              (map (lambda (ch) (+ (string-length (car ch))
                              1))
                   big-str))
       1)))
