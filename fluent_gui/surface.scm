;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Surface
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Syntax:
;;
;;
      surface-vertex-max
      surface-vertex-min
      surface-vertex-average
      surface-facet-max
      surface-facet-min
      surface-facet-average
      surface-sum
      surface-massavg
      surface-mass-average
      surface-volume-rate
      surface-mass-flow-rate
      surface-flow-rate
      surface-standard-deviation
      surface-integral
      (surface-average '(surface list))
      (surface-area '(surface list))
      (surface-integrate '(surface list) type var) ;; (surface-integrate '(0) 'average)
      
      %surface-integrate
      
      inquire-surface-line-names
      inquire-surface-plane-names
      inquire-surface-group-names-of-type
      inquire-surface-group-names
      
      surface-facets
      rename-surface
      temp-surface?
      surface?
      (list-surfaces)
      surface-name
      (get-surface id)
      new-surface-id
      
      cx-get-surface-ids
      surface-name/id?
      valid-surf-name?
      check-surf-name
      
      cx-list-surfaces
      cx-surface-area-vectors
      surface-id->name
      surface-ids->names
      surface-name->id
      
      inquire-point-surface-names
      inquire-surface-names
      inquire-surface-ids
      
      surface-id
      cx-active-surface
      cx-set-surface
      cx-get-surface
      cx-set-surface-type-all
      cx-set-surface-type
      cx-set-surface-lists
      set-surface-version
      surface-version
      cx-save-surface-lists
      
      (surface-values id)
      (surface-coordinates id)
      cx-surface-coordinates
      cx-surface-values