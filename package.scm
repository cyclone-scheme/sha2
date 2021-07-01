(package
 (name           sha2)
 (version        "0.3.0")
 (license        "BSD")
 (authors        "Alex Shinn, Alexei Lozovsky")
 (maintainers    "Justin Ethier <justin.ethier at gmail dot com>")
 (description    "Native implementation of SHA-2 digest algorithms")
 (tags           "crypto" "hash")
 (docs           "https://github.com/cyclone-scheme/cyclone-winds/wiki/sha2")
 (test           "test-sha2.scm")

 (program
   (name sha256sum-cyc)
   (description "A simple program to compute SHA256 of a file"))

 (library
     (name (cyclone crypto sha2))
   (description "SHA-2 library")))
