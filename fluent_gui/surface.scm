;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Surface
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Syntax:
;;
;;
;;   surface-vertex-max
;;   surface-vertex-min
;;   surface-vertex-average
;;   surface-facet-max
;;   surface-facet-min
;;   surface-facet-average
;;   surface-sum
;;   surface-massavg
;;   surface-mass-average
;;   surface-volume-rate
;;   surface-mass-flow-rate
;;   surface-flow-rate
;;   surface-standard-deviation
;;   surface-integral
;;   (surface-average surf-list)
;;   (surface-area surf-list)
;;   (surface-integrate surf-list type var) ;; (surface-integrate '(0) 'average)
;;
;;   %surface-integrate
;;
;;   gui-surface-integrals
;;   ti-surface-integral
;;
;;   (cx-surface-get-min-max surf-list . cell-values?)
;;
;;   inquire-surface-line-names
;;   inquire-surface-plane-names
;;   inquire-surface-group-names-of-type
;;   inquire-surface-group-names

;;   cx-delete-srf-ref-in-grp
;;   surface-ids->surface-groups
;;   surface-id->surface-group
;;   cx-reset-surface-groups
;;   cx-init-surface-groups
;;   make-surface-group

;;   grp->srf
;;   cx-rename-srf-group
;;   cx-get-group-srfs
;;   srf-grp?
;;   cx-delete-group
;;   cx-add-new-srf-group
;;   cx-ungroup
;;   remove-from-grp-list
;;   cx-group-grps
;;   flatten-surface-groups

;;   ((surface-facets surf-desc) ;; ??
;;   (rename-surface id name)
;;
;;   (temp-surface? id)
;;
;;   (surface? id)
;;   (list-surfaces)
;;   ((surface-name surf-desc) ;; ???
;;   (get-surface id)
;;   (new-surface-id)

;;   (cx-get-surface-ids surf-name-list)

;;   (surface-name/id? name/id)
;;   (valid-surf-name? name)
;;   (check-surf-name name)

;;   (cx-list-surfaces)
;;
;;   cx-surface-area-vectors
;;
;;   (surface-id->name id)
;;   (surface-ids->names id-list)
;;   (surface-name->id name)

;;   (inquire-point-surface-names)
;;   (inquire-surface-names)
;;   (inquire-surface-ids)

;;   (surface-id id-list)
;;
;;   (cx-active-surface surf-desc) ;; ??
;;   cx-set-surface
;;   cx-get-surface
;;   cx-set-surface-type-all
;;   cx-set-surface-type
;;   cx-set-surface-lists
;;   set-surface-version
;;   surface-version
;;   cx-save-surface-lists
;;
;;   (surface-values id)
;;   (surface-coordinates id)
;;   (cx-surface-coordinates id)
;;   (cx-surface-values id)
;;
;;   (show-surface-units name/id)
;;   (show-surface-quantity name/id)
;;   (show-surface-type id/name)
;;
;;   (num-surf id/name-list)
;;   (max-surf-id surf-list)
;;
;;   (multi-surface?) ;; ???
;;
;;   (surface-ids->name-pair sid)
;;
;;   (read-new-surface-id.name)
;;
;;
;;   rake-surface
;;   mrake-surface
;;   line-surface
;;   mline-surface
;;   plane-surface
;;   mplane-surface
;;   quadric-surface
;;
;;   (surface-id->zone-id sids)
;;
;;   (zone-id->surface-id  zids)
;;
;;   cx-update-surface-attr
;;   cx-update-all-surface-attr
;;   cx-ancestor-surfaces-id
;;   cx-ancestor-surfaces-id-list
;;   cx-purge-surface-def-list
;;   cx-create-surface-from-def
;;   cx-generate-susp-surface-defs
;;   cx-add-surface-def
;;   cx-get-def-coarse-surface

;;   cx-suspend-surfaces
;;   cx-suspend-all-surfaces
;;   cx-update-surface
;;   cx-create-surface
;;   cx-rename-surface
;;   cx-delete-surface
;;   cx-add-surface
;;   cx-new-temp-surface-index
;;   set-next-surface-index
;;   new-surface-index
;;   cx-active-surface-ids
;;   cx-store-surface-all-attr
;;   cx-store-surface-desc-attr
;;   cx-store-surface-attr
;;   cx-get-surface-attr
;;   cx-get-surface-desc-attr
;;
;;   cleanup-case-surfaces


