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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  panel
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Syntax:
;;
;;  cx-create-panel
;;  cx-show-panel
;;
;;

;;;
;;(rampant-menubar)

;;;
(define hy-panel
  (let ((panel #f)(hy-value))
    ;;
    (define (update-cb)
        (cx-set-text-entry hy-value "Hello")
    )
    ;;
    (define (hello-cb)
      (display  (cx-show-text-entry hy-value))
      (newline)
    )
    (lambda ()
      (if (not panel)
        (let ((table) (form) (tbutton) (tframe) (buttonbox) (tdroplist) (tscale) (tlisttree) (tlist) (tripanel) (tchecks))
          ;
          (set! panel (cx-create-panel "Hello Panel" 'update-callback update-cb 'apply-callback hello-cb))
          ;
          (set! table (cx-create-table panel "Table" 'border #t 'below 0 'right-of 0))
          ;;
          (set! hy-value (cx-create-text-entry table "Text" 'width 14 'row 1 'col 0))
          ;
          (cx-create-integer-entry table "int" 'width 20 'value 22 'minimum 0 'maximum 100 'row 2)
          ;
          (cx-create-real-entry table "int" 'width 20 'value 22.2 'minimum 0 'maximum 100 'row 3)
          ;
          (set! tbutton (cx-create-toggle-button table "tbutton" 'state #t 'row 4 'col 0 'activate-callback hello-cb))
          ;
          ;(set! tframe (cx-create-frame panel "frame" 'tabbed #t 'border #t 'width 20 'below table))
          ;
          ;(set! tdroplist (cx-create-drop-down-list panel "dropdownlist" 'multiple-selections #f 'width 20 'editable #f 'below table))
          (set! tdroplist (cx-create-drop-down-list panel "dropdownlist" 'width 20 'editable #f 'below table))
          (cx-set-list-items tdroplist (list "xxx" "yyy"))
          ;
          (set! tscale (cx-create-scale panel "scale" 'below tdroplist 'value 33 'minimum 0 'maximum 100))
          ;;
          (set! tlisttree (cx-create-list-tree panel "list-tree" 'width 20 'below tscale))
          ;;
          (cx-create-dial panel "dial" 'value 33 'minimum 0 'maximum 100 'below tlisttree)
          ;;
          (set! buttonbox (cx-create-button-box panel "buttonbox" 'below tframe 'panel-buttons #t 'right-of tdroplist))
          ;;
          ;(cx-create-button buttonbox "-" 'icon-button #t)
          (cx-create-button buttonbox "-")
          ;;
          (cx-create-text buttonbox "Text:" 'border #f)
          ;;
          (set! tlist (cx-create-list buttonbox "list" 'multiple-selections #t 'width 20))
          (cx-set-list-items tlist (list "xxx" "yyy"))
          (cx-add-list-items tlist (list "111" "222") 2)
          ;;
          (set! tripanel (cx-create-button-box panel "tri" 'panel-buttons #t 'right-of buttonbox))
          ;;
          (cx-create-draw-area tripanel "Draw" 'width 160 'number-of-colors 4 'selection-x 50 'selection-y 20)
          ;;
          (cx-add-toggle-button tripanel "ddd" #t hello-cb)
          (cx-create-profile tripanel "profile")
          ;;
          (set! tchecks (cx-add-check-button-box tripanel "xx" (list "jeee" 1 "jecc" 2 "jvvecc" 3) 3 'width 40))
          ;;
          (cx-add-check-buttons tchecks "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3)))
          ;;
          (cx-add-radio-buttons tchecks "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3)))


          ;
          (cx-create-button panel "hide" 'panel-button #t 'activate-callback
            (lambda ()
              (cx-set-toggle-button tbutton #f "newtext")
              (cx-delete-list-items tlist (list "111"))
              (cx-set-list-selections tlist (list "xxx" "yyy"))
              (cx-toggle-list-selections tlist (list "222"))
              (cx-rename-list-items tlist (list (cons "222" "dddd")))
              (cx-delete-list-selections tlist (list "dddd"))
              (cx-add-list-selections tlist (list "dddd"))
              (cx-hide-item tripanel)
            )
          )
          ;
          (cx-create-button panel "show" 'panel-button #t 'activate-callback
            (lambda ()
              (cx-show-item tripanel)
            )
          )
          ;
        )
      )
      (cx-show-panel panel)
    )
  )
)

(if
  (symbol-bound? 'hy-menu-load? (the-environment))
  (display (format #f "hy-menu-load? : ~a" hy-menu-load?))
  (define hy-menu-load? #f)
)

(define hy-menu-name "HyLab")
(define *hylab-menu* ())

(if (not hy-menu-load?)
  (begin
    (set! *hylab-menu* (cx-add-menu hy-menu-name #\H))
    (cx-add-item hy-menu-name "hello" #\H #f cx-client? hy-panel)
    (cx-add-hitem hy-menu-name "submenu" #\H)
    (gui-menu-insert-subitem! "HyLab" "submenu" "hi" #\H #f cx-client? (lambda () (display 'Hi)))
    (cx-add-separator hy-menu-name)
    (cx-add-item hy-menu-name "exit" #\H #f cx-client? (lambda () (exit)))
    (set! hy-menu-load? #t)
  )
)
