(define hy-panel-list
  (let ((panel #f)(hy-value))
    (define (update-cb)
        ()
    )
    (define (hello-cb)
      (display  "")
      (newline)
    )
    (lambda ()
      (if (not panel)
        (let ((table) (tframe) (ttab1) (ttab2) (ttab3) (tdroplist))
          ;
          (set! panel (cx-create-panel "Hello Panel" 'update-callback update-cb 'apply-callback hello-cb))
          ;
          (set! tframe (cx-create-frame-tabbed panel "Tabs" 'border #t 'below 0 'right-of 0))
          (set! ttab1 (cx-create-tab tframe "tab1"))
          (set! ttab2 (cx-create-tab tframe "tab2"))
          (set! ttab3 (cx-create-tab tframe "tab3"))
          ;
          (cx-add-check-buttons ttab1 "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3)))
          ;
          (cx-add-radio-buttons ttab2 "ccc" (list "jeee" 1 "jecc" 2 "jvvecc" 3) (list (cons "www" 22) (cons "jvvecc" 3)))
          ;
          ;(set! tdroplist (cx-create-drop-down-list ttab3 "dropdownlist" 'multiple-selections #f 'width 20 'editable #f))
          (set! tdroplist (cx-create-drop-down-list ttab3 "dropdownlist" 'width 20 'editable #f))
          (cx-set-list-items tdroplist (list "xxx" "yyy"))
          ;
          (cx-create-button panel "hide" 'panel-button #t 'activate-callback
            (lambda ()
              ()
            )
          )
          (cx-create-button panel "show" 'panel-button #t 'activate-callback
            (lambda ()
              (display (cx-show-symbol-list-selections tdroplist))
            )
          )
          ;
        )
      )
      (cx-show-panel panel)
    )
  )
)

(if (not (cx-get-menu-id "HyLab"))
  (cx-add-menu "HyLab" #\H)
)

(if (not (cx-get-item-id "HyLab" "list"))
  (begin
    (cx-add-separator "HyLab")
    (cx-add-item "HyLab" "list" #\H #f cx-client? hy-panel-list)
  )
)


