; target:
; (let ((o (new-test-object 1 "a")))
;   (o f1)
;   (o f3 2)
;   )

(define (get-symbolic blob name)
  (let ((data (car blob))
        (funcs (car (cdr blob)))
        (symbols (car (cdr (cdr blob))))
        )
    ; throw if no such symbol
    (if (null? symbols)
      (throw 'no-such-symbol)
      (if (eq? name (car symbols))
        ; bind the "this" data to the method and allow calling it with any
        ; number of parameters
        (lambda params
          ; apply binds the objects that follow as parameters to the procedure
          ; that is the first parameter (or something like that)
          (apply (car funcs) data params)
          )
        (get-symbolic (list data (cdr funcs) (cdr symbols)) name)
        )
      )
    )
  )

(define new-test-object
  ; this is the constructor, so it is a lambda with contructor parameters
  (lambda (a b)
    ; we start building the prototype
    (let (
          ; for this prototype, we want an int and a string
          ; the default values are taken from the lambda's captures
          ; how to support optional parameters? constructor overloads?
          (my-test-data (list a b))
          ; object methods
          ; func0
          (f1 (lambda (data)
                (display "from f1: changing field0 from ")
                (display (car data))
                (set-car!
                  data
                  (+ (car data) 1)
                  )
                (display " to ")
                (display (car data))
                (newline)
                )
              )
          ; func1
          (f2 (lambda (data)
                (display "from f2: field0 = ")
                (display (car data))
                (display "; field1 = ")
                (display (car (cdr data)))
                )
              )
          ; func2
          (f3 (lambda (data param1)
                (display "from f3: ")
                (display (+
                           (car data)
                           param1
                           )
                         )
                (newline)
                )
              )
          ; func3
          (f4 (lambda (data param1 param2)
                (display "from f4: ")
                (display (exact->inexact (/
                                           (+ (car data) param1)
                                           param2
                                           )
                                         )
                         )
                (newline)
                )
              )
          ; support calling by name? how would that look like?
          )
      ; bundle the data and the functions together
      (let ((my-test-funcs (list f1 f2 f3 f4))
            (my-symbolic-names (list 'f1 'f2 'f3 'f4))
            )
        (let ((self (list my-test-data my-test-funcs my-symbolic-names)))
          (lambda (name . params)
            (if (null? params)
              ((get-symbolic self name))
              (apply (get-symbolic self name) params)
              )
            )
          )
        )
      )
    )
  )
; of course, there's always the question of "how do we support inheritance/interfaces?"

(let ((o (new-test-object 0 "hello"))
      (k (new-test-object 0 "other"))
      )
  (o 'f1)
  (k 'f1)
  (k 'f1)
  (o 'f2) (newline)
  (k 'f2) (newline)
  (k 'f3 1)
  (o 'f4 2 2)
  (k 'f4 2 2)
  )

; next target:
; bind the "self" data into the member methods and not rely on an external parameter or something like that

; a combination between this is schemeoop6 would be great for implementing
; inheritance in such a way that you can freely access the base members
; in schemeoop6 there is no way to do this other than providing
; getters and setters for all fields
