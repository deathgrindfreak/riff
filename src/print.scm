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
(use ncurses srfi-13)


;;; Global constants
(define HEADER_BUFFER 12)             ; y buffer spacing for header
(define X_BUFFER 2)                   ; x buffer for side spacing
(define Y_BUFFER 2)                   ; y buffer for top spacing
(define TAB_OFFSET 25)                ; offset for the tabbed and email 
(define WIN_HEADER 3)                 ; y buffer spacing for window header
(define WIN_X_BUFFER 2)               ; x buffer spacing for window
(define WIN_Y_BUFFER 2)               ; y buffer spacing for window
(define MIN_WIDTH 70)                 ; Minimum width for terminal
(define MIN_HEIGHT 35)                ; Minimum height for terminal
(define TITLE_WINDOW_WIDTH  60)       ; Title window width
(define TITLE_WINDOW_HEIGHT 29)       ; Title window height
(define BUTTON_WIDTH 8)               ; Space between the OK and CANCEL buttons


;;; Header Fields 
(define strings 6)
(define tuning "EADGBE")
(define song "")
(define artist "")
(define tabbed "")
(define email "")

;;; Window Fields
(define title-welcome "Welcome to Riff!")
(define project "")
(define win-song "")
(define win-artist "")
(define win-tabbed "")
(define win-email "")
(define win-title-strings "")
(define win-tuning "")

;;; Window Labels
(define project-label "Project Title: ")
(define song-label "Song Title: ")
(define artist-label "Artist: ")
(define tabbed-label "Tabbed by: ")
(define email-label "Email: ")
(define title-strings-label "Number of Strings: ")
(define tuning-label "Tuning: ")


;;; empty-string? checks for an empty string
(define (empty-string? str)
  (if (= (string-length str) 0) '#t '#f))


;;; print-header - prints the header at the top of the screen
(define (print-header col)
  (if (empty-string? song)
      (print-big-string "untitled" (- Y_BUFFER 1) (quotient (- col (big-string-length "untitled")) 2))
      (print-big-string song (- Y_BUFFER 1) (quotient (- col (big-string-length song)) 2)))
  (if (empty-string? artist)
      (mvprintw (+ Y_BUFFER 3) (+ (quotient (- col (string-length "Unknown")) 2) 3) "by ~a" "Unknown")
      (mvprintw (+ Y_BUFFER 3) (+ (quotient (- col (string-length artist)) 2) 3) "by ~a" artist))
  (if (empty-string? tabbed)
      (mvprintw (- HEADER_BUFFER 4) (- col TAB_OFFSET) "Tabbed by: ~a" "Unknown")
      (mvprintw (- HEADER_BUFFER 4) (- col TAB_OFFSET) "Tabbed by: ~a" tabbed))
  (if (empty-string? email)
      (mvprintw (- HEADER_BUFFER 3) (- col TAB_OFFSET) "Email: ~a" "Unknown")
      (mvprintw (- HEADER_BUFFER 3) (- col TAB_OFFSET) "Email: ~a" email))
  (mvprintw (- HEADER_BUFFER 3) X_BUFFER "Tuning: ~a" tuning))


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


;;; print-window-underlines - prints the underlines for the window
;;; fields
(define (window-underlines label str)
  (let ([line-length (- TITLE_WINDOW_WIDTH
                        (+ (string-length label)
                           (string-length str)
                           (* 2 WIN_X_BUFFER)))])
    (make-string line-length #\_)))


;;; print-window - prints the window text
(define (print-window win)
  (let ([BUTTON_BUFFER (- TITLE_WINDOW_WIDTH
                           WIN_Y_BUFFER
                           (string-length "OK")
                           (string-length "CANCEL")
                           11)]
        [proj (string-append project-label
                             project
                             (window-underlines project-label project))]
        [song (string-append song-label
                             win-song
                             (window-underlines song-label win-song))]
        [artist (string-append artist-label
                               win-artist
                               (window-underlines artist-label win-artist))]
        [tabbed (string-append tabbed-label
                               win-tabbed
                               (window-underlines tabbed-label win-tabbed))]
        [email (string-append email-label
                              win-email
                              (window-underlines email-label win-email))]
        [title-strings (string-append title-strings-label
                                      win-title-strings)]
        [tuning (string-append tuning-label
                               win-tuning)])
    
    ;; Title
    (wattron win A_BOLD)
    (mvwprintw win
               WIN_Y_BUFFER
               (/ (- TITLE_WINDOW_WIDTH
                     (string-length title-welcome))
                  2)
               "~a" title-welcome)
    (wattroff win A_BOLD)
    
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER)    WIN_X_BUFFER "~a" proj)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 4)  WIN_X_BUFFER "~a" song)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 6)  WIN_X_BUFFER "~a" artist)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 8)  WIN_X_BUFFER "~a" tabbed)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 10) WIN_X_BUFFER "~a" email)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 14) WIN_X_BUFFER "~a" title-strings)
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 16) WIN_X_BUFFER "~a" tuning)

    ;; Buttons
    (wattron win (bitwise-ior A_REVERSE A_BOLD))
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 19) BUTTON_BUFFER "~a" "      ")
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 20) BUTTON_BUFFER "~a" "  OK  ")
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 21) BUTTON_BUFFER "~a" "      ")
    
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 19) (+ BUTTON_BUFFER 8) "~a" "          ")
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 20) (+ BUTTON_BUFFER 8) "~a" "  CANCEL  ")
    (mvwprintw win (+ WIN_Y_BUFFER WIN_HEADER 21) (+ BUTTON_BUFFER 8) "~a" "          ")
    (wattroff win (bitwise-ior A_REVERSE A_BOLD))))
