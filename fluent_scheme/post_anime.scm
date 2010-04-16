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

(define (hy-create-plane-surface-x name pos)
  (ti-menu-load-string
    (format #f "/surface/plane ~a ~a 0 0 ~a 1 0 ~a 0 1" name pos pos pos)
  )
)

(define (hy-create-plane-surface-z name pos)
  (ti-menu-load-string
    (format #f "/surface/plane ~a 0 0 ~a 0 1 ~a 1 0 ~a" name pos pos pos)
  )
)

(define (hy-export-surface-tecplot-1 fname surface)
  (ti-menu-load-string
    (format #f "/file/export/tecplot ~a ~a () temperature ()" fname surface)
  )
)

(define (hy-export-surface-tecplot fname surface1 surface2 surface3 surface4 surface5)
  (ti-menu-load-string
    (format #f "/file/export/tecplot ~a ~a ~a ~a ~a ~a () temperature ()" fname surface1 surface2 surface3 surface4 surface5)
  )
)

(define (hy-do-export-data)
  (do ((i  0 (+ i 10)))
    ((> i 1000))
    (begin
      (newline)
      (read-data (format #f "cond3d_volume_mesh_ccc-~04d" i))
      (hy-export-surface-tecplot (format #f "face_t_~04d" i) "hy-plane-z" "hy-plane-x-000" "hy-plane-x-007" "hy-plane-x-018" "hy-plane-x-024")
    )
  )
)

(define (hy-do-export-data-outside)
  (do ((i  0 (+ i 10)))
    ((> i 1000))
    (begin
      (newline)
      (read-data (format #f "cond3d_volume_mesh_ccc-~04d" i))
      (hy-export-surface-tecplot-1 (format #f "outside_t_~04d" i) "outside_w")
    )
  )
)

(hy-create-plane-surface-x "hy-plane-x-000" 0.00)
(hy-create-plane-surface-x "hy-plane-x-007" 0.07)
(hy-create-plane-surface-x "hy-plane-x-018" 0.18)
(hy-create-plane-surface-x "hy-plane-x-024" 0.24)
(hy-create-plane-surface-z "hy-plane-z" 0)
(hy-do-export-data)
(hy-do-export-data-outside)
