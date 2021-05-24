;;;; package.lisp

(defpackage #:automaton1
  (:use #:cl #:opticl)
  (:export
    #:render
    #:make-seed
    #:next-iteration))
