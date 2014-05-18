; scratchpad for working with encapsulated objects in scheme
; maybe scheme supports OOP anyway, but I'm practising here

; framework function; call the nth function on the supplied data
; parameters:
;   blob: ( data (funcs) )
;   n:    the nth function to call
(define (call-nth blob n)
  (let ((data (car blob))
        (funcs (car (cdr blob)))
        )
    (if (eq? n 0)
      ; found the function, call it on the data
      ((car funcs) data)
      ; else, pop the head of funcs and continue without it
      (call-nth (list data (cdr funcs)) (- n 1))
      )
    )
  )
; how to support member functions that are called with parameters?
; maybe:
; ( (get-nth blob n) param1 param2)
(define (get-nth blob n)
  (let ((data (car blob))
        (funcs (car (cdr blob)))
        )
    (if (eq? n 0)
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


; prototype for a new object
; to build this object, e.g. (let ((x (new-test-object 0 "hello"))) dostuff)
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
        (list my-test-data my-test-funcs))
      )
    )
  )
; of course, there's always the question of "how do we support inheritance/interfaces?"

; spawn 2 objects
(let ((o (new-test-object 0 "hello"))
      (k (new-test-object 0 "other"))
      )
  ; dump them
  (display "o: ")
  (display o)
  (newline)

  (display "k: ")
  (display k)
  (newline)

  ; call methods on them
  (call-nth o 0)
  (call-nth o 1)
  (newline)
  (call-nth k 1)
  (newline)
  ; test out calling a method with a random number of parameters
  ((get-nth k 2) 42)
  ((get-nth k 2) 999)
  ((get-nth o 2) 42)
  ((get-nth o 2) 999)
  ((get-nth k 3) 10 2)
  ((get-nth o 3) 10 2)

  ; dump them again
  (display "o: ")
  (display o)
  (newline)

  (display "k: ")
  (display k)
  (newline)
  )
