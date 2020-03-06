Implementation of the SHA-2 (Secure Hash Algorithm) cryptographic hash. This library was ported from Chibi Scheme's `(chibi crypto sha2)` library.

# API

## `(sha-224 src)`

Computes SHA-224 digest of the src which can be a string, a bytevector, or a binary input port. Returns a hexadecimal string (in lowercase).

## `(sha-256 src)`

Computes SHA-256 digest of the src which can be a string, a bytevector, or a binary input port. Returns a hexadecimal string (in lowercase).
