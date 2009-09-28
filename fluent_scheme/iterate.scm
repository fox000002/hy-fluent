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

;;
;; iterate.scm
;;
;; The iteration script with transcripting and autosaving.
;;
(define casename "coal_inj_00")
(define iternum  1000)
(define casename-iw
  (format #f "~a-i~a" casename iternum)
)

;
(define hy-start-transcript
  (lambda (filename)
    (if (file-exists? filename)
      (remove-file filename)
      #t
    )
    (ti-start-transcript filename #t)
    #t
  )
)

;;
(define (hy-stop-transcript)
  (if transcript-open?
    (stop-transcript #t)
    #t
  )
  #t
)

;;
(define (log-exec-proc exec-proc trn-file)
  (begin
    (hy-start-transcript trn-file)
    (exec-proc)
    (newline)
    (hy-stop-transcript)
  )
)

;;
(define my-iterate
  (lambda (num)
    (if (number? num)
      (begin
        (read-case-data casename)
        (iterate num)
        (write-case-data casename-iw)
        #t
      )
      #f
    )
  )
)

;;
(log-exec-proc (lambda () (my-iterate iternum)) (string-append casename ".trn"))

