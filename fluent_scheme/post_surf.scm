;/surface> line-su
;new surface id/name [line-6] 
;x0 (m) [0] 
;y0 (m) [0] 
;x1 (m) [0] 
;y1 (m) [0]

(define (hy-create-line-surface name pos)
  (ti-menu-load-string
    (format #f "/surface/line-surface ~a ~a 0 ~a 1" name pos pos)
  )
  
)

(define (hy-export-xy-plot fname surfname)
  ;; direction y
  (ti-menu-load-string
    (format #f "/plot/plot-direction 0 1")
  )
  (newline)
  (ti-menu-load-string
    (format #f "/plot/plot yes ~a yes no no total-temperature no no y-coordinate ~a" fname surfname)
  )
  (newline)
)

(define (hy-export-report-favg fname surfname)
  (ti-menu-load-string
    (format #f "/report/surface-integrals/facet-avg ~a () total-temperature yes ~a"
      surfname
      fname
    )
  )
)

(do ((i  0 (+ i 5)))
  ((> i 340))
  (begin
    (newline)
    (hy-create-line-surface (format #f "surf_~03d" i) (/ i 1000))
    (hy-export-xy-plot (format #f "th~03d.xy" i) (format #f "surf_~03d" i))
    (hy-export-report-favg (format #f "favg~03d.srp" i) (format #f "surf_~03d" i))
  )
)

