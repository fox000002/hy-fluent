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

;/surface> plane
;new surface id/name [plane-0]
;x0 (m) [0]
;y0 (m) [0]
;z0 (m) [0]
;x1 (m) [0]
;y1 (m) [0]
;z1 (m) [0]
;x2 (m) [0]
;y2 (m) [0]
;z2 (m) [0]
(define (hy-create-plane-surface-x name pos)
  (ti-menu-load-string
    (format #f "/surface/plane ~a ~a 0 0 ~a 1 0 ~a 0 1" name pos pos pos)
  )
)

(define (hy-create-plane-surface-z name pos)
  (ti-menu-load-string
    (format #f "/surface/plane ~a 0 0 ~a  1 0 ~a  0 1 ~a" name pos pos pos)
  )
)

(define (hy-export-xy-plot fname surfname varname)
  ;; direction y
  (ti-menu-load-string
    (format #f "/plot/plot-direction 0 1")
  )
  (newline)
  (ti-menu-load-string
    ;(format #f "/plot/plot yes ~a yes no no total-temperature no no y-coordinate ~a" fname surfname)
    (format #f "/plot/plot yes ~a yes no no  ~a no no y-coordinate ~a" fname varname surfname)
  )
  (newline)
)

(define (hy-export-report-favg fname surfname varname)
  (ti-menu-load-string
    ;(format #f "/report/surface-integrals/facet-avg ~a () total-temperature yes ~a"
    (format #f "/report/surface-integrals/facet-avg ~a () ~a yes ~a"
      surfname
      varname
      fname
    )
  )
)

(define (hy-do-export-data)
  (do ((i  0 (+ i 5)))
    ((> i 340))
    (begin
      (newline)
      (hy-create-line-surface (format #f "surf_~03d" i) (/ i 1000))
      (hy-export-xy-plot (format #f "th~03d.xy" i) (format #f "surf_~03d" i))
      (hy-export-report-favg (format #f "favg~03d.srp" i) (format #f "surf_~03d" i))
    )
  )
)

(define (hy-do-export-regcool-total varname)
  (do ((i  0 (+ i 1)))
    ((> i 360))
    (begin
      (newline)
      ;(hy-create-plane-surface-z (format #f "surf_~03d" i) (/ i 1000))
      ;(hy-export-report-favg (format #f "favg~03d.srp" i) (format #f "surf_~03d" i) "density")
      (hy-export-report-favg (format #f "~a_favg~03d.srp" varname i) (format #f "surf_~03d" i) varname)
    )
  )
)

(for-each
  hy-do-export-regcool-total
  '(
      density total-pressure total-temperature gamma mach-number mean-molecular-weight
  )
)

