;;;
;;;     Copyright 1991-1994 Fluent Inc.
;;;     All Rights Reserved.
;;;
;;;     This is unpublished proprietary source code of Fluent Inc.
;;;     It is protected by U.S. copyright law as an unpublished work
;;;     and is furnished pursuant to a written license agreement.  It
;;;     is considered by Fluent to be confidential and may not be
;;;     used, copied, or disclosed to others except in accordance with
;;;     the terms and conditions of the afore mentioned agreement.
;;
;; Reports CPU times of Cortex and solver processes for specified function.
;; Example:  (benchmark '(iterate 50))

(define (benchmark thunk)
  (let ((t0/cortex)
	(t0/solver)
	(t0/clock)
	(t1/cortex)
	(t1/solver)
	(t1/clock)
	(result))
    (set! t0/clock (time))
    (set! t0/solver (cx-send '(cpu-time)))
    (set! t0/cortex (cpu-time))
    (set! result (eval thunk user-initial-environment))
    (set! t1/cortex (cpu-time))
    (set! t1/solver (cx-send '(cpu-time)))
    (set! t1/clock (time))
    (format "cpu-time: cortex=~a, solver=~a~%elapsed-time: ~a~%"
	    (- t1/cortex t0/cortex)
	    (- t1/solver t0/solver)
	    (- t1/clock t0/clock))
    result
  )
)
