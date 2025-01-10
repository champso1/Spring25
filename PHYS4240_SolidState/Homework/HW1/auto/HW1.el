;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "HW1"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "titlepage")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("import" "") ("Preamble" "")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art10"
    "import"
    "Preamble"))
 :latex)

