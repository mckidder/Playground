;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

"Hello World"
"Hello World"
'Matt Kidder'
(+ 1 2 (+ 3))
6
(* (+ 1 1) (- 1 100)) ;multiply the result of 1+1 and -99
-198
(< 1 2) ;compare 1<2, return true
t
(message "Hello World!") ;echo string Hello World!
"Hello World!"
(if (< 1 1)(message "True")(message "False"));compare 1<1, print string false
"False"
(min 4 3 2 -1 2 3 4);find minimum in provided arguments
-1
foo ;create an error as foo is not defined
(setq x 2);set global variable x to 2
2
(let ((x 5)) x) ;create a local x and set it to 5, print the x
5
x ;print the global x
2
(quote (jan feb mar apr may jun jul aug sep oct nov dec)) ;return argument without evaluation
(jan feb mar apr may jun jul aug sep oct nov dec)
'(jan feb mar apr may jun jul aug sep oct nov dec) ;using quote operator to do the same thing as above
(jan feb mar apr may jun jul aug sep oct nov dec)
(car (cdr '(jan feb mar apr may jun jul aug sep oct nov dec))) ;take everything but the head "jan" and then print the head of that "feb"
feb
() ;create an empty list
nil
(defun 2x (x) "twice the input" (* x 2)) ;create a function called 2x that multiplies the input x by 2 and a definition describing what it does
(2x 4) ;call the above function with input x = 4
8
(message "My age is: %d" 20) ;print message and use the format operator to replaced with base 10 representation of 20
"My age is: 20"

