;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;      HyLab Scheme File                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;    Author: huys03@gmail.com           ;;
;;    Update: 2009.12.24                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;    TODO:                              ;;
;;                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Global variables
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define hy-menu-load? #f)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; GUI Menu
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;
(define hy-restore-mainmenu
  (lambda ()
    ;;(rampant-menubar)
    ;(display "Not implemented yet!")
    (set! hy-menu-load? #t)
  )
)

(define hy-add-mainmenu
  (lambda (name)
    (cx-add-menu name #\H)
  )
)

(define hy-add-menuitem
  (lambda (menuname itemname myproc)
    (cx-add-item menuname itemname #\H #f cx-client? myproc)
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; TUI Command
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define hy-exec-command
  (lambda (cmd)
    (ti-menu-load-string cmd)
  )
)

(define hy-add-ti-menu
  (lambda (name menu test value help)
    (ti-menu-insert-item!
      menu
      (make-menu-item name test value help)
    )
    #t
  )
)

(define hy-add-ti-mainmenu
  (lambda (name test value help)
    (ti-menu-insert-item!
      main-menu
      (make-menu-item name test value help)
    )
    #t
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Grid
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Default: mm -> m
;;
;; Usage: (scale-grid fx fy fz)
;;        or (%scale-grid fx fy fz)
(define hy-scale-grid
  (lambda ()
    (scale-grid 0.001 0.001 0.001)
  )
)

;;
(define hy-check-grid
  (lambda ()
    (if grid-check-done?
      (display "Grid has been checked!\n")
      (begin
        (display "Grid has not been checked!\n")
        (grid-check)
      )
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Model
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; gui-models-energy
;; rf-energy?


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; UDF
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define hy-open-udf-library
  (lambda (libname)
    (open-udf-library libname)
  )
)

(define hy-close-udf-library
  (lambda (libname)
    (close-udf-library libname)
  )
)

(define hy-run-udf-proc
  (lambda (procname libname)
    (%udf-on-demand (format #f "~a::~a" procname libname))
  )
)

;;; Call UDF_TYPE_EXECUTE_FROM_GUI
(define hy-run-udf-gui-proc
  (lambda (mode)
    (%run-udf-apply mode)
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; DPM
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;
(define hy-check-surf-name
  (lambda (surfname)
    (if
      (not
        (memq
          (if (string? surfname)
            (string->symbol surfname)
            surfname
          )
          (inquire-surface-names)
        )
      )
      (begin
        (display (format #f "No surface with the name : ~a\n" surfname) )
        (newline)
        #f
      )
      (begin
        (display (format #f "~a is already existed!" surfname) )
        (newline)
        #t
      )
    )
  )
)

(define hy-check-inj-name
  (lambda (surfname)
    (if
      (not
        (memq
          (if (string? surfname)
            (string->symbol surfname)
            surfname
          )
          (inquire-injection-names)
        )
      )
      (begin
        (display (format #f "No injection with the name : ~a\n" surfname) )
        (newline)
        #f
      )
      (begin
        (display (format #f "Injection ~a is already existed!" surfname) )
        (newline)
        #t
      )
    )
  )
)

(define hy-delete-inj
  (lambda (injname)
    (ti-menu-load-string
      (format #f "/define/injections/delete-injection ~a" injname)
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Display
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define hy-setup-display
  (lambda ()
    (ti-menu-load-string
      (format #f "/display/set/lights/lights-on? yes")
    )
    (ti-menu-load-string
      (format #f "(ti-toggle-headlight) yes")
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Panel
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define hy-panel
  (lambda (update-cb apply-cb)
    (let ((panel #f)(hy-value))
      ;(define (update-cb)
      ; ()
      ;)
      ;(define (apply-cb)
      ;  (display  "")
      ;  (newline)
      ;)
      ;(lambda ()
        (if (not panel)
          (let ((table) (tframe) (table2) (table3)  (table4) (ttab1) (ttab2) (ttab3) (check_bn1) (entry_volatile) (entry_carbon))
            ;
            (set! panel (cx-create-panel "设置参数" 'update-callback update-cb 'apply-callback apply-cb))
            ;
            (set! tframe (cx-create-frame-tabbed panel "Tabs" 'border #t 'below 0 'right-of 0))
            (set! ttab1 (cx-create-tab tframe "煤粉参数"))
            (set! ttab2 (cx-create-tab tframe "边界条件"))
            ;
            ;(set! check_bn1 (cx-add-check-buttons ttab1 "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3))))
            ;
            ;(cx-add-radio-buttons ttab2 "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3)))

            (set! table (cx-create-table ttab1 "工业分析" 'border #t))
            (set! entry_volatile (cx-create-real-entry table "挥发分(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'row 1))
            (cx-create-real-entry table "固定炭(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'row 2)
            (cx-create-real-entry table "灰分(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'row 3)
            (cx-create-real-entry table "水分(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'row 4)
            ;
            (set! table2 (cx-create-table ttab1 "元素分析" 'border #t 'right-of table))
            (cx-create-real-entry table2 "C(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 1)
            (cx-create-real-entry table2 "H(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 2)
            (cx-create-real-entry table2 "O(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 3)
            (cx-create-real-entry table2 "N(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 4)
            (cx-create-real-entry table2 "S(%)" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 5)

            (set! table3 (cx-create-table ttab1 "煤粉物性" 'border #t 'below table))
            (cx-create-real-entry table3 "低位发热量" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 1)
            (cx-create-real-entry table3 "比热" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 2)
            (cx-create-real-entry table3 "密度" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 3)

            (set! table4 (cx-create-table ttab2 "风速风温" 'border #t))
            (cx-create-real-entry table4 "一次风速度" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 1)
            (cx-create-real-entry table4 "一次风温度" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 1 'row 2)
          )
        )
        (cx-show-panel panel)
      ;)
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Utility
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;takes any string as argument 'str', breaks it on the basis of blank-space or
;newline character into multiple strings. These multiple strings are returned
;as a list.
(define (tokenizer str)
  (let ( (l (string->list str))
         (result '())
         (delimitters (list #\space #\newline #\) #\())
         (started? #f)
         (temp '())
       )
    (for-each
      (lambda (c)
        (if started?
          (if (not (memv c delimitters))
            (set! temp (append temp (list c)))
            (begin
              (set! result (append result (list (list->string temp))))
              (set! temp '())
              (set! started? #f)
            )
          )
         (if (not (memv c delimitters))
           (begin
             (set! temp (append temp (list c)))
             (set! started? #t)
            )
          )
        )
      )
      l
    )
    (if started?
      (set! result (append result (list (list->string temp))))
    )
    result
  )
)

;formats the output to show the place of a string in the list.
(define (show-output tok)
  (let ((f "~10a ~10a~%") (i -1))
    (format f "S.No." "String")
    (format f (make-string 10 #\-) (make-string 10 #\-))
    (for-each
      (lambda (s)
        (set! i (+ 1 i))
        (format f i s)
      )
      tok
    )
  )
)

;;
(define (hy-read-rp-file filename)
  (let ((p (open-input-file (format #f "~a" filename))))
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

;;
(define (hy-sec->hms t)
  (let*
    (
      (h (truncate (/ t 3600)))
      (t1 (- t (* h 3600)))
      (m (truncate (/ t1 60)))
      (s (truncate (- t1 (* m 60))))
    )
    (format #f "~02d:~02d:~02d" h m s)
  )
)

(define (hy-change-time filename)
  (let
    (
      (p (open-input-file filename))
      (found #f)
      (t -1)
    )
    (do
      ((x (read p) (read p)))
      ((or found (eof-object? x)) (close-input-port p))
      (if (eqv? (car x) 37)
        (begin
          (for-each
            (lambda (y)
              (if (eqv? (car y) 'flow-time)
                (begin
                  (set! found #t)
                  (set! t (cadre y))
                )
              )
            )
            (cadr x)
          )
          (newline)
        )
      )
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; External Command
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
(define hy-remove-file
  (lambda (filename)
    (if unix?
      (system (format #f "rm -vf ~a" filename))
      (system (format #f "del /F /Q ~a" filename))
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Post Process
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Test
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define hy-menu-name "HyLab")

(define hy-hello
    (lambda ()
      (display "Hello HyLab!\n")
    )
)

(hy-open-udf-library "libhylab")

;;;
(hy-restore-mainmenu)

(hy-add-mainmenu hy-menu-name)

(hy-add-menuitem hy-menu-name "SayHello" hy-hello)

(hy-add-menuitem hy-menu-name "CheckSurface" (lambda () (hy-check-surf-name "inlet") (hy-delete-inj "injection-0")))

(hy-add-menuitem hy-menu-name "ShowDialog" (lambda () (hy-panel hy-hello (lambda () (hy-run-udf-proc "showMsg" "libhylab")))))

(hy-add-menuitem hy-menu-name "About" (lambda () (hy-run-udf-proc "showMsg" "libhylab")))
