
;;> Implementation of the SHA-2 (Secure Hash Algorithm) cryptographic
;;> hash.

(define-library (sha2)
  (import (scheme base))
  (export sha-224 sha-256)
  (include-c-header "sha2-native.c")

;  (include "sha2-native.scm")
;  (include-shared "crypto")

;;> \procedure{(sha-224 src)}
;;>
;;> Computes SHA-224 digest of the \var{src} which can be a string,
;;> a bytevector, or a binary input port. Returns a hexadecimal string
;;> (in lowercase).

;;> \procedure{(sha-256 src)}
;;>
;;> Computes SHA-256 digest of the \var{src} which can be a string,
;;> a bytevector, or a binary input port. Returns a hexadecimal string
;;> (in lowercase).

  (begin
     (define type-sha-224 0)
     (define type-sha-256 1)

;    (define (process-sha-data! context src)
;      (cond ((or (bytevector? src) (string? src))
;             (add-sha-data! context src))
;            ((input-port? src)
;             (let lp ((chunk (read-bytevector 1024 src)))
;               (unless (eof-object? chunk)
;                 (add-sha-data! context chunk)
;                 (lp (read-bytevector 1024 src)))))
;            (else
;             (error "unknown digest source: " src))))
    
    (define (sha-224 src)
      (let ((context (start-sha type-sha-224)))
        ;(process-sha-data! context src)
        ;(get-sha context)
        'todo))
    
    (define (sha-256 src)
      (let ((context (start-sha type-sha-256)))
        ;(process-sha-data! context src)
        ;(get-sha context)
        'todo))

; TODO: 
; add-sha-data! 
; get-sha

    (define-c start-sha
      "(void *data, int argc, closure _, object k, object type)"
      " start_sha(data, k, type);")
))
