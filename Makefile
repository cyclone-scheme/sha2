all: cyclone/crypto/sha2.o

cyclone/crypto/sha2.o: cyclone/crypto/sha2.sld
	cyclone cyclone/crypto/sha2.sld

test: test-sha2.scm
	cyclone test-sha2.scm && ./test-sha2

.PHONY: clean

clean:
	git clean -fdx
