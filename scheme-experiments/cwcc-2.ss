; practise call/cc, part deux
; start printing a list of strings until a condition is met or end-of-list
(define (print-until-question . strings)
  (if (null? strings)
    #t
    (
     ; call no-questions and execute the returned statement
     ; it can either return (apply f (cdr strings)) or a break
     (call/cc-and-params no-questions (car strings) print-until-question (cdr strings))
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
           (cont (lambda () (display "no questions allowed, aborting.")(newline) #f ))
           )
          (else 
            ; else, return a lambda that continues the loop
            (cont (lambda () (display s)(newline) (apply f rest)))
            )
          )
    )
  )

(print-until-question "string one!" "string two?" "should not see this")
(print-until-question "?" "no" "no" "no")
(print-until-question "3" "2" "1" "0?" "sike")
