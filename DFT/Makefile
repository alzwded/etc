dfttest: dft.c
	gcc --std=c99 -o dfttest dft.c -DTEST -lm

fftest: fft.c
	gcc --std=c99 -g -o fftest fft.c -DTEST -lm

.PHONY: run
run: dfttest fftest
	time -p ./dfttest 440 a > log.slow
	time -p ./fftest 440 a > log.fft

clean:
	rm -f test log.* fftest
