#!/usr/bin/env guile
!#
; found this on the internet (harlan code, used like twice, and it had the
; comment "I'm evil for defining this..." -- I'm intrigued
; now... what does it do?
(define-syntax >::>
  (syntax-rules ()
                ((_ e) e)
                ((_ e (x a ...) e* ...)
                 (>::> (x e a ...) e* ...))
                ((_ e x e* ...)
                 (>::> (x e) e* ...))))
(display "the macro is:
(define-syntax >::>
  (syntax-rules ()
                ((_ e) e)
                ((_ e (x a ...) e* ...)
                 (>::> (x e a ...) e* ...))
                ((_ e x e* ...)
                 (>::> (x e) e* ...))))
")

(display "testing the logging macros
")
; silly macros to do some dumping and/or tracing
; somehow, they're more complicated than C's
; #define T(X) printf(#X "\n"); X
; but that's hygienic macros for you
(define-syntax debug-print
  (syntax-rules ()
                ((_ x) (begin (display 'x)(display " :: ")(display x)(newline)))
                ((_ x ...) (begin (display (quote (x ...)))(display " :: ")(display ((lambda () x ...)))(newline)))
                )
  )
(define-syntax debug-log
  (syntax-rules ()
                ((_ x) (begin 
                         (display "> ")(display (quote x))(newline) 
                         (eval `x (interaction-environment))))))

; silence warning
(define j #f)
(define TESTTMP #f)
(define NUMBER2 #f)
(define NUMBER3 #f)
(define NUMBER4 #f)
(define NUMBER5 #f)
(define f #f)
(define g #f)

(debug-print (+ 1 2))
(debug-print (+ 2 3) (+ 3 4))
(debug-log (display "lol"))
(debug-log (define TESTTMP 1))
(debug-print TESTTMP)
(newline)(newline)(newline)

; what it does:
;   (>::> e) -> e
;   (>::> head (proc param1)) -> (>::> (proc head param1))
;       -> (proc head param1)
;   (>::> head (proc param1) proc2) -> (>::> (proc head param1) proc2)
;       -> (>::> (proc2 (proc head param1))) -> (proc2 (proc head param1))
;   (>::> a b c) -> (>::> (b a) c) -> (>::> (c (b a))) -> (c (b a))
;   etc

(define (x . args)
  (if (null? args)
    '()
    (+ (car args) (apply x (cdr args)))
    )
  )

(define (y . args)
  (if (null? args)
    (lambda tt 0)
    (lambda (head . tail)
      (+ head (car args) (apply (apply y (cdr args)) tail))
      )
    )
  )

(debug-log (define WHATISIT (>::> 3 (y 1 2))))
(debug-log (WHATISIT 4 5 6))

(debug-log (define QWE (lambda (x) (lambda (y) (+ x y)))))
(debug-log (define ASD (lambda (x) (lambda (y) (x y)))))
(debug-log (define NUMBER2 (>::> 1 QWE ASD)))
(debug-print NUMBER2)
(debug-print (NUMBER2 3))

(debug-log (define NUMBER3 (>::> 1 (QWE) ASD)))
(debug-print (NUMBER3 3))

(debug-log (define ZXC (lambda (x) (lambda (y) (* (x y) y)))))
(debug-log (define NUMBER4 (>::> 1 (QWE) ZXC)))
(debug-print (NUMBER4 3))

(debug-log (define EWR (lambda (x y) (lambda (z) (x y z)))))
(debug-log (define NUMBER5 (>::> + (EWR 5))))
(debug-print (NUMBER5 100))

(debug-log (define MMM (lambda (x) (lambda (y) (display "mmm")(newline) (* (x y) 10)))))
(debug-log (define QWE (lambda (x) (lambda (y) (display "qwe")(newline) (+ (x y) 100)))))
(debug-log (define EWR (lambda (x y) (lambda (z) (display "ewr")(newline) (x y z)))))
(debug-log (define NUMBER5 (>::> + (EWR 1000) QWE MMM)))
(debug-print (NUMBER5 5))

(newline)

(debug-log (define (return x) (if (null? x) (begin (display "null") '()) x)))
(debug-log (define (square x) (lambda (y) (let ((val (x y))) (* val val)))))
(debug-log (define (increase x) (lambda (y) (let ((val (x y))) (+ val 1)))))
(debug-log (define f (>::> return increase square )))
(debug-print (f 2))
(debug-log (define g (>::> return square increase)))
(debug-print (g 2))

(debug-log (define (maybe f)
  (lambda (x)
    (if (integer? x)
      (let ((val (f x)))
        val)
      '()))))

(debug-log (define g (>::> return increase maybe)))
(debug-print (g 2))
(debug-print (g "asd"))
(debug-log (define g (>::> return increase square maybe)))
(debug-print (g 2))
(debug-print (g "asd"))

(newline)

(debug-log (define j (>::> #t not not)))
(debug-print j)

(debug-log (define return (lambda (x) x)))
(debug-log (define (scaler o factor)
  (lambda (x) (* factor (o x)))))

(debug-log (define j (>::> return (scaler 2) (scaler 3))))
(debug-print (j 2))

(newline)

(debug-log (define return (lambda (x) (if (null? x) '() x))))
(debug-log (define (scaler o factor)
  (lambda (x) 
    (let ((val (o x)))
      (if (or (null? val)
              (not (number? val)))
        '()
        (* factor val))))))

(debug-log (define j (>::> return (scaler 2) (scaler 3))))
(debug-print (j 2))
(debug-print (j '()))
(debug-print (j "asd"))

(newline)

(debug-log (define (fail _) (lambda (x) '())))
(debug-log (define j (>::> return (scaler 2) fail (scaler 3))))
(debug-print (j 2))
(debug-print (j '()))
