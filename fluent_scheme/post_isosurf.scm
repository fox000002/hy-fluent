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

;/surface> iso-surface
;
;iso-surface of> x-coordinate
;new surface id/name [x-coordinate-23] x=0.1
;()
;from surface(1) [()] bottom_channel_b-shadow
;from surface(2) [()]
;#f
;from zone(1) [()] fl
;from zone(2) [()]
;range [0, 0.36025998]
;()
;iso-value(1) (m) [()] 0.1
;iso-value(2) (m) [()]

(define hy_from_zone 'f_v)
(define hy_from_surface 'bottom_channel_b-shadow)
(define hy_variables
   '(
      temperature heat-transfer-coef-wall heat-flux
    )
)
(define hy_filenames
  '(t h q)
)

(define hy_prefix 'x)
(define hy_delta_x 10)


;; /surface/iso-surface x-coordinate temperature_000 bottom_channel_b-shadow () f_v ()range [0, 0.36025998] 0
(define (hy-create-iso-surface-x name from-surf from-zone pos)
  (ti-menu-load-string
    (format #f "/surface/iso-surface x-coordinate ~a ~a () ~a () ~a ()" name from-surf from-zone pos)
  )
)

(define (hy-export-report-favg fname surfname varname)
  (ti-menu-load-string
    (format #f "/report/surface-integrals/area-weighted-avg ~a () ~a yes ~a"
      surfname
      varname
      fname
    )
  )
)


(define (hy-do-export-regcool-iso-surf varname)
  (do ((i  0 (+ i hy_delta_x)))
    ((> i 360))
    (begin
      (newline)
      (if (valid-surf-name? (format #f "~a_~03d" hy_prefix i))
        (hy-create-iso-surface-x (format #f "~a_~03d" prefix i) hy_from_surface hy_from_zone (/ i 1000))
      )
      (hy-export-report-favg (format #f "~a_favg~03d.srp" varname i) (format #f "~a_~03d" hy_prefix i) varname)
    )
  )
)



(for-each
  hy-do-export-regcool-iso-surf
  hy_variables
)

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


(define (hy-srp-to-dat varname fname)
  (let
    ((p (open-output-file (format #f "~a.dat" fname))))
    (do ((i  0 (+ i hy_delta_x))(x 0.0)(y 0.0))
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
  hy_variables
  hy_filenames
)

(if (not (file-directory? "srp"))
  (system "mkdir srp")
)

(system "mv *.srp ./srp")



