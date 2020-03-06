(import (scheme base) (scheme write) (cyclone crypto sha2))

(when (null? (command-line-arguments))
  (display "Usage: sha256sum FILE")
  (newline)
  (exit 1))

(display
  (sha-256 (open-binary-input-file (car (command-line-arguments)))))
(display "  ")
(display (car (command-line-arguments)))
(newline)
