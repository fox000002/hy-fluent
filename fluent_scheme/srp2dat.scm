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

(define pfile (open-output-file "favg.dat"))

(do ((i  0 (+ i 5))(x 0.0)(y 0.0))
  ((> i 340))
  (begin
    (newline)
    (set! x (/ i 1000))
    (set! y (read-report-file (format #f "favg~03d.srp" i)))
    (display (format #f "~a     ~a" x  y) pfile)
    (newline pfile)
  )
)

(close-input-port pfile)
