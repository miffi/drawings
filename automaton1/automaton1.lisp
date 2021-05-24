;;;; automaton1.lisp
(in-package #:automaton1)

;;; DISCLAIMER: a lot of list building is used cause I'm a lazy unoptimising idiot
;; Make an image, loop over all the pixel and make the next iteration after every row
(defun render (h w seed rule)
  (let ((img (make-8-bit-rgb-image h w)))
    (with-image-bounds (height width) img
      (let ((now (next-iteration seed rule)))
        (dotimes (i height)
          (dotimes (j width)
            (let ((x (nth j now)))
              (setf (pixel img i j) (values (* 255 x) (* 255 x) (* 255 x)))))
          (setf now (next-iteration now rule)))))
    (write-png-file "file.png" img)))

;; The automaton figures out the state of the next iteration by its previous state and adjacent members
;; so this function returns it in list form
(defun pattern (location src)
  `(,(nth (f (- location 1) (length src)) src) ,(nth location src) ,(nth (mod (+ location 1) (length src)) src)))

;; Helper function to do closed loop list indexing
(defun f (num x)
  (if (>= num 0)
      num
      (+ x num)))

;; Interprets list returned by pattern as binary
(defun bin (lst)
  (let ((acc 0)
        (i 0))
    (dolist (x (reverse lst))
      (incf acc (* (expt 2 i) x))
      (incf i))
    acc))

;; Find the next-iteration iteration using the rules, the default is rule 110
(defun next-iteration (lst &optional (rules '(0 1 1 0 1 1 1 0)))
  (let ((acc nil)
        (rules (reverse rules)))
    (dotimes (x (length lst))
      (push (nth (bin (pattern x lst)) rules) acc))
    (nreverse acc)))

;; Not really needed by the program, just to make a random seed
(setf *random-state* (make-random-state))
(defun make-seed (bleh)
  (let ((acc nil))
    (dotimes (i bleh)
      (push (random 2) acc))
    acc))
