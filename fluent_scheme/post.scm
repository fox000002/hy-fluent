(define casename "20100224_final_ft14000xc")
(define zoneid 7)

;;;;
(define (hy-export-bc-names)
  (for-each
    (lambda (name)
      (display
        (format #f "{~a, \"~a\", \"~a\"},\n"
          (zone-name->id name)
          name
          (zone-type (get-zone name))
        )
      )
    )
    (inquire-zone-names)
  )
)

;/plot>  plot
;node values? [yes]
;filename [""] xxx
;order points? [no] yes
;Y Axis direction vector? [no]
;Y Axis curve length? [no]

;cell function> temperature
;X Axis direction vector? [no]
;X Axis curve length? [no]

;cell function>  y-coordinate
;(7)
;surface id/name(1) [7]
;surface id/name(2) [()]


(define (hy-export-xy-plot fname zone-id)
  ;; direction y
  (ti-menu-load-string
    (format #f "plot/plot-direction 0 1")
  )

  (ti-menu-load-string
    (format #f "plot/plot yes ~a yes no no temperature no no y-coordinate ~a" fname zone-id)
  )
)

;;
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
            (display x)
            (newline)
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

(define hy-write-list-tec-file
  (lambda (fname ll t)
    (let ((p (open-output-file (format #f "~a" fname))))
      (begin
        (display "VARIABLES = \"Time (s)\", \"Y (m)\", \"Temperature\"" p)
        (newline p)
        (display (format #f "ZONE I=~a, J=~a, DATAPACKING=POINT" 2 (/ (length ll) 2)) p)
        (newline p)
        (do ((i  0 (+ i 2)))
          ((>= i (length ll)))
          (begin
            (display (format #f "~8a    ~8a    ~8a" t (list-ref ll i) (list-ref ll (+ i 1))) p)
            (newline p)
            (display (format #f "~8a    ~8a    ~8a" (+ t 400) (list-ref ll i) (list-ref ll (+ i 1))) p)
            (newline p)
          )
        )
        (close-input-port p)
        #t
      )
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;       Test section                ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;(hy-display-list (hy-read-data-to-list "xxx"))

;(hy-display-list (hy-read-xyplot-file "xxx"))
;(hy-write-list-tec-file "xxx.dat" (hy-read-xyplot-file "xxx") 0)

;;; load t=0 case-dat
(read-case-data casename)

;;; export xy data
(hy-export-xy-plot "th0000.xy" zoneid)

(hy-write-list-tec-file
  (format #f "th~04d.dat" 0)
    (hy-read-xyplot-file (format #f "th~04d.xy" 0) 0)
  )
)

(do ((i  20 (+ i 20)) (ctime-step 10))
  ((>= i 1400))
  (begin
    (display (format #f "Time : ~04d" (* i ctime-step)))
    (newline)
    (read-data (format #f "~a-~04d" casename i))
    (hy-export-xy-plot (format #f "th~04d.xy" i) zoneid)
    (hy-write-list-tec-file
      (format #f "th~04d.dat" i)
      (hy-read-xyplot-file (format #f "th~04d.xy" i) (* i ctime-step))
    )
  )
)
