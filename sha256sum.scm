;; TODO: weird that the sum is different for some files between this test program, chibi's library, and the sha56sum utility.
;;       must be a bug somewhere

(import (scheme base) (scheme write) (cyclone crypto sha2))

;(write (command-line-arguments))
;(newline)
;
;(display
;  (sha-256 (open-binary-input-file (car (command-line-arguments)))))
;(newline)

;; Test case, is there a bug in read-bytevector?
(let ((src (open-binary-input-file (car (command-line-arguments)))))
             (let lp ((chunk (read-bytevector 1024 src)))
               (unless (eof-object? chunk)
                 (display (utf8->string chunk))
                 (lp (read-bytevector 1024 src)))))
