;;;; automaton1.asd

(asdf:defsystem #:automaton1
  :description "Describe ray-tracing here"
  :author "Your Name <your.name@example.com>"
  :license  "Specify license here"
  :version "0.0.1"
  :serial t
  :depends-on (#:opticl)
  :components ((:file "package")
               (:file "automaton1")))
