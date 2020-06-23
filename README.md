# sha2

## Index 
- [Intro](#Intro)
- [Dependencies](#Dependencies)
- [Test dependencies](#Test-dependencies)
- [Foreign dependencies](#Foreign-dependencies)
- [API](#API)
- [Examples](#Examples)
- [Author(s)](#Author(s))
- [Maintainer(s)](#Maintainer(s))
- [Version](#Version) 
- [License](#License) 
- [Tags](#Tags) 

## Intro 
Implementation of the SHA-2 (Secure Hash Algorithm) cryptographic hash, ported from Chibi Scheme's `(chibi crypto sha2)` library.

## Dependencies 
None

## Test-dependencies 
None

## Foreign-dependencies 
None

## API 

### (cyclone crypto sha2)

#### [procedure]   `(sha-224 src)`

Computes SHA-224 digest of the src which can be a string, a bytevector, or a binary input port. Returns a hexadecimal string (in lowercase).

#### [procedure]   `(sha-256 src)`

Computes SHA-256 digest of the src which can be a string, a bytevector, or a binary input port. Returns a hexadecimal string (in lowercase).


## Examples
```scheme
;; An example program to compute SHA256 of a file
(import (scheme base) (scheme write) (cyclone crypto sha2))

(when (null? (command-line-arguments))
  (display "Usage: sha256sum-cyc FILE")
  (newline)
  (exit 1))

(display
  (sha-256 (open-binary-input-file (car (command-line-arguments)))))
(display "  ")
(display (car (command-line-arguments)))
(newline)
```

## Author(s)
Justin Ethier

## Maintainer(s) 
Justin Ethier

## Version 
0.2

## License 
MIT

## Tags 
crypto networking

