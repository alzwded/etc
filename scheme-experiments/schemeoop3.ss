; target:
; (let ((o (new-object-x 1)))
;   (o 0)
;   (o 1 2)
;   )

(define (get-nth blob n)
  (let ((data (car blob))
        (funcs (car (cdr blob)))
        )
    (if (= n 0)
      ; bind the "this" data to the method and allow calling it with any
      ; number of parameters
      (lambda params
        ; apply binds the objects that follow as parameters to the procedure
        ; that is the first parameter (or something like that)
        (apply (car funcs) data params)
        )
      (get-nth (list data (cdr funcs)) (- n 1))
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
      (let ((my-test-funcs (list f1 f2 f3 f4)))
        (let ((self (list my-test-data my-test-funcs)))
          (lambda (n . params)
            (if (null? params)
              ((get-nth self n))
              (apply (get-nth self n) params)
              )
            ;((get-nth (list my-test-data my-test-funcs) n) params) ; pushes an empty list if params is empty.......
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
  (o 0)
  (k 0)
  (k 0)
  (o 1) (newline)
  (k 1) (newline)
  ; now, all that's left is to call methods by symbolic name and not by index
  (o 3 2 2)
  (k 3 2 2)
  )

; idea on calling by symbolic name:
; self also has a literal list of the symbolic names of the functions
; then, keep popping the head of both the symbolic names and my-test-funcs
; until the name_parameter matches the symbol
; then, call that head with the parameters
