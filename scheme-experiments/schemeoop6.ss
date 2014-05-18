#!/usr/bin/env tinyscheme
;!#
; lemme explain the two rows above:
;   - guile wants a !# to end the #!
;   - tinyscheme doesn't
;
; works in tinyscheme 1.37!
; target:
; (let ((o (new-test-object 1 "a")))
;   (o f1)
;   (o f3 2)
;   )

(define new-test-object
  ; this is the constructor, so it is a lambda with contructor parameters
  (lambda (a b)
    ; we start building the prototype
    (let* (
           ; for this prototype, we want an int and a string
           ; the default values are taken from the lambda's captures
           ; how to support optional parameters? constructor overloads?
           (my-a a)
           (my-b b)
           ; object methods
           (f1 (lambda ()
                 (display "from f1: changing field0 from ")
                 (display my-a)
                 (set!
                   my-a
                   (+ my-a 1)
                   )
                 (display " to ")
                 (display my-a)
                 (newline)
                 )
               )
           (f2 (lambda ()
                 (display "from f2: field0 = ")
                 (display my-a)
                 (display "; field1 = ")
                 (display my-b)
                 )
               )
           (f3 (lambda (param1)
                 (display "from f3: ")
                 (display (+ my-a param1))
                 (newline)
                 )
               )
           (f4 (lambda (param1 param2)
                 (display "from f4: ")
                 (display (exact->inexact (/
                                            (+ my-a param1)
                                            param2
                                            )
                                          )
                          )
                 (newline)
                 )
               )
           (set-b (lambda (param1)
                    (set! my-b param1)
                    )
                  )
           (get-b (lambda ()
                    my-b
                    )
                  )
           )
      ; return a callable procedure/object monster
      (lambda (name . params)
        (cond ((eqv? name 'f1) (apply f1 params))
              ((eqv? name 'f2) (apply f2 params))
              ((eqv? name 'f3) (apply f3 params))
              ((eqv? name 'f4) (apply f4 params))
              ((eqv? name 'set-b) (apply set-b params))
              ((eqv? name 'get-b) (apply get-b params))
              (else (throw 'no-such-method name))
              )
        )
      )
    )
  )

(define new-derived-object
  (lambda (a)
    (let* (
           (base (new-test-object a "derived"))
           (f1 (lambda ()
                 ; this isn't gonna work...
                 ; no way to access base members
                 ; how to gain access to private members?
                 (base 'f1) 
                 (base 'f1)
                 )
               )
           )
      (lambda (name . params)
        (cond ((eqv? name 'f1) (apply f1 params))
              (else (apply base (cons name params)))
              )
        )
      )
    )
  )

(let ((o (new-test-object 0 "hello"))
      (k (new-test-object 0 "other"))
      (q (new-derived-object 12))
      )
  (o 'f1)
  (k 'f1)
  (k 'f1)
  (o 'f2) (newline)
  (k 'f2) (newline)
  (k 'f3 1)
  (o 'f4 2 2)
  (k 'f4 2 2)
  (o 'set-b "manfred")
  (k 'set-b "george")
  (o 'f2) (newline)
  (k 'f2) (newline)
  (display (o 'get-b))(newline)
  (let ((b (o 'get-b)))
    (set! b "asd")
    (display b)(newline)
    )
  (display (o 'get-b))(newline)

  (q 'f1)
  (q 'f2)(newline)
  (q 'f3 2)
  (q 'f4 2 2)
  (q 'set-b "newname")
  (q 'f2)(newline)
  ;(q 'fail)
  )

; next target: better inheritance!
