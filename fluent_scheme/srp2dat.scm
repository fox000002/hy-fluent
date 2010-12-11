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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; The function for reading the report file
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (hy-srp-to-dat varname fname)
  (let 
    ((p (open-output-file (format #f "~a.dat" fname))))
    (do ((i  0 (+ i 10))(x 0.0)(y 0.0))
      ((> i 360))
      (begin
        (newline)
        (set! x (/ i 1000))
        (set! y (read-report-file (format #f "~a_favg~03d.srp" varname i)))
        (display (format #f "~a     ~a" x  y) p)
        (newline p)
      )
    )
    (close-input-port p)
    #t
  )
)

(for-each
  hy-srp-to-dat
  ;;'(
  ;;    density total-pressure total-temperature gamma mach-number mean-molecular-weight
      ;total-pressure total-temperature gamma mach-number mean-molecular-weight
  ;;)
  ;;'(
   ;;   density pressure temperature gamma ma molecular_weight
 ;;)
  '(
      temperature heat-transfer-coef-wall heat-flux
      ;total-pressure total-temperature gamma mach-number mean-molecular-weight
  )
  '(
      t h q 
  )
)

