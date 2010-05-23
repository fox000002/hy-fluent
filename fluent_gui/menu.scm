;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Item
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; *main-menu*
;;
;; main-menu
;;
;; (rampant-menubar)
;; (update-menubar)
;; (client-update-menubar)
;; (cx-update-menubar)
;; (cx-clear-menubar)
;;
;; (cx-add-menu menuname mnemonic) ;; (cx-add-menu "foo" #\F)
;;
;; (cx-add-item menuname itemname #\H #f cx-client? callback)
;;
;; (cx-add-hitem menuname itemname #\H)
;;
;; (cx-add-separator menuname)
;;
;; (cx-delete-item menuname itemname)
;;
;; (cx-get-item-id menuname itemname)
;;
;; (cx-get-menu-id name)
;;
;; (cx-enable-item id enable?)
;;
;; (make-menu-item name test value help)
;;
;; disable-items
;; enable-items
;; hide-items
;; show-items
;;
;; insert-menu-item!
;;
;; ti-menu-insert-item!
;;
;; ti-menu-item->help
;;
;; ti-menu-item->thunk
;;
;; ti-menu-item->name
;;
;; ti-menu-item->test
;;
;; (gui-menu-insert-subitem! menuname itemname subitemname #\H #f cx-client? callback)
;;
;; (gui-menu-insert-item! menuname itemname #\H #f cx-client? callback)
;;
;; set-menu-processing
;;
;;
;; *ti-menu-load-delay*
;; *menu-prompt*
;; *menu-print-always*
;;
;;
;; menu-get
;; menu-do-1
;; menu-do
;; ti-menu-print
;;
;;
;;=================================================================================
;;
;; Fluent 12.1.2
;;
;;=================================================================================
;;
;; update-popup-menus
;; cx-add-separator-in-menu-and-popup
;; cx-add-item-in-menu-and-popup
;; cx-get-popup-menu-id
;; cx-create-popup-menu-item
;; cx-create-popup-menu
;; cx-create-menu-item
;; cx-create-submenu
;; cx-create-menu
;;
;; cx-add-new-menu-item
;; add-import-menu-item
;;