(lang dune 3.8)

(generate_opam_files)

(using melange 0.1)

(source
 (github veikkaus/bs-winston))

(name bs-winston)

(license "MIT")

(maintainers "Kundi team - Veikkaus")

(authors "Kundi team - Veikkaus")

(homepage https://github.com/veikkaus/bs-winston)

(bug_reports https://github.com/veikkaus/bs-winston/issues)

(package
 (name bs-winston)
 (synopsis "Bs Winston")
 (description "Winston for ReasonML")
 (depends
  dot-merlin-reader
  (melange
   (>= 2.0.0))
  (ocaml
   (>= 5.1.0))
  (reason
   (>= 3.10.0))
  ocaml-lsp-server
  opam-check-npm-deps))
