test: dft.c
	gcc --std=c99 -o dfttest dft.c -DTEST -lm

.PHONY: run
run: test
	./dfttest > log

clean:
	rm -f test log
