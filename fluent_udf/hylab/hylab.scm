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
;; Version check
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define is-fluent-release?
  (lambda (r)
    (equal? r (car (inquire-release)))
  )
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Global variables
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(if
  (symbol-bound? 'hy-menu-load? (the-environment))
  (display (format #f "hy-menu-load? : ~a" hy-menu-load?))
  (define hy-menu-load? #f)
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; rp-var
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (set-new-var s v t)
  (if (not (rp-var-object s))
    (rp-var-define s v t #f)
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; GUI Menu
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; syntax:
;;   (hy-restore-mainmenu)
;;   (hy-add-mainmenu name)
;;   (hy-add-menuitem nemuname itemname myproc)
;;   (hy-delete-menuitem itemname)
;;
  
;;;
(define hy-restore-mainmenu
  (lambda ()
    ;;(rampant-menubar)
    ;(display "Not implemented yet!")
    (cx-clear-menubar)
    (set! hy-menu-load? #f)
  )
)

;;;
(define hy-add-mainmenu
  (lambda (name)
    (cx-add-menu name #\H)
  )
)

;;;
(define hy-add-menuitem
  (lambda (menuname itemname myproc)
    (cx-add-item menuname itemname #\H #f cx-client? myproc)
  )
)

;;;
(define hy-delete-menuitem
  (lambda (itemname)
    (cx-delete-item itemname)
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Toolbar
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  cx-add-button-to-toolbar
;;  cx-refresh-toolbar
;;

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

(define (hy-exec-command-ex comm)
  (with-output-to-string
    (lambda ()
      (ti-menu-load-string comm)
    )
  )
)

(define (hy-exec-proc-ex proc)
  (with-output-to-string
    (lambda ()
      (proc)
    )
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

(define hy-get-report-t-value
  (lambda (t f)
    (let ((l) (s (hy-exec-command-ex (format #f "/report/surface-integrals/~a ~a () no" t f))))
      (set! l (string-split s "\n"))
      (display (list-ref  l (- (length l) 1)))
    )
  )
)

(define hy-get-report-tfv-value
  (lambda (t f v)
    (let ((l) (s (hy-exec-command-ex (format #f "/report/surface-integrals/~a ~a () ~a no" t f v))))
      (set! l (string-split s "\n"))
      (display (list-ref  l (- (length l) 1)))
    )
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
;; reactions
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; %list-reactions
;; client-list-reactions


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; profile
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; write-profiles
;; pick-profile
;; profile-name
;; update-profiles
;; delete-profile
;; list-profile-fields
;; list-profiles
;; profile-list
;; gui-write-existing-profile
;; ti-write-existing-profile
;; write-existing-profile
;; gui-read-profile
;; ti-read-profile
;; read-profile
;; %display-profile-points
;; %write-fan-profile
;; %profile-list
;; %delete-profile
;; %create-oriented-profile
;; %any-thread-has-profile?
;; %write-profile-section
;; %read-profile-file
;; %read-profile-section
;; ti-profile
;; profile-options
;; profile
;; display-profile-points
;; ti-write-profiles
;; gui-write-profiles
;; gui-profiles-manage
;; gui-profile-orient
;; axial-profile-to-xy
;; radial-profile-to-xy


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; UDM
;;
;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ti-udm
;; set-udm-defaults
;; gui-user-memory
;;
(define hy-set-udm-num
  (lambda (n)
    (hy-exec-command-ex (format #f "(ti-udm) ~a" n))
  )
)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; UDF
;;
;; libname:
;;   6   -- <libname> 
;;   12  -- <libname>.dll
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; latest-udf-name
;; udf-from-lib?
;; udf-latest-lib-part
;; udf-lib-part
;; udf-fcn-part
;; split-udf-name
;; ti-open-udf-library
;; ti-udf-compile
;; gui-udf-compile
;; close-udf-library
;; open-udf-library
;; load-udf-scm-file
;; open-udf-libraries
;; load-udf-scm-files
;; udf-compile


(define hy-libname-conv
  (lambda (libname)
    (if (is-fluent-release? 6)
      (format #f "~a" libname)
      (format #f "~a.dll" libname)
    )
  )
)


(define hy-open-udf-library
  (lambda (libname)
    (if (file-exists? (format #f "~a.dll" libname))
      (open-udf-library (hy-libname-conv libname))
      (begin
        (newline)
        (display "Failed to open the library")
        (newline)
      )
    )
  )
)

(define hy-close-udf-library
  (lambda (libname)
    (close-udf-library (hy-libname-conv libname))
  )
)

(define hy-run-udf-proc
  (lambda (procname libname)
    (%udf-on-demand (format #f "~a::~a" procname (hy-libname-conv libname)))
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
            (cx-create-real-entry table4 "二次风速度" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 2 'row 1)
            (cx-create-real-entry table4 "二次风温度" 'width 20 'value 22.2 'minimum 0 'maximum 100 'col 2 'row 2)
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
(define (hy-tokenizer str)
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
(define (hy-show-output tok)
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

;; bc-map-thread-ids?
;; check-bc-compat
;; set-bc
;; list-bc
;; read-bc
;; write-bc
;; iread-bc
;; iwrite-bc
;; ti-copy-bc
;; copy-thread-bc
;; check-bcs
;; update-bcs
;; bcs-changed
;; client-inquire-bc-name


;/solve/animate/define> define-monitor
;First display the post processing results and then define
;the animation sequence.
;Also, for all type of monitors(surface,residuals....etc),
;first set the monitor parameters and then define the animation monitor
;Animation Name ["sequence-7"] "myanime"
;Enter Window ID [0]
;set animation window? [no]
;Every Timestep? [no]
;Frequency [1] 5
;Animation Of: (grid contour pathline ptracks vector monitor xy-plot)
;Enter Name [contour]
;Storage Type: (memory hmf ppm)
;Enter Storage Type [hmf] ppm
;Storage Path Name [""] "C:"
(define hy-def-anime-monitor
  (lambda (name step path)
    (hy-exec-command
      (format #f "/solve/animate/define define-monitor \"~a\" 0 no yes ~a contour ppm \"~a\"" name step path)
    )
  )
)

;;; MPEG stuff
;; (create-mpeg)
;; (mpeg-open)
;; (play-mpeg)
;; (cx-set-mpeg-compression)
;; *mpeg-compression?*
;; *mpeg-options*
;; *mpeg-qscale*
;; *mpeg-bsearch*
;; *mpeg-psearch*
;; *mpeg-range*
;; *mpeg-pattern*
;; *mpeg-command*
;; (create-mpeg-animation)
;; (cx-set-hardcopy-options-for-mpeg)

;;; Animation stuff
;; (ani-monitor-update)
;; (set-animon-active?)
;; (animon->active?)
;; (ani-monitor-delete)
;; (build-ani-monitor-list-element)
;; (show-ani-monitors)
;; (show-one-ani-monitor <which>)
;; (get-ani-monitors)
;; (ani-monitor-active?)
;; (ani-monitor-name->seq)
;; (ani-monitor-seq->name)
;; (ani-monitor-deactivate)
;; (ani-monitor-activate)
;; (ani-monitor-change-freq)
;; (ani-monitor-rename)
;; (remove-ani-monitor)
;; (add-ani-monitor-command)
;; (run-ani-monitors)
;; (animation-init)



(define hy-create-animation
  (lambda (datafilename imagefile-prefix first-index last-index delta disp-proc)
    (let ((break #f))
      ;;; options:
      ;;;;   dump-window
      ;;;;   jpeg
      ;;;;   post-script
      ;;;;   list
      ;;;;   ppm
      ;;;;   hpgl
      ;;;;   options
      ;;;;   tiff
      ;;;;   image
      ;;;    vrml
      (hy-exec-command "/display/set/hardcopy/driver/tiff")
      ;;; options:
      ;;;; fast-raster
      ;;;; rle-raster
      ;;;; raster
      ;;;; vector
      (hy-exec-command "/display/set/hardcopy/driver/post-format raster")
      (hy-exec-command "/display/set/hardcopy/invert-background? no")
      (hy-exec-command "/display/set/hardcopy/color-mode/color") ; default "gray"
      (hy-exec-command "/display/set/hardcopy/x-resolution 0")
      (hy-exec-command "/display/set/hardcopy/y-resolution 0")
      (do
        (
          (j first-index (+ j delta))
          (i 1 (+ i 1))
        )
        ((or (> j last-index) break))
        (set! break
          (not
            (and
              (read-date (format #f "~a~04d.dat" datafilename j))
              (begin
                (if (= i 1)
                  (set! t0 (time))
                )
                (disp-proc)
                (system "rm temp.tif")
                (hy-exec-command "/disply/hardcopy temp.tif")
                (system (format #f "convert temp.tif ~a~02d.bmp &" imagefile-prefix i))
              )
            )
          )
        )
        (if break
          (begin
            (newline)
            (newline)
            (display "scheme interrupted!")
            (newline)
          )
        )
      )
    )
  )
)

;;
;;
;;
(define hy-display-contour
  (lambda (var min max)
    (hy-exec-command
      (format #f "/display/contour ~a ~a ~a" var min max)
    )
  )
)

(define hy-display-vector-uvw
  (lambda (colorby min max scale skip)
    (hy-exec-command
      (format #f "/display/vector velocity ~a ~a ~a ~a ~a" colorby min max scale skip)
    )
  )
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Iteration
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Test
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;(hy-restore-mainmenu)

(define hy-menu-name "HyLab")

(define hy-hello
    (lambda ()
      (display "Hello HyLab!")
      (newline)
    )
)

(hy-open-udf-library "libhylab")

;;;
(for-each
  (lambda (var) (apply set-new-var var))
  '(
    (my/int   1   integer)  ; user integer
    (my/real  0.5  real)    ; user real
    (my/text  "abc" string) ; user text
  )
)

(define gui-my-input-panel
  (let
    ((panel #f) (mybox) (my-int-entry) (my-real-entry) (my-text-entry))
    ;;;
    (define (update-cb . args)
      (cx-set-integer-entry my-int-entry (rpgetvar 'my/int))
      (cx-set-real-entry my-real-entry (rpgetvar 'my/real))
      (cx-set-text-entry my-text-entry (rpgetvar 'my/text))
    )
    ;;;
    (define (apply-cb . args)
      (rpsetvar 'my/int (cx-show-integer-entry my-int-entry))
      (rpsetvar 'my/real (cx-show-real-entry my-real-entry))
      (rpsetvar 'my/text (cx-show-text-entry my-text-entry))
    )
    ;;;
    (lambda args
      (if (not panel)
        ;; create panel
        (let ((table))
            (set! panel (cx-create-panel "My Input Panel" apply-cb update-cb))
            ;;
            (set! table (cx-create-table panel "" 'border #f 'below 0 'right-of 0))
            ;;
            (set! mybox (cx-create-table table "My Inputs" 'border #t 'row 0))
            (set! my-int-entry (cx-create-integer-entry mybox "my integer" 'row 0 'col 0))
            (set! my-real-entry (cx-create-real-entry mybox "my real" 'row 1 'col 0))
            (set! my-text-entry (cx-create-text-entry mybox "my text" 'row 2 'col 0))
        )
      )
      ;;
      (cx-show-panel panel)
    )
  )
)

(define hy-test-udf
  (lambda ()
    (hy-run-udf-proc "hy_test_fluent_pointer" "libhylab")
    (hy-run-udf-proc "hy_test_myudf_func" "libhylab")
    (hy-run-udf-proc "hy_test_myvector" "libhylab")
    (newline)
  )
)


(define hy-test-export-bc
  (lambda ()
    (display (hy-exec-proc-ex hy-export-bc-names))
  )
)

;;;

(if (not hy-menu-load?)
  (begin
    ;;;
    (hy-add-mainmenu hy-menu-name)

    (hy-add-menuitem hy-menu-name "Reload" (lambda () (load "hylab.scm")))
    
    (hy-add-menuitem hy-menu-name "SetUDM" (lambda () (hy-set-udm-num 3)))
    
    (hy-add-menuitem hy-menu-name "SayHello" hy-hello)

    (hy-add-menuitem hy-menu-name "TestUDF" hy-test-udf)
    
    (hy-add-menuitem hy-menu-name "TestFlow" (lambda ()(hy-run-udf-proc "hy_test_fluent_flow" "libhylab")))
    
    (hy-add-menuitem hy-menu-name "Post" (lambda () (hy-display-contour "velocity" 0 2)))

    (hy-add-menuitem hy-menu-name "ExportBC" hy-test-export-bc)

    (hy-add-menuitem hy-menu-name "ShowInput" gui-my-input-panel)

    (hy-add-menuitem hy-menu-name "CheckSurface" (lambda () (hy-check-surf-name "inlet") (hy-delete-inj "injection-0")))

    (hy-add-menuitem hy-menu-name "ShowDialog" (lambda () (hy-panel hy-hello (lambda () (hy-run-udf-proc "showMsg" "libhylab")))))

    (hy-add-menuitem hy-menu-name "About" (lambda () (hy-run-udf-proc "showMsg" "libhylab")))
    
    (hy-add-menuitem hy-menu-name "Ext" (lambda () ()))
    
    ;;(hy-add-menuitem "HyLabMenu*Ext" "Test" (lambda () ()))

    (set! hy-menu-load? #t)
  )
)


