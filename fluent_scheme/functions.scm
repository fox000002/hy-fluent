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
(define factorial
  (lambda (n)
    (let fact ((i n))
      (if (= i 0)
        1
        (* i (fact (- i 1)))
      )
    )
  )
)

(define factorial-it
  (lambda (n)
    (let fact ((i n) (a 1))
      (if (= i 0)
        a
        (fact (- i 1) (* a i))
      )
    )
  )
)

(define abs-all
  (lambda (l)
    (if (null? l)
      '()
      (cons (abs (car ls)) (abs-all (cdr ls)))
    )
  )
)

;;;
(define display-file-content
  (lambda (fname)
    (let ((p (open-input-file (format #f "~a" fname))))
      (let f ((x (read-line p))) 
        (if (eof? p)
          (begin
            (close-input-port p)
          )
          (begin
            (display x)
            (newline)
            (f (read-line p))
          )
        )
      )
    )
  )
)


;;;
(define hy-read-data-file
  (lambda (fname)
    (let ((p (open-input-file (format #f "~a" fname))))
      (let f ((x (read p))) 
        (if (eof? p)
          (begin
            (close-input-port p)
          )
          (begin
            (if (number? x)
              (begin
                (display x)
                (newline)
              )
            )
            (f (read p))
          )
        )
      )
    )
  )
)

;;;
(define hy-read-data-to-list
  (lambda (fname)
    (let ((p (open-input-file (format #f "~a" fname))))
      (let f ((x (read p)) (l '())) 
        (if (or (eof? p) (eof-object? x))
          (begin
            (close-input-port p)
            l
          )
          (begin
            (if (number? x)
                (set! l (append l (list x)))
            )
            (f (read p) l)
          )
        )
      )
    )
  )
)

;;;
(define hy-display-list
  (lambda (l)
    (do ((i  0 (+ i 1)))
      ((>= i (length l)))
      (display (format #f "~a --- ~a\n" i (list-ref l i)))
    )
  )
)