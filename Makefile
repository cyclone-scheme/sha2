all: cyclone/crypto/sha2.o sha256sum-cyc

cyclone/crypto/sha2.o: cyclone/crypto/sha2.sld
	cyclone cyclone/crypto/sha2.sld

sha256sum-cyc: sha256sum-cyc.scm cyclone/crypto/sha2.o
	cyclone sha256sum-cyc.scm

test: test-sha2.scm
	cyclone test-sha2.scm && ./test-sha2

.PHONY: clean

clean:
	git clean -fdx
