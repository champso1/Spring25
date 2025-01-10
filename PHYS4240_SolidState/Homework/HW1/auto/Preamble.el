;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "Preamble"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("geometry" "margin=1.0in") ("cite" "") ("soul" "") ("hyperref" "") ("caption" "") ("subcaption" "") ("multirow" "") ("multicol" "") ("enumitem" "") ("amsmath" "") ("amssymb" "") ("siunitx" "") ("mathtools" "") ("bm" "") ("esdiff" "") ("esint" "")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "href")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "geometry"
    "cite"
    "soul"
    "hyperref"
    "caption"
    "subcaption"
    "multirow"
    "multicol"
    "enumitem"
    "amsmath"
    "amssymb"
    "siunitx"
    "mathtools"
    "bm"
    "esdiff"
    "esint")
   (TeX-add-symbols
    '("vv" 1)
    '("abs" 1)
    "grad"
    "dd"
    "ddp"
    "sep"
    "sepbig"
    "sepmajor"))
 :latex)

