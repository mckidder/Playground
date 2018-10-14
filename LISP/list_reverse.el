;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

(defun list-reverse (L) 
  (if (null L)
    L 
    (append (list-reverse (cdr L)) (list (car L))));cdr gives us everything accept head, car then cdr will give us the last item, then just build and append a list of those
  )
(list-reverse '(j a n))
(n a j)
(list-reverse '(1 2 (jan feb) mar))
(mar (jan feb) 2 1)
(list-reverse (list 'a 'b 'c))
(c b a)

(defun item-at (N L)
  (if (or (> N (length L)) (< N 0));see if current n larger or smaller than upper and lower bound
    nil
    (if (= N 0)
      (car L);use similar principle of cdr and car interaction above to find the item-at N
      (item-at (- N 1) (cdr L)))))
(item-at -1 '(red blue green white))
nil
(item-at 1 '(red blue green white))
blue
(item-at 3 '(red blue green white))
white
(item-at 5 '(red blue green white))
nil










