;; TODO: weird that the sum is different for some files between this test program, chibi's library, and the sha56sum utility.
;;       must be a bug somewhere

(import (scheme base) (scheme write) (cyclone crypto sha2))

(write (command-line-arguments))
(newline)

(display
  (sha-256 (open-binary-input-file (car (command-line-arguments)))))
(newline)
