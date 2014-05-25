; practise call/cc, part the thirde
; print a list of strings only if all strings fail/pass a certain condition

; this procedure walks a list of strings until a condition is met
; it relies on no-questions returning a close containing the 3rd and 4th
; parameters if it wants this loop to continue, or a bool if it wants to
; break
(define (walk-until-question . strings)
  (if (null? strings)
    #t
    (
     ; call no-questions and execute the returned statement
     ; it can either return (apply f (cdr strings)) or a break
     (call/cc-and-params
       no-questions (car strings) walk-until-question (cdr strings))
     )
    )
  )

; create a continuation and further call a function that supports
; receiving the continuation and some arbitrary number of parameters
(define (call/cc-and-params f . params)
  (call/cc (lambda (cont)
             (apply f cont params))))

; this function supports call/cc-and-params
; it expects:
;   - escape function
;   - the current string
;   - the function that should be called if everything is okay
;   - the parameters to that last function
(define (no-questions cont s f rest)
  ; define an internal check function
  ; this function checks if the string continains a '?' character
  ; it is meant to be call/cc'd with parameters
  (letrec ((is-head-question-mark? (lambda (return l)
                                     ; if we've reached the end of the
                                     ; string or a '?' char, return
                                     (cond ((null? l) (return #f))
                                           ((equal? (car l) #\?) (return #t))
                                           ; else, loop
                                           (else
                                             (is-head-question-mark? return (cdr l))
                                             )
                                           )
                                     )
                                   )
           )
    ; check if our conditions are not met
    (cond ((call/cc-and-params is-head-question-mark? (string->list s))
           ; if they aren't, notify that they have been breached
           (cont (lambda ()  #f ))
           )
          (else 
            ; else, return a lambda that continues the loop
            (cont (lambda () (apply f rest)))
            )
          )
    )
  )

; simply routine that prints the elements of a list on one line
(define (display-list l)
  (if (null? l)
    (newline)
    (begin
      (display (car l))
      (display " ")
      (display-list (cdr l))
      )
    )
  )

; define some lists of strings
(let ((lists '(("string one!" "string two?" "should not see this")
               ("3" "2" "1" "0!")
               ("?" "no" "no" "no")
               ("!" "yes" "yes" "yes")
               ("!" "yes" "yes" "maybe?")
               )
             )
      )
  (for-each (lambda (x)
              ; print only the lists we like
              (cond ((apply walk-until-question x)
                     (display "I like this list: ")
                     (display-list x))))
            lists)
  ; if you'd prefer returning a set of lists you like, you could change this for-each to a
  ; (filter (lambda (x) (not (apply walk-until-question x))) lists)
  )

