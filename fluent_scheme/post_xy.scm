(define hy-read-xyplot-file
  (lambda (fname)
    (let ((p (open-input-file (format #f "~a" fname))) (l '()))
      (begin
        (read p)
        (read p)
        (set! l (cdr (read p)))
        (close-input-port p)
        l
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

(define hy-process-data-vector
  (lambda (v fname)
    (let ((p (open-output-file (format #f "~a" fname)))(j 0)(x 0.0)(y 0.0))
      (begin
        (do ((i  0 (+ i 2)))
          ((>= i (- (vector-length v) 1)))
          (begin
            (if (< y 1)
              (begin
                (set! x (vector-ref v i))
                (set! y (vector-ref v (+ i 1)))
              )
              (if (> (abs (- (vector-ref v i) x)) 0.0001)
                (begin
                  (set! x (vector-ref v i))
                  (set! y (vector-ref v (+ i 1)))
                  (display (format #f "~8a  ~8a" x y)  p)
                  (newline p)
                )
                (begin
                  (set! y
                    (/ 
                      (+
                        (vector-ref v (+ i 1))
                        y
                      )
                      2
                    )
                  )
                )
              )
            )
          
          )
        )
      )
      (close-input-port p)
      #t
    )
  )
)

(define hy-process-data-list
  (lambda (l fname)
    (hy-process-data-vector (list->vector l) fname)
  )
)

;;; Get rid of data  < 1e4
(define hy-clear-data-vector
  (lambda (v fname)
    (let ((p (open-output-file (format #f "~a" fname))) (x 0.0) (y 0.0))
      (do ((i  0 (+ i 2)))
        ((>= i (- (vector-length v) 1)))
        (begin
          (set! x (vector-ref v i))
          (set! y (vector-ref v (+ i 1)))
          (display y)
          (if (< 1.e4 y)
            (begin
              (display (format #f "~8a  ~8a" x y)  p)
              (newline p)
            )
          )
        )
      )
      (close-input-port p)
      #t
    )
  )
)
