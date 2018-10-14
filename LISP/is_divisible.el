;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

(- 10 (cos 90) (- 10 2))
2.44807361612917
(format "%.4f" (- 10 (cos 90) (- 10 2)))
"2.4481"
(mod 100 3)
1
(let ((myname "Matt Kidder")) (format "Hello %s" myname))
"Hello Matt Kidder"
(car (cdr(cdr '(Python Fortran Algol Lisp C Java))))
Algol

(defun foo (a)
 (setq num (mod a 2))
 (if(= num 0)
     (format "%d is divisible by 2" a)
 (format "%d is not divisible by 2" a))
)
(foo 100)
"100 is divisible by 2"
(foo 3)
"3 is not divisible by 2"
(foo 2)
"2 is divisible by 2"










