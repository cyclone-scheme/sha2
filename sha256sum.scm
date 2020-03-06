(import (scheme base) (scheme write) (cyclone crypto sha2))

(display
  (sha-256 (open-binary-input-file (car (command-line-arguments)))))
(display "  ")
(display (car (command-line-arguments)))
(newline)
