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

(define WHATISIT (>::> 3 (y 1 2)))
(WHATISIT 4 5 6)

(define QWE (lambda (x) (lambda (y) (+ x y))))
(define ASD (lambda (x) (lambda (y) (x y))))
(define NUMBER2 (>::> 1 QWE ASD))
(display NUMBER2)(newline)
(display (NUMBER2 3))(newline)

(define NUMBER3 (>::> 1 (QWE) ASD))
(display (NUMBER3 3))(newline)

(define ZXC (lambda (x) (lambda (y) (* (x y) y))))
(define NUMBER4 (>::> 1 (QWE) ZXC))
(display (NUMBER4 3))(newline)

(define EWR (lambda (x y) (lambda (z) (x y z))))
(define NUMBER5 (>::> + (EWR 5) +))
(display (NUMBER5 100))(newline)

(define MMM (lambda (x) (lambda (y) (display "mmm")(newline) (* (x y) 10))))
(define QWE (lambda (x) (lambda (y) (display "qwe")(newline) (+ (x y) 100))))
(define EWR (lambda (x y) (lambda (z) (display "ewr")(newline) (x y z))))
(define NUMBER5 (>::> + (EWR 1000) QWE MMM))
(display (NUMBER5 5))(newline)

(define (return x) (if (null? x) (begin (display null) '()) x))
(define (square x) (lambda (y) (let ((val (x y))) (* val val))))
(define (increase x) (lambda (y) (let ((val (x y))) (+ val 1))))
(define f (>::> return increase square ))
(display (f 2))(newline)
(define g (>::> return square increase))
(display (g 2))(newline)

(define (maybe f)
  (lambda (x)
    (if (integer? x)
      (let ((val (f x)))
        val)
      '())))

(define g (>::> return increase maybe))
(display (g 2))(newline)
(display (g "asd"))(newline)
(define g (>::> return increase square maybe))
(display (g 2))(newline)
(display (g "asd"))(newline)
