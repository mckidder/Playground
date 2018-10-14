;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

;Your Name:Matt Kidder


(require 'cl)

;======================
;make the function call mlto-test work
;======================

;---------------
(defun my-LT(x y) (< x y))

;---------------
(defun my-GT(x y) (> x y ))

(defun my-EQ(x y) (= x y ))

;---------------
(defun my-list-total-order(fn L)
  (setq listL (list L))
  (if (<= 2 (length L))
      (apply (lambda (x);apply specific function to entire list, use lambda so we can call multiple inputs for the function being applied
                (funcall fn (car x) (cadr x))) listL)
    (print "need length of at least 2"))
)
;---------------
(defun mlto-test(fn L)
  (dolist (l L) (print l) (print (my-list-total-order fn l))))

;---------------
; call 1 ; check whether atoms within each list are equal; call and output are shown below.
(mlto-test #'my-EQ (list '(1) '(2 1) '(5 4 3 2 1) '(1 2) '(1 2 3 4 5) '(1 2 3 4 5 1) '(4 1 2 3 2 3) '(1 1 1 2 1 1) '(2 2 2 2)))

(1)

"need length of at least 2"

"need length of at least 2"

(2 1)

nil

(5 4 3 2 1)

nil

(1 2)

nil

(1 2 3 4 5)

nil

(1 2 3 4 5 1)

nil

(4 1 2 3 2 3)

nil

(1 1 1 2 1 1)

t

(2 2 2 2)

t
nil

;---------------
; call 2 ; check whether atoms within each list are less than the next atom; call and output are shown below.;
(mlto-test #'my-LT (list '(1) '(2 1) '(5 4 3 2 1) '(1 2) '(1 2 3 4 5) '(1 2 3 4 5 1) '(4 1 2 3 2 3) '(1 1 1 2 1 1) '(2 2 2 2)))

(1)

"need length of at least 2"

"need length of at least 2"

(2 1)

nil

(5 4 3 2 1)

nil

(1 2)

t

(1 2 3 4 5)

t

(1 2 3 4 5 1)

t

(4 1 2 3 2 3)

nil

(1 1 1 2 1 1)

nil

(2 2 2 2)

nil
nil

;---------------
; call 3 ; check whether atoms within each list are greater than the next atom; call and output are shown below.;

(mlto-test #'my-GT (list '(1) '(2 1) '(5 4 3 2 1) '(1 2) '(1 2 3 4 5) '(1 2 3 4 5 1) '(4 1 2 3 2 3) '(1 1 1 2 1 1) '(2 2 2 2)))

(1)

"need length of at least 2"

"need length of at least 2"

(2 1)

t

(5 4 3 2 1)

t

(1 2)

nil

(1 2 3 4 5)

nil

(1 2 3 4 5 1)

nil

(4 1 2 3 2 3)

t

(1 1 1 2 1 1)

nil

(2 2 2 2)

nil
nil

;======================
; same as previous problem, but using a different approach
;======================

;---------------
(defun my-test(fn2 x y) (funcall fn2 x y))

;---------------
(defun my-list(fn fn2 L)
  (setq listL (list L))
  (if (<= 2 (length L))
      (apply (lambda (x)
	       (funcall fn fn2 (car x) (cadr x))) listL)
    (print "need length of at least 2"))

)
;---------------
;calls and corresponding results are shown

(my-list #'my-test #'= '(1))

"need length of at least 2"
"need length of at least 2"

(my-list #'my-test #'> '(2 1))
t

(my-list #'my-test #'< '(3 2 1))
nil

(my-list #'my-test #'> '(5 4 3 2 1))
t

(my-list #'my-test #'> '(1 2))
nil

(my-list #'my-test #'< '(1 2 3))
t

(my-list #'my-test #'= '(1 2 3 4))
nil

(my-list #'my-test #'< '(1 2 3 4 5))
t

(my-list #'my-test #'> '(1 2 3 4 5 1))
nil

(my-list #'my-test #'< '(4 1 2 3 2 3))
nil

(my-list #'my-test #'string< '(a b c d e))
t

(my-list #'my-test #'string< '(b a d))
nil

(my-list #'my-test #'string= '(abc abc abc))
t

(my-list #'my-test #'string= '(abc abc abe))
t
