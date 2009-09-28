;;;
; Copyright (C) 2009  huys, huys03@gmail.com
;
; This program is free software: you can redistribute it and;or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.


;;;
(define list-position
  (lambda (o l)
    (let loop ((i 0) (l l))
      (if (null? l) #f
        (if
          (and (>= o (car l)) (<= o (car (cdr l))))
          i
          (loop (+ i 1) (cdr l))
        )
      )
    )
  )
)

;;
(define interplate
  (lambda (x1 x2 y1 y2 x)
    (let ((i 0))
      (+ y1
        (*
          (- y2 y1)
          (/
            (- x x1)
            (- x2 x1)
          )
        )
      )
    )
  )
)

;;
(define search-interplate
  (lambda (ox lx ly)
    (let ((i (list-position ox lx)))
      (interplate
        (list-ref lx i)
        (list-ref lx (+ i 1))
        (list-ref ly i)
        (list-ref ly (+ i 1))
        ox
      )
    )
  )
)

;; The function for reading the report file
(define (read-report-file filename)
  (let
    ((p (open-input-file (format #f "~a" filename))))
    (do
      ((x (read p) (read p)))
      (
        (or (number? x) (eof-object? x))
        (close-input-port p)
        (if (number? x) x #f)
      )
    )
  )
)

;;; For test
(display "=========================")
(newline)
(display " Enter Testing: proc.scm")
(newline)
(display "=========================")
(newline)

(define pos (list-position 2 (list 1.0 2.2 3.0)))
(define val (interplate 1 2 2 3 1.5))
(display val)

(define pos_val (search-interplate 1.5 (list 1 2 3) (list 2 3 4)))

(display pos)

(newline)

(display pos_val)

(newline)

;(exit)
(display "=========================")
(newline)
(display " Exit Testing: proc.scm")
(newline)
(display "=========================")
(newline)
