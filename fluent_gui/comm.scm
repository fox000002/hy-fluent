;;
;;
;;
;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Item
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  disable-items
;;  enable-items
;;  hide-items
;;  show-items
;;  cx-hide-item
;;  cx-get-item-id
;;  cx-get-menu-id
;;  cx-delete-item
;;  cx-add-hitem
;;  cx-add-item
;;  cx-enable-item
;;  cx-activate-item

;; Execute the menu command.
(cx-gui-do cx-activate-item "MenuBar*HyLabMenu*About")

;; Enable/Disable the menu item
(cx-gui-do cx-enable-item "MenuBar*HyLabMenu*About")
(cx-gui-do cx-enable-item "MenuBar*HyLabMenu*About" #f)

(cx-gui-do cx-activate-item "MenuBar*HyLabMenu*ShowInput")
(cx-gui-do cx-activate-item "My Input Panel*PanelButtons*PushButton1(OK)")
